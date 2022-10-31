#include "D3D12_Core.h"

#include "D3D12_Resources.h"
#include "D3D12_Surface.h"

#include <mutex>

#ifdef PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

#include <unordered_map>

#include "imgui/imgui_impl_dx12.h"
#include "imgui/imgui_impl_win32.h"

#include "Core/Event.h"

#include "d3dx12.h"

namespace Anomaly::graphics::d3d12::core
{

    // TODO: REMOVE THIS TO A MEMORY SPECIFIC FILE
    std::string GetMemorySizeUnit(u32 size)
    {
        const u64 gib = 1024 * 1024 * 1024;
        const u64 mib = 1024 * 1024;
        const u64 kib = 1024;

        char unit[4] = "XiB";

        if (size >= gib) {
            unit[0] = 'G';
        } else if (size >= mib) {
            unit[0] = 'M';
        } else if (size >= kib) {
            unit[0] = 'K';
        } else {
            unit[0] = 'B';
            unit[1] = 0;
        }

        return unit;
    }
    float GetMemorySizeFloat(u32 size) {
        const u64 gib = 1024 * 1024 * 1024;
        const u64 mib = 1024 * 1024;
        const u64 kib = 1024;

        char unit[4] = "XiB";
        float amount = 1.0f;

        if (size >= gib) {
            amount = size / (float)gib;
        } else if (size >= mib) {
            amount = size / (float)mib;
        } else if (size >= kib) {
            amount = size / (float)kib;
        } else {
            amount = (float)size;
        }

        return amount;
    }

    namespace 
    {
        // TODO: Create a copy command for uploading data
        // TODO: Create a compute command for compute shaders

        class D3D12_Command
        {
        public:
            D3D12_Command() = default;
            DISABLE_COPY_AND_MOVE(D3D12_Command);
            explicit D3D12_Command(ComPtr<ID3D12Device8> device, D3D12_COMMAND_LIST_TYPE type)
            {
                D3D12_COMMAND_QUEUE_DESC desc{};
                desc.Type = type;
                desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
                desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
                desc.NodeMask = 0;

                AWINDOWS_ASSERT(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue)));
        
                NAME_D3D12_OBJECT(m_CommandQueue, type == D3D12_COMMAND_LIST_TYPE_DIRECT ? 
                                                "GFX Command Queue" :
                                                type == D3D12_COMMAND_LIST_TYPE_COMPUTE ? 
                                                "Compute Command Queue" : "Command Queue");

