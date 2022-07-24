#ifndef ENCODER_2CH_CPP
#define ENCODER_2CH_CPP

#include "Encoder_2ch.h"

yh::rec::Encoder_2ch_timer_int::Encoder_2ch_timer_int (const Encoder_2ch_timer_int &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    log_idx(0),
    log_len(init_obj.log_len > 1000 ? 1000 : init_obj.log_len),
    logged_displacements(NULL),
    current_displacement(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0)
{
    //
}

yh::rec::Encoder_2ch_timer_int::Encoder_2ch_timer_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    log_idx(0),
    log_len(request_log_len > 1000 ? 1000 : request_log_len),
    logged_displacements(NULL),
    current_displacement(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0)
{
    ( logged_displacements = (DISPLACEMENT_UNIT *)calloc(log_len, sizeof(DISPLACEMENT_UNIT)) );
}

yh::rec::Encoder_2ch_timer_int::~Encoder_2ch_timer_int () {
    if (logged_displacements != NULL)
        free(logged_displacements);
}

void yh::rec::Encoder_2ch_timer_int::begin () {
    ( logged_displacements = (DISPLACEMENT_UNIT *)calloc(log_len, sizeof(DISPLACEMENT_UNIT)) );
    pinMode(signal_A_pin, INPUT);
    pinMode(signal_B_pin, INPUT);
    signal_A_input_reg = portInputRegister(digitalPinToPort(signal_A_pin));
    signal_A_mask = digitalPinToBitMask(signal_A_pin);
    signal_B_input_reg = portInputRegister(digitalPinToPort(signal_B_pin));
    signal_B_mask = digitalPinToBitMask(signal_B_pin);
    prev_A_state = (*signal_A_input_reg) & signal_A_mask;
    prev_B_state = (*signal_B_input_reg) & signal_B_mask;
}

/*
yh::rec::Encoder_2ch_timer_int_light::Encoder_2ch_timer_int_light (const Encoder_2ch_timer_int_light &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    log_idx(0),
    log_len(init_obj.log_len > 1000 ? 1000 : init_obj.log_len),
    current_displacement(0),
    instantaneous_velocity(0)
{
    //
}

yh::rec::Encoder_2ch_timer_int_light::Encoder_2ch_timer_int_light (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    log_idx(0),
    log_len(request_log_len > 1000 ? 1000 : request_log_len),
    current_displacement(0),
    instantaneous_velocity(0)
{
    //
}

void yh::rec::Encoder_2ch_timer_int_light::begin () {
    pinMode(signal_A_pin, INPUT);
    pinMode(signal_B_pin, INPUT);
    signal_A_input_reg = portInputRegister(digitalPinToPort(signal_A_pin));
    signal_A_mask = digitalPinToBitMask(signal_A_pin);
    signal_B_input_reg = portInputRegister(digitalPinToPort(signal_B_pin));
    signal_B_mask = digitalPinToBitMask(signal_B_pin);
    prev_A_state = (*signal_A_input_reg) & signal_A_mask;
    prev_B_state = (*signal_B_input_reg) & signal_B_mask;
}
//*/

yh::rec::Encoder_2ch_ext_int::Encoder_2ch_ext_int (const Encoder_2ch_ext_int &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    prev_time(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0),
    full_spd_velocity_val(255),
    full_spd_signal_change_time(84),
    max_waiting_time(full_spd_signal_change_time * full_spd_velocity_val)
{
    //
}

yh::rec::Encoder_2ch_ext_int::Encoder_2ch_ext_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t resolution, const uint8_t sig_change_time) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    prev_time(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0),
    full_spd_velocity_val(resolution), // also interpreted as the resolution of the velocity
    full_spd_signal_change_time(sig_change_time), // the minimum time for an interrupt to be triggered when the encoder is running at full speed
    max_waiting_time(full_spd_signal_change_time * full_spd_velocity_val)
{
    //
}

void yh::rec::Encoder_2ch_ext_int::begin () {
    pinMode(signal_A_pin, INPUT);
    pinMode(signal_B_pin, INPUT);
    signal_A_input_reg = portInputRegister(digitalPinToPort(signal_A_pin));
    signal_A_mask = digitalPinToBitMask(signal_A_pin);
    signal_B_input_reg = portInputRegister(digitalPinToPort(signal_B_pin));
    signal_B_mask = digitalPinToBitMask(signal_B_pin);
    prev_A_state = (*signal_A_input_reg) & signal_A_mask;
    prev_B_state = (*signal_B_input_reg) & signal_B_mask;
    prev_time = micros();
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

yh::rec::Encoder_1ch_pulse::Encoder_1ch_pulse (const Encoder_1ch_pulse &init_obj) :
    sig_pin(init_obj.sig_pin),
    amplifying_factor(init_obj.amplifying_factor)
{
    //
}

yh::rec::Encoder_1ch_pulse::Encoder_1ch_pulse (const uint8_t sig_pin, const uint16_t resolution, const uint16_t shortest_sig_chng_time) :
    sig_pin(sig_pin),
    amplifying_factor(resolution * shortest_sig_chng_time)
{
    //
}

void yh::rec::Encoder_1ch_pulse::begin () {
    pinMode(sig_pin, INPUT);
    sig_pin_in_reg = portInputRegister(digitalPinToPort(sig_pin));
    sig_pin_mask = digitalPinToBitMask(sig_pin);
}

uint16_t yh::rec::Encoder_1ch_pulse::get_spd_simple () {
    // unsigned long delta_time = pulseIn(sig_pin, !digitalRead(sig_pin), amplifying_factor);
    // if (delta_time == 0)
    //     return 0;
    // return amplifying_factor / delta_time;

    #if F_CPU == 16000000UL
    const unsigned long maxloops = amplifying_factor;
    #else
    const unsigned long maxloops = microsecondsToClockCycles(amplifying_factor)/16;
    #endif

    const uint8_t sig_pin_prev_state = ((*sig_pin_in_reg) & sig_pin_mask);
    const uint8_t stateMask = (sig_pin_prev_state ^ sig_pin_mask);

    const unsigned long width = countPulseASM(sig_pin_in_reg, sig_pin_mask, stateMask, maxloops);

    if (width)
        return amplifying_factor / clockCyclesToMicroseconds(width * 16 + 16);
    else
        return 0;
}

uint16_t yh::rec::Encoder_1ch_pulse::get_spd_mean () {
    const unsigned long prev = micros();
    uint32_t temp_spd = 0;
    uint16_t i = 0;
    while ((micros() - prev) < 10000) {
        temp_spd += get_spd_simple();
        i++;
    }
    uint16_t mean_spd = 0;
    while (temp_spd >= i) {
        temp_spd -= i;
        mean_spd++;
    }
    return mean_spd;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------------------

yh::rec::Encoder_2ch_pulse::Encoder_2ch_pulse (const Encoder_2ch_pulse &init_obj) :
    sig_pin(init_obj.sig_pin),
    dir_pin(init_obj.dir_pin),
    amplifying_factor(init_obj.amplifying_factor)
{
    //
}

yh::rec::Encoder_2ch_pulse::Encoder_2ch_pulse (const uint8_t sig_A_pin, const uint8_t sig_B_pin, const uint16_t resolution, const uint16_t shortest_sig_chng_time) :
    sig_pin(sig_A_pin),
    dir_pin(sig_B_pin),
    amplifying_factor(resolution * shortest_sig_chng_time)
{
    //
}

void yh::rec::Encoder_2ch_pulse::begin () {
    pinMode(sig_pin, INPUT);
    pinMode(dir_pin, INPUT);
    sig_pin_in_reg = portInputRegister(digitalPinToPort(sig_pin));
    sig_pin_mask = digitalPinToBitMask(sig_pin);
    dir_pin_in_reg = portInputRegister(digitalPinToPort(dir_pin));
    dir_pin_mask = digitalPinToBitMask(dir_pin);
}

int16_t yh::rec::Encoder_2ch_pulse::get_spd_simple () {
    #if F_CPU == 16000000UL
    const unsigned long maxloops = amplifying_factor;
    #else
    const unsigned long maxloops = microsecondsToClockCycles(amplifying_factor)/16;
    #endif

    const uint8_t sig_pin_prev_state = ((*sig_pin_in_reg) & sig_pin_mask);
    const uint8_t stateMask = (sig_pin_prev_state ^ sig_pin_mask);

    const unsigned long width = countPulseASM(sig_pin_in_reg, sig_pin_mask, stateMask, maxloops);

    const uint8_t dir_pin_curr_state = ((*dir_pin_in_reg) & dir_pin_mask);

    if (width) {
        uint32_t numerator = amplifying_factor;
        int16_t absolute_spd = 0;
        const unsigned long actual_time = clockCyclesToMicroseconds(width * 16U + 16U);
        while (numerator >= actual_time) {
            numerator -= actual_time;
            absolute_spd++;
        }
        if ((dir_pin_curr_state ? 1 : 0) ^ (sig_pin_prev_state ? 1 : 0)) {
            return -absolute_spd;
        } else {
            return absolute_spd;
        }
    } else {
        return 0;
    }
}

int16_t yh::rec::Encoder_2ch_pulse::get_spd_mean () {
    const unsigned long prev = micros();
    int32_t temp_spd = 0;
    uint8_t i = 0;
    while ((micros() - prev) < 2500) {
        temp_spd += get_spd_simple();
        i++;
    }
    return temp_spd / i;
}

#endif // #ifndef ENCODER_2CH_CPP