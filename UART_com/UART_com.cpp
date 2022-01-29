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
    const uint8_t who_am_i = get_who_am_i();
    who_am_i_error = ((who_am_i < '0') || (who_am_i > '9')); // checks if the who_am_i ranges from 0x30 to 0x39
}

uint8_t yh::rec::UART_com::get_who_am_i () {
    return request_data(WHO_AM_I, 10000);
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
// ====================================== motor ======================================
// ===================================================================================

void yh::rec::UART_com::set_motor_direction (const uint8_t direction) {
    write_data(SET_MOTOR_DIR, direction);
}

void yh::rec::UART_com::set_motor_0_spd (const uint8_t direction) {
    write_data(MOTOR_0_SPD, direction);
}

void yh::rec::UART_com::set_motor_1_spd (const uint8_t direction) {
    write_data(MOTOR_1_SPD, direction);
}

void yh::rec::UART_com::set_motor_2_spd (const uint8_t direction) {
    write_data(MOTOR_2_SPD, direction);
}

void yh::rec::UART_com::set_motor_3_spd (const uint8_t direction) {
    write_data(MOTOR_3_SPD, direction);
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
    return request_data(SUN_DIR_H) << 1;
}

uint16_t yh::rec::UART_com::sun_intensity_HIGH () {
    return request_data(SUN_VAL_H) << 1;
}

uint8_t yh::rec::UART_com::sun_direction_LOW () {
    return request_data(SUN_DIR_L);
}

uint8_t yh::rec::UART_com::sun_intensity_LOW () {
    return request_data(SUN_VAL_L);
}

uint16_t yh::rec::UART_com::filter_ball_direction_HIGH () {
    return request_data(FILT_DIR_H) << 1;
}

uint16_t yh::rec::UART_com::filter_ball_intensity_HIGH () {
    return request_data(FILT_VAL_H) << 1;
}

uint8_t yh::rec::UART_com::filter_ball_direction_LOW () {
    return request_data(FILT_DIR_L);
}

uint8_t yh::rec::UART_com::filter_ball_intensity_LOW () {
    return request_data(FILT_VAL_L);
}

// ===================================================================================
// =================================== ultrasounds ===================================
// ===================================================================================

uint16_t yh::rec::UART_com::find_ball_ultrasound_HIGH () {
    return request_data(FB_UTS_H, 688) << 1;
}

uint8_t yh::rec::UART_com::find_ball_ultrasound_LOW () {
    return request_data(FB_UTS_L, 688);
}

uint16_t yh::rec::UART_com::ultrasound_0_HIGH () {
    return request_data(UTS0_H) << 1;
}

uint8_t yh::rec::UART_com::ultrasound_0_LOW () {
    return request_data(UTS0_L);
}

uint16_t yh::rec::UART_com::ultrasound_1_HIGH () {
    return request_data(UTS1_H) << 1;
}

uint8_t yh::rec::UART_com::ultrasound_1_LOW () {
    return request_data(UTS1_L);
}

uint16_t yh::rec::UART_com::ultrasound_2_HIGH () {
    return request_data(UTS2_H) << 1;
}

uint8_t yh::rec::UART_com::ultrasound_2_LOW () {
    return request_data(UTS2_L);
}

uint16_t yh::rec::UART_com::ultrasound_3_HIGH () {
    return request_data(UTS3_H) << 1;
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

void yh::rec::UART_com::set_ultrasound_0_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS0_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::UART_com::set_ultrasound_1_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS1_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::UART_com::set_ultrasound_2_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS2_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::UART_com::set_ultrasound_3_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS3_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

// ===================================================================================
// ===================================== compass =====================================
// ===================================================================================

uint16_t yh::rec::UART_com::compass_HIGH () {
    return request_data(CMPAS_H) << 1;
}

uint8_t yh::rec::UART_com::compass_LOW () {
    return request_data(CMPAS_L);
}

uint16_t yh::rec::UART_com::compass_9bit () {
    return request_2_data(CMPAS_9);
}

void yh::rec::UART_com::reset_compass () {
    write_key(RST_CMPAS_0);
}

void yh::rec::UART_com::set_compass_direction (const uint16_t direction) {
    write_data(SET_CMPAS, (direction >> 1) & 0xff, (direction & 0xff));
}

// ===================================================================================
// ==================================== dribbler =====================================
// ===================================================================================

void yh::rec::UART_com::set_dribbler_spd (const int8_t spd) {
    write_data(DRBLR_SPD, spd);
}

// ===================================================================================
// ===================================== testcom =====================================
// ===================================================================================

uint8_t yh::rec::UART_com::test_com_stable () {
    uint8_t error_count = 0;
    write_key(MIRROR_TEST);
    for (uint8_t i = 0; i < QUIT_MIRROR; i++) {
        if (request_data(i) != i) error_count++;
    }
    for (uint8_t i = QUIT_MIRROR + 1; i < 255; i++) {
        if (request_data(i) != i) error_count++;
    }
    write_key(QUIT_MIRROR);
    return error_count;
}

// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

yh::rec::UART_com_slave::UART_com_slave (Serial_ &init_serial_obj) :
    uart_serial(init_serial_obj), who_am_i('3')
{
    //
}

inline void yh::rec::UART_com_slave::begin (const uint32_t baud) {
    uart_serial.begin(baud);
}

void yh::rec::UART_com_slave::write_data (const uint8_t data) {
    uart_serial.write(data);
}

void yh::rec::UART_com_slave::write_2_data (const uint16_t data) {
    uart_serial.write(data >> 8);
    uart_serial.write(data & 0xff);
}

// not updated
void yh::rec::UART_com_slave::serial_received_key () {
    switch (uart_serial.read()) {
        // grayscales
        case GRYSCL_8_DIR:
            break;
        case HORI_GRYSCL:
            break;
        case VERT_GRYSCL:
            break;
        // compass
        case CMPAS_H:
            break;
        case CMPAS_9:
            break;
        case CMPAS_L:
            break;
        // compoundeyes
        case SUN_DIR_H:
            break;
        case SUN_VAL_H:
            break;
        case SUN_DIR_L:
            break;
        case SUN_VAL_L:
            break;
        // ultrasounds
        case FB_UTS_H:
            break;
        case UTS0_H:
            break;
        case UTS1_H:
            break;
        case UTS2_H:
            break;
        case UTS3_H:
            break;
        case FB_UTS_9:
            break;
        case UTS0_9:
            break;
        case UTS1_9:
            break;
        case UTS2_9:
            break;
        case UTS3_9:
            break;
        case FB_UTS_L:
            break;
        case UTS0_L:
            break;
        case UTS1_L:
            break;
        case UTS2_L:
            break;
        case UTS3_L:
            break;
        // motors
        case SET_MOTOR_DIR:
            break;
        case MOTOR_0_SPD:
            break;
        case MOTOR_1_SPD:
            break;
        case MOTOR_2_SPD:
            break;
        case MOTOR_3_SPD:
            break;
        // dribbler
        case DRBLR_SPD:
            break;
        // calibrate grayscale
        case CAL_GRYSCL:
            break;
        // reset compass to 0
        case RST_CMPAS_0:
            break;
        // set compass
        case SET_CMPAS:
            break;
        // who am I
        case WHO_AM_I:
            break;
    }
}

#endif // #ifndef UART_COM_CPP