                for(u32 i{0}; i < c_FrameBufferCount; i++)
                {
                    CommandFrame& frame{ m_CommandFrames[i] };
                    AWINDOWS_ASSERT(device->CreateCommandAllocator(type, IID_PPV_ARGS(&frame.CommandAllocator)));
                    NAME_D3D12_OBJECT_INDEXED(m_CommandQueue, i, type == D3D12_COMMAND_LIST_TYPE_DIRECT ? 
                                                                 "GFX Command Allocator" :
                                                                 type == D3D12_COMMAND_LIST_TYPE_COMPUTE ? 
                                                                 "Compute Command Allocator" : "Command Allocator");
                    AWINDOWS_ASSERT(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&frame.Fence)));
                    NAME_D3D12_OBJECT_INDEXED(frame.Fence, i, "D3D12 Fence");
                }

                AWINDOWS_ASSERT(device->CreateCommandList(0, type, m_CommandFrames[0].CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_CommandList)));
                AWINDOWS_ASSERT(m_CommandList->Close());

                NAME_D3D12_OBJECT(m_CommandQueue, type == D3D12_COMMAND_LIST_TYPE_DIRECT ? 
                                                "GFX Command List" :
                                                type == D3D12_COMMAND_LIST_TYPE_COMPUTE ? 
                                                "Compute Command List" : "Command List");

                

                m_FenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
                AASSERT(m_FenceEvent);
            };

            ~D3D12_Command()
            {
                Flush();

                CloseHandle(m_FenceEvent);
                m_FenceEvent = nullptr;
            }

            void BeginFrame()
            {
                CommandFrame& frame{ m_CommandFrames[m_FrameIndex] };
                frame.Wait(m_FenceEvent, frame.Fence.Get());

                AWINDOWS_ASSERT(frame.CommandAllocator->Reset());
                AWINDOWS_ASSERT(m_CommandList->Reset(frame.CommandAllocator.Get(), nullptr));
            }

            void EndFrame()
            {
                AWINDOWS_ASSERT(m_CommandList->Close());
                ID3D12CommandList *const commandLists[]{m_CommandList.Get()};
                m_CommandQueue->ExecuteCommandLists(_countof(commandLists), &commandLists[0]);

                CommandFrame& frame{m_CommandFrames[m_FrameIndex]};
                u64 fenceValueForSignal = ++frame.FenceValue;
                m_CommandQueue->Signal(frame.Fence.Get(), fenceValueForSignal);

                m_FrameIndex = (m_FrameIndex+1) % c_FrameBufferCount;
            }

            void Flush()
            {
                for(u32 i{0}; i < c_FrameBufferCount; i++)
                {
                    CommandFrame& frame{m_CommandFrames[i]};
                    //frame.Wait(m_FenceEvent, m_Fence.Get());

                    u64 fenceValueForSignal = ++frame.FenceValue;
                    m_CommandQueue->Signal(frame.Fence.Get(), fenceValueForSignal);
                    if (frame.Fence->GetCompletedValue() < frame.FenceValue)
                    {
                        frame.Fence->SetEventOnCompletion(fenceValueForSignal, m_FenceEvent);
                        WaitForSingleObject(m_FenceEvent, INFINITE);
                    }
                }
                m_FrameIndex = 0;
            }

        	ID3D12CommandQueue *const CommandQueue() const { return m_CommandQueue.Get(); }
            ID3D12GraphicsCommandList6 *const CommandList() const { return m_CommandList.Get(); }
            u32 FrameIndex() const { return m_FrameIndex; }
        private:

            struct CommandFrame
            {
                ComPtr<ID3D12CommandAllocator> CommandAllocator{nullptr};
                u64 FenceValue{0};
                ComPtr<ID3D12Fence1> Fence{nullptr};

                void Wait(HANDLE fenceEvent, ID3D12Fence1* fence)
                {
                    AASSERT(fence && fenceEvent);

                    if(fence->GetCompletedValue() < FenceValue)
                    {
                        AWINDOWS_ASSERT(fence->SetEventOnCompletion(FenceValue, fenceEvent));
                        WaitForSingleObject(fenceEvent, INFINITE);
                    }
                }
            };

            ComPtr<ID3D12CommandQueue> m_CommandQueue{nullptr};
            ComPtr<ID3D12GraphicsCommandList6> m_CommandList{nullptr};
            CommandFrame m_CommandFrames[c_FrameBufferCount]{};
            HANDLE m_FenceEvent{nullptr};
            u32 m_FrameIndex{0};
        };

        void PrintAdapterDescriptor(const DXGI_ADAPTER_DESC1& adapterDesc, const u32& i)
        {
            ADEBUG("    Adapter: %u", i);
            ADEBUG("        Description: %ls",              adapterDesc.Description);
            ADEBUG("        DedicatedVideoMemory: %.2f%s",  GetMemorySizeFloat((u32)adapterDesc.DedicatedVideoMemory), GetMemorySizeUnit((u32)adapterDesc.DedicatedVideoMemory).c_str());
            ADEBUG("        DedicatedSystemMemory: %.2f%s", GetMemorySizeFloat((u32)adapterDesc.DedicatedSystemMemory), GetMemorySizeUnit((u32)adapterDesc.DedicatedSystemMemory).c_str());
            ADEBUG("        SharedSystemMemory: %.2f%s",    GetMemorySizeFloat((u32)adapterDesc.SharedSystemMemory), GetMemorySizeUnit((u32)adapterDesc.SharedSystemMemory).c_str());
        }

        constexpr DXGI_FORMAT c_RenderTragetFormat{DXGI_FORMAT_R8G8B8A8_UNORM};

        struct GraphicsCoreState
        {
            ComPtr<ID3D12Device8> device{nullptr};
            ComPtr<IDXGIFactory7> factory{nullptr};
            D3D12_Command GraphicsCommand;

            DescriptorHeap rtvDescHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_RTV };
            DescriptorHeap dsvDescHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_DSV };
            DescriptorHeap srvDescHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };
            DescriptorHeap uavDescHeap{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV };

            DescriptorHandle ImguiImageHandle{};

            u32 DeferredReleasesFlag[c_FrameBufferCount]{};
            std::mutex DeferredReleasesMutex{};

            std::unordered_map<Window*, Ref<D3D12_Surface>> Surfaces{};
            Window* MainWindow{nullptr};
        };

        static Scope<GraphicsCoreState> s_State;

        constexpr D3D_FEATURE_LEVEL c_MinimumFeatureLevel{D3D_FEATURE_LEVEL_11_0};

        ComPtr<IDXGIAdapter4> GetAdapter(ComPtr<IDXGIFactory6> factory)
        {
            HRESULT hr{S_OK};
            
            ComPtr<IDXGIAdapter4> adapter{nullptr};

            ADEBUG("Going trhough adapters: ");
            for(u32 i = 0; factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++)
            {
                DXGI_ADAPTER_DESC1 adapterDesc{};
                adapter->GetDesc1(&adapterDesc);
                PrintAdapterDescriptor(adapterDesc, i);

                hr = D3D12CreateDevice(adapter.Get(), c_MinimumFeatureLevel, __uuidof(ID3D12Device1), nullptr);
                if(SUCCEEDED(hr))
                {
                    return adapter;
                }
            }

            AASSERT_MSG(false, "Could not find a suiting adapter!");
            return nullptr;
        }

        D3D_FEATURE_LEVEL GetMaxFeatureLevel(IDXGIAdapter1* adapter)
        {
            constexpr D3D_FEATURE_LEVEL c_FeatureLevels[4]
            {
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_12_0,
                D3D_FEATURE_LEVEL_12_1
            };

            D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelInfo{};
            featureLevelInfo.NumFeatureLevels = _countof(c_FeatureLevels);
            featureLevelInfo.pFeatureLevelsRequested = c_FeatureLevels;

            ComPtr<ID3D12Device> device;
            AWINDOWS_ASSERT(D3D12CreateDevice(adapter, c_MinimumFeatureLevel, IID_PPV_ARGS(&device)));
            AWINDOWS_ASSERT(device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelInfo, sizeof(featureLevelInfo)));
            return featureLevelInfo.MaxSupportedFeatureLevel;
        }

        void CreateDevice()
        {
            if(s_State->device)
            {
                AASSERT_MSG(false, "Device already exists! cannot create it again!");
                Shutdown();
            }

            u32 createFactoryFlags{0};

            #ifdef DEBUG
            {
                ComPtr<ID3D12Debug3> debugInterface;
                AWINDOWS_ASSERT(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
                debugInterface->EnableDebugLayer();
                debugInterface->SetEnableGPUBasedValidation(true);
                createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
            #endif

            AWINDOWS_ASSERT(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&s_State->factory)));

            ComPtr<IDXGIAdapter1> adapter = GetAdapter(s_State->factory);
            if(!adapter)
            {
                AERROR("Could not find Adapter!");
                return;
            }
            DXGI_ADAPTER_DESC1 adapterDesc{};
            adapter->GetDesc1(&adapterDesc);
            ADEBUG("Selected Adapter: %ls", adapterDesc.Description);

            D3D_FEATURE_LEVEL maxFeatureLevel{GetMaxFeatureLevel(adapter.Get())};
            AASSERT(maxFeatureLevel >= c_MinimumFeatureLevel);
            if(maxFeatureLevel < c_MinimumFeatureLevel) return;

            AWINDOWS_ASSERT(D3D12CreateDevice(adapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&s_State->device)));

            NAME_D3D12_OBJECT(s_State->device, "Main Device");

            #ifdef DEBUG
            {
                ComPtr<ID3D12InfoQueue> infoQueue;
                AWINDOWS_ASSERT(GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue)));
                infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
                infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
                infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
            }
            #endif
        }

        void __declspec(noinline) ProcessDeferredReleases(u32 frameIndex)
        {
             std::lock_guard lock{s_State->DeferredReleasesMutex};
             
             s_State->DeferredReleasesFlag[frameIndex] = 0;

             s_State->rtvDescHeap.ProcessDeferredFree(frameIndex);
             s_State->dsvDescHeap.ProcessDeferredFree(frameIndex);
             s_State->srvDescHeap.ProcessDeferredFree(frameIndex);
             s_State->uavDescHeap.ProcessDeferredFree(frameIndex);
        }
    }

    DXGI_FORMAT GetDefaultRenderTargetFormat() 
    {
        return c_RenderTragetFormat;
    }

    bool Initialize() 
    {
        s_State = CreateScope<GraphicsCoreState>();

        // Create Device
        CreateDevice();
        if(!s_State->device)
        {
            return false;
        }

        // Create Command queues, lists and allocators
        new (&s_State->GraphicsCommand) D3D12_Command(GetDevice(), D3D12_COMMAND_LIST_TYPE_DIRECT);

        // Create Discriptor heaps
        bool result{ true };
        result &= s_State->rtvDescHeap.Initialize(512, false);
        result &= s_State->dsvDescHeap.Initialize(512, false);
        result &= s_State->srvDescHeap.Initialize(4096, true);
        result &= s_State->uavDescHeap.Initialize(512, false);
        if(!result) return false;

        NAME_D3D12_OBJECT(s_State->rtvDescHeap.Heap(), "RTV Descriptor Heap");
        NAME_D3D12_OBJECT(s_State->dsvDescHeap.Heap(), "DSV Descriptor Heap");
        NAME_D3D12_OBJECT(s_State->srvDescHeap.Heap(), "SRV Descriptor Heap");
        NAME_D3D12_OBJECT(s_State->uavDescHeap.Heap(), "UAV Descriptor Heap");

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Create swap chains for each window
        
        // Create Root signature
        
        // Create Pipline state object
        
        // Create Vertex / Index Buffers

        return true;
    }

    void Shutdown()
    {
        if(s_State->MainWindow)
        {
            ImGui_ImplDX12_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }

        for(auto& surface : s_State->Surfaces)
        {
            surface.second = nullptr;
        }

        for(u32 i{0}; i < c_FrameBufferCount; i++)
        {
            ProcessDeferredReleases(i);
        }

        s_State = nullptr;

        #ifdef DEBUG
        {
            ComPtr<IDXGIDebug1> debugDevice;
            DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debugDevice));
            debugDevice->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
        }
        #endif
    }

    void RenderSurface(Window* index)
    {
        // create a new command list
        // For all the shadow maps in the scene
        //  Render all the objects in the scene that contribute to the shadows
        // Bind the diferred render texture (postions, normals, diffuse, metallic/roughness/abiantocclusion, Emission, depth)
        //  For all the materials in the scene
        //   Render all the objects that make us of this material
        // Bind the deferred render texture
        // Render to the deferred render result texture using the the deferred render textures
        // Render FAA using the deferred render result
        // different post processing effects
        // render Imgui render data
        // Render to the backbuffer of the window
        // Close the command list

        const u32 frameIndex{CurrentFrameIndex()};
        if(s_State->DeferredReleasesFlag[frameIndex])
        {
            ProcessDeferredReleases(frameIndex);
        }

        s_State->GraphicsCommand.BeginFrame();
        ID3D12GraphicsCommandList6* CommandList{ s_State->GraphicsCommand.CommandList() };

        Ref<D3D12_Surface> surface = s_State->Surfaces[index];

        {
            CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(surface->GetResource(), 
                                                                                D3D12_RESOURCE_STATE_PRESENT, 
                                                                                D3D12_RESOURCE_STATE_RENDER_TARGET);
            CommandList->ResourceBarrier( 1, &barrier);
        }

        CommandList->RSSetViewports(1, &surface->GetViewPort());
        CommandList->RSSetScissorRects(1, &surface->GetRect());
        CommandList->OMSetRenderTargets(1, &surface->GetRtv(), false, nullptr);

        //Imgui render
        if(s_State->MainWindow)
        {
            // Start the Dear ImGui frame
            ImGui_ImplDX12_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            event::EventData data{};
            data.code = event::eSystemEventCode::EVENT_CODE_IMGUI_EVENT;
            event::FireEvent(event::eSystemEventCode::EVENT_CODE_IMGUI_EVENT, data);

            ImGui::Render();

            auto heap = s_State->srvDescHeap.Heap();
            CommandList->SetDescriptorHeaps(1, &heap);
            ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CommandList);

            // Update and Render additional Platform Windows
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault(NULL, (void*)CommandList);
            }
        }

        {
            CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(surface->GetResource(), 
                                                                                D3D12_RESOURCE_STATE_RENDER_TARGET, 
                                                                                D3D12_RESOURCE_STATE_PRESENT);

            CommandList->ResourceBarrier( 1, &barrier);
        }

        s_State->GraphicsCommand.EndFrame();
        surface->SwapBuffers();
    }

    Surface* CreateSurface(Window* index)
    {
        if(s_State->Surfaces.empty())
        {
            WindowsWindow* windowsWindow = reinterpret_cast<WindowsWindow*>(index);
            HWND hwnd = windowsWindow->GetHandle();
            ImGui_ImplWin32_Init(hwnd);
            s_State->ImguiImageHandle = s_State->srvDescHeap.Allocate();
            ImGui_ImplDX12_Init(GetDevice().Get(), 3,
                c_RenderTragetFormat, s_State->srvDescHeap.Heap(),
                s_State->ImguiImageHandle.CPU,
                s_State->ImguiImageHandle.GPU);

            s_State->MainWindow = index;
        }
        s_State->Surfaces[index] = CreateRef<D3D12_Surface>(index);
        return s_State->Surfaces[index].get();
    }

    void DestroySurface(Window* index)
    {
        if(s_State->Surfaces[index])
        {
            s_State->Surfaces[index] = nullptr;
            s_State->Surfaces.erase(index);
        }
    }

    ComPtr<ID3D12Device8> const GetDevice() 
    {
        if(!s_State || !s_State->device)
        {
            AASSERT_MSG(false, "No device initialized!");
            return nullptr;
        }

        return s_State->device;
    }

    void Flush()
    {
        if(s_State)
           s_State->GraphicsCommand.Flush();
    }

    u32 CurrentFrameIndex() 
    {
        return s_State->GraphicsCommand.FrameIndex();
    }

    void SetDeferredReleasesFlag() 
    {
        s_State->DeferredReleasesFlag[CurrentFrameIndex()] = 1;
    }

    ComPtr<IDXGISwapChain4> CreateSwapchain(Window* window, const DXGI_SWAP_CHAIN_DESC1& desc) 
    {
        IDXGIFactory7* factory = s_State->factory.Get();
        ID3D12CommandQueue* commandQueue = s_State->GraphicsCommand.CommandQueue();

        ComPtr<IDXGISwapChain1> swapChain;
        ComPtr<IDXGISwapChain4> OutSwapChain;
        #ifdef PLATFORM_WINDOWS
        WindowsWindow* windowsWindow = reinterpret_cast<WindowsWindow*>(window);
        HWND hwnd = windowsWindow->GetHandle();
        AWINDOWS_ASSERT(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &desc, nullptr, nullptr, &swapChain));
        AWINDOWS_ASSERT(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));
        #endif

        AWINDOWS_ASSERT(swapChain->QueryInterface(IID_PPV_ARGS(&OutSwapChain)));

        return OutSwapChain;
    }

    DescriptorHeap& GetRtvHeap() 
    {
        return s_State->rtvDescHeap;
    }

    DescriptorHeap& GetDsvHeap() 
    {
        return s_State->dsvDescHeap;
    }

    DescriptorHeap& GetSrvHeap() 
    {
        return s_State->srvDescHeap;
    }

    DescriptorHeap& GetUavHeap() 
    {
        return s_State->uavDescHeap;
    }


    void UploadTextureData(ID3D12Resource* resource, D3D12_SUBRESOURCE_DATA* textureData)
    {
        // TODO: make use of the copy command and wait until it is finished
        ComPtr<ID3D12Resource> textureUploadHeap;

        const u64 uploadBufferSize = GetRequiredIntermediateSize(resource, 0, 1);

        CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        
        CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
        
        GetDevice()->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&textureUploadHeap));

        Flush();

        s_State->GraphicsCommand.BeginFrame();
        ID3D12GraphicsCommandList6* CommandList{ s_State->GraphicsCommand.CommandList() };


        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(resource, 
                                                                                D3D12_RESOURCE_STATE_COPY_DEST, 
                                                                                D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        UpdateSubresources(CommandList, resource, textureUploadHeap.Get(), 0, 0, 1, textureData);
        CommandList->ResourceBarrier(1, &barrier);

        s_State->GraphicsCommand.EndFrame();
        Flush();
    }
}