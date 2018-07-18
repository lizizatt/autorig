// (c)2013, Arthur van Hoff

#ifndef __common_types__
#define __common_types__

#include <half.hpp>

typedef struct {
    float mx[3][4];
} Transform;

typedef unsigned char uchar;
typedef uchar4 rgba;
typedef float3 rgb;

typedef uchar3 brgb;

typedef struct {
    half_float::half x;
    half_float::half y;
    half_float::half z;
} hrgb;

template<class T> class Buffer;
class Props;

//
// make types
//

__device__ __host__ inline rgba make_rgba(unsigned char r, unsigned char g, unsigned char b)
{
    return make_uchar4(r, g, b, 0xFF);
}
__device__ __host__ inline rgba make_rgba(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return make_uchar4(r, g, b, a);
}
__device__ __host__ inline rgb make_rgb(float r, float g, float b)
{
    return make_float3(r, g, b);
}
__device__ __host__ inline int2 make_int2(float2 p)
{
    return make_int2((int)p.x, (int)p.y);
}

__device__ __host__ inline rgb convert_brgb_rgb(brgb in)
{
    rgb out;
    out.x = (float)(in.x)/255.0;
    out.y = (float)(in.y)/255.0;
    out.z = (float)(in.z)/255.0;
    
    return out;
}

__device__ __host__ inline brgb convert_rgb_brgb(rgb in)
{
    brgb out;
    out.x = (uchar)(in.x)*255.0;
    out.y = (uchar)(in.y)*255.0;
    out.z = (uchar)(in.z)*255.0;
    
    return out;
}

//
// clamping
//

__device__ __host__ inline float clamp(float v, float a, float b)
{
    return fmax(a, fmin(b, v));
}
__device__ __host__ inline unsigned char clamp(float v)
{
    return (unsigned char)clamp(v, 0, 255);
}
__device__ __host__ inline float3 clamp(float3 v, float a, float b)
{
    return make_float3(clamp(v.x, a, b), clamp(v.y, a, b), clamp(v.z, a, b));
}

__device__ __host__ inline int clamp(int v, int a, int b)
{
    return ::max(a, ::min(b, v));
}

//
// type conversions
//

__device__ __host__ inline rgb rgba2rgb(rgba v)
{
    return make_float3(v.x/255.0f, v.y/255.0f, v.z/255.0f);
}
__device__ __host__ inline rgba rgb2rgba(rgb f)
{
    return make_uchar4(clamp(f.x*255.0f), clamp(f.y*255.0f), clamp(f.z*255.0f), 0xFF);
}
__device__ __host__ inline float rgb2float(rgb p)
{
    return p.x*0.2126f + p.y*0.7152f + p.z*0.0722f;
}

//
// colors
//

#define RGB_WHITE	make_rgb(1, 1, 1)
#define RGB_BLACK	make_rgb(0, 0, 0)
#define RGB_GRAY	make_rgb(0.5, 0.5, 0.5)
#define RGB_RED		make_rgb(1, 0, 0)
#define RGB_GREEN	make_rgb(0, 1, 0)
#define RGB_BLUE	make_rgb(0, 0, 1)
#define RGB_ORANGE	make_rgb(1, 0.5, 0)
#define RGB_YELLOW	make_rgb(1, 1, 0)

//
// type operators
//
__device__ __host__ inline float2 operator-(const float2 &v)
{
    return make_float2(-v.x, -v.y);
}
__device__ __host__ inline float3 operator-(const float3 &v)
{
    return make_float3(-v.x, -v.y, -v.z);
}

__device__ __host__ inline float2 operator+(const float2 &v1, const float2 &v2)
{
    return make_float2(v1.x+v2.x, v1.y+v2.y);
}
__device__ __host__ inline float3 operator+(const float3 &v1, const float3 &v2)
{
    return make_float3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}
