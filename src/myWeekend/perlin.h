#ifndef PERLIN_H
#define PERLIN_H

#include "myWeekend/rtweekend.h"

class perlin {
    public:
        perlin() {
            for (int i = 0; i < pointCount; i++) {
                doubles[i] = randomDouble();
            }
            perlinGeneratePerm(permX);
            perlinGeneratePerm(permY);
            perlinGeneratePerm(permZ);
        }

        double noise(const point3& p) const {
            auto i = int(p.x() * 4) & 255;
            auto j = int(p.y() * 4) & 255;
            auto k = int(p.z() * 4) & 255;
            return doubles[permX[i]^permY[j]^permZ[k]];
        }

    private:
        static const int pointCount = 256;
        int permX[pointCount];
        int permY[pointCount];
        int permZ[pointCount];
        double doubles[pointCount];

        static void perlinGeneratePerm(int* p) {
            for (int i = 0; i < pointCount; i++) {
                p[i] = i;
            }

            permute(p, pointCount);
        }

        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = randomInt(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

};

#endif
