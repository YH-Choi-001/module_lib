#ifndef BC_COM_CPP
#define BC_COM_CPP __DATE__ ", " __TIME__

#include "BC_com.h"

yh::rec::BC_com_master::BC_com_master () :
    request_timeout(0)
{
    //
}

uint8_t yh::rec::BC_com_master::get_who_am_i () {
    return request_data(WHO_AM_I, 10000);
}

// ===================================================================================
// ====================================== motor ======================================
// ===================================================================================

void yh::rec::BC_com_master::set_motor_direction (const uint8_t direction) {
    write_data(SET_MOTOR_DIR, direction);
}

void yh::rec::BC_com_master::set_motor_0_spd (const uint8_t direction) {
    write_data(MOTOR_0_SPD, direction);
}

void yh::rec::BC_com_master::set_motor_1_spd (const uint8_t direction) {
    write_data(MOTOR_1_SPD, direction);
}

void yh::rec::BC_com_master::set_motor_2_spd (const uint8_t direction) {
    write_data(MOTOR_2_SPD, direction);
}

void yh::rec::BC_com_master::set_motor_3_spd (const uint8_t direction) {
    write_data(MOTOR_3_SPD, direction);
}

// ===================================================================================
// =================================== grayscales ====================================
// ===================================================================================

uint8_t yh::rec::BC_com_master::grayscales_8_sides_touch_white () {
    return request_data(GRYSCL_8_DIR);
}

uint8_t yh::rec::BC_com_master::hori_grayscales_touch_white () {
    return request_data(HORI_GRYSCL);
}

uint8_t yh::rec::BC_com_master::vert_grayscales_touch_white () {
    return request_data(VERT_GRYSCL);
}

void yh::rec::BC_com_master::request_calibrate_grayscales () {
    request_data(CAL_GRYSCL, 30000000U);
}

// ===================================================================================
// ================================== compoundeyes ===================================
// ===================================================================================

uint16_t yh::rec::BC_com_master::sun_direction_HIGH () {
    return request_data(SUN_DIR_H) << 1;
}

uint16_t yh::rec::BC_com_master::sun_intensity_HIGH () {
    return request_data(SUN_VAL_H) << 1;
}

uint8_t yh::rec::BC_com_master::sun_direction_LOW () {
    return request_data(SUN_DIR_L);
}

uint8_t yh::rec::BC_com_master::sun_intensity_LOW () {
    return request_data(SUN_VAL_L);
}

uint16_t yh::rec::BC_com_master::filter_ball_direction_HIGH () {
    return request_data(FILT_DIR_H) << 1;
}

uint16_t yh::rec::BC_com_master::filter_ball_intensity_HIGH () {
    return request_data(FILT_VAL_H) << 1;
}

uint8_t yh::rec::BC_com_master::filter_ball_direction_LOW () {
    return request_data(FILT_DIR_L);
}

uint8_t yh::rec::BC_com_master::filter_ball_intensity_LOW () {
    return request_data(FILT_VAL_L);
}

// ===================================================================================
// =================================== ultrasounds ===================================
// ===================================================================================

uint16_t yh::rec::BC_com_master::find_ball_ultrasound_HIGH () {
    return request_data(FB_UTS_H, 688) << 1;
}

uint8_t yh::rec::BC_com_master::find_ball_ultrasound_LOW () {
    return request_data(FB_UTS_L, 688);
}

uint16_t yh::rec::BC_com_master::ultrasound_0_HIGH () {
    return request_data(UTS0_H) << 1;
}

uint8_t yh::rec::BC_com_master::ultrasound_0_LOW () {
    return request_data(UTS0_L);
}

uint16_t yh::rec::BC_com_master::ultrasound_1_HIGH () {
    return request_data(UTS1_H) << 1;
}

uint8_t yh::rec::BC_com_master::ultrasound_1_LOW () {
    return request_data(UTS1_L);
}

uint16_t yh::rec::BC_com_master::ultrasound_2_HIGH () {
    return request_data(UTS2_H) << 1;
}

