#ifndef MATHS_EXTENSION_CPP
#define MATHS_EXTENSION_CPP __DATE__ ", " __TIME__

#include "Maths_extension.h"

yh::rec::Quaternion operator * (const yh::rec::Quaternion lhs, const yh::rec::Quaternion rhs) {

    // definitions:
    // i^2 = j^2 = k^2 = ijk = -1
    // ij = k, ji = -k
    // ki = j, ik = -j
    // jk = i, kj = -i

    //      Quaternion 1     *     Quaternion 2
    //   (a + bi + cj + dk)  *  (e + fi + gj + hk)
    // = a(e + fi + gj + hk)  + bi(e + fi + gj + hk)     + cj(e + fi + gj + hk)     + dk(e + fi + gj + hk)
    // = ae + afi + agj + ahk + bie + bifi + bigj + bihk + cje + cjfi + cjgj + cjhk + dke + dkfi + dkgj + dkhk
    // = ae + afi + agj + ahk + bei - bf   +bgk   - bhj  + cej - cfk  - cg   + chi  + dek + dfj  - dgi  - dh
    // = (ae - bf - cg - dh) + (af + be + ch - dg)i + (ag - bh + ce + df)j + (ah + bg - cf + de)k

    // Quaternion multiplication is defined by:
    //     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
    //     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
    //     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
    //     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
    return yh::rec::Quaternion(
        (lhs.w * rhs.w   -   lhs.x * rhs.x   -   lhs.y * rhs.y   -   lhs.z * rhs.z),
        (lhs.w * rhs.x   +   lhs.x * rhs.w   +   lhs.y * rhs.z   -   lhs.z * rhs.y),
        (lhs.w * rhs.y   -   lhs.x * rhs.z   +   lhs.y * rhs.w   +   lhs.z * rhs.x),
        (lhs.w * rhs.z   +   lhs.x * rhs.y   -   lhs.y * rhs.x   +   lhs.z * rhs.w)
    );
}

#endif // #ifndef MATHS_EXTENSION_CPP