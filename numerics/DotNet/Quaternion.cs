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

using System.Globalization;

using SM = System.Math;

namespace System.Numerics
{
    public struct Quaternion : IEquatable<Quaternion>
    {
        public float X;
        public float Y;
        public float Z;
        public float W;


        public static Quaternion Identity
        {
            get { return new Quaternion(0, 0, 0, 1); }
        }


        public bool IsIdentity
        {
            get { return X == 0f && Y == 0f && Z == 0f && W == 1f; }
        }


        public Quaternion(float x, float y, float z, float w)
        {
            X = x; 
            Y = y; 
            Z = z; 
            W = w;
        }


        public Quaternion(Vector3 vectorPart, float scalarPart)
        {
            X = vectorPart.X; 
            Y = vectorPart.Y; 
            Z = vectorPart.Z; 
            W = scalarPart;
        }


        public float Length()
        {
            float ls = X * X + Y * Y + Z * Z + W * W;

            return (float)SM.Sqrt((double)ls);
        }


        public float LengthSquared()
        {
            return X * X + Y * Y + Z * Z + W * W;
        }


        public static Quaternion Normalize(Quaternion value)
        {
            Quaternion ans;

            float ls = value.X * value.X + value.Y * value.Y + value.Z * value.Z + value.W * value.W;
            
            float invNorm = 1.0f / (float)SM.Sqrt((double)ls);

            ans.X = value.X * invNorm;
            ans.Y = value.Y * invNorm;
            ans.Z = value.Z * invNorm;
            ans.W = value.W * invNorm;

            return ans;
        }


        public static Quaternion Conjugate(Quaternion value)
        {
            Quaternion ans;
            
            ans.X = -value.X;
            ans.Y = -value.Y;
            ans.Z = -value.Z;
            ans.W = value.W;

            return ans;
        }


        public static Quaternion Inverse(Quaternion value)
        {
            //  -1   (       a              -v       )
            // q   = ( -------------   ------------- )
            //       (  a^2 + |v|^2  ,  a^2 + |v|^2  )

            Quaternion ans;
            
            float ls = value.X * value.X + value.Y * value.Y + value.Z * value.Z + value.W * value.W;
            float invNorm = 1.0f / ls;

            ans.X = -value.X * invNorm;
            ans.Y = -value.Y * invNorm;
            ans.Z = -value.Z * invNorm;
            ans.W = value.W * invNorm;

            return ans;
        }
        
        
        public static Quaternion CreateFromAxisAngle(Vector3 axis, float angle)
        {
            Quaternion ans;

            float halfAngle = angle * 0.5f;
            float s = (float)SM.Sin(halfAngle);
            float c = (float)SM.Cos(halfAngle);

            ans.X = axis.X * s;
            ans.Y = axis.Y * s;
            ans.Z = axis.Z * s;
            ans.W = c;

            return ans;
        }


        public static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
        {
            //  Roll first, about axis the object is facing, then
            //  pitch upward, then yaw to face into the new heading
            float sr, cr, sp, cp, sy, cy;

            float halfRoll = roll * 0.5f;
            sr = (float)SM.Sin(halfRoll); cr = (float)SM.Cos(halfRoll);

            float halfPitch = pitch * 0.5f;
            sp = (float)SM.Sin(halfPitch); cp = (float)SM.Cos(halfPitch);

            float halfYaw = yaw * 0.5f;
            sy = (float)SM.Sin(halfYaw); cy = (float)SM.Cos(halfYaw);

            Quaternion result;
            
            result.X = cy * sp * cr + sy * cp * sr;
            result.Y = sy * cp * cr - cy * sp * sr;
            result.Z = cy * cp * sr - sy * sp * cr;
            result.W = cy * cp * cr + sy * sp * sr;

            return result;
        }


