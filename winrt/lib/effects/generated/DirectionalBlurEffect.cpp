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

// This file was automatically generated. Please do not edit it manually.

#include "pch.h"
#include "DirectionalBlurEffect.h"

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas { namespace Effects
{
    DirectionalBlurEffect::DirectionalBlurEffect()
        : CanvasEffect(CLSID_D2D1DirectionalBlur, 4, 1, true)
    {
        // Set default values
        SetProperty<float>(D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION, 3.0f);
        SetProperty<float>(D2D1_DIRECTIONALBLUR_PROP_ANGLE, 0.0f);
        SetProperty<uint32_t>(D2D1_DIRECTIONALBLUR_PROP_OPTIMIZATION, D2D1_DIRECTIONALBLUR_OPTIMIZATION_BALANCED);
        SetProperty<uint32_t>(D2D1_DIRECTIONALBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_SOFT);
    }

    IMPLEMENT_PROPERTY_WITH_VALIDATION(DirectionalBlurEffect,
        BlurAmount,
        float,
        float,
        D2D1_DIRECTIONALBLUR_PROP_STANDARD_DEVIATION,
        (value >= 0.0f) && (value <= 250.0f))

    IMPLEMENT_PROPERTY(DirectionalBlurEffect,
        Angle,
        ConvertRadiansToDegrees,
        float,
        D2D1_DIRECTIONALBLUR_PROP_ANGLE)

    IMPLEMENT_PROPERTY(DirectionalBlurEffect,
        Optimization,
        uint32_t,
        EffectOptimization,
        D2D1_DIRECTIONALBLUR_PROP_OPTIMIZATION)

    IMPLEMENT_PROPERTY(DirectionalBlurEffect,
        BorderMode,
        uint32_t,
        EffectBorderMode,
        D2D1_DIRECTIONALBLUR_PROP_BORDER_MODE)

    IMPLEMENT_INPUT_PROPERTY(DirectionalBlurEffect,
        Source,
        0)

    ActivatableClass(DirectionalBlurEffect);
}}}}}