__device__ __host__ inline double2 operator+(const double2 &v1, const double2 &v2)
{
    return make_double2(v1.x+v2.x, v1.y+v2.y);
}
__device__ __host__ inline double3 operator+(const double3 &v1, const double3 &v2)
{
    return make_double3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

__device__ __host__ inline float2 operator+=(float2 &v1, const float2 &v2)
{
    return v1 = v1 + v2;
}
__device__ __host__ inline float3 operator+=(float3 &v1, const float3 &v2)
{
    return v1 = v1 + v2;
}
__device__ __host__ inline double3 operator+=(double3 &v1, const double3 &v2)
{
    return v1 = v1 + v2;
}
__device__ __host__ inline double3 operator+=(double3 &v1, const float3 &v2)
{
    return v1 = v1 + make_double3(v2.x, v2.y, v2.z);
}

__device__ __host__ inline float2 operator-(const float2 &d1, const float2 &d2)
{
    return make_float2(d1.x - d2.x, d1.y - d2.y);
}
__device__ __host__ inline float3 operator-(const float3 &d1, const float3 &d2)
{
    return make_float3(d1.x - d2.x, d1.y - d2.y, d1.z - d2.z);
}
__device__ __host__ inline double2 operator-(const double2 &d1, const double2 &d2)
{
    return make_double2(d1.x - d2.x, d1.y - d2.y);
}
__device__ __host__ inline double3 operator-(const double3 &d1, const double3 &d2)
{
    return make_double3(d1.x - d2.x, d1.y - d2.y, d1.z - d2.z);
}

__device__ __host__ inline float2 operator*(const float2 &d1, float f)
{
    return make_float2(d1.x * f, d1.y * f);
}
__device__ __host__ inline float3 operator*(const float3 &d1, float f)
{
    return make_float3(d1.x * f, d1.y * f, d1.z * f);
}
__device__ __host__ inline float4 operator*(const float4 &d1, float f)
{
    return make_float4(d1.x * f, d1.y * f, d1.z * f, d1.w * f);
}
__device__ __host__ inline float3 operator*(const float3 &f1, const float3& f2)
{
    return make_float3(f1.x * f2.x, f1.y * f2.y, f1.z * f2.z);
}
__device__ __host__ inline float2 operator*(const float2 &f1, const float2& f2)
{
    return make_float2(f1.x * f2.x, f1.y * f2.y);
}
__device__ __host__ inline double2 operator*(const double2 &d1, double f)
{
    return make_double2(d1.x * f, d1.y * f);
}
__device__ __host__ inline double3 operator*(const double3 &d1, double f)
{
    return make_double3(d1.x * f, d1.y * f, d1.z * f);
}
__device__ __host__ inline double3 operator*(const double3 &f1, const double3& f2)
{
    return make_double3(f1.x * f2.x, f1.y * f2.y, f1.z * f2.z);
}

__device__ __host__ inline float2 operator/(const float2 &v, float f)
{
    return make_float2(v.x/f, v.y/f);
}
__device__ __host__ inline float3 operator/(const float3 &v, float f)
{
    return make_float3(v.x/f, v.y/f, v.z/f);
}
__device__ __host__ inline float3 operator/(const float3& v1, const float3& v2)
{
    return make_float3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}
__device__ __host__ inline double2 operator/(const double2 &v, double f)
{
    return make_double2(v.x/f, v.y/f);
}
__device__ __host__ inline double3 operator/(const double3 &v, double f)
{
    return make_double3(v.x/f, v.y/f, v.z/f);
}
__device__ __host__ inline double3 operator/(const double3& v1, const double3& v2)
{
    return make_double3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

__device__ __host__ inline bool operator==(const int2& v1, const int2& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}
__device__ __host__ inline bool operator==(const float2& v1, const float2& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}
__device__ __host__ inline bool operator==(const float3& v1, const float3& v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
__device__ __host__ inline bool operator==(const double2& v1, const double2& v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}
__device__ __host__ inline bool operator==(const double3& v1, const double3& v2)
{
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}

__device__ __host__ inline bool operator!=(const int2& v1, const int2& v2)
{
    return v1.x != v2.x || v1.y != v2.y;
}
__device__ __host__ inline bool operator!=(const float2& v1, const float2& v2)
{
    return v1.x != v2.x || v1.y != v2.y;
}
__device__ __host__ inline bool operator!=(const float3& v1, const float3& v2)
{
    return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}
__device__ __host__ inline bool operator!=(const double2& v1, const double2& v2)
{
    return v1.x != v2.x || v1.y != v2.y;
}
__device__ __host__ inline bool operator!=(const double3& v1, const double3& v2)
{
    return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}

//
// type functions
//

__device__ __host__ inline float dot(const float3& v1, const float3& v2)
{
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

__device__ __host__ inline float length2(float x, float y)
{
    return x*x + y*y;
}
__device__ __host__ inline float length2(const float2 &v)
{
    return v.x*v.x + v.y*v.y;
}
__device__ __host__ inline float length2(const float3 &v)
{
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

__device__ __host__ inline float length(const float2 &v)
{
    return sqrtf(length2(v));
}
__device__ __host__ inline float length(float x, float y)
{
    return sqrtf(length2(x, y));
}
__device__ __host__ inline float length(const float3 &v)
{
    return sqrtf(length2(v));
}

__device__ __host__ inline float3 sqrtf(const float3& v)
{
    return make_float3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z));
}
__device__ __host__ inline double3 sqrtf(const double3& v)
{
    return make_double3(sqrtf(v.x), sqrtf(v.y), sqrtf(v.z));
}

__device__ __host__ inline float3 abs(const float3& v)
{
    return make_float3(fabs(v.x), fabs(v.y), fabs(v.z));
}

__device__ __host__ inline float max(const float3& v)
{
    return fmax(v.x, fmax(v.y, v.z));
}

__device__ __host__ inline float2 normalize(const float2 &v)
{
    return v / length(v);
}
__device__ __host__ inline float3 normalize(const float3 &v)
{
    return v / length(v);
}

__device__ __host__ inline float4 blend(float a1, float4 p1, float4 p2)
{
    float a2 = 1 - a1;
    return make_float4(a1*p1.x + a2*p2.x, a1*p1.y + a2*p2.y, a1*p1.z + a2*p2.z, 255.0f);
}

__device__ __host__ inline float3 blend(float a1, float3 p1, float3 p2)
{
    float a2 = 1 - a1;
    return make_float3(a1*p1.x + a2*p2.x, a1*p1.y + a2*p2.y, a1*p1.z + a2*p2.z);
}

//
// 3D affine transforms
//

__device__ __host__ inline Transform make_Transform(
    float v00, float v01, float v02, float v03,
    float v10, float v11, float v12, float v13,
    float v20, float v21, float v22, float v23
)
{
    Transform v = {v00, v01, v02, v03, v10, v11, v12, v13, v20, v21, v22, v23};
    return v;
}

__device__ __host__ inline bool operator==(const Transform& t1, const Transform& t2)
{
    return t1.mx[0][0] == t2.mx[0][0] && t1.mx[0][1] == t2.mx[0][1] && t1.mx[0][2] == t2.mx[0][2] && t1.mx[0][3] == t2.mx[0][3] &&
           t1.mx[1][0] == t2.mx[1][0] && t1.mx[1][1] == t2.mx[1][1] && t1.mx[1][2] == t2.mx[1][2] && t1.mx[1][3] == t2.mx[1][3] &&
           t1.mx[2][0] == t2.mx[2][0] && t1.mx[2][1] == t2.mx[2][1] && t1.mx[2][2] == t2.mx[2][2] && t1.mx[2][3] == t2.mx[2][3];
}

__device__ __host__ inline bool operator!=(const Transform& t1, const Transform& t2)
{
    return !(t1 == t2);
}

__device__ __host__ inline float3 operator*(const Transform& t, const float3& p)
{
    return make_float3(
        t.mx[0][0]*p.x + t.mx[0][1]*p.y + t.mx[0][2]*p.z + t.mx[0][3],
        t.mx[1][0]*p.x + t.mx[1][1]*p.y + t.mx[1][2]*p.z + t.mx[1][3],
        t.mx[2][0]*p.x + t.mx[2][1]*p.y + t.mx[2][2]*p.z + t.mx[2][3]
    );
}

__device__ __host__ inline Transform operator*(const Transform& t, const float &s)
{
    return make_Transform(
        t.mx[0][0]*s, t.mx[0][1]*s, t.mx[0][2]*s, t.mx[0][3]*s,
        t.mx[1][0]*s, t.mx[1][1]*s, t.mx[1][2]*s, t.mx[1][3]*s,
        t.mx[2][0]*s, t.mx[2][1]*s, t.mx[2][2]*s, t.mx[2][3]*s
    );
}

__device__ __host__ inline Transform operator*(const Transform& t1, const Transform& t2)
{
    return make_Transform(
        t1.mx[0][0] * t2.mx[0][0] + t1.mx[0][1] * t2.mx[1][0] + t1.mx[0][2] * t2.mx[2][0],
        t1.mx[0][0] * t2.mx[0][1] + t1.mx[0][1] * t2.mx[1][1] + t1.mx[0][2] * t2.mx[2][1],
        t1.mx[0][0] * t2.mx[0][2] + t1.mx[0][1] * t2.mx[1][2] + t1.mx[0][2] * t2.mx[2][2],
        t1.mx[0][0] * t2.mx[0][3] + t1.mx[0][1] * t2.mx[1][3] + t1.mx[0][2] * t2.mx[2][3] + t1.mx[0][3],

        t1.mx[1][0] * t2.mx[0][0] + t1.mx[1][1] * t2.mx[1][0] + t1.mx[1][2] * t2.mx[2][0],
        t1.mx[1][0] * t2.mx[0][1] + t1.mx[1][1] * t2.mx[1][1] + t1.mx[1][2] * t2.mx[2][1],
        t1.mx[1][0] * t2.mx[0][2] + t1.mx[1][1] * t2.mx[1][2] + t1.mx[1][2] * t2.mx[2][2],
        t1.mx[1][0] * t2.mx[0][3] + t1.mx[1][1] * t2.mx[1][3] + t1.mx[1][2] * t2.mx[2][3] + t1.mx[1][3],

        t1.mx[2][0] * t2.mx[0][0] + t1.mx[2][1] * t2.mx[1][0] + t1.mx[2][2] * t2.mx[2][0],
        t1.mx[2][0] * t2.mx[0][1] + t1.mx[2][1] * t2.mx[1][1] + t1.mx[2][2] * t2.mx[2][1],
        t1.mx[2][0] * t2.mx[0][2] + t1.mx[2][1] * t2.mx[1][2] + t1.mx[2][2] * t2.mx[2][2],
        t1.mx[2][0] * t2.mx[0][3] + t1.mx[2][1] * t2.mx[1][3] + t1.mx[2][2] * t2.mx[2][3] + t1.mx[2][3]
    );
}

__device__ __host__ inline Transform scale_Transform(float s)
{
    return make_Transform(s, 0, 0, 0, 0, s, 0, 0, 0, 0, s, 0);
}
__device__ __host__ inline Transform identity_Transform()
{
    return scale_Transform(1);
}

__device__ __host__ inline Transform translation_Transform(const float3 &t)
{
    return make_Transform(1, 0, 0, t.x, 0, 1, 0, t.y, 0, 0, 1, t.z);
}

__device__ __host__ inline Transform linear_Transform(const float &s, const float &t)
{
    return make_Transform(s, 0, 0, t, 0, s, 0, t, 0, 0, s, t);
}

__device__ __host__ inline Transform rotation_Transform(const float3 &r)
{
    float sx = sinf(r.x), sy = sinf(r.y), sz = sinf(r.z);
    float cx = cosf(r.x), cy = cosf(r.y), cz = cosf(r.z);

    Transform t = make_Transform(cy, 0, sy, 0, 0, 1, 0, 0, -sy, 0, cy, 0); // rotate around y-axis (yaw)
    t = t *    make_Transform(1, 0, 0, 0, 0, cx, -sx, 0, 0, sx, cx, 0); // rotate around x-axis (pitch)
    t = t *    make_Transform(cz, -sz, 0, 0, sz, cz, 0, 0, 0, 0, 1, 0); // rotate around z-axis (roll)
    return t;
}

__device__ __host__ inline Transform rotation_Transform(float yaw, float pitch, float roll)
{
    return rotation_Transform(make_float3(pitch, yaw, roll));
}

__device__ __host__ inline Transform inverse_Transform(const Transform& t)
{
    float A = t.mx[1][1] * t.mx[2][2] - t.mx[2][1] * t.mx[1][2];
    float B = t.mx[1][0] * t.mx[2][2] - t.mx[1][2] * t.mx[2][0];
    float C = t.mx[1][0] * t.mx[2][1] - t.mx[2][0] * t.mx[1][1];

    float D = t.mx[0][1] * t.mx[2][2] - t.mx[0][2] * t.mx[2][1];
    float E = t.mx[0][0] * t.mx[2][2] - t.mx[0][2] * t.mx[2][0];
    float F = t.mx[0][0] * t.mx[2][1] - t.mx[2][0] * t.mx[0][1];

    float G = t.mx[0][1] * t.mx[1][2] - t.mx[0][2] * t.mx[1][1];
    float H = t.mx[0][0] * t.mx[1][2] - t.mx[1][0] * t.mx[0][2];
    float K = t.mx[0][0] * t.mx[1][1] - t.mx[1][0] * t.mx[0][1];

    float det = t.mx[0][0] * A - t.mx[0][1] * B + t.mx[0][2] * C;

    if (det == 0) {
        return identity_Transform();
    }
    A /= det; B /= det; C /= det;
    D /= det; E /= det; F /= det;
    G /= det; H /= det; K /= det;

    return make_Transform(
        A, -D,  G, -A*t.mx[0][3] + D*t.mx[1][3] - G*t.mx[2][3],
       -B,  E, -H,  B*t.mx[0][3] - E*t.mx[1][3] + H*t.mx[2][3],
        C, -F,  K, -C*t.mx[0][3] + F*t.mx[1][3] - K*t.mx[2][3]
    );
}

// Conversions assume RGB in range [0, 1]
// Full swing assumes YUV (AKA YCbCr) in range [0, 1]
// Studio swing assumes Y in range [16, 235]
// Studio swing assumes U,V in range [16, 240]

//BT709 full swing - from Rec. 709 Standard
#define YUV2RGB make_Transform( \
        1, 0, 1.5748, 0., \
        1, -0.1873, -0.4681, 0., \
        1, 1.8556, 0., 0. )

#define  RGB2YUV make_Transform( \
        0.2126, 0.7152, 0.0722, 0., \
        -0.1146, -0.3854, 0.5, 0., \
        0.5, -0.4542, -0.0458, 0.)

#define YUV2RGB_709_FULL YUV2RGB
#define RGB2YUV_709_FULL RGB2YUV

//BT601 full swing - From Hardware team
#define YUV2RGB_601_FULL  make_Transform( \
        1., 0, 1.402, 0, \
        1., -0.344, -0.714, 0, \
        1., 1.772, 0, 0 )

