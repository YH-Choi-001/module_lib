#if 0

//#define HC_SR04_USE_COUNTER_TICKS 1
#define HC_SR04_USE_TEST_TIMER 1

#include <Hc_sr04.h>
#include <Led_w.h>

// copied from Timer_int.h

inline int8_t setup_timer_1A_interrupt (const unsigned long us_per_interrupt, const uint16_t request_prescaler = 0);

yh::rec::Hc_sr04_timer_int_beta uts (12, 13);

yh::rec::Led_w_fast leds [8] = {
  2,3,4,5,6,7,8,9
};

void setup () {
//    Serial.begin(2000000);
    for (int i = 0; i < 8; i++) {
      leds[i].begin();
    }
    uts.begin();
    setup_timer_1A_interrupt(59);
}

ISR(TIMER1_COMPA_vect) {
    uts.timer_int_isr_update();
}

void loop () {
//    uts.timer_int_isr_update();
//    Serial.println(uts.read_dist_cm());
    leds[0].set_led(uts.read_dist_cm() & 0x01);
    leds[1].set_led(uts.read_dist_cm() & 0x02);
    leds[2].set_led(uts.read_dist_cm() & 0x04);
    leds[3].set_led(uts.read_dist_cm() & 0x08);
    leds[4].set_led(uts.read_dist_cm() & 0x10);
    leds[5].set_led(uts.read_dist_cm() & 0x20);
    leds[6].set_led(uts.read_dist_cm() & 0x40);
    leds[7].set_led(uts.read_dist_cm() == 888);
    uts.polling_refresh();
}

// copied from Timer_int.cpp

