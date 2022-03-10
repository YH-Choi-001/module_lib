#ifndef CUSTOM_PRINT_CPP
#define CUSTOM_PRINT_CPP __DATE__ ", " __TIME__

#include "Custom_print.h"

void Custom_print::print (const char char_to_be_printed) {
    print_single_char(char_to_be_printed);
}

void Custom_print::print (const char *str, const char terminating_char) {
    while (*str != terminating_char) {
        print_single_char(*str);
        str++;
    }
}

double pow_base_10 (const uint8_t exp) {
    double to_be_returned = 1.0;
    for (uint8_t i = 0; i < exp; to_be_returned *= 10.0, i++) { }
    return to_be_returned;
}

void Custom_print::print (const uint8_t val) {
    bool began_printing = false;
    for (uint8_t base = 100U; base; base /= 10U) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const uint16_t val) {
    bool began_printing = false;
    for (uint16_t base = 10000U; base; base /= 10U) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const uint32_t val) {
    bool began_printing = false;
    for (uint32_t base = 1000000000U; base; base /= 10U) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const int8_t val) {
    if (val < 0) print_single_char('-');
    bool began_printing = false;
    for (int8_t base = 100; base; base /= 10) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const int16_t val) {
    if (val < 0) print_single_char('-');
    bool began_printing = false;
    for (int16_t base = 10000; base; base /= 10) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const int32_t val) {
    if (val < 0) print_single_char('-');
    bool began_printing = false;
    for (int32_t base = 1000000000; base; base /= 10) {
        const uint8_t to_be_printed = (val / base) % 10;
        if (began_printing || to_be_printed) {
            print_single_char(static_cast<char>('0' + to_be_printed));
            began_printing = true;
        }
    }
}

void Custom_print::print (const double val, const uint8_t dp) {
    double pcs_val;
    if (val < 0) {
        print_single_char('-');
        pcs_val = -val;
    } else {
        pcs_val = val;
    }
    print_single_char(static_cast<uint32_t>(pcs_val));
    print_single_char('.');
    pcs_val -= static_cast<uint32_t>(pcs_val);
    pcs_val += 0.5 / pow_base_10(dp);
    for (uint8_t i = 0; i < dp; i++) {
        print_single_char(static_cast<char>('0' + static_cast<uint32_t>(pcs_val *= 10) % 10));
    }
}

#endif // #ifndef CUSTOM_PRINT_CPP