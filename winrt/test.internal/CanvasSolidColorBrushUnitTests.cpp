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
#include "TestDeviceResourceCreationAdapter.h"
#include "CanvasControlTestAdapter.h"

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI;

struct SolidColorBrushCounters
{
    SolidColorBrushCounters()
        : NumSetColorCalls(0)
        , NumGetColorCalls(0)
        , NumSetOpacityCalls(0)
        , NumGetOpacityCalls(0)
        , NumSetTransformCalls(0)
        , NumGetTransformCalls(0)
    {}

    int NumSetColorCalls;
    int NumGetColorCalls;

    int NumSetOpacityCalls;
    int NumGetOpacityCalls;

    int NumSetTransformCalls;
    int NumGetTransformCalls;
};

class TestD2DSolidColorBrush : public MockD2DSolidColorBrush
{
public:
    TestD2DSolidColorBrush(D2D1_COLOR_F const& color, SolidColorBrushCounters* counters)
        : m_color(color)
        , m_opacity(1.0f)
        , m_counters(counters)
    {
        m_transform = D2D1::Matrix3x2F::Identity();
    }

    //
    // ID2D1SolidColorBrush
    //
    STDMETHOD_(void, SetColor)(_In_ CONST D2D1_COLOR_F *color) override
    {
        m_counters->NumSetColorCalls++;
        m_color = *color;
    }

    STDMETHOD_(D2D1_COLOR_F, GetColor)() CONST override
    {
        m_counters->NumGetColorCalls++;
        return m_color;
    }

    //
    // ID2D1Brush
    //
    STDMETHOD_(void, SetOpacity)(FLOAT opacity) override
    {
        m_counters->NumSetOpacityCalls++;
        m_opacity = opacity;
    }

    STDMETHOD_(FLOAT, GetOpacity)() CONST override
    {
        m_counters->NumGetOpacityCalls++;
        return m_opacity;
    }

    STDMETHOD_(void, SetTransform)(_In_ CONST D2D1_MATRIX_3X2_F *transform) override
    {
        m_counters->NumSetTransformCalls++;
        m_transform = *transform;
    }

    STDMETHOD_(void, GetTransform)(_Out_ D2D1_MATRIX_3X2_F *transform) CONST override
    {
        m_counters->NumGetTransformCalls++;
        *transform = m_transform;
    }

    D2D1_COLOR_F m_color;
    D2D1_MATRIX_3X2_F m_transform;
    float m_opacity;

    SolidColorBrushCounters* m_counters;
};

