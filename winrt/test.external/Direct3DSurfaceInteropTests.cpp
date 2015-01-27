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

#include "pch.h"

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::DirectX::Direct3D11;
using namespace Microsoft::WRL::Wrappers;

TEST_CLASS(Direct3DSurfaceInteropTests)
{
    CanvasDevice^ m_canvasDevice;

    TEST_METHOD_INITIALIZE(Reset)
    {
        m_canvasDevice = ref new CanvasDevice();
    }

    TEST_METHOD(CreateBitmapFromDirect3DSurface_RoundTripsDxgiSurface)
    {
        auto surface = CreateSurface(D3D11_BIND_SHADER_RESOURCE);
        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        auto expectedDxgiSurface = GetDXGIInterface<IDXGISurface>(surface);
        auto actualDxgiSurface   = GetDXGIInterface<IDXGISurface>(bitmap);
        Assert::AreEqual(expectedDxgiSurface.Get(), actualDxgiSurface.Get());
    }

    TEST_METHOD(CreateBitmapFromDirect3DSurface_AlphaAndDpiAreSet)
    {
        auto surface = CreateSurface(D3D11_BIND_SHADER_RESOURCE);
        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, surface, CanvasAlphaMode::Ignore, 35.0f);
        auto d2dBitmap = GetWrappedResource<ID2D1Bitmap1>(bitmap);

        VerifyDpiAndAlpha(d2dBitmap, 35.0f, D2D1_ALPHA_MODE_IGNORE);
    }

    TEST_METHOD(CreateBitmapFromNonRenderTargetDirect3DSurface_CreatesBitmap)
    {
        auto surface = CreateSurface(D3D11_BIND_SHADER_RESOURCE);
        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        AssertTypeName<CanvasBitmap>(bitmap);
    }

    TEST_METHOD(CreateBitmapFromRenderTargetDirect3DSurface_CreatesRenderTarget)
    {
        auto surface = CreateSurface(D3D11_BIND_RENDER_TARGET);
        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        AssertTypeName<CanvasRenderTarget>(bitmap);
    }

    TEST_METHOD(CreateRenderTargetFromDirect3DSurface_RoundTripsDxgiSurface)
    {
        auto surface = CreateSurface(D3D11_BIND_RENDER_TARGET);
        auto renderTarget = CanvasRenderTarget::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        auto expectedDxgiSurface = GetDXGIInterface<IDXGISurface>(surface);
        auto actualDxgiSurface   = GetDXGIInterface<IDXGISurface>(renderTarget);
        Assert::AreEqual(expectedDxgiSurface.Get(), actualDxgiSurface.Get());
    }

    TEST_METHOD(CreateRenderTargetFromNonRenderTargetDirect3DSurface_Fails)
    {
        auto surface = CreateSurface(D3D11_BIND_SHADER_RESOURCE);

        Assert::ExpectException<Platform::InvalidArgumentException^>(
            [&]
            {
                CanvasRenderTarget::CreateFromDirect3D11Surface(m_canvasDevice, surface);
            });
    }

    TEST_METHOD(CreateRenderTargetFromRenderTargetDirect3DSurface_CreatesRenderTarget)
    {
        auto surface = CreateSurface(D3D11_BIND_RENDER_TARGET);
        auto renderTarget = CanvasRenderTarget::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        AssertTypeName<CanvasRenderTarget>(renderTarget);
    }

    TEST_METHOD(CreateBitmapFromDirect3DSurface_Subresource)
    {
        //
        // Verify bitmaps and render targets are createable off of subresourced surfaces.
        //
        // Mip slices are used for this test rather than an arrayed texture
        // because they are supported on a wider range of hardware.
        //
        ComPtr<ID3D11Device> d3dDevice;
        ThrowIfFailed(GetDXGIInterface<ID3D11Device>(m_canvasDevice, &d3dDevice));

        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Width = 8;
        textureDesc.Height = 8;
        textureDesc.MipLevels = 3;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDesc.CPUAccessFlags = 0;

        ComPtr<ID3D11Texture2D> texture2D;
        ThrowIfFailed(d3dDevice->CreateTexture2D(
            &textureDesc,
            nullptr,
            &texture2D));

        ComPtr<IDXGIResource1> resource;
        ThrowIfFailed(texture2D.As(&resource));

        ComPtr<IDXGISurface2> surface;
        ThrowIfFailed(resource->CreateSubresourceSurface(2, &surface));

        auto wrappedSurface = CreateDirect3DSurface(surface.Get());

        auto renderTarget = CanvasRenderTarget::CreateFromDirect3D11Surface(m_canvasDevice, wrappedSurface);

        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, wrappedSurface);

        // Verify each resource size reflects the slice size, not the whole texture.
        Assert::AreEqual(2u, renderTarget->SizeInPixels.Width);
        Assert::AreEqual(2u, renderTarget->SizeInPixels.Height);
        Assert::AreEqual(2u, bitmap->SizeInPixels.Width);
        Assert::AreEqual(2u, bitmap->SizeInPixels.Height);
    }

    TEST_METHOD(CreateRenderTargetFromDirect3DSurface_AlphaAndDpiAreSet)
    {
        auto surface = CreateSurface(D3D11_BIND_RENDER_TARGET);
        auto renderTarget = CanvasRenderTarget::CreateFromDirect3D11Surface(m_canvasDevice, surface, CanvasAlphaMode::Ignore, 75.0f);
        auto d2dBitmap = GetWrappedResource<ID2D1Bitmap1>(renderTarget);

        VerifyDpiAndAlpha(d2dBitmap, 75.0f, D2D1_ALPHA_MODE_IGNORE);
    }

    TEST_METHOD(CanvasBitmapDescriptionMatchesDxgiSurfaceDescription)
    {
        auto surface = CreateSurface(static_cast<D3D11_BIND_FLAG>(D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET));
        auto bitmap = CanvasBitmap::CreateFromDirect3D11Surface(m_canvasDevice, surface);

        auto canvasDescription = bitmap->Description;

        auto dxgiSurface = GetDXGIInterface<IDXGISurface>(bitmap);
        DXGI_SURFACE_DESC dxgiDesc{};
        ThrowIfFailed(dxgiSurface->GetDesc(&dxgiDesc));

        Assert::AreEqual<uint32_t>(dxgiDesc.Width, canvasDescription.Width);
        Assert::AreEqual<uint32_t>(dxgiDesc.Height, canvasDescription.Height);
        Assert::AreEqual<uint32_t>(dxgiDesc.Format, static_cast<uint32_t>(canvasDescription.Format));
        Assert::AreEqual<uint32_t>(dxgiDesc.SampleDesc.Count, canvasDescription.MultisampleDescription.Count);
        Assert::AreEqual<uint32_t>(dxgiDesc.SampleDesc.Quality, canvasDescription.MultisampleDescription.Quality);
    }

    IDirect3DSurface^ CreateSurface(D3D11_BIND_FLAG bindFlags)
    {
        //
        // We create a new DXGI surface by creating Texture2D's from the same
        // D3D device that CanvasDevice is bound to.  This way we know that D2D
        // and Win2D will have no prior-knowledge of the surface we create.
        //
        ComPtr<ID3D11Device> d3dDevice;
        ThrowIfFailed(GetDXGIInterface<ID3D11Device>(m_canvasDevice, &d3dDevice));

        D3D11_TEXTURE2D_DESC textureDesc{};
        textureDesc.Width = 1;
        textureDesc.Height = 1;
        textureDesc.MipLevels = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = bindFlags;
        textureDesc.CPUAccessFlags = 0;

        ComPtr<ID3D11Texture2D> texture2D;
        ThrowIfFailed(d3dDevice->CreateTexture2D(
            &textureDesc,
            nullptr,
            &texture2D));

        return CreateDirect3DSurface(As<IDXGISurface>(texture2D).Get());
    }
};