inline int8_t setup_timer_1A_interrupt (const unsigned long us_per_interrupt, const uint16_t request_prescaler) {
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0

  // prescaler is used to store the desired speed for the timer to increment
  // the larger the value, the slower the timer will increment
  // prescaler can only be 1, 8, 64, 256, 1024
  //
  // timer speed (Hz) == 16 MHz / prescaler
  //
  // the fastest increment speed == 16 MHz == 0.0000000625s per increment (achieved by setting prescaler to 1)
  //                                      0.0625us per increment
  //
  // the slowest increment speed == 15625 Hz == 0.000064s per increment (achieved by setting prescaler to 1024)
  //                                                 64 us per increment

// when prescaler is 1,
// desired_interrupt_frequency must be greater than 244.13689976654408959888531427395719219930469989776611 Hz

// when prescaler is 1024,
// desired_interrupt_frequency must be greater than 0.23841494117826571249891143972066132050713349599391 Hz

// us_per_interrupt < 4096.0625

      // OCR1A = (16*10^6) / (   prescaler * (interrupts per every second in Hz )   ) - 1 (must be <65536)
      // OCR1A =  16*10^6  / (   prescaler * ( 1 / (us_per_interrupt / 1000000) )   ) - 1 (must be <65536)
      // OCR1A =  16*10^6  / (   prescaler * (    1000000 / us_per_interrupt    )   ) - 1 (must be <65536)
      // OCR1A =  16*10^6  / (   prescaler * (       10^6 / us_per_interrupt    )   ) - 1 (must be <65536)
      // OCR1A =  16*10^6  / (   prescaler *         10^6 / us_per_interrupt        ) - 1 (must be <65536)
      // OCR1A =  16*10^6  * (   us_per_interrupt /         prescaler / 10^6        ) - 1 (must be <65536)
      // OCR1A =  16 * 10^6 *    us_per_interrupt /         prescaler / 10^6          - 1 (must be <65536)
      // OCR1A =  16           * us_per_interrupt /         prescaler                 - 1 (must be <65536)
      // OCR1A = 16 * us_per_interrupt / prescaler - 1 (must be <65536)

      // 16 * us_per_interrupt / prescaler - 1 < 65536
      // 16 * us_per_interrupt / prescaler     < 65537
      //      us_per_interrupt / prescaler     < 65537 / 16
      //      us_per_interrupt                 < prescaler * 65537 / 16


      // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536

      // // template:
      // if (us_per_interrupt < request_prescaler * 4096.0625) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
      //   // request_prescaler is suitable
      //   // set bit for prescaler
      //   if (request_prescaler == 1) TCCR1B |= (1 << CS10);
      //   else if (request_prescaler == 8) TCCR1B |= (1 << CS11);
      //   else if (request_prescaler == 64) TCCR1B |= (1 << CS11) | (1 << CS10);
      //   else if (request_prescaler == 256) TCCR1B |= (1 << CS12);
      //   else if (request_prescaler == 1024) TCCR1B |= (1 << CS12) | (1 << CS10);
      //   // set compare match register
      //   OCR1A = static_cast<uint16_t>(16 * us_per_interrupt / request_prescaler - 1);
      // }
      // else {
      //   return -1;
      // }

if (!request_prescaler) {
  // auto scan to check which prescaler is most suitable
  if (us_per_interrupt < 4096.0625) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // prescaler 1 is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(16 * us_per_interrupt - 1);
    // set bit for prescaler 1
    TCCR1B |= (1 << CS10);
  } else if (us_per_interrupt < 32768.5) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // prescaler 8 is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(2 * us_per_interrupt - 1);
    // set bit for prescaler 8
    TCCR1B |= (1 << CS11);
  } else if (us_per_interrupt < 262148) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // prescaler 64 is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 4 - 1);
    // set bit for prescaler 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
  } else if (us_per_interrupt < 1048592) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // prescaler 256 is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 16 - 1);
    // set bit for prescaler 256
    TCCR1B |= (1 << CS12);
  } else if (us_per_interrupt < 4194368) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // prescaler 1024 is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 64 - 1);
    // set bit for prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
  } else {
    // no prescaler is suitable
    return -1;
  }
} else if (request_prescaler == 1) {
  // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536
  if (us_per_interrupt < 4096.0625) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // request_prescaler is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(16 * us_per_interrupt - 1);
    // set bit for prescaler 1
    TCCR1B |= (1 << CS10);
  }
  else {
    return -1;
  }
} else if (request_prescaler == 8) {
  // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536
  if (us_per_interrupt < 32768.5) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // request_prescaler is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(2 * us_per_interrupt - 1);
    // set bit for prescaler 8
    TCCR1B |= (1 << CS11);
  }
  else {
    return -1;
  }
} else if (request_prescaler == 64) {
  // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536
  if (us_per_interrupt < 262148) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // request_prescaler is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 4 - 1);
    // set bit for prescaler 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
  }
  else {
    return -1;
  }
} else if (request_prescaler == 256) {
  // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536
  if (us_per_interrupt < 1048592) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // request_prescaler is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 16 - 1);
    // set bit for prescaler 256
    TCCR1B |= (1 << CS12);
  }
  else {
    return -1;
  }
} else if (request_prescaler == 1024) {
  // compare match register OCR1A = 16 * us_per_interrupt / prescaler - 1, where the register must be < 65536
  if (us_per_interrupt < 4194368) { // 16 * us_per_interrupt / request_prescaler - 1 < 65536
    // request_prescaler is suitable
    // set compare match register
    OCR1A = static_cast<uint16_t>(us_per_interrupt / 64 - 1);
    // set bit for prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
  }
  else {
    return -1;
  }
} else {
  return -2;
}

  // set prescaler
  // prescaler 1    == 1 << CS10
  // prescaler 8    == 1 << CS11
  // prescaler 64   == (1 << CS11) | (1 << CS10)
  // prescaler 256  == 1 << CS12
  // prescaler 1024 == (1 << CS12) | (1 << CS10)
  // turn on CTC mode (compare-time clear)
  TCCR1B |= (1 << WGM12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  return 0;
}

#endif