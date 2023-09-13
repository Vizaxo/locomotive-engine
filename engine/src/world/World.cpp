#include "World.h"

#include "Noise.h"

float World::getHeight(HexCoord c) {
    v2 p = c.cartesian() / 0.7895f; // avoid integer coords, they give 0
    float height = 0.;

    height += Noise::perlin(p);
    height += Noise::perlin(p/2.f)*2.;
    height += Noise::perlin(p/4.f)*4.;
    height += Noise::perlin(p/8.f)*8.;
    height += Noise::perlin(p/16.f)*16.;

    return height / 8.f;
}
