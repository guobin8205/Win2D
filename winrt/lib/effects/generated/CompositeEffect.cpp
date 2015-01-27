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
#include "CompositeEffect.h"

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas { namespace Effects
{
    CompositeEffect::CompositeEffect()
        : CanvasEffect(CLSID_D2D1Composite, 1, 0, false)
    {
        // Set default values
        SetProperty<uint32_t>(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_SOURCE_OVER);
    }

    IMPLEMENT_PROPERTY(CompositeEffect,
        Mode,
        uint32_t,
        CanvasComposite,
        D2D1_COMPOSITE_PROP_MODE)

    ActivatableClass(CompositeEffect);
}}}}}
