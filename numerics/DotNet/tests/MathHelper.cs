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

using System;
using System.Numerics;

namespace NumericsTests
{
    static class MathHelper
    {
        public const float Pi = (float)Math.PI;
        public const float PiOver2 = (float)Math.PI / 2f;
        public const float PiOver4 = (float)Math.PI / 4f;


        // Angle conversion helper.
        public static float ToRadians(float degrees)
        {
            return degrees * (float)Math.PI / 180f;
        }


        // Comparison helpers with small tolerance to allow for floating point rounding during computations.
        public static bool Equal(float a, float b)
        {
            return (Math.Abs(a - b) < 1e-5);
        }

        public static bool Equal(Vector2 a, Vector2 b)
        {
            return Equal(a.X, b.X) && Equal(a.Y, b.Y);
        }

        public static bool Equal(Vector3 a, Vector3 b)
        {
            return Equal(a.X, b.X) && Equal(a.Y, b.Y) && Equal(a.Z, b.Z);
        }

        public static bool Equal(Vector4 a, Vector4 b)
        {
            return Equal(a.X, b.X) && Equal(a.Y, b.Y) && Equal(a.Z, b.Z) && Equal(a.W, b.W);
        }

        public static bool Equal(Matrix4x4 a, Matrix4x4 b)
        {
            return
                Equal(a.M11, b.M11) && Equal(a.M12, b.M12) && Equal(a.M13, b.M13) && Equal(a.M14, b.M14) &&
                Equal(a.M21, b.M21) && Equal(a.M22, b.M22) && Equal(a.M23, b.M23) && Equal(a.M24, b.M24) &&
                Equal(a.M31, b.M31) && Equal(a.M32, b.M32) && Equal(a.M33, b.M33) && Equal(a.M34, b.M34) &&
                Equal(a.M41, b.M41) && Equal(a.M42, b.M42) && Equal(a.M43, b.M43) && Equal(a.M44, b.M44);
        }

        public static bool Equal(Matrix3x2 a, Matrix3x2 b)
        {
            return
                Equal(a.M11, b.M11) && Equal(a.M12, b.M12) &&
                Equal(a.M21, b.M21) && Equal(a.M22, b.M22) &&
                Equal(a.M31, b.M31) && Equal(a.M32, b.M32);
        }

        public static bool Equal(Plane a, Plane b)
        {
            return Equal(a.Normal, b.Normal) && Equal(a.D, b.D);
        }

        public static bool Equal(Quaternion a, Quaternion b)
        {
            return Equal(a.X, b.X) && Equal(a.Y, b.Y) && Equal(a.Z, b.Z) && Equal(a.W, b.W);
        }

        public static bool EqualRotation(Quaternion a, Quaternion b)
        {
            return Equal(a, b) || Equal(a, -b);
        }
    }
}