uint8_t yh::rec::BC_com_master::ultrasound_2_LOW () {
    return request_data(UTS2_L);
}

uint16_t yh::rec::BC_com_master::ultrasound_3_HIGH () {
    return request_data(UTS3_H) << 1;
}

uint8_t yh::rec::BC_com_master::ultrasound_3_LOW () {
    return request_data(UTS3_L);
}

uint16_t yh::rec::BC_com_master::find_ball_ultrasound_9bit () {
    return request_2_data(FB_UTS_9, 688);
}

uint16_t yh::rec::BC_com_master::ultrasound_0_9bit () {
    return request_2_data(UTS0_9);
}

uint16_t yh::rec::BC_com_master::ultrasound_1_9bit () {
    return request_2_data(UTS1_9);
}

uint16_t yh::rec::BC_com_master::ultrasound_2_9bit () {
    return request_2_data(UTS2_9);
}

uint16_t yh::rec::BC_com_master::ultrasound_3_9bit () {
    return request_2_data(UTS3_9);
}

void yh::rec::BC_com_master::set_ultrasound_0_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS0_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::BC_com_master::set_ultrasound_1_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS1_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::BC_com_master::set_ultrasound_2_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS2_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

void yh::rec::BC_com_master::set_ultrasound_3_range (const uint16_t dist_in_cm) {
    write_data(SET_UTS3_RNG, (dist_in_cm >> 8), (dist_in_cm & 0xff));
}

// ===================================================================================
// ===================================== compass =====================================
// ===================================================================================

uint16_t yh::rec::BC_com_master::compass_HIGH () {
    return request_data(CMPAS_H) << 1;
}

uint8_t yh::rec::BC_com_master::compass_LOW () {
    return request_data(CMPAS_L);
}

uint16_t yh::rec::BC_com_master::compass_9bit () {
    return request_2_data(CMPAS_9);
}

void yh::rec::BC_com_master::reset_compass () {
    write_key(RST_CMPAS_0);
}

void yh::rec::BC_com_master::set_compass_direction (const uint16_t direction) {
    write_data(SET_CMPAS, (direction >> 1) & 0xff, (direction & 0xff));
}

// ===================================================================================
// ==================================== dribbler =====================================
// ===================================================================================

void yh::rec::BC_com_master::set_dribbler_spd (const int8_t spd) {
    write_data(DRBLR_SPD, spd);
}

// ===================================================================================
// ===================================== testcom =====================================
// ===================================================================================

