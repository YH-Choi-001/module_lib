#ifndef ENCODER_2CH_CPP
#define ENCODER_2CH_CPP

#include "Encoder_2ch.h"

yh::rec::Encoder_2ch::Encoder_2ch (Encoder_2ch &init_obj) :
    signal_A_pin(init_obj.signal_A_pin),
    signal_B_pin(init_obj.signal_B_pin),
    log_len(init_obj.log_len > 1000 ? 1000 : init_obj.log_len),
    current_displacement(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0)
{
    free(init_obj.logged_displacements);
    init_obj.logged_displacements = NULL;
    ( logged_displacements = (DISPLACEMENT_UNIT *)calloc(log_len, sizeof(DISPLACEMENT_UNIT)) );
}
yh::rec::Encoder_2ch::Encoder_2ch (const uint8_t init_signal_A_pin, const uint8_t init_signal_B_pin, const uint16_t request_log_len) :
    signal_A_pin(init_signal_A_pin),
    signal_B_pin(init_signal_B_pin),
    log_len(request_log_len > 1000 ? 1000 : request_log_len),
    current_displacement(0),
    instantaneous_velocity(0),
    instantaneous_acceleration(0)
{
    ( logged_displacements = (DISPLACEMENT_UNIT *)calloc(log_len, sizeof(DISPLACEMENT_UNIT)) );
}

yh::rec::Encoder_2ch::~Encoder_2ch () {
    if (logged_displacements != NULL)
        free(logged_displacements);
}

void yh::rec::Encoder_2ch::begin () {
    pinMode(signal_A_pin, INPUT);
    pinMode(signal_B_pin, INPUT);
    signal_A_input_reg = portInputRegister(digitalPinToPort(signal_A_pin));
    signal_A_mask = digitalPinToBitMask(signal_A_pin);
    signal_B_input_reg = portInputRegister(digitalPinToPort(signal_B_pin));
    signal_B_mask = digitalPinToBitMask(signal_B_pin);
    prev_A_state = (*signal_A_input_reg) & signal_A_mask;
    prev_B_state = (*signal_B_input_reg) & signal_B_mask;
}

#endif // #ifndef ENCODER_2CH_CPP