#define RGB2YUV_601_FULL make_Transform( \
        0.2990,  0.5871,  0.1140, 0, \
        -0.1687, -0.3313,  0.5000, 0, \
         0.5000, -0.4187, -0.0813, 0  )

//BT601 studio swing - Jack, "Video Demystified"
#define YUV2RGB_601_STUDIO  make_Transform( \
        1.1644, 0, 1.596, 0, \
        1.1644, -0.391, -.813, 0, \
        1.1644, 2.018, 0, 0 )
#define RGB2YUV_601_STUDIO make_Transform( \
        0.2569, 0.5042, 0.0977, 0, \
        -0.1482, -0.2910, 0.4392, 0, \
        0.4392, -0.3679, -0.0713, 0 )

//BT709 studio swing - Jack, "Video Demystified"
#define YUV2RGB_709_STUDIO  make_Transform( \
        1.1644, 0 , 1.793, 0, \
        1.1644, -0.213, -0.534, 0, \
        1.1644,  2.115,  0, 0  )

#define RGB2YUV_709_STUDIO  make_Transform( \
        0.1829, 0.6141, 0.0618, 0, \
        -.1007, -0.3381, 0.4388, 0, \
        0.4390, -0.3988, -0.0402, 0 )

#define Scale_10Bits (1023./(4*255.))

