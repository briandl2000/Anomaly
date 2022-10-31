#include "D3D12_Resources.h"

#include "D3D12_Core.h"

namespace Anomaly::graphics::d3d12
{

#pragma region DescriptorHeap

    bool DescriptorHeap::Initialize(u32 capacity, bool isShaderVisible) 
    {
        std::lock_guard lock{m_Mutex};
        
        AASSERT(capacity && capacity < D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2);
        AASSERT(!(m_Type == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER && capacity > D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE));
        
        if(m_Type == D3D12_DESCRIPTOR_HEAP_TYPE_DSV && m_Type == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
        {
            isShaderVisible = false;
        }

        ID3D12Device8 *const device{core::GetDevice().Get()};
        AASSERT(device);

        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = m_Type;
        desc.NumDescriptors = capacity;
        desc.Flags = isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0;

        AWINDOWS_ASSERT(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Heap)));

        m_FreeHandles = std::move(CreateScope<u32[]>(capacity));
        m_Capacity = capacity;
        m_Size = 0;
        
        for(u32 i = 0; i < capacity; i++) m_FreeHandles[i] = i;
        for(u32 i = 0; i < c_FrameBufferCount; i++) 
        {
            AASSERT(m_DeferredFreeIndices[i].empty());
        }

        m_DescriptorSize = device->GetDescriptorHandleIncrementSize(m_Type);
        m_CPUStart = m_Heap->GetCPUDescriptorHandleForHeapStart();
        m_GPUStart = isShaderVisible ? m_Heap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE{ 0 };

        return true;
    }
    
    void DescriptorHeap::ProcessDeferredFree(u32 frameIndex) 
    {
        std::lock_guard lock{m_Mutex};
        AASSERT(frameIndex < c_FrameBufferCount);

        std::vector<u32>& indices{ m_DeferredFreeIndices[frameIndex]};
        if(!indices.empty())
        {
            for(auto index : indices)
            {
                m_Size--;
                m_FreeHandles[m_Size] = index;
            }
            indices.clear();
        }
    }
    
    [[nodiscard]] DescriptorHandle DescriptorHeap::Allocate() 
    {
        std::lock_guard lock{m_Mutex};

        AASSERT(m_Heap);
        AASSERT(m_Size < m_Capacity);

        const u32 index{m_FreeHandles[m_Size]};
        const u32 offset{index * m_DescriptorSize};
        m_Size++;

        DescriptorHandle handle;
        handle.CPU.ptr = m_CPUStart.ptr + offset;

        if(IsShaderVisible())
            handle.GPU.ptr = m_GPUStart.ptr + offset;

        #ifdef DEBUG
        handle.Container = this;
        handle.Index = index;
        #endif

        return handle;
    }

    void DescriptorHeap::Free(DescriptorHandle& handle) 
    {
        if(!handle.IsValid()) return;
        std::lock_guard lock{m_Mutex};
        AASSERT(m_Heap && m_Size);
        AASSERT(handle.CPU.ptr >= m_CPUStart.ptr);
        AASSERT((handle.CPU.ptr - m_CPUStart.ptr) % m_DescriptorSize == 0);
        const u32 index{(u32)(handle.CPU.ptr - m_CPUStart.ptr) / m_DescriptorSize};
        #ifdef DEBUG
        AASSERT(handle.Container == this);
        AASSERT(handle.Index < m_Capacity);
        AASSERT(handle.Index == index);
        #endif

        const u32 frameIndex{ core::CurrentFrameIndex() };
        m_DeferredFreeIndices[frameIndex].push_back(index);
        core::SetDeferredReleasesFlag();

        handle = {};
    }


#pragma endregion


}