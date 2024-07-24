#include "PCH.h"

#include "Noise.h"

v2 randomGradient(int ix, int iy) {
    const u32 w = 8u * 4u;
    const u32 s = w / 2u; // rotation width
    u32 a = u32(ix), b = u32(iy);
    a *= 3284157443u; b ^= a << s | a >> w-s;
    b *= 1911520717u; a ^= b << s | b >> w-s;
    a *= 2048419325u;
    float random = float(a) * (2.*3.14159265 / (float)UINT32_MAX); // in [0, 2*Pi]
    v2 v;
    v.x = std::cos(random);
    v.y = std::sin(random);
    return v;
}

float f (float t) {
    return 3.*t*t - 2.*t*t*t;
}

float Noise::perlin(v2 p) {
    v2 v0 = floor(p);
    v2 v1 = ceil(p);
    float x0 = v0.x;
    float y0 = v0.y;
    float x1 = v1.x;
    float y1 = v1.y;
    v2 g00 = randomGradient(int((v0.x)), int((v0.y)));
    v2 g01 = randomGradient(int((v0.x)), int((v1.y)));
    v2 g10 = randomGradient(int((v1.x)), int((v0.y)));
    v2 g11 = randomGradient(int((v1.x)), int((v1.y)));

    float s = dot(g00, p - v2{x0,y0});
    float t = dot(g10, p - v2{x1,y0});
    float u = dot(g01, p - v2{x0,y1});
    float v = dot(g11, p - v2{x1,y1});

    float Sx = f(p.x-x0);
    float a = s+Sx*(t-s);
    float b = u+Sx*(v-u);

    float Sy = f(p.y-y0);
    float z = a+Sy*(b-a);
    return z;
}