// Happens in firmware now
//// NEO Color Correction Matrix
//#define NEO_CCM make_Transform( \
//        1.386, -0.385, -0.001, 0, \
//        -0.38, 1.75, -0.37, 0, \
//        0.061, -1.15, 2.09, 0 )

#define NEO_STD_AE_TARGET 128.
#define NEO_EDR_01_AE_TARGET 35.
#define NEO_EDR_02_AE_TARGET 32.

#if 0
// REMIND: this class does not seem to be correct
// REMIND: let me know if you need to use it - avh

//
// Used to describe unambiguous rotation (yaw, pitch roll)
//
class Quaternion
{
 public:
    float x, y, z, w;

 public:
    __host__ __device__ Quaternion() : x(0), y(0), z(0), w(1) {}
    __host__ __device__ Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    __host__ __device__ Quaternion(const Quaternion& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}

    __host__ __device__ Quaternion(float yaw, float pitch)
    {
        pitch = -pitch;
        x = cosf(yaw/2)*sinf(pitch/2);
        y = sinf(yaw/2)*cosf(pitch/2);
        z = -sinf(yaw/2)*sinf(pitch/2);
        w = cosf(yaw/2)*cosf(pitch/2);
    }
    __host__ __device__ Quaternion(float yaw, float pitch, float roll)
    {
        pitch = -pitch;
        roll = -roll;
        x = cosf(yaw/2)*sinf(pitch/2)*cosf(roll/2) + sinf(yaw/2)*cosf(pitch/2)*sinf(roll/2);
        y = sinf(yaw/2)*cosf(pitch/2)*cosf(roll/2) - cosf(yaw/2)*sinf(pitch/2)*sinf(roll/2);
        z = cosf(yaw/2)*cosf(pitch/2)*sinf(roll/2) - sinf(yaw/2)*sinf(pitch/2)*cosf(roll/2);
        w = cosf(yaw/2)*cosf(pitch/2)*cosf(roll/2) + sinf(yaw/2)*sinf(pitch/2)*sinf(roll/2);
    }

