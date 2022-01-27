#ifndef UART_COM_CPP
#define UART_COM_CPP __DATE__ ", " __TIME__

#include "UART_com.h"

yh::rec::UART_com::UART_com (Serial_ &init_serial_obj) :
    uart_serial(init_serial_obj), request_timeout(0)
{
    //
}

inline void yh::rec::UART_com::begin (const uint32_t baud) {
    uart_serial.begin(baud);
}

// ===================================================================================
// ========================= protected: direct communication =========================
// ===================================================================================

// write a key to replying arduino
void yh::rec::UART_com::write_key (const uint8_t key) {
    uart_serial.write(key);
}

// write a byte of data to replying arduino
void yh::rec::UART_com::write_data (const uint8_t key, const uint8_t data) {
    uart_serial.write(key);
    uart_serial.write(data);
}

// write 2 bytes of data to replying arduino
void yh::rec::UART_com::write_data (const uint8_t key, const uint8_t data0, const uint8_t data1) {
    uart_serial.write(key);
    uart_serial.write(data0);
    uart_serial.write(data1);
}

// request a byte from writing the key to replying arduino
uint8_t yh::rec::UART_com::request_data (const uint8_t key) {
    uart_serial.write(key);
    while (uart_serial.available() < 1) {}
    return uart_serial.read();
}

// request a byte from writing the key to replying arduino, with a timeout
uint8_t yh::rec::UART_com::request_data (const uint8_t key, const unsigned long timeout) {
    uart_serial.write(key);
    const unsigned long init_micros = micros();
    while (uart_serial.available() < 1) {
        if ((micros() - init_micros) > timeout) {
            request_timeout = 1;
            return 0;
        }
    }
    return uart_serial.read();
}

// request a byte from writing the key to replying arduino
uint16_t yh::rec::UART_com::request_2_data (const uint8_t key) {
    uart_serial.write(key);
    while (uart_serial.available() < 2) {}
    return (uart_serial.read() << 8) | uart_serial.read();
}

// request a byte from writing the key to replying arduino, with a timeout
uint16_t yh::rec::UART_com::request_2_data (const uint8_t key, const unsigned long timeout) {
    uart_serial.write(key);
    const unsigned long init_micros = micros();
    while (uart_serial.available() < 2) {
        if ((micros() - init_micros) > timeout) {
            request_timeout = 1;
            return 0;
        }
    }
    return (uart_serial.read() << 8) | uart_serial.read();
}

// ===================================================================================
// =================================== grayscales ====================================
// ===================================================================================

uint8_t yh::rec::UART_com::grayscales_8_sides_touch_white () {
    return request_data(GRYSCL_8_DIR);
}

uint8_t yh::rec::UART_com::hori_grayscales_touch_white () {
    return request_data(HORI_GRYSCL);
}

uint8_t yh::rec::UART_com::vert_grayscales_touch_white () {
    return request_data(VERT_GRYSCL);
}

void yh::rec::UART_com::request_calibrate_grayscales () {
    request_data(CAL_GRYSCL, 30000000U);
}

// ===================================================================================
// ================================== compoundeyes ===================================
// ===================================================================================

uint16_t yh::rec::UART_com::sun_direction_HIGH () {
    return request_data(SUN_DIR_H) << 2;
}

uint16_t yh::rec::UART_com::sun_intensity_HIGH () {
    return request_data(SUN_VAL_H) << 2;
}

uint8_t yh::rec::UART_com::sun_direction_LOW () {
    return request_data(SUN_DIR_L);
}

uint8_t yh::rec::UART_com::sun_intensity_LOW () {
    return request_data(SUN_VAL_L);
}

// ===================================================================================
// =================================== ultrasounds ===================================
// ===================================================================================

uint16_t yh::rec::UART_com::find_ball_ultrasound_HIGH () {
    return request_data(FB_UTS_H, 688) << 2;
}

uint8_t yh::rec::UART_com::find_ball_ultrasound_LOW () {
    return request_data(FB_UTS_L, 688);
}

uint16_t yh::rec::UART_com::ultrasound_0_HIGH () {
    return request_data(UTS0_H) << 2;
}

uint8_t yh::rec::UART_com::ultrasound_0_LOW () {
    return request_data(UTS0_L);
}

uint16_t yh::rec::UART_com::ultrasound_1_HIGH () {
    return request_data(UTS1_H) << 2;
}

uint8_t yh::rec::UART_com::ultrasound_1_LOW () {
    return request_data(UTS1_L);
}

uint16_t yh::rec::UART_com::ultrasound_2_HIGH () {
    return request_data(UTS2_H) << 2;
}

uint8_t yh::rec::UART_com::ultrasound_2_LOW () {
    return request_data(UTS2_L);
}

uint16_t yh::rec::UART_com::ultrasound_3_HIGH () {
    return request_data(UTS3_H) << 2;
}

uint8_t yh::rec::UART_com::ultrasound_3_LOW () {
    return request_data(UTS3_L);
}

uint16_t yh::rec::UART_com::find_ball_ultrasound_9bit () {
    return request_2_data(FB_UTS_9, 688);
}

uint16_t yh::rec::UART_com::ultrasound_0_9bit () {
    return request_2_data(UTS0_9);
}

uint16_t yh::rec::UART_com::ultrasound_1_9bit () {
    return request_2_data(UTS1_9);
}

uint16_t yh::rec::UART_com::ultrasound_2_9bit () {
    return request_2_data(UTS2_9);
}

uint16_t yh::rec::UART_com::ultrasound_3_9bit () {
    return request_2_data(UTS3_9);
}

// ===================================================================================
// ===================================== compass =====================================
// ===================================================================================

uint16_t yh::rec::UART_com::compass_HIGH () {
    return request_data(CMPAS_H) << 2;
}

uint8_t yh::rec::UART_com::compass_LOW () {
    return request_data(CMPAS_L);
}

uint16_t yh::rec::UART_com::compass_9bit () {
    return request_2_data(CMPAS_9);
}

void yh::rec::UART_com::reset_compass () {
    write_data(RST_CMPAS_0);
}

void yh::rec::UART_com::set_compass_direction (const uint16_t direction) {
    write_data(SET_CMPAS, (direction >> 2) & 0xff, (direction & 0xff));
}

// ===================================================================================
// ==================================== dribbler =====================================
// ===================================================================================

void yh::rec::UART_com::set_dribbler_spd (const int8_t spd) {
    write_data(DRBLR_SPD, spd);
}

#endif // #ifndef UART_COM_CPP