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
#include "HueRotationEffect.h"

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas { namespace Effects
{
    HueRotationEffect::HueRotationEffect()
        : CanvasEffect(CLSID_D2D1HueRotation, 1, 1, true)
    {
        // Set default values
        SetProperty<float>(D2D1_HUEROTATION_PROP_ANGLE, 0.0f);
    }

    IMPLEMENT_PROPERTY(HueRotationEffect,
        Angle,
        ConvertRadiansToDegrees,
        float,
        D2D1_HUEROTATION_PROP_ANGLE)

    IMPLEMENT_INPUT_PROPERTY(HueRotationEffect,
        Source,
        0)

    ActivatableClass(HueRotationEffect);
}}}}}
