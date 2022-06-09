#ifndef FLOATING_SYS_H
#define FLOATING_SYS_H __DATE__ ", " __TIME__

#include <Arduino.h>

namespace yh {
    namespace rec {
        // Arduino floating-point system:
        // [sign bit : 1] [expon : 8] [mag : 23]
        // bit count:  [s] [7] [6] [5] [4] [3] [2] [1] [0] [22] [21] [20] ... [16] [15] [14] ... [8] [7] [6] ... [0]
        // byte count: [0] [0] [0] [0] [0] [0] [0] [0] [1] [ 1] [ 1] [ 1] ... [ 1] [ 2] [ 2] ... [2] [3] [3] ... [3]
        namespace floating_syss {
            // @return the actual value stored in bits
            uint32_t get_bin (const double &val) {
                return *( (uint32_t *)((uint8_t *)&val) );
            }
            #define GET_BIN(val) (*( (uint32_t *)((uint8_t *)&val) ))
            // @return the exponent of the floating-point value in powers of 2
            uint8_t get_exp (const double &val) {
                return get_bin(val) >> (7 + 8 + 8);
            }
            #define GET_EXP(val) ((*( (uint32_t *)((uint8_t *)&val) )) >> (7 + 8 + 8))
            // @return 0 if the value is +ive, 1 if the value is -ive
            uint8_t get_sign (const double &val) {
                return get_bin(val) >> (7 + 8 + 8 + 8);
            }
            #define GET_SIGN(val) ((*( (uint32_t *)((uint8_t *)&val) )) >> (7 + 8 + 8 + 8))
            // @return the 23-bit binary mantissa
            uint32_t get_man (const double &val) {
                return get_bin(val) & 0x7fffff;
            }
            #define GET_MAN(val) ((*( (uint32_t *)((uint8_t *)&val) )) & 0x7fffff)
            // @param lhs_var the floating-point number to be written
            // @param val the binary value to write to lhs_var
            void write_bin (double &lhs_var, const uint32_t val) {
                *( (uint32_t *)((uint8_t *)&lhs_var) ) = val;
            }
            // @param lhs_var the floating-point number to be modified
            // @param exp the binary exponential value to write to lhs_var
            void write_exp (double &lhs_var, const uint8_t exp) {
                (*( (uint32_t *)((uint8_t *)&lhs_var) )) =
                    ( (*( (uint32_t *)((uint8_t *)&lhs_var) )) & (~0x7f800000) ) | (exp << (7 + 8 + 8));
            }
            // @param lhs_var the floating-point number to be modified
            // @param sign the signed bit to write to lhs_var
            void write_sign (double &lhs_var, const int8_t sign) {
                sign
                    ?
                        (*( (uint32_t *)((uint8_t *)&lhs_var) )) |= 0x80000000
                    :
                        (*( (uint32_t *)((uint8_t *)&lhs_var) )) &= (~0x80000000)
                ;
            }
            // @param lhs_var the floating-point number to be modified
            // @param val the 23-bit binary mantissa to write to lhs_var
            void write_man (double &lhs_var, const uint32_t val) {
                (*( (uint32_t *)((uint8_t *)&lhs_var) )) =
                    ( (*( (uint32_t *)((uint8_t *)&lhs_var) )) & (~0x7fffff) ) | (val & 0x7fffff);
            }
        }
    }
}

#endif // #ifndef FLOATING_SYS_H