uint8_t yh::rec::BC_com_master::test_com_stable () {
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

yh::rec::BC_com_slave::BC_com_slave () :
    who_am_i('3')
{
    //
}

// not updated
/*
{
            // capital letters first
            FB_UTS_H       = 'A', // find-ball ultrasound HIGH byte
            // SET_BAUD       = 'B', // set baud rate
            CMPAS_H        = 'C', // compass HIGH byte
            DRBLR_SPD      = 'D', // set dribbler speed (-1, 0, 1)
            SUN_DIR_H      = 'E', // the direction of the strongest ball (Sun) HIGH byte
            SUN_VAL_H      = 'F', // the intensity of the strongest ball (Sun) HIGH byte
            GRYSCL_8_DIR   = 'G', // 8 simple directions of grayscale (F, LF, L, LB, RF, R, RB, B)
            HORI_GRYSCL    = 'H', // horizontal grayscales (backup)
            FILT_DIR_H     = 'I', // the direction of the filtered ball HIGH byte
            FILT_VAL_H     = 'J', // the intensity of the filtered ball HIGH byte
            UTS0_9         = 'K', // 9-bit ultrasound 0
            UTS1_9         = 'L', // 9-bit ultrasound 1
            UTS2_9         = 'M', // 9-bit ultrasound 2
            UTS3_9         = 'N', // 9-bit ultrasound 3
            FB_UTS_9       = 'O', // 9-bit find-ball ultrasound
            CMPAS_9        = 'P', // 9-bit compass
            RST_CMPAS_0    = 'R', // reset compass to 0
            SET_CMPAS      = 'S', // set compass to an inputed direction
            MIRROR_TEST    = 'T', // 8-bit test communication stability mode
            VERT_GRYSCL    = 'V', // vertical grayscales (backup)
            UTS0_H         = 'W', // ultrasound 0 HIGH byte
            UTS1_H         = 'X', // ultrasound 1 HIGH byte
            UTS2_H         = 'Y', // ultrasound 2 HIGH byte
            UTS3_H         = 'Z', // ultrasound 3 HIGH byte
            // small letters next
            FB_UTS_L       = 'a', // find-ball ultrasound LOW byte
            CMPAS_L        = 'c', // compass LOW byte
            SUN_DIR_L      = 'e', // the direction of the strongest ball (Sun) LOW byte
            SUN_VAL_L      = 'f', // the intensity of the strongest ball (Sun) LOW byte
            CAL_GRYSCL     = 'g', // request calibrate grayscales
            FILT_DIR_L     = 'i', // the direction of the filtered ball LOW byte
            FILT_VAL_L     = 'j', // the intensity of the filtered ball LOW byte
            SET_UTS0_RNG   = 'k', // set the upper limit of ultrasound 0
            SET_UTS1_RNG   = 'l', // set the upper limit of ultrasound 1
            SET_UTS2_RNG   = 'm', // set the upper limit of ultrasound 2
            SET_UTS3_RNG   = 'n', // set the upper limit of ultrasound 3
            SET_MOTOR_DIR  = 'o', // set the direction of the motor
            MOTOR_0_SPD    = 'p', // set the speed of motor 0
            MOTOR_1_SPD    = 'q', // set the speed of motor 1
            MOTOR_2_SPD    = 'r', // set the speed of motor 2
            MOTOR_3_SPD    = 's', // set the speed of motor 3
            QUIT_MIRROR    = 't', // quit 8-bit testing com stable mode
            WHO_AM_I       = 0x75,// get the identity of the replying arduino
            UTS0_L         = 'w', // ultrasound 0 LOW byte
            UTS1_L         = 'x', // ultrasound 1 LOW byte
            UTS2_L         = 'y', // ultrasound 2 LOW byte
            UTS3_L         = 'z'  // ultrasound 3 LOW byte
            // the priority of returning data from replying arduino:
                // 0. grayscale   (8-DIR > HORI > VERT) // should not access grayscale through UART, but digital pins
                // 1. compass     (HIGH > 9-bit > LOW)
                // 2. compoundeye (HIGH > LOW)
                // 3. ultrasound  (HIGH > 9-bit > LOW)
            // then the priority of configuring data in replying aruino:
                // 4. set motor dir and speed
                // 5. set dribbler speed
                // 6. calibrate grayscale
                // 7. software reset compass
                // 8. software set compass direction
                // 9. set range of ultrasounds
                // 10. return WHO_AM_I
        };
*/
void yh::rec::BC_com_slave::on_receive () {
    switch (read_key_or_data()) {
        // grayscales
        case GRYSCL_8_DIR:
            write_data(grayscales_8_sides_touch_white());
            break;
        case HORI_GRYSCL:
            write_data(hori_grayscales_touch_white());
            break;
        case VERT_GRYSCL:
            write_data(vert_grayscales_touch_white());
            break;
        // compass
        case CMPAS_H:
            write_data(compass_HIGH());
            break;
        case CMPAS_9:
            write_data_9bit(compass_9bit());
            break;
        case CMPAS_L:
            write_data(compass_LOW());
            break;
        // compoundeyes
        case SUN_DIR_H:
            write_data(sun_direction_HIGH());
            break;
        case SUN_VAL_H:
            write_data(sun_intensity_HIGH());
            break;
        case SUN_DIR_L:
            write_data(sun_direction_LOW());
            break;
        case SUN_VAL_L:
            write_data(sun_intensity_LOW());
            break;
        // ultrasounds
        case FB_UTS_H:
            write_data(find_ball_ultrasound_HIGH());
            break;
        case UTS0_H:
            write_data(ultrasound_0_HIGH());
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