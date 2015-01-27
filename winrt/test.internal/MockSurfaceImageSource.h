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

namespace canvas
{
    using namespace ABI::Windows::UI::Xaml::Media::Imaging;
    using namespace ABI::Windows::UI::Xaml::Media;

    class MockSurfaceImageSource : public RuntimeClass<
        RuntimeClassFlags<WinRtClassicComMix>,
        ISurfaceImageSource,
        IImageSource,
        ISurfaceImageSourceNativeWithD2D>
    {
    public:
        CALL_COUNTER_WITH_MOCK(SetDeviceMethod, HRESULT(IUnknown*));
        CALL_COUNTER_WITH_MOCK(BeginDrawMethod, HRESULT(RECT const&, IID const&, void**, POINT*));
        CALL_COUNTER(EndDrawMethod);

        // ISurfaceImageSourceNativeWithD2D
        IFACEMETHODIMP SetDevice(IUnknown* device) override 
        {
            return SetDeviceMethod.WasCalled(device);
        }

        IFACEMETHODIMP BeginDraw(RECT const& updateRect, IID const& iid, void** updateObject, POINT* offset) override 
        {
            return BeginDrawMethod.WasCalled(updateRect, iid, updateObject, offset);
        }

        IFACEMETHODIMP EndDraw() override 
        {
            EndDrawMethod.WasCalled();
            return S_OK;
        }

        IFACEMETHODIMP SuspendDraw() override 
        {
            Assert::Fail(L"Unexpected call to SuspendDraw");
            return E_NOTIMPL;
        }

        IFACEMETHODIMP ResumeDraw() override 
        {
            Assert::Fail(L"Unexpected call to ResumeDraw");
            return E_NOTIMPL;
        }
    };
}
