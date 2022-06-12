#ifndef TRIGO_LOOKUP_H
#define TRIGO_LOOKUP_H __DATE__ ", " __TIME__

#include <Arduino.h>

namespace yh {
    namespace rec {
        namespace trigo_lookups {
            // occupies 4 * 91 = 364 bytes in PROGMEM
            static const PROGMEM double sin_table_lookup [] = {
                0.0000000000000, // angle == 000
                0.0174524064373, // angle == 001
                0.0348994967025, // angle == 002
                0.0523359562429, // angle == 003
                0.0697564737441, // angle == 004
                0.0871557427477, // angle == 005
                0.1045284632677, // angle == 006
                0.1218693434051, // angle == 007
                0.1391731009601, // angle == 008
                0.1564344650402, // angle == 009
                0.1736481776669, // angle == 010
                0.1908089953765, // angle == 011
                0.2079116908178, // angle == 012
                0.2249510543439, // angle == 013
                0.2419218955997, // angle == 014
                0.2588190451025, // angle == 015
                0.2756373558170, // angle == 016
                0.2923717047227, // angle == 017
                0.3090169943749, // angle == 018
                0.3255681544572, // angle == 019
                0.3420201433257, // angle == 020
                0.3583679495453, // angle == 021
                0.3746065934159, // angle == 022
                0.3907311284893, // angle == 023
                0.4067366430758, // angle == 024
                0.4226182617407, // angle == 025
                0.4383711467891, // angle == 026
                0.4539904997395, // angle == 027
                0.4694715627859, // angle == 028
                0.4848096202463, // angle == 029
                0.5000000000000, // angle == 030
                0.5150380749101, // angle == 031
                0.5299192642332, // angle == 032
                0.5446390350150, // angle == 033
                0.5591929034707, // angle == 034
                0.5735764363510, // angle == 035
                0.5877852522925, // angle == 036
                0.6018150231520, // angle == 037
                0.6156614753257, // angle == 038
                0.6293203910498, // angle == 039
                0.6427876096865, // angle == 040
                0.6560590289905, // angle == 041
                0.6691306063589, // angle == 042
                0.6819983600625, // angle == 043
                0.6946583704590, // angle == 044
                0.7071067811865, // angle == 045
                0.7193398003387, // angle == 046
                0.7313537016192, // angle == 047
                0.7431448254774, // angle == 048
                0.7547095802228, // angle == 049
                0.7660444431190, // angle == 050
                0.7771459614570, // angle == 051
                0.7880107536067, // angle == 052
                0.7986355100473, // angle == 053
                0.8090169943749, // angle == 054
                0.8191520442890, // angle == 055
                0.8290375725550, // angle == 056
                0.8386705679454, // angle == 057
                0.8480480961564, // angle == 058
                0.8571673007021, // angle == 059
                0.8660254037844, // angle == 060
                0.8746197071394, // angle == 061
                0.8829475928589, // angle == 062
                0.8910065241884, // angle == 063
                0.8987940462992, // angle == 064
                0.9063077870366, // angle == 065
                0.9135454576426, // angle == 066
                0.9205048534524, // angle == 067
                0.9271838545668, // angle == 068
                0.9335804264972, // angle == 069
                0.9396926207859, // angle == 070
                0.9455185755993, // angle == 071
                0.9510565162952, // angle == 072
                0.9563047559630, // angle == 073
                0.9612616959383, // angle == 074
                0.9659258262891, // angle == 075
                0.9702957262760, // angle == 076
                0.9743700647852, // angle == 077
                0.9781476007338, // angle == 078
                0.9816271834477, // angle == 079
                0.9848077530122, // angle == 080
                0.9876883405951, // angle == 081
                0.9902680687416, // angle == 082
                0.9925461516413, // angle == 083
                0.9945218953683, // angle == 084
                0.9961946980917, // angle == 085
                0.9975640502598, // angle == 086
                0.9986295347546, // angle == 087
                0.9993908270191, // angle == 088
                0.9998476951564, // angle == 089
                1.0000000000000  // angle == 090
            };
            double pvt_sinA (int16_t angle);
            double pvt_cosA (int16_t angle);
            double sinA_switch_lookup (int8_t folded_angle) __attribute__((__always_inline__));
            double sinA_switch_lookup (int8_t folded_angle) {
                double result = 0.0;
                switch (folded_angle) {
                    case 0:
                        result = 0.0000000000000;
                        break;
                    case 1:
                        result = 0.0174524064373;
                        break;
                    case 2:
                        result = 0.0348994967025;
                        break;
                    case 3:
                        result = 0.0523359562429;
                        break;
                    case 4:
                        result = 0.0697564737441;
                        break;
                    case 5:
                        result = 0.0871557427477;
                        break;
                    case 6:
                        result = 0.1045284632677;
                        break;
                    case 7:
                        result = 0.1218693434051;
                        break;
                    case 8:
                        result = 0.1391731009601;
                        break;
                    case 9:
                        result = 0.1564344650402;
                        break;
                    case 10:
                        result = 0.1736481776669;
                        break;
                    case 11:
                        result = 0.1908089953765;
                        break;
                    case 12:
                        result = 0.2079116908178;
                        break;
                    case 13:
                        result = 0.2249510543439;
                        break;
                    case 14:
                        result = 0.2419218955997;
                        break;
                    case 15:
                        result = 0.2588190451025;
                        break;
                    case 16:
                        result = 0.2756373558170;
                        break;
                    case 17:
                        result = 0.2923717047227;
                        break;
                    case 18:
                        result = 0.3090169943749;
                        break;
                    case 19:
                        result = 0.3255681544572;
                        break;
                    case 20:
                        result = 0.3420201433257;
                        break;
                    case 21:
                        result = 0.3583679495453;
                        break;
                    case 22:
                        result = 0.3746065934159;
                        break;
                    case 23:
                        result = 0.3907311284893;
                        break;
                    case 24:
                        result = 0.4067366430758;
                        break;
                    case 25:
                        result = 0.4226182617407;
                        break;
                    case 26:
                        result = 0.4383711467891;
                        break;
                    case 27:
                        result = 0.4539904997395;
                        break;
                    case 28:
                        result = 0.4694715627859;
                        break;
                    case 29:
                        result = 0.4848096202463;
                        break;
                    case 30:
                        result = 0.5000000000000;
                        break;
                    case 31:
                        result = 0.5150380749101;
                        break;
                    case 32:
                        result = 0.5299192642332;
                        break;
                    case 33:
                        result = 0.5446390350150;
                        break;
                    case 34:
                        result = 0.5591929034707;
                        break;
                    case 35:
                        result = 0.5735764363510;
                        break;
                    case 36:
                        result = 0.5877852522925;
                        break;
                    case 37:
                        result = 0.6018150231520;
                        break;
                    case 38:
                        result = 0.6156614753257;
                        break;
                    case 39:
                        result = 0.6293203910498;
                        break;
                    case 40:
                        result = 0.6427876096865;
                        break;
                    case 41:
                        result = 0.6560590289905;
                        break;
                    case 42:
                        result = 0.6691306063589;
                        break;
                    case 43:
                        result = 0.6819983600625;
                        break;
                    case 44:
                        result = 0.6946583704590;
                        break;
                    case 45:
                        result = 0.7071067811865;
                        break;
                    case 46:
                        result = 0.7193398003387;
                        break;
                    case 47:
                        result = 0.7313537016192;
                        break;
                    case 48:
                        result = 0.7431448254774;
                        break;
                    case 49:
                        result = 0.7547095802228;
                        break;
                    case 50:
                        result = 0.7660444431190;
                        break;
                    case 51:
                        result = 0.7771459614570;
                        break;
                    case 52:
                        result = 0.7880107536067;
                        break;
                    case 53:
                        result = 0.7986355100473;
                        break;
                    case 54:
                        result = 0.8090169943749;
                        break;
                    case 55:
                        result = 0.8191520442890;
                        break;
                    case 56:
                        result = 0.8290375725550;
                        break;
                    case 57:
                        result = 0.8386705679454;
                        break;
                    case 58:
                        result = 0.8480480961564;
                        break;
                    case 59:
                        result = 0.8571673007021;
                        break;
                    case 60:
                        result = 0.8660254037844;
                        break;
                    case 61:
                        result = 0.8746197071394;
                        break;
                    case 62:
                        result = 0.8829475928589;
                        break;
                    case 63:
                        result = 0.8910065241884;
                        break;
                    case 64:
                        result = 0.8987940462992;
                        break;
                    case 65:
                        result = 0.9063077870366;
                        break;
                    case 66:
                        result = 0.9135454576426;
                        break;
                    case 67:
                        result = 0.9205048534524;
                        break;
                    case 68:
                        result = 0.9271838545668;
                        break;
                    case 69:
                        result = 0.9335804264972;
                        break;
                    case 70:
                        result = 0.9396926207859;
                        break;
                    case 71:
                        result = 0.9455185755993;
                        break;
                    case 72:
                        result = 0.9510565162952;
                        break;
                    case 73:
                        result = 0.9563047559630;
                        break;
                    case 74:
                        result = 0.9612616959383;
                        break;
                    case 75:
                        result = 0.9659258262891;
                        break;
                    case 76:
                        result = 0.9702957262760;
                        break;
                    case 77:
                        result = 0.9743700647852;
                        break;
                    case 78:
                        result = 0.9781476007338;
                        break;
                    case 79:
                        result = 0.9816271834477;
                        break;
                    case 80:
                        result = 0.9848077530122;
                        break;
                    case 81:
                        result = 0.9876883405951;
                        break;
                    case 82:
                        result = 0.9902680687416;
                        break;
                    case 83:
                        result = 0.9925461516413;
                        break;
                    case 84:
                        result = 0.9945218953683;
                        break;
                    case 85:
                        result = 0.9961946980917;
                        break;
                    case 86:
                        result = 0.9975640502598;
                        break;
                    case 87:
                        result = 0.9986295347546;
                        break;
                    case 88:
                        result = 0.9993908270191;
                        break;
                    case 89:
                        result = 0.9998476951564;
                        break;
                    case 90:
                        result = 1.0000000000000;
                        break;
                }
                return result;
            }
            // @param angle the angle expressed in degrees
            // @return sin(angle)
            double sinA (int16_t angle) __attribute__((__always_inline__));
            double sinA (int16_t angle) {
                if (__builtin_constant_p(angle)) {
                    while (angle < 0) {
                        angle += 360;
                    }
                    while (angle >= 360) {
                        angle -= 360;
                    }
                    bool neg_flag = (angle > 180);
                    if (neg_flag)
                        angle -= 180;
                    if (angle > 90)
                        angle = 180 - angle;
                    double result = sinA_switch_lookup(angle);
                    if (neg_flag)
                        result = -result;
                    return result;
                } else {
                    return pvt_sinA(angle);
                }
            }
            double pvt_sinA (int16_t angle) {
                while (angle < 0) {
                    angle += 360;
                }
                while (angle >= 360) {
                    angle -= 360;
                }
                bool neg_flag = (angle > 180);
                if (neg_flag)
                    angle -= 180;
                if (angle > 90)
                    angle = 180 - angle;
                double result = pgm_read_float_near(sin_table_lookup + angle);
                if (neg_flag)
                    result = -result;
                return result;
            }
            // @param angle the angle expressed in degrees
            // @return cos(angle)
            double cosA (int16_t angle) __attribute__((__always_inline__));
            double cosA (int16_t angle) {
                if (__builtin_constant_p(angle)) {
                    if (angle < 0)
                        angle = -angle;
                    angle += 90;
                    while (angle >= 360) {
                        angle -= 360;
                    }
                    bool neg_flag = (angle > 180);
                    if (neg_flag)
                        angle -= 180;
                    if (angle > 90)
                        angle = 180 - angle;
                    double result = sinA_switch_lookup(angle);
                    if (neg_flag)
                        result = -result;
                    return result;
                } else {
                    return pvt_cosA(angle);
                }
            }
            double pvt_cosA (int16_t angle) {
                if (angle < 0)
                    angle = -angle;
                angle += 90;
                while (angle >= 360) {
                    angle -= 360;
                }
                bool neg_flag = (angle > 180);
                if (neg_flag)
                    angle -= 180;
                if (angle > 90)
                    angle = 180 - angle;
                double result = pgm_read_float_near(sin_table_lookup + angle);
                if (neg_flag)
                    result = -result;
                return result;
            }
        }
    }
}
#endif // #ifndef TRIGO_LOOKUP_H