    __host__ __device__ Quaternion operator*(const Quaternion& q) const
    {
        return Quaternion(
            q.w*x + q.x*w + q.y*z - q.z*q.y,
            q.w*y + q.y*w + q.z*x - q.x*q.z,
            q.w*z + q.z*w + q.x*y - q.y*q.x,
            q.w*w - q.x*x - q.y*y - q.z*q.z
        );
    }

    inline __host__ __device__ operator Transform() const
    {
        return make_Transform(
            1 - 2*(y*y + z*z), 2*(x*y - z*w), 2*(x*z + y*w), 0,
            2*(x*y + z*w), 1 - 2*(x*x + z*z), 2*(y*z - x*w), 0,
            2*(x*z - y*w), 2*(y*z + x*w), 1 - 2*(x*x + y*y), 0
        );
    }

    inline __host__ __device__ float getYaw() const
    {
        float t = y*x + z*w;
        int pole = (t < -0.499f) ? -1 : (t > 0.499f) ?  1 : 0;
        return pole == 0 ? atan2f(2*(y*w + x*z), 1 - 2*(y*y + x*x)) : 0;
    }

    inline __host__ __device__ float getPitch() const
    {
        float t = y*x + z*w;
        int pole = (t < -0.499f) ? -1 : (t > 0.499f) ?  1 : 0;
        return pole == 0 ? -asinf(clamp(2*(w*x - z*y), -1.0f, 1.0f)) : -pole*M_PI*0.5;
    }

    inline __host__ __device__ float getRoll() const
    {
        float t = y*x + z*w;
        int pole = (t < -0.499f) ? -1 : (t > 0.499f) ?  1 : 0;
        return pole == 0 ? -atan2f(2*(w*z + y*x), 1 - 2*(x*x + z*z)) : -pole*2*atan2f(y, w);
    }

    inline __host__ __device__ float3 getRotation() const
    {
        return make_float3(getYaw(), getPitch(), getRoll());
    }
};
#endif

#endif
