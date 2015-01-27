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

#include "../WindowsNumerics.h"

#pragma warning(disable: 4505)  // "unreferenced local function"

#include <SDKDDKVer.h>
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define NUMERICS_TEST_CLASS(ClassName) TEST_CLASS(ClassName)
#define NUMERICS_TEST_CLASS_INNER(ClassName)

#define NUMERICS_ABI_NAMESPACE Microsoft::Graphics::Canvas::Numerics

#include "Helpers.h"
