// Please do the following:
// connect Vcc     on GY-6500 to 3.3V on arduino
// connect Gnd     on GY-6500 to GND  on arduino
// connect SDA/SDI on GY-6500 to MOSI on arduino
// connect SCL/SCK on GY-6500 to SCK  on arduino
// connect AD0/SDO on GY-6500 to MISO on arduino
// connect nCS     on GY-6500 to pin 10 on arduino

#include <Mpu_6500.h>

yh::rec::Mpu_6500_default_SPI mpu (10); // input the CS pin

yh::rec::Gyro_packet temp_gyro_packet;

// copied from Timer_int.h
inline int8_t setup_timer_1A_interrupt (const unsigned long us_per_interrupt, const uint16_t request_prescaler = 0);

void update_progress () {
    static uint16_t progress = 0;
    Serial.println((progress++) / 400.0);
}

void setup () {

    // sets baud rate to 9600
    Serial.begin(9600);

    // sets the GY-521 chip to desired settings
    mpu.begin();

    // calibrate the gyroscope
    mpu.cal_gyro(400, update_progress);

    // note: it is suggested not to input a time interval that is less than 80 microseconds
    // as update_gyro() takes 24 - 28 microseconds
    // mpu.update_gyro_isr() takes 24 - 28 µs
    // mpu.update_gyro(mpu.update_gyro_isr()) takes 1400 - 1600 µs
    setup_timer_1A_interrupt(1000);
}

void loop () {

    // update the gyroscope
    mpu.update_gyro(temp_gyro_packet);
    Serial.print(mpu.d_roll);
    Serial.print('\t');
    Serial.print(mpu.d_pitch);
    Serial.print('\t');
    Serial.print(mpu.d_yaw);
    Serial.print("\t\t\t");
    Serial.print(mpu.get_roll());
    Serial.print('\t');
    Serial.print(mpu.get_pitch());
    Serial.print('\t');
    // prints out the current heading of the chip
    Serial.print(mpu.get_yaw());
    Serial.print('\n');
    #endif // !defined(FIND_TIME_INTERVAL)
}

ISR(TIMER1_COMPA_vect) {
    interrupts(); // allow interrupts as I2C relies on interrupts to send and receive data
    temp_gyro_packet = mpu.update_gyro_isr();
    noInterrupts(); // disable interrupts as this is an ISR (interrupt service routine)
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
