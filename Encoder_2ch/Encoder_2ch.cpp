#ifndef ENCODER_2CH_CPP
#define ENCODER_2CH_CPP

#include "Encoder_2ch.h"

yh::rec::Encoder_2ch_timer_int::Encoder_2ch_timer_int (const Encoder_2ch_timer_int &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    log_idx(0),
    log_len(init_obj.log_len > 1000 ? 1000 : init_obj.log_len),
    current_displacement(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0)
{
    free(init_obj.logged_displacements);
    init_obj.logged_displacements = NULL;
    ( logged_displacements = (DISPLACEMENT_UNIT *)calloc(log_len, sizeof(DISPLACEMENT_UNIT)) );
}

yh::rec::Encoder_2ch_timer_int::Encoder_2ch_timer_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    log_idx(0),
    log_len(request_log_len > 1000 ? 1000 : request_log_len),
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
    pinMode(signal_A_pin, INPUT);
    pinMode(signal_B_pin, INPUT);
    signal_A_input_reg = portInputRegister(digitalPinToPort(signal_A_pin));
    signal_A_mask = digitalPinToBitMask(signal_A_pin);
    signal_B_input_reg = portInputRegister(digitalPinToPort(signal_B_pin));
    signal_B_mask = digitalPinToBitMask(signal_B_pin);
    prev_A_state = (*signal_A_input_reg) & signal_A_mask;
    prev_B_state = (*signal_B_input_reg) & signal_B_mask;
}

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

yh::rec::Encoder_2ch_ext_int::Encoder_2ch_ext_int (const Encoder_2ch_ext_int &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    prev_time(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0),
    full_spd_velocity_val(255),
    full_spd_signal_change_time(84),
    max_waiting_time(full_spd_signal_change_time * full_spd_velocity_val / 0.5)
{
    //
}

yh::rec::Encoder_2ch_ext_int::Encoder_2ch_ext_int (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    prev_time(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0),
    full_spd_velocity_val(255),
    full_spd_signal_change_time(84),
    max_waiting_time(full_spd_signal_change_time * full_spd_velocity_val / 0.5)
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

#endif // #ifndef ENCODER_2CH_CPP