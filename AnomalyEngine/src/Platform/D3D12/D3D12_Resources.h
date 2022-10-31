#pragma once

#include "D3D12_CommonHeaders.h"

// TODO: Create mutex abstraction
#include <mutex>

namespace Anomaly::graphics::d3d12
{
    
    class DescriptorHeap;

    struct DescriptorHandle
    {
        D3D12_CPU_DESCRIPTOR_HANDLE CPU{};
        D3D12_GPU_DESCRIPTOR_HANDLE GPU{};

        constexpr bool IsValid() const {return CPU.ptr != 0; };
        constexpr bool IsShaderVisible() const {return GPU.ptr != 0; };

    #ifdef DEBUG
    private:
        friend class DescriptorHeap;
        DescriptorHeap* Container{nullptr};
        u32 Index{};
    #endif
    };

    class DescriptorHeap
    {
    public:
        explicit DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type) : m_Type(type) {}
        DISABLE_COPY_AND_MOVE(DescriptorHeap);
        ~DescriptorHeap() {};

        bool Initialize(u32 capacity, bool isShaderVisible);
        void ProcessDeferredFree(u32 frameIndex);

        [[nodiscard]] DescriptorHandle Allocate();
        void Free(DescriptorHandle& handle);

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const { return m_Type; };
        D3D12_CPU_DESCRIPTOR_HANDLE CPUStart() const { return m_CPUStart; };
        D3D12_GPU_DESCRIPTOR_HANDLE GPUStart() const { return m_GPUStart; };
        ID3D12DescriptorHeap *const Heap() const { return m_Heap.Get(); };
        u32 Capacity() const { return m_Capacity; };
        u32 Size() const { return m_Size; };
        u32 DescriptorSize() const { return m_DescriptorSize; };
        bool IsShaderVisible() const { return m_GPUStart.ptr != 0; };

    private:
        ComPtr<ID3D12DescriptorHeap> m_Heap{nullptr};
    
        D3D12_CPU_DESCRIPTOR_HANDLE m_CPUStart{};
        D3D12_GPU_DESCRIPTOR_HANDLE m_GPUStart{};

        std::mutex m_Mutex{};

        u32 m_Capacity{0};
        u32 m_Size{0};
        u32 m_DescriptorSize{0};
        const D3D12_DESCRIPTOR_HEAP_TYPE m_Type{};

        Scope<u32[]> m_FreeHandles{};
        std::vector<u32> m_DeferredFreeIndices[c_FrameBufferCount]{};
    };

}