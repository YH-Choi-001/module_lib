#ifndef HC_SR04_CPP
#define HC_SR04_CPP __DATE__ ", " __TIME__

#include "Hc_sr04.h"

namespace yh {
    namespace rec {
        // get the protected: Uts::dist_read for ISR ONLY
        // uint16_t *dist_read [6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        // get the protected: Uts::trig_time for ISR ONLY
        // unsigned long *trig_time [6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        // get the protected: Uts::trig_time for ISR ONLY
        // bool *return_wave_received [6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    }
}

yh::rec::Hc_sr04::Hc_sr04 (const Hc_sr04 &init_obj) :
    trig_pin(init_obj.trig_pin), echo_pin(init_obj.echo_pin)// , return_wave_received(init_obj.return_wave_received)
{
    // if (digitalPinToInterrupt(echo_pin) != NOT_AN_INTERRUPT) {
    //     // yh::rec::dist_read[digitalPinToInterrupt(echo_pin)] = &(this->dist_read);
    //     // yh::rec::trig_time[digitalPinToInterrupt(echo_pin)] = &(this->trig_time);
    //     // yh::rec::return_wave_received[digitalPinToInterrupt(echo_pin)] = &(this->return_wave_received);
    // }
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

yh::rec::Hc_sr04::Hc_sr04 (const uint16_t init_trig_and_echo_pin) :
    trig_pin(init_trig_and_echo_pin >> 8), echo_pin(init_trig_and_echo_pin & 0xff)// , return_wave_received(true)
{
    // if (digitalPinToInterrupt(echo_pin) != NOT_AN_INTERRUPT) {
    //     // yh::rec::dist_read[digitalPinToInterrupt(echo_pin)] = &(this->dist_read);
    //     // yh::rec::trig_time[digitalPinToInterrupt(echo_pin)] = &(this->trig_time);
    //     // yh::rec::return_wave_received[digitalPinToInterrupt(echo_pin)] = &(this->return_wave_received);
    // }
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

yh::rec::Hc_sr04::Hc_sr04 (const uint8_t init_trig_pin, const uint8_t init_echo_pin) :
    trig_pin(init_trig_pin), echo_pin(init_echo_pin)// , return_wave_received(true)
{
    // if (digitalPinToInterrupt(echo_pin) != NOT_AN_INTERRUPT) {
    //     // yh::rec::dist_read[digitalPinToInterrupt(echo_pin)] = &(this->dist_read);
    //     // yh::rec::trig_time[digitalPinToInterrupt(echo_pin)] = &(this->trig_time);
    //     // yh::rec::return_wave_received[digitalPinToInterrupt(echo_pin)] = &(this->return_wave_received);
    // }
    trig_pin_mask = (digitalPinToBitMask(trig_pin));
    trig_pin_output_port = (portOutputRegister(digitalPinToPort(trig_pin)));
}

inline void yh::rec::Hc_sr04::begin () {
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
}

void yh::rec::Hc_sr04::trig_wave () {
    // HC-SR04 usage:
    // set trig_pin low for 2 ms
    // set trig_pin high for 10 ms
    // set trig_pin low
    // read the length of high pulse on echo pin with arduino built-in function pulseIn()
    (*trig_pin_output_port) &= ~trig_pin_mask; // digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // send the pulse
    (*trig_pin_output_port) |= trig_pin_mask; // digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    (*trig_pin_output_port) &= ~trig_pin_mask; // digitalWrite(trig_pin, LOW);
    // trig_time = micros();

}

/*
void yh::rec::Hc_sr04::trig_wave_and_wait () {
    // HC-SR04 usage:
    // set trig_pin low for 2 ms
    // set trig_pin high for 10 ms
    // set trig_pin low
    // read the length of high pulse on echo pin with arduino built-in function pulseIn()
    // return_wave_received = false;
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(2);
    // send the pulse
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    while (!digitalRead(echo_pin)) {} // wait till echo pin raises
    // trig_time = micros();

}
//*/

uint16_t yh::rec::Hc_sr04::raw_uts_by_pulseIn (const unsigned long limiting_time_in_us) {
    // HC-SR04 usage:
    // set trig_pin low for 2 ms
    // set trig_pin high for 10 ms
    // set trig_pin low
    // read the length of high pulse on echo pin with arduino built-in function pulseIn()
    const unsigned long duration_in_us = pulseIn(echo_pin, HIGH, limiting_time_in_us);
    dist_read_mm = (duration_in_us ? duration_in_us * 0.17 : 8888); // duration_in_us* 0.17 is from duration_in_us * 0.034 / 2, then because using mm, * 10
    if (dist_read_mm > 4000) dist_read_mm = 8888;
    return dist_read_mm;
}

/*
void yh::rec::Hc_sr04::int_2_coll_echo_data_template () { // ISR for echo pin
    unsigned long duration_in_us = micros() - trig_time;
    dist_read = (duration_in_us > 13000UL ? 888 : duration_in_us * 0.017); // duration_in_us * 0.017 is from duration_in_us * 0.034 / 2
    if (dist_read > 400) dist_read = 888;
}

#define INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(pin,int_num) \
void int_2_coll_echo_data_##pin () { \
    const unsigned long duration_in_us = micros() - (*(yh::rec::trig_time[int_num])); \
    (*(yh::rec::dist_read[int_num])) = (duration_in_us > 13000UL ? 399 : duration_in_us * 0.017); \
    if ( (*(yh::rec::dist_read[int_num])) > 400 ) (*(yh::rec::dist_read[int_num])) = 401; \
    (*(yh::rec::return_wave_received[int_num])) = true; \
    detachInterrupt(int_num); \
}

INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(2,digitalPinToInterrupt(2))
INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(3,digitalPinToInterrupt(3))
INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(18,digitalPinToInterrupt(18))
INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(19,digitalPinToInterrupt(19))
INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(20,digitalPinToInterrupt(20))
INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION(21,digitalPinToInterrupt(21))

#undef INT_2_COLL_ECHO_DATA_ISR_IMPLEMENTATION

inline void yh::rec::Hc_sr04::attach_data_collecting_method_to_interrupt () {
    // attachInterrupt(digitalPinToInterrupt(echo_pin), (void (*)(void))int_2_coll_echo_data_template, LOW); // not allowed
    noInterrupts();
    switch (echo_pin) {
        #define ATTACH_INTERRUPT_(pin) \
        case pin: \
            attachInterrupt(digitalPinToInterrupt(pin), int_2_coll_echo_data_##pin, LOW); \
            break;
        ATTACH_INTERRUPT_(2)
        ATTACH_INTERRUPT_(3)
        ATTACH_INTERRUPT_(18)
        ATTACH_INTERRUPT_(19)
        ATTACH_INTERRUPT_(20)
        ATTACH_INTERRUPT_(21)
        #undef ATTACH_INTERRUPT_
    }
    interrupts();
}

inline void yh::rec::Hc_sr04::detach_data_collecting_method_to_interrupt () {
    detachInterrupt(digitalPinToInterrupt(echo_pin));
}
//*/

uint16_t yh::rec::Hc_sr04::read_dist_mm (const bool refresh, const unsigned long limiting_time_in_us) {
    if (refresh) {
        trig_wave();
        raw_uts_by_pulseIn(limiting_time_in_us);
    }
    return dist_read_mm;
}

uint16_t yh::rec::Hc_sr04::read_dist_cm (const bool refresh, const unsigned long limiting_time_in_us) {
    if (refresh) {
        trig_wave();
        raw_uts_by_pulseIn(limiting_time_in_us);
    }
    return dist_read_mm / 10U;
}

/*
inline void yh::rec::Hc_sr04::timer_ISR () {
    if ((micros() - trig_time) > 23530UL) {
        // shut the interrupt and return 888
        detach_data_collecting_method_to_interrupt();
        return_wave_received = true;
        dist_read = 888;
    }
    if (return_wave_received) {
        // activate another ultrasound session again
        trig_wave(); // for most of the time, no need to call Uts::trig_wave_and_wait()
        attach_data_collecting_method_to_interrupt();
    }
}

inline void yh::rec::Hc_sr04::turnoff_interrupt_on_echo_pin () {
    detach_data_collecting_method_to_interrupt();
}
//*/

#endif //#ifndef HC_SR04_CPP