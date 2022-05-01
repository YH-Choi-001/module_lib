#ifndef MATHS_EXTENSION_CPP
#define MATHS_EXTENSION_CPP __DATE__ ", " __TIME__

#if 1
#include "Maths_extension.h"
#endif

yh::rec::Quaternion yh::rec::operator * (const yh::rec::Quaternion lhs, const yh::rec::Quaternion rhs) {
    __FILE__;

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


namespace yh {
    namespace rec {

        static const
        #ifdef ARDUINO
        PROGMEM
        #endif
        i_sin_type i_sin_table [91] = { // ranges from 0 - 90
            0.0000000000000 * INT_MUL_CONST, // angle == 000
            0.0174524064373 * INT_MUL_CONST, // angle == 001
            0.0348994967025 * INT_MUL_CONST, // angle == 002
            0.0523359562429 * INT_MUL_CONST, // angle == 003
            0.0697564737441 * INT_MUL_CONST, // angle == 004
            0.0871557427477 * INT_MUL_CONST, // angle == 005
            0.1045284632677 * INT_MUL_CONST, // angle == 006
            0.1218693434051 * INT_MUL_CONST, // angle == 007
            0.1391731009601 * INT_MUL_CONST, // angle == 008
            0.1564344650402 * INT_MUL_CONST, // angle == 009
            0.1736481776669 * INT_MUL_CONST, // angle == 010
            0.1908089953765 * INT_MUL_CONST, // angle == 011
            0.2079116908178 * INT_MUL_CONST, // angle == 012
            0.2249510543439 * INT_MUL_CONST, // angle == 013
            0.2419218955997 * INT_MUL_CONST, // angle == 014
            0.2588190451025 * INT_MUL_CONST, // angle == 015
            0.2756373558170 * INT_MUL_CONST, // angle == 016
            0.2923717047227 * INT_MUL_CONST, // angle == 017
            0.3090169943749 * INT_MUL_CONST, // angle == 018
            0.3255681544572 * INT_MUL_CONST, // angle == 019
            0.3420201433257 * INT_MUL_CONST, // angle == 020
            0.3583679495453 * INT_MUL_CONST, // angle == 021
            0.3746065934159 * INT_MUL_CONST, // angle == 022
            0.3907311284893 * INT_MUL_CONST, // angle == 023
            0.4067366430758 * INT_MUL_CONST, // angle == 024
            0.4226182617407 * INT_MUL_CONST, // angle == 025
            0.4383711467891 * INT_MUL_CONST, // angle == 026
            0.4539904997395 * INT_MUL_CONST, // angle == 027
            0.4694715627859 * INT_MUL_CONST, // angle == 028
            0.4848096202463 * INT_MUL_CONST, // angle == 029
            0.5000000000000 * INT_MUL_CONST, // angle == 030
            0.5150380749101 * INT_MUL_CONST, // angle == 031
            0.5299192642332 * INT_MUL_CONST, // angle == 032
            0.5446390350150 * INT_MUL_CONST, // angle == 033
            0.5591929034707 * INT_MUL_CONST, // angle == 034
            0.5735764363510 * INT_MUL_CONST, // angle == 035
            0.5877852522925 * INT_MUL_CONST, // angle == 036
            0.6018150231520 * INT_MUL_CONST, // angle == 037
            0.6156614753257 * INT_MUL_CONST, // angle == 038
            0.6293203910498 * INT_MUL_CONST, // angle == 039
            0.6427876096865 * INT_MUL_CONST, // angle == 040
            0.6560590289905 * INT_MUL_CONST, // angle == 041
            0.6691306063589 * INT_MUL_CONST, // angle == 042
            0.6819983600625 * INT_MUL_CONST, // angle == 043
            0.6946583704590 * INT_MUL_CONST, // angle == 044
            0.7071067811865 * INT_MUL_CONST, // angle == 045
            0.7193398003387 * INT_MUL_CONST, // angle == 046
            0.7313537016192 * INT_MUL_CONST, // angle == 047
            0.7431448254774 * INT_MUL_CONST, // angle == 048
            0.7547095802228 * INT_MUL_CONST, // angle == 049
            0.7660444431190 * INT_MUL_CONST, // angle == 050
            0.7771459614570 * INT_MUL_CONST, // angle == 051
            0.7880107536067 * INT_MUL_CONST, // angle == 052
            0.7986355100473 * INT_MUL_CONST, // angle == 053
            0.8090169943749 * INT_MUL_CONST, // angle == 054
            0.8191520442890 * INT_MUL_CONST, // angle == 055
            0.8290375725550 * INT_MUL_CONST, // angle == 056
            0.8386705679454 * INT_MUL_CONST, // angle == 057
            0.8480480961564 * INT_MUL_CONST, // angle == 058
            0.8571673007021 * INT_MUL_CONST, // angle == 059
            0.8660254037844 * INT_MUL_CONST, // angle == 060
            0.8746197071394 * INT_MUL_CONST, // angle == 061
            0.8829475928589 * INT_MUL_CONST, // angle == 062
            0.8910065241884 * INT_MUL_CONST, // angle == 063
            0.8987940462992 * INT_MUL_CONST, // angle == 064
            0.9063077870366 * INT_MUL_CONST, // angle == 065
            0.9135454576426 * INT_MUL_CONST, // angle == 066
            0.9205048534524 * INT_MUL_CONST, // angle == 067
            0.9271838545668 * INT_MUL_CONST, // angle == 068
            0.9335804264972 * INT_MUL_CONST, // angle == 069
            0.9396926207859 * INT_MUL_CONST, // angle == 070
            0.9455185755993 * INT_MUL_CONST, // angle == 071
            0.9510565162952 * INT_MUL_CONST, // angle == 072
            0.9563047559630 * INT_MUL_CONST, // angle == 073
            0.9612616959383 * INT_MUL_CONST, // angle == 074
            0.9659258262891 * INT_MUL_CONST, // angle == 075
            0.9702957262760 * INT_MUL_CONST, // angle == 076
            0.9743700647852 * INT_MUL_CONST, // angle == 077
            0.9781476007338 * INT_MUL_CONST, // angle == 078
            0.9816271834477 * INT_MUL_CONST, // angle == 079
            0.9848077530122 * INT_MUL_CONST, // angle == 080
            0.9876883405951 * INT_MUL_CONST, // angle == 081
            0.9902680687416 * INT_MUL_CONST, // angle == 082
            0.9925461516413 * INT_MUL_CONST, // angle == 083
            0.9945218953683 * INT_MUL_CONST, // angle == 084
            0.9961946980917 * INT_MUL_CONST, // angle == 085
            0.9975640502598 * INT_MUL_CONST, // angle == 086
            0.9986295347546 * INT_MUL_CONST, // angle == 087
            0.9993908270191 * INT_MUL_CONST, // angle == 088
            0.9998476951564 * INT_MUL_CONST, // angle == 089
            1.0000000000000 * INT_MUL_CONST  // angle == 090
        };

    }
}

yh::rec::i_sin_type yh::rec::i_sin_raw (int16_t x) {
    // limit x to 0-360
    x %= 360;
    if (x < 0) x += 360;
    const bool neg_flag = x > 180;
    // limit x to 0-180
    if (neg_flag)
        x -= 180;
    // limit x to 0-90
    if (x > 90)
        x = 180 - x;
    #ifdef ARDUINO
    i_sin_type value = 0;
    switch (sizeof(i_sin_type)) {
        case 1:
            value = pgm_read_byte_near(i_sin_table + x);
            break;
        case 2:
            value = pgm_read_word_near(i_sin_table + x);
            break;
        case 4:
            value = pgm_read_dword_near(i_sin_table + x);
            break;
        default:
            value = 0;
            break;
    }
    return (neg_flag) ? (-value) : value;
    #else
    return i_sin_table[x];
    #endif
}

double yh::rec::sin (double x) {
    const int16_t x_int = static_cast<int16_t>(x);
    const i_sin_type a = i_sin_raw(x_int);
    const i_sin_type b = i_sin_raw(x_int + 1);
    return (a + (x - x_int) * (b - a)) / INT_MUL_CONST;
}

double yh::rec::cos (double x) {
    const int16_t x_int = static_cast<int16_t>(x) + 90;
    const i_sin_type a = i_sin_raw(x_int);
    const i_sin_type b = i_sin_raw(x_int + 1);
    return (a + (x - static_cast<int16_t>(x)) * (b - a)) / INT_MUL_CONST;
}

#endif // #ifndef MATHS_EXTENSION_CPP