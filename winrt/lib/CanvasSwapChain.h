// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use these files except in compliance with the License. You may obtain
// a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#pragma once

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas
{
    using namespace ABI::Microsoft::Graphics::Canvas::Numerics;
    using namespace ABI::Windows::Foundation;
    using namespace ::Microsoft::WRL;

    class CanvasSwapChainManager;

    class CanvasSwapChainFactory
        : public ActivationFactory<
        ICanvasSwapChainFactory,
        CloakedIid<ICanvasDeviceResourceWithDpiFactoryNative>> ,
        public PerApplicationManager<CanvasSwapChainFactory, CanvasSwapChainManager>
    {
        InspectableClassStatic(RuntimeClass_Microsoft_Graphics_Canvas_CanvasSwapChain, BaseTrust);

    public:
        CanvasSwapChainFactory();

        IFACEMETHOD(CreateWithSize)(
            ICanvasResourceCreatorWithDpi* resourceCreator,
            float width,
            float height,
            ICanvasSwapChain** SwapChain) override;

        IFACEMETHOD(CreateWithSizeAndDpi)(
            ICanvasResourceCreator* resourceCreator,
            float width,
            float height,
            float dpi,
            ICanvasSwapChain** SwapChain) override;

        IFACEMETHOD(CreateWithAllOptions)(
            ICanvasResourceCreatorWithDpi* resourceCreator,
            float width,
            float height,
            DirectXPixelFormat format,
            int32_t bufferCount,
            CanvasAlphaMode alphaMode,
            ICanvasSwapChain** SwapChain) override;

        IFACEMETHOD(CreateWithAllOptionsAndDpi)(
            ICanvasResourceCreator* resourceCreator,
            float width,
            float height,
            DirectXPixelFormat format,
            int32_t bufferCount,
            CanvasAlphaMode alphaMode,
            float dpi,
            ICanvasSwapChain** SwapChain) override;

        //
        // ICanvasDeviceResourceWithDpiFactoryNative
        //

        IFACEMETHOD(GetOrCreate)(
            ICanvasDevice* device,
            IUnknown* resource,
            float dpi,
            IInspectable** wrapper) override;
    };

    struct CanvasSwapChainTraits
    {
        typedef IDXGISwapChain2 resource_t;
        typedef CanvasSwapChain wrapper_t;
        typedef ICanvasSwapChain wrapper_interface_t;
        typedef CanvasSwapChainManager manager_t;
    };

    class CanvasSwapChain : RESOURCE_WRAPPER_RUNTIME_CLASS(
        CanvasSwapChainTraits,
        IClosable)
    {
        InspectableClass(RuntimeClass_Microsoft_Graphics_Canvas_CanvasSwapChain, BaseTrust);

        ClosablePtr<ICanvasDevice> m_device;
        float m_dpi;

    public:
        CanvasSwapChain(
            ICanvasResourceCreator* resourceCreator,
            std::shared_ptr<CanvasSwapChainManager> swapChainManager,
            IDXGISwapChain2* dxgiSwapChain,
            float dpi);

        IFACEMETHOD(CreateDrawingSession)(
            Color clearColor,
            ICanvasDrawingSession** drawingSession) override;

        // ICanvasSwapChain
        IFACEMETHOD(get_Device)(ICanvasDevice** value) override;
        IFACEMETHOD(get_Size)(Size* value) override;
        IFACEMETHOD(get_SizeInPixels)(BitmapSize* value) override;
        IFACEMETHOD(get_Dpi)(float* value) override;
        IFACEMETHOD(get_Format)(DirectXPixelFormat* value) override;
        IFACEMETHOD(get_BufferCount)(int32_t* value) override;
        IFACEMETHOD(get_AlphaMode)(CanvasAlphaMode* value) override;
        
        IFACEMETHOD(get_Rotation)(CanvasSwapChainRotation* value) override;
        IFACEMETHOD(put_Rotation)(CanvasSwapChainRotation value) override;

        IFACEMETHOD(get_SourceSize)(Size* value) override;
        IFACEMETHOD(put_SourceSize)(Size value) override;

        IFACEMETHOD(get_TransformMatrix)(Matrix3x2* value) override;
        IFACEMETHOD(put_TransformMatrix)(Matrix3x2 value) override;

        IFACEMETHODIMP ConvertPixelsToDips(int pixels, float* dips) override;
        IFACEMETHODIMP ConvertDipsToPixels(float dips, int* pixels) override;

        IFACEMETHOD(Present)() override;
        IFACEMETHOD(PresentWithSyncInterval)(int32_t syncInterval) override;

        IFACEMETHOD(ResizeBuffersWithSize)(
            float newWidth,
            float newHeight) override;

        IFACEMETHOD(ResizeBuffersWithAllOptions)(
            float newWidth,
            float newHeight,
            DirectXPixelFormat newFormat,
            int32_t bufferCount) override;

        // IClosable
        IFACEMETHOD(Close)() override;

    private:
        DXGI_SWAP_CHAIN_DESC1 GetResourceDescription(); // Expected to be called from exception boundary.

    };
    
    class CanvasSwapChainManager : public ResourceManager<CanvasSwapChainTraits>
    {
    public:
        ComPtr<CanvasSwapChain> CreateNew(
            ICanvasResourceCreator* resourceCreator,
            float width,
            float height,
            DirectXPixelFormat format,
            int32_t bufferCount,
            CanvasAlphaMode alphaMode,
            float dpi);

        ComPtr<CanvasSwapChain> CreateWrapper(
            ICanvasDevice* device,
            IDXGISwapChain2* resource,
            float dpi);
    };

}}}}