        public static Quaternion CreateFromRotationMatrix(Matrix4x4 matrix)
        {
            float trace = matrix.M11 + matrix.M22 + matrix.M33;
            
            Quaternion q = new Quaternion();
            
            if (trace > 0.0f)
            {
                float s = (float)SM.Sqrt(trace + 1.0f);
                q.W = s * 0.5f;
                s = 0.5f / s;
                q.X = (matrix.M23 - matrix.M32) * s;
                q.Y = (matrix.M31 - matrix.M13) * s;
                q.Z = (matrix.M12 - matrix.M21) * s;
            }
            else
            {
                if (matrix.M11 >= matrix.M22 && matrix.M11 >= matrix.M33)
                {
                    float s = (float)SM.Sqrt(1.0f + matrix.M11 - matrix.M22 - matrix.M33);
                    float invS = 0.5f / s;
                    q.X = 0.5f * s;
                    q.Y = (matrix.M12 + matrix.M21) * invS;
                    q.Z = (matrix.M13 + matrix.M31) * invS;
                    q.W = (matrix.M23 - matrix.M32) * invS;
                }
                else if (matrix.M22 > matrix.M33)
                {
                    float s = (float)SM.Sqrt(1.0f + matrix.M22 - matrix.M11 - matrix.M33);
                    float invS = 0.5f / s;
                    q.X = (matrix.M21 + matrix.M12) * invS;
                    q.Y = 0.5f * s;
                    q.Z = (matrix.M32 + matrix.M23) * invS;
                    q.W = (matrix.M31 - matrix.M13) * invS;
                }
                else
                {
                    float s = (float)SM.Sqrt(1.0f + matrix.M33 - matrix.M11 - matrix.M22);
                    float invS = 0.5f / s;
                    q.X = (matrix.M31 + matrix.M13) * invS;
                    q.Y = (matrix.M32 + matrix.M23) * invS;
                    q.Z = 0.5f * s;
                    q.W = (matrix.M12 - matrix.M21) * invS;
                }
            }

            return q;
        }


        public static float Dot(Quaternion quaternion1, Quaternion quaternion2)
        {
            return quaternion1.X * quaternion2.X + 
                   quaternion1.Y * quaternion2.Y + 
                   quaternion1.Z * quaternion2.Z + 
                   quaternion1.W * quaternion2.W;
        }


        public static Quaternion Slerp(Quaternion quaternion1, Quaternion quaternion2, float amount)
        {
            const float epsilon = 1e-6f;

            float t = amount;
            
            float cosOmega = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y +
                             quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;

            bool flip = false;
            
            if (cosOmega < 0.0f)
            {
                flip = true;
                cosOmega = -cosOmega;
            }

            float s1, s2;
            
            if (cosOmega > (1.0f - epsilon))
            {
                // Too close, do straight linear interpolation.
                s1 = 1.0f - t;
                s2 = (flip) ? -t : t;
            }
            else
            {
                float omega = (float)SM.Acos(cosOmega);
                float invSinOmega = (float)(1 / SM.Sin(omega));

                s1 = (float)SM.Sin((1.0f - t) * omega) * invSinOmega;
                s2 = (flip)
                    ? (float)-SM.Sin(t * omega) * invSinOmega
                    : (float)SM.Sin(t * omega) * invSinOmega;
            }

            Quaternion ans;

            ans.X = s1 * quaternion1.X + s2 * quaternion2.X;
            ans.Y = s1 * quaternion1.Y + s2 * quaternion2.Y;
            ans.Z = s1 * quaternion1.Z + s2 * quaternion2.Z;
            ans.W = s1 * quaternion1.W + s2 * quaternion2.W;

            return ans;
        }


        public static Quaternion Lerp(Quaternion quaternion1, Quaternion quaternion2, float amount)
        {
            float t = amount;
            float t1 = 1.0f - t;

            Quaternion r = new Quaternion();

            float dot = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y +
                        quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;

            if (dot >= 0.0f)
            {
                r.X = t1 * quaternion1.X + t * quaternion2.X;
                r.Y = t1 * quaternion1.Y + t * quaternion2.Y;
                r.Z = t1 * quaternion1.Z + t * quaternion2.Z;
                r.W = t1 * quaternion1.W + t * quaternion2.W;
            }
            else
            {
                r.X = t1 * quaternion1.X - t * quaternion2.X;
                r.Y = t1 * quaternion1.Y - t * quaternion2.Y;
                r.Z = t1 * quaternion1.Z - t * quaternion2.Z;
                r.W = t1 * quaternion1.W - t * quaternion2.W;
            }

            // Normalize it.
            float ls = r.X * r.X + r.Y * r.Y + r.Z * r.Z + r.W * r.W;
            float invNorm = 1.0f / (float)SM.Sqrt((double)ls);

            r.X *= invNorm;
            r.Y *= invNorm;
            r.Z *= invNorm;
            r.W *= invNorm;

            return r;
        }