TEST_CLASS(CanvasSolidColorBrushTests)
{
public:
    std::shared_ptr<CanvasSolidColorBrushManager> m_colorBrushManager;

    class TestCanvasDevice : public StubCanvasDevice
    {
        SolidColorBrushCounters m_counters;
    public:
        TestCanvasDevice()
        {
        }

        virtual ComPtr<ID2D1SolidColorBrush> CreateSolidColorBrush(D2D1_COLOR_F const& color) override
        {
            return Make<TestD2DSolidColorBrush>(color, &m_counters);
        }
    };

    TEST_METHOD_INITIALIZE(Reset)
    {
        m_colorBrushManager = std::make_shared<CanvasSolidColorBrushManager>();
    } 

    TEST_METHOD_EX(CanvasSolidColorBrush_Construction)
    {
        auto canvasDevice = Make<TestCanvasDevice>();

        Color red = { 255, 255, 0, 0 };
        D2D1_COLOR_F d2dRed = D2D1::ColorF(1, 0, 0);

        auto canvasSolidColorBrush = m_colorBrushManager->Create(canvasDevice.Get(), red);
        auto d2dSolidColorBrush = canvasSolidColorBrush->GetResource();

        Assert::AreEqual(d2dRed, d2dSolidColorBrush->GetColor());
        Assert::AreEqual(1.0f, d2dSolidColorBrush->GetOpacity());
        
        D2D1_MATRIX_3X2_F actualTransform;
        d2dSolidColorBrush->GetTransform(&actualTransform);

        Assert::AreEqual<D2D1_MATRIX_3X2_F>(D2D1::Matrix3x2F::Identity(), actualTransform);
    }

    TEST_METHOD_EX(CanvasSolidColorBrush_Implements_Expected_Interfaces)
    {
        auto canvasDevice = Make<TestCanvasDevice>();
        auto brush = m_colorBrushManager->Create(canvasDevice.Get(), Color{ 255, 0, 0, 0 });

        ASSERT_IMPLEMENTS_INTERFACE(brush, ICanvasSolidColorBrush);
        ASSERT_IMPLEMENTS_INTERFACE(brush, ICanvasBrush);
        ASSERT_IMPLEMENTS_INTERFACE(brush, ICanvasBrushInternal);
        ASSERT_IMPLEMENTS_INTERFACE(brush, ABI::Windows::Foundation::IClosable);
        ASSERT_IMPLEMENTS_INTERFACE(brush, ICanvasResourceWrapperNative);
    }

    TEST_METHOD_EX(CanvasSolidColorBrush_Properties)
    {
        Color red = { 255, 255, 0, 0 };
        Color cyan = { 255, 0, 255, 255 };
        D2D1_COLOR_F d2dRed = D2D1::ColorF(1, 0, 0);
        D2D1_COLOR_F d2dCyan = D2D1::ColorF(0, 1, 1);

        SolidColorBrushCounters counters;
        auto testD2DSolidColorBrush = Make<TestD2DSolidColorBrush>(d2dRed, &counters);
        auto canvasDevice = Make<TestCanvasDevice>();

        auto canvasSolidColorBrush = m_colorBrushManager->GetOrCreate(canvasDevice.Get(), testD2DSolidColorBrush.Get());

        // Verify the brush resource got initialized correctly and didn't call
        // any unexpected methods.
        Assert::AreEqual(0, counters.NumGetColorCalls);
        Assert::AreEqual(0, counters.NumSetColorCalls);
        Assert::AreEqual(0, counters.NumGetOpacityCalls);
        Assert::AreEqual(0, counters.NumSetOpacityCalls);
        Assert::AreEqual(0, counters.NumGetTransformCalls);
        Assert::AreEqual(0, counters.NumSetTransformCalls);

        // Verify the Color getter returns the right thing.
        Color colorActual;
        ThrowIfFailed(canvasSolidColorBrush->get_Color(&colorActual));
        Assert::AreEqual(red, colorActual);
        Assert::AreEqual(1, counters.NumGetColorCalls);

        // Set the Color to a new value, and verify it is correct.
        ThrowIfFailed(canvasSolidColorBrush->put_Color(cyan));
        Assert::AreEqual(1, counters.NumSetColorCalls);
        Assert::AreEqual(d2dCyan, testD2DSolidColorBrush->m_color);

        // Verify the getter works once more.
        ThrowIfFailed(canvasSolidColorBrush->get_Color(&colorActual));
        Assert::AreEqual(cyan, colorActual);
        Assert::AreEqual(2, counters.NumGetColorCalls);

        // put_Opacity
        ThrowIfFailed(canvasSolidColorBrush->put_Opacity(0.75f));
        Assert::AreEqual(0.75f, testD2DSolidColorBrush->m_opacity);
        Assert::AreEqual(1, counters.NumSetOpacityCalls);

        // get_Opacity
        float opacityActual = 0;
        ThrowIfFailed(canvasSolidColorBrush->get_Opacity(&opacityActual));
        Assert::AreEqual(0.75f, opacityActual);
        Assert::AreEqual(1, counters.NumGetOpacityCalls);
        
        // put_Transform
        Numerics::Matrix3x2 scaleAndTranslate = { 2, 0, 0, 2, 5, 10 };
        D2D1_MATRIX_3X2_F d2dScaleAndTranslate = D2D1::Matrix3x2F(2, 0, 0, 2, 5, 10);
        canvasSolidColorBrush->put_Transform(scaleAndTranslate);
        Assert::AreEqual(d2dScaleAndTranslate, testD2DSolidColorBrush->m_transform);
        Assert::AreEqual(1, counters.NumSetTransformCalls);

        // get_Transform
        Numerics::Matrix3x2 transformActual;
        canvasSolidColorBrush->get_Transform(&transformActual);
        Assert::AreEqual(scaleAndTranslate, transformActual);
        Assert::AreEqual(1, counters.NumGetTransformCalls);

        // get_Device
        ComPtr<ICanvasDevice> actualDevice;
        canvasSolidColorBrush->get_Device(&actualDevice);
        Assert::AreEqual<ICanvasDevice*>(canvasDevice.Get(), actualDevice.Get());
    }

    TEST_METHOD_EX(CanvasSolidColorBrush_Closed)
    {
        Color color = { 255, 127, 127, 127 };

        auto d2dBrush = Make<MockD2DSolidColorBrush>();
        auto canvasDevice = Make<TestCanvasDevice>();
        auto canvasSolidColorBrush = m_colorBrushManager->GetOrCreate(canvasDevice.Get(), d2dBrush.Get());

        Assert::IsNotNull(canvasSolidColorBrush.Get());

        Assert::AreEqual(S_OK, canvasSolidColorBrush->Close());

        Color colorActual;
        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->get_Color(&colorActual));

        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->put_Color(color));

        float opacityActual;
        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->get_Opacity(&opacityActual));

        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->put_Opacity(1));

        Numerics::Matrix3x2 transformActual;
        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->get_Transform(&transformActual));

        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->put_Transform(transformActual));

        ComPtr<ICanvasDevice> actualDevice;
        Assert::AreEqual(RO_E_CLOSED, canvasSolidColorBrush->get_Device(&actualDevice));
    }

    TEST_METHOD_EX(CanvasSolidColorBrush_CreateThroughCanvasControl)
    {
        auto canvasDevice = Make<StubCanvasDevice>();

        bool createSolidColorBrushCalled = false;
        canvasDevice->MockCreateSolidColorBrush =
            [&](D2D1_COLOR_F const& color)
            {
                createSolidColorBrushCalled = true;
                return Make<MockD2DSolidColorBrush>();
            };

        auto canvasControlAdapter = std::make_shared<CanvasControlTestAdapter>();
        canvasControlAdapter->DeviceFactory->ActivateInstanceMethod.SetExpectedCalls(1,
            [=](IInspectable** value)
            {
                return canvasDevice.CopyTo(value);
            });
        canvasControlAdapter->CreateCanvasImageSourceMethod.AllowAnyCall();

        ComPtr<CanvasControl> canvasControl = Make<CanvasControl>(canvasControlAdapter);

        // Get the control to a point where it has created the device.
        auto userControl = dynamic_cast<StubUserControl*>(As<IUserControl>(canvasControl).Get());
        ThrowIfFailed(userControl->LoadedEventSource->InvokeAll(nullptr, nullptr));
        canvasControlAdapter->RaiseCompositionRenderingEvent();

        auto brush = m_colorBrushManager->Create(canvasControl.Get(), Color{ 255, 0, 0, 0 });

        // This will only be true if the brush was constructed on the correct device.
        Assert::IsTrue(createSolidColorBrushCalled);

        ComPtr<ICanvasDevice> verifyDevice;
        ThrowIfFailed(canvasControl->get_Device(&verifyDevice));
        Assert::AreEqual(static_cast<ICanvasDevice*>(canvasDevice.Get()), verifyDevice.Get());
    }

    TEST_METHOD_EX(CanvasSolidColorBrush_CreateThroughDrawingSession)
    {
        ComPtr<MockCanvasDevice> canvasDevice = Make<MockCanvasDevice>();
        ComPtr<MockD2DSolidColorBrush> expectedBrush = Make<MockD2DSolidColorBrush>();

        bool createSolidColorBrushCalled = false;
        canvasDevice->MockCreateSolidColorBrush =
            [&](D2D1_COLOR_F const& color)
            {
                createSolidColorBrushCalled = true;
                return expectedBrush;
            };

        ComPtr<StubD2DDeviceContextWithGetFactory> d2dDeviceContext =
            Make<StubD2DDeviceContextWithGetFactory>();

        auto manager = std::make_shared<CanvasDrawingSessionManager>();
        ComPtr<CanvasDrawingSession> drawingSession = manager->Create(
            canvasDevice.Get(),
            d2dDeviceContext.Get(),
            std::make_shared<StubCanvasDrawingSessionAdapter>());

        auto createdBrush = m_colorBrushManager->Create(drawingSession.Get(), Color{ 255, 0, 0, 0 });

        // Verify that the Create occurred on the DrawingSession device, and not on some
        // other device. Verify it wrapped around the correct brush.
        Assert::IsTrue(createSolidColorBrushCalled);
        ComPtr<ICanvasBrushInternal> createdBrushInternal;
        ThrowIfFailed(createdBrush.As(&createdBrushInternal));
        ComPtr<ID2D1Brush> createdD2DBrush = createdBrushInternal->GetD2DBrush(nullptr);
        Assert::AreEqual(static_cast<ID2D1Brush*>(expectedBrush.Get()), createdD2DBrush.Get());
    }
};
