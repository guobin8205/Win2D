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
    using namespace ABI::Windows::Foundation;
    using namespace ABI::Windows::UI;

    class MockCanvasDrawingSession : public RuntimeClass<
        RuntimeClassFlags<WinRtClassicComMix>,
        ICanvasDrawingSession,
        IClosable,
        CloakedIid<ICanvasResourceWrapperNative>>
    {
    public:
        CALL_COUNTER_WITH_MOCK(CloseMethod, HRESULT());

        MockCanvasDrawingSession()
        {
            CloseMethod.AllowAnyCall();
        }

        IFACEMETHODIMP Close() override
        {
            return CloseMethod.WasCalled();
        }

#define DONT_EXPECT(name, ...)                                  \
        IFACEMETHODIMP name(__VA_ARGS__) override               \
        {                                                       \
            Assert::Fail(L"Unexpected call to " WIDEN(#name));  \
            return E_NOTIMPL;                                   \
        }

        DONT_EXPECT(Clear , Color);

        DONT_EXPECT(DrawImage                                                                   , ICanvasImage*, Vector2);
        DONT_EXPECT(DrawImageAtCoords                                                           , ICanvasImage*, float, float);
        DONT_EXPECT(DrawImageAtOrigin                                                           , ICanvasImage*);
        DONT_EXPECT(DrawImageWithSourceRect                                                     , ICanvasImage*, Vector2, Rect);
        DONT_EXPECT(DrawImageWithSourceRectAndInterpolation                                     , ICanvasImage*, Vector2, Rect, CanvasImageInterpolation);
        DONT_EXPECT(DrawImageWithSourceRectAndInterpolationAndComposite                         , ICanvasImage*, Vector2, Rect, CanvasImageInterpolation, CanvasComposite);
        DONT_EXPECT(DrawImageAtCoordsWithSourceRect                                             , ICanvasImage*, float, float, Rect);
        DONT_EXPECT(DrawImageAtCoordsWithSourceRectAndInterpolation                             , ICanvasImage*, float, float, Rect, CanvasImageInterpolation);
        DONT_EXPECT(DrawImageAtCoordsWithSourceRectAndInterpolationAndComposite                 , ICanvasImage*, float, float, Rect, CanvasImageInterpolation, CanvasComposite);
        DONT_EXPECT(DrawBitmapWithDestRect                                                      , ICanvasBitmap*, Rect);
        DONT_EXPECT(DrawBitmapWithDestRectAndSourceRect                                         , ICanvasBitmap*, Rect, Rect);
        DONT_EXPECT(DrawBitmapWithDestRectAndSourceRectAndOpacity                               , ICanvasBitmap*, Rect, Rect, float);
        DONT_EXPECT(DrawBitmapWithDestRectAndSourceRectAndOpacityAndInterpolation               , ICanvasBitmap*, Rect, Rect, float, CanvasImageInterpolation);
        DONT_EXPECT(DrawBitmapWithDestRectAndSourceRectAndOpacityAndInterpolationAndPerspective , ICanvasBitmap*, Rect, Rect, float, CanvasImageInterpolation, Matrix4x4);

        DONT_EXPECT(DrawLineWithBrush                                     , Vector2, Vector2, ICanvasBrush*);
        DONT_EXPECT(DrawLineAtCoordsWithBrush                             , float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawLineWithColor                                     , Vector2, Vector2, Color);
        DONT_EXPECT(DrawLineAtCoordsWithColor                             , float, float, float, float, Color);
        DONT_EXPECT(DrawLineWithBrushAndStrokeWidth                       , Vector2, Vector2, ICanvasBrush*, float);
        DONT_EXPECT(DrawLineAtCoordsWithBrushAndStrokeWidth               , float, float, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawLineWithColorAndStrokeWidth                       , Vector2, Vector2, Color, float);
        DONT_EXPECT(DrawLineAtCoordsWithColorAndStrokeWidth               , float, float, float, float, Color, float);
        DONT_EXPECT(DrawLineWithBrushAndStrokeWidthAndStrokeStyle         , Vector2, Vector2, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawLineAtCoordsWithBrushAndStrokeWidthAndStrokeStyle , float, float, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawLineWithColorAndStrokeWidthAndStrokeStyle         , Vector2, Vector2, Color, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawLineAtCoordsWithColorAndStrokeWidthAndStrokeStyle , float, float, float, float, Color, float, ICanvasStrokeStyle*);

        DONT_EXPECT(DrawRectangleWithBrush                                     , Rect, ICanvasBrush*);
        DONT_EXPECT(DrawRectangleAtCoordsWithBrush                             , float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawRectangleWithColor                                     , Rect, Color);
        DONT_EXPECT(DrawRectangleAtCoordsWithColor                             , float, float, float, float, Color);
        DONT_EXPECT(DrawRectangleWithBrushAndStrokeWidth                       , Rect, ICanvasBrush*, float);
        DONT_EXPECT(DrawRectangleAtCoordsWithBrushAndStrokeWidth               , float, float, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawRectangleWithColorAndStrokeWidth                       , Rect, Color, float);
        DONT_EXPECT(DrawRectangleAtCoordsWithColorAndStrokeWidth               , float, float, float, float, Color, float);
        DONT_EXPECT(DrawRectangleWithBrushAndStrokeWidthAndStrokeStyle         , Rect, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRectangleAtCoordsWithBrushAndStrokeWidthAndStrokeStyle , float, float, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRectangleWithColorAndStrokeWidthAndStrokeStyle         , Rect, Color, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRectangleAtCoordsWithColorAndStrokeWidthAndStrokeStyle , float, float, float, float, Color, float, ICanvasStrokeStyle*);

        DONT_EXPECT(FillRectangleWithBrush         , Rect, ICanvasBrush*);
        DONT_EXPECT(FillRectangleAtCoordsWithBrush , float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(FillRectangleWithColor         , Rect, Color);
        DONT_EXPECT(FillRectangleAtCoordsWithColor , float, float, float, float, Color);

        DONT_EXPECT(DrawRoundedRectangleWithBrush                                     , Rect, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithBrush                             , float, float, float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawRoundedRectangleWithColor                                     , Rect, float, float, Color);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithColor                             , float, float, float, float, float, float, Color);
        DONT_EXPECT(DrawRoundedRectangleWithBrushAndStrokeWidth                       , Rect, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithBrushAndStrokeWidth               , float, float, float, float, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawRoundedRectangleWithColorAndStrokeWidth                       , Rect, float, float, Color, float);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithColorAndStrokeWidth               , float, float, float, float, float, float, Color, float);
        DONT_EXPECT(DrawRoundedRectangleWithBrushAndStrokeWidthAndStrokeStyle         , Rect, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithBrushAndStrokeWidthAndStrokeStyle , float, float, float, float, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRoundedRectangleWithColorAndStrokeWidthAndStrokeStyle         , Rect, float, float, Color, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawRoundedRectangleAtCoordsWithColorAndStrokeWidthAndStrokeStyle , float, float, float, float, float, float, Color, float, ICanvasStrokeStyle*);
        
        DONT_EXPECT(FillRoundedRectangleWithBrush         , Rect, float, float, ICanvasBrush*);
        DONT_EXPECT(FillRoundedRectangleAtCoordsWithBrush , float, float, float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(FillRoundedRectangleWithColor         , Rect, float, float, Color);
        DONT_EXPECT(FillRoundedRectangleAtCoordsWithColor , float, float, float, float, float, float, Color);
        
        DONT_EXPECT(DrawEllipseWithBrush                                     , Vector2, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawEllipseAtCoordsWithBrush                             , float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawEllipseWithColor                                     , Vector2, float, float, Color);
        DONT_EXPECT(DrawEllipseAtCoordsWithColor                             , float, float, float, float, Color);
        DONT_EXPECT(DrawEllipseWithBrushAndStrokeWidth                       , Vector2, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawEllipseAtCoordsWithBrushAndStrokeWidth               , float, float, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawEllipseWithColorAndStrokeWidth                       , Vector2, float, float, Color, float);
        DONT_EXPECT(DrawEllipseAtCoordsWithColorAndStrokeWidth               , float, float, float, float, Color, float);
        DONT_EXPECT(DrawEllipseWithBrushAndStrokeWidthAndStrokeStyle         , Vector2, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawEllipseAtCoordsWithBrushAndStrokeWidthAndStrokeStyle , float, float, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawEllipseWithColorAndStrokeWidthAndStrokeStyle         , Vector2, float, float, Color, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawEllipseAtCoordsWithColorAndStrokeWidthAndStrokeStyle , float, float, float, float, Color, float, ICanvasStrokeStyle*);

        DONT_EXPECT(FillEllipseWithBrush         , Vector2, float, float, ICanvasBrush*);
        DONT_EXPECT(FillEllipseAtCoordsWithBrush , float, float, float, float, ICanvasBrush*);
        DONT_EXPECT(FillEllipseWithColor         , Vector2, float, float, Color);
        DONT_EXPECT(FillEllipseAtCoordsWithColor , float, float, float, float, Color);
        
        DONT_EXPECT(DrawCircleWithBrush                                     , Vector2, float, ICanvasBrush*);
        DONT_EXPECT(DrawCircleAtCoordsWithBrush                             , float, float, float, ICanvasBrush*);
        DONT_EXPECT(DrawCircleWithColor                                     , Vector2, float, Color);
        DONT_EXPECT(DrawCircleAtCoordsWithColor                             , float, float, float, Color);
        DONT_EXPECT(DrawCircleWithBrushAndStrokeWidth                       , Vector2, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawCircleAtCoordsWithBrushAndStrokeWidth               , float, float, float, ICanvasBrush*, float);
        DONT_EXPECT(DrawCircleWithColorAndStrokeWidth                       , Vector2, float, Color, float);
        DONT_EXPECT(DrawCircleAtCoordsWithColorAndStrokeWidth               , float, float, float, Color, float);
        DONT_EXPECT(DrawCircleWithBrushAndStrokeWidthAndStrokeStyle         , Vector2, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawCircleAtCoordsWithBrushAndStrokeWidthAndStrokeStyle , float, float, float, ICanvasBrush*, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawCircleWithColorAndStrokeWidthAndStrokeStyle         , Vector2, float, Color, float, ICanvasStrokeStyle*);
        DONT_EXPECT(DrawCircleAtCoordsWithColorAndStrokeWidthAndStrokeStyle , float, float, float, Color, float, ICanvasStrokeStyle*);

        DONT_EXPECT(FillCircleWithBrush         , Vector2, float, ICanvasBrush*);
        DONT_EXPECT(FillCircleAtCoordsWithBrush , float, float, float, ICanvasBrush*);
        DONT_EXPECT(FillCircleWithColor         , Vector2, float, Color);
        DONT_EXPECT(FillCircleAtCoordsWithColor , float, float, float, Color);

        DONT_EXPECT(DrawTextAtPointWithColor                , HSTRING, Vector2, Color);
        DONT_EXPECT(DrawTextAtPointCoordsWithColor          , HSTRING, float, float, Color);
        DONT_EXPECT(DrawTextAtPointWithBrushAndFormat       , HSTRING, Vector2, ICanvasBrush*, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtRectWithBrushAndFormat        , HSTRING, Rect, ICanvasBrush*, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtPointCoordsWithBrushAndFormat , HSTRING, float, float, ICanvasBrush*, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtRectCoordsWithBrushAndFormat  , HSTRING, float, float, float, float, ICanvasBrush*, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtPointWithColorAndFormat       , HSTRING, Vector2, Color, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtRectWithColorAndFormat        , HSTRING, Rect, Color, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtPointCoordsWithColorAndFormat , HSTRING, float, float, Color, ICanvasTextFormat*);
        DONT_EXPECT(DrawTextAtRectCoordsWithColorAndFormat  , HSTRING, float, float, float, float, Color, ICanvasTextFormat*);

        DONT_EXPECT(get_Antialiasing     , CanvasAntialiasing*);
        DONT_EXPECT(put_Antialiasing     , CanvasAntialiasing);
        DONT_EXPECT(get_Blend            , CanvasBlend*);
        DONT_EXPECT(put_Blend            , CanvasBlend);
        DONT_EXPECT(get_TextAntialiasing , CanvasTextAntialiasing*);
        DONT_EXPECT(put_TextAntialiasing , CanvasTextAntialiasing);
        DONT_EXPECT(get_Transform        , ABI::Microsoft::Graphics::Canvas::Numerics::Matrix3x2*);
        DONT_EXPECT(put_Transform        , ABI::Microsoft::Graphics::Canvas::Numerics::Matrix3x2);
        DONT_EXPECT(get_Units            , CanvasUnits*);
        DONT_EXPECT(put_Units            , CanvasUnits);

        // ICanvasResourceWrapperNative
        DONT_EXPECT(GetResource, REFIID iid, void**);

#undef DONT_EXPECT

    };
}