        public static Quaternion Concatenate(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            // Concatenate rotation is actually q2 * q1 instead of q1 * q2.
            // So that's why value2 goes q1 and value1 goes q2.
            float q1x = value2.X;
            float q1y = value2.Y;
            float q1z = value2.Z;
            float q1w = value2.W;

            float q2x = value1.X;
            float q2y = value1.Y;
            float q2z = value1.Z;
            float q2w = value1.W;

            // cross(av, bv)
            float cx = q1y * q2z - q1z * q2y;
            float cy = q1z * q2x - q1x * q2z;
            float cz = q1x * q2y - q1y * q2x;

            float dot = q1x * q2x + q1y * q2y + q1z * q2z;

            ans.X = q1x * q2w + q2x * q1w + cx;
            ans.Y = q1y * q2w + q2y * q1w + cy;
            ans.Z = q1z * q2w + q2z * q1w + cz;
            ans.W = q1w * q2w - dot;

            return ans;
        }


        public static Quaternion Negate(Quaternion value)
        {
            Quaternion ans;

            ans.X = -value.X;
            ans.Y = -value.Y;
            ans.Z = -value.Z;
            ans.W = -value.W;

            return ans;
        }


        public static Quaternion Add(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            ans.X = value1.X + value2.X;
            ans.Y = value1.Y + value2.Y;
            ans.Z = value1.Z + value2.Z;
            ans.W = value1.W + value2.W;

            return ans;
        }


        public static Quaternion Subtract(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            ans.X = value1.X - value2.X;
            ans.Y = value1.Y - value2.Y;
            ans.Z = value1.Z - value2.Z;
            ans.W = value1.W - value2.W;

            return ans;
        }


        public static Quaternion Multiply(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            float q1x = value1.X;
            float q1y = value1.Y;
            float q1z = value1.Z;
            float q1w = value1.W;

            float q2x = value2.X;
            float q2y = value2.Y;
            float q2z = value2.Z;
            float q2w = value2.W;

            // cross(av, bv)
            float cx = q1y * q2z - q1z * q2y;
            float cy = q1z * q2x - q1x * q2z;
            float cz = q1x * q2y - q1y * q2x;

            float dot = q1x * q2x + q1y * q2y + q1z * q2z;

            ans.X = q1x * q2w + q2x * q1w + cx;
            ans.Y = q1y * q2w + q2y * q1w + cy;
            ans.Z = q1z * q2w + q2z * q1w + cz;
            ans.W = q1w * q2w - dot;

            return ans;
        }


        public static Quaternion Multiply(Quaternion value1, float value2)
        {
            Quaternion ans;

            ans.X = value1.X * value2;
            ans.Y = value1.Y * value2;
            ans.Z = value1.Z * value2;
            ans.W = value1.W * value2;

            return ans;
        }


        public static Quaternion Divide(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            float q1x = value1.X;
            float q1y = value1.Y;
            float q1z = value1.Z;
            float q1w = value1.W;

            //-------------------------------------
            // Inverse part.
            float ls = value2.X * value2.X + value2.Y * value2.Y +
                       value2.Z * value2.Z + value2.W * value2.W;
            float invNorm = 1.0f / ls;

            float q2x = -value2.X * invNorm;
            float q2y = -value2.Y * invNorm;
            float q2z = -value2.Z * invNorm;
            float q2w = value2.W * invNorm;

            //-------------------------------------
            // Multiply part.

            // cross(av, bv)
            float cx = q1y * q2z - q1z * q2y;
            float cy = q1z * q2x - q1x * q2z;
            float cz = q1x * q2y - q1y * q2x;

            float dot = q1x * q2x + q1y * q2y + q1z * q2z;

            ans.X = q1x * q2w + q2x * q1w + cx;
            ans.Y = q1y * q2w + q2y * q1w + cy;
            ans.Z = q1z * q2w + q2z * q1w + cz;
            ans.W = q1w * q2w - dot;

            return ans;
        }


        public static Quaternion operator -(Quaternion value)
        {
            Quaternion ans;

            ans.X = -value.X;
            ans.Y = -value.Y;
            ans.Z = -value.Z;
            ans.W = -value.W;

            return ans;
        }


        public static Quaternion operator +(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            ans.X = value1.X + value2.X;
            ans.Y = value1.Y + value2.Y;
            ans.Z = value1.Z + value2.Z;
            ans.W = value1.W + value2.W;

            return ans;
        }


        public static Quaternion operator -(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            ans.X = value1.X - value2.X;
            ans.Y = value1.Y - value2.Y;
            ans.Z = value1.Z - value2.Z;
            ans.W = value1.W - value2.W;

            return ans;
        }


        public static Quaternion operator *(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            float q1x = value1.X;
            float q1y = value1.Y;
            float q1z = value1.Z;
            float q1w = value1.W;

            float q2x = value2.X;
            float q2y = value2.Y;
            float q2z = value2.Z;
            float q2w = value2.W;

            // cross(av, bv)
            float cx = q1y * q2z - q1z * q2y;
            float cy = q1z * q2x - q1x * q2z;
            float cz = q1x * q2y - q1y * q2x;

            float dot = q1x * q2x + q1y * q2y + q1z * q2z;

            ans.X = q1x * q2w + q2x * q1w + cx;
            ans.Y = q1y * q2w + q2y * q1w + cy;
            ans.Z = q1z * q2w + q2z * q1w + cz;
            ans.W = q1w * q2w - dot;

            return ans;
        }


        public static Quaternion operator *(Quaternion value1, float value2)
        {
            Quaternion ans;

            ans.X = value1.X * value2;
            ans.Y = value1.Y * value2;
            ans.Z = value1.Z * value2;
            ans.W = value1.W * value2;

            return ans;
        }


        public static Quaternion operator /(Quaternion value1, Quaternion value2)
        {
            Quaternion ans;

            float q1x = value1.X;
            float q1y = value1.Y;
            float q1z = value1.Z;
            float q1w = value1.W;

            //-------------------------------------
            // Inverse part.
            float ls = value2.X * value2.X + value2.Y * value2.Y +
                       value2.Z * value2.Z + value2.W * value2.W;
            float invNorm = 1.0f / ls;

            float q2x = -value2.X * invNorm;
            float q2y = -value2.Y * invNorm;
            float q2z = -value2.Z * invNorm;
            float q2w = value2.W * invNorm;

            //-------------------------------------
            // Multiply part.

            // cross(av, bv)
            float cx = q1y * q2z - q1z * q2y;
            float cy = q1z * q2x - q1x * q2z;
            float cz = q1x * q2y - q1y * q2x;

            float dot = q1x * q2x + q1y * q2y + q1z * q2z;

            ans.X = q1x * q2w + q2x * q1w + cx;
            ans.Y = q1y * q2w + q2y * q1w + cy;
            ans.Z = q1z * q2w + q2z * q1w + cz;
            ans.W = q1w * q2w - dot;

            return ans;
        }


        public static bool operator ==(Quaternion value1, Quaternion value2)
        {
            return (value1.X == value2.X &&
                    value1.Y == value2.Y &&
                    value1.Z == value2.Z &&
                    value1.W == value2.W);
        }


        public static bool operator !=(Quaternion value1, Quaternion value2)
        {
            return (value1.X != value2.X ||
                    value1.Y != value2.Y ||
                    value1.Z != value2.Z ||
                    value1.W != value2.W);
        }


        public bool Equals(Quaternion other)
        {
            return (X == other.X &&
                    Y == other.Y &&
                    Z == other.Z &&
                    W == other.W);
        }


        public override bool Equals(object obj)
        {
            if (obj is Quaternion)
            {
                return Equals((Quaternion)obj);
            }

            return false;
        }


        public override string ToString()
        {
            CultureInfo ci = CultureInfo.CurrentCulture;

            return String.Format(ci, "{{X:{0} Y:{1} Z:{2} W:{3}}}", X.ToString(ci), Y.ToString(ci), Z.ToString(ci), W.ToString(ci));
        }


        public override int GetHashCode()
        {
            return X.GetHashCode() + Y.GetHashCode() + Z.GetHashCode() + W.GetHashCode();
        }


#if INCLUDE_WINRT_CANVAS_INTEROP

        public static unsafe implicit operator Microsoft.Graphics.Canvas.Numerics.Quaternion(Quaternion value)
        {
            return *(Microsoft.Graphics.Canvas.Numerics.Quaternion*)&value;
        }


        public static unsafe implicit operator Quaternion(Microsoft.Graphics.Canvas.Numerics.Quaternion value)
        {
            return *(Quaternion*)&value;
        }

#endif
    }
}
