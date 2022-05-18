#ifndef CUSTOM_PINS_MEGA_H
#define CUSTOM_PINS_MEGA_H

#include <Arduino.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define PIN_TO_P(prefix,pin,unknown) ( \
    (pin >= 22 && pin <= 29) ? \
        (prefix##A) : \
        ( \
            (pin >= 30 && pin <= 37) ? \
                (prefix##C) : \
                ( \
                    (pin >= 42 && pin <= 49) ? \
                        (prefix##L) : \
                        ( \
                            (pin >= 54 && pin <= 61) ? \
                                (prefix##F) : \
                                ( \
                                    (pin >= 62 && pin <= 69) ? \
                                        (prefix##K) : \
                                        ( \
                                            ((pin >= 10 && pin <= 13) || (pin >= 50 && pin <= 53)) ? \
                                                (prefix##B) : \
                                                ( \
                                                    ((pin >= 6 && pin <= 9) || (pin == 16 || pin == 17)) ? \
                                                        (prefix##H) : \
                                                        ( \
                                                            ((pin >= 0 && pin <= 3) || pin == 5) ? \
                                                                (prefix##E) : \
                                                                ( \
                                                                    ((pin >= 18 && pin <= 21) || pin == 38) ? \
                                                                        (prefix##D) : \
                                                                        ( \
                                                                            ((pin >= 39 && pin <= 41) || pin == 4) ? \
                                                                                (prefix##G) : \
                                                                                ( \
                                                                                    (pin == 14 || pin == 15) ? \
                                                                                        (prefix##J) : \
                                                                                        ( \
                                                                                            unknown \
                                                                                        ) \
                                                                                ) \
                                                                        ) \
                                                                ) \
                                                        ) \
                                                ) \
                                        ) \
                                ) \
                        ) \
                ) \
        ) \
)

#define PIN_TO_P_NUM(prefix,pin,unknown) ( \
    (pin >= 22 && pin <= 29) ? \
        (prefix##0) : \
        ( \
            (pin >= 30 && pin <= 37) ? \
                (prefix##2) : \
                ( \
                    (pin >= 42 && pin <= 49) ? \
                        (prefix##10) : \
                        ( \
                            (pin >= 54 && pin <= 61) ? \
                                (prefix##5) : \
                                ( \
                                    (pin >= 62 && pin <= 69) ? \
                                        (prefix##9) : \
                                        ( \
                                            ((pin >= 10 && pin <= 13) || (pin >= 50 && pin <= 53)) ? \
                                                (prefix##1) : \
                                                ( \
                                                    ((pin >= 6 && pin <= 9) || (pin == 16 || pin == 17)) ? \
                                                        (prefix##7) : \
                                                        ( \
                                                            ((pin >= 0 && pin <= 3) || pin == 5) ? \
                                                                (prefix##4) : \
                                                                ( \
                                                                    ((pin >= 18 && pin <= 21) || pin == 38) ? \
                                                                        (prefix##3) : \
                                                                        ( \
                                                                            ((pin >= 39 && pin <= 41) || pin == 4) ? \
                                                                                (prefix##6) : \
                                                                                ( \
                                                                                    (pin == 14 || pin == 15) ? \
                                                                                        (prefix##8) : \
                                                                                        ( \
                                                                                            unknown \
                                                                                        ) \
                                                                                ) \
                                                                        ) \
                                                                ) \
                                                        ) \
                                                ) \
                                        ) \
                                ) \
                        ) \
                ) \
        ) \
)

#define PIN_TO_BIT(pin,unknown) ( \
    ((pin >= 2 && pin <= 3) || (pin >= 10 && pin <= 13) || (pin >= 22 && pin <= 29) || (pin >= 54 && pin <= 69)) ? \
        ((pin + 2) & 0b111) : \
        ( \
            ((pin >= 18 && pin <= 21) || (pin >= 30 && pin <= 37) || (pin >= 50 && pin <= 53)) ? \
                (7 - ((pin + 2) & 0b111)) : \
                ( \
                    (pin >= 42 && pin <= 49) ? \
                        (49 - pin) : \
                        ( \
                            (pin >= 6 && pin <= 9) ? \
                                (pin - 3) : \
                                ( \
                                    (pin >= 14 && pin <= 17) ? \
                                        ((pin & 0x01) ? 0 : 1) : \
                                        ( \
                                            (pin >= 39 && pin <= 41) ? \
                                                (41 - pin) : \
                                                ( \
                                                    (pin == 0 || pin == 1) ? \
                                                        (pin) : \
                                                        ( \
                                                            (pin == 4) ? \
                                                                (5) : \
                                                                ( \
                                                                    (pin == 5) ? \
                                                                        (3) : \
                                                                        ( \
                                                                            (pin == 38) ? \
                                                                                (7) : \
                                                                                ( \
                                                                                    unknown \
                                                                                ) \
                                                                        ) \
                                                                ) \
                                                        ) \
                                                ) \
                                        ) \
                                ) \
                        ) \
                ) \
        ) \
)

#define PIN_TO_BITMSK(pin) ( (1 << PIN_TO_BIT(pin, 8)) & 0xff )

// timer/counter control register
#define PIN_TO_TIMER(pin,prefix,suffix,unknown) ( \
    (pin == 4 || pin == 13) ? \
        (prefix##0##suffix) : \
        ( \
            (pin == 11 || pin == 12) ? \
                (prefix##1##suffix) : \
                ( \
                    (pin == 9 || pin == 10) ? \
                        (prefix##2##suffix) : \
                        ( \
                            (pin == 2 || pin == 3 || pin == 5) ? \
                                (prefix##3##suffix) : \
                                ( \
                                    (pin >= 6 && pin <= 8) ? \
                                        (prefix##4##suffix) : \
                                        ( \
                                            (pin >= 44 && pin <= 46) ? \
                                                (prefix##5##suffix) : \
                                                ( \
                                                    unknown \
                                                ) \
                                        ) \
                                ) \
                        ) \
                ) \
        ) \
)

// timer compare
#define PIN_TO_COMPARE(pin,prefix,suffix,unknown) ( \
    (pin == 2) ? \
        (prefix##3B##suffix) : \
        ( \
            (pin == 3) ? \
                (prefix##3C##suffix) : \
                ( \
                    (pin == 4) ? \
                        (prefix##0B##suffix) : \
                        ( \
                            (pin == 5) ? \
                                (prefix##3A##suffix) : \
                                ( \
                                    (pin == 6) ? \
                                        (prefix##4A##suffix) : \
                                        ( \
                                            (pin == 7) ? \
                                                (prefix##4B##suffix) : \
                                                ( \
                                                    (pin == 8) ? \
                                                        (prefix##4C##suffix) : \
                                                        ( \
                                                            (pin == 9) ? \
                                                                (prefix##2B##suffix) : \
                                                                ( \
                                                                    (pin == 10) ? \
                                                                        (prefix##2A##suffix) : \
                                                                        ( \
                                                                            (pin == 11) ? \
                                                                                (prefix##1A##suffix) : \
                                                                                ( \
                                                                                    (pin == 12) ? \
                                                                                        (prefix##1B##suffix) : \
                                                                                        ( \
                                                                                            (pin == 13) ? \
                                                                                                (prefix##0A##suffix) : \
                                                                                                ( \
                                                                                                    (pin == 44) ? \
                                                                                                        (prefix##5C##suffix) : \
                                                                                                        ( \
                                                                                                            (pin == 45) ? \
                                                                                                                (prefix##5B##suffix) : \
                                                                                                                ( \
                                                                                                                    (pin == 46) ? \
                                                                                                                        (prefix##5A##suffix) : \
                                                                                                                        ( \
                                                                                                                            unknown \
                                                                                                                        ) \
                                                                                                                ) \
                                                                                                        ) \
                                                                                                ) \
                                                                                        ) \
                                                                                ) \
                                                                        ) \
                                                                ) \
                                                        ) \
                                                ) \
                                        ) \
                                ) \
                        ) \
                ) \
        ) \
)

#if 0
(A) ?
    (1) :
    (
        (B) ?
            (1) :
            (
                (C) ?
                    (1) :
                    (
                        (D) ?
                            (1) :
                            (
                                (E) ?
                                    (1) :
                                    (
                                        (F) ?
                                            (1) :
                                            (
                                                (G) ?
                                                    (1) :
                                                    (
                                                        (H) ?
                                                            (1) :
                                                            (
                                                                (I) ?
                                                                    (1) :
                                                                    (
                                                                        (J) ?
                                                                            (1) :
                                                                            (
                                                                                (K) ?
                                                                                    (1) :
                                                                                    (
                                                                                        (L) ?
                                                                                            (1) :
                                                                                            (
                                                                                                (M) ?
                                                                                                    (1) :
                                                                                                    (
                                                                                                        (N) ?
                                                                                                            (1) :
                                                                                                            (
                                                                                                                0
                                                                                                            )
                                                                                                    )
                                                                                            )
                                                                                    )
                                                                            )
                                                                    )
                                                            )
                                                    )
                                            )
                                    )
                            )
                    )
            )
    )
#endif

namespace yh {
    namespace rec {
        #define PIN_TO_PORT_N_BIT_TEMP_ONLY(pin) \
            ((PIN_TO_P_NUM( , pin, 8) & 0b1111) << 4) | (PIN_TO_BIT(pin, 8) & 0b111)
        static const PROGMEM uint8_t pin_to_port_n_bit [] = {
            PIN_TO_PORT_N_BIT_TEMP_ONLY(0),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(1),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(2),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(3),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(4),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(5),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(6),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(7),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(8),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(9),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(10),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(11),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(12),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(13),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(14),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(15),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(16),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(17),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(18),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(19),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(20),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(21),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(22),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(23),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(24),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(25),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(26),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(27),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(28),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(29),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(30),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(31),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(32),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(33),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(34),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(35),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(36),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(37),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(38),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(39),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(40),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(41),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(42),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(43),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(44),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(45),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(46),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(47),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(48),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(49),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(50),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(51),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(52),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(53),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(54),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(55),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(56),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(57),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(58),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(59),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(60),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(61),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(62),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(63),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(64),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(65),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(66),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(67),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(68),
            PIN_TO_PORT_N_BIT_TEMP_ONLY(69)
        };
        #undef PIN_TO_PORT_N_BIT_TEMP_ONLY
        #define pin_to_port_n_bit(x) pgm_read_byte_near(pin_to_port_n_bit + x)
        void pin_mode (const uint8_t pin, const uint8_t direction) __attribute__((__always_inline__));
        void pin_mode (const uint8_t pin, const uint8_t direction) {
            uint8_t rubbish = 0;
            switch (direction) {
                case INPUT:
                    // input
                    PIN_TO_P(DDR, pin, rubbish) &= (~PIN_TO_BITMSK(pin));
                    // connect ground
                    PIN_TO_P(PORT, pin, rubbish) &= (~PIN_TO_BITMSK(pin));
                    break;
                case INPUT_PULLUP:
                    // input
                    PIN_TO_P(DDR, pin, rubbish) &= (~PIN_TO_BITMSK(pin));
                    // connect vcc with resistor
                    PIN_TO_P(PORT, pin, rubbish) |= PIN_TO_BITMSK(pin);
                    break;
                default:
                    // output
                    PIN_TO_P(DDR, pin, rubbish) |= PIN_TO_BITMSK(pin);
                    break;
            }
        }
        void digital_write (const uint8_t pin, const uint8_t state) __attribute__((__always_inline__));
        void digital_write (const uint8_t pin, const uint8_t state) {
            #if 0
            volatile uint8_t *port;
            uint8_t mask;
            #endif

            #if 0
            // simple port configuration
            if (pin == 255)
                return;
            else if ((pin >= 0 && pin <= 3) || pin == 5) // 5 pins
                port = &PORTE;
            else if ((pin >= 39 && pin <= 41) || pin == 4) // 4 pins
                port = &PORTG;
            else if ((pin >= 6 && pin <= 9) || (pin == 16 || pin == 17)) // 6 pins
                port = &PORTH;
            else if ((pin >= 10 && pin <= 13) || (pin >= 50 && pin <= 53)) // 8 pins
                port = &PORTB;
            else if (pin == 14 || pin == 15) // 2 pins
                port = &PORTJ;
            else if ((pin >= 18 && pin <= 21) || pin == 38) // 5 pins
                port = &PORTD;
            else if (pin >= 22 && pin <= 29) // 8 pins
                port = &PORTA;
            else if (pin >= 30 && pin <= 37) // 8 pins
                port = &PORTC;
            else if (pin >= 42 && pin <= 49) // 8 pins
                port = &PORTL;
            else if (pin >= 54 && pin <= 61) // 8 pins
                port = &PORTF;
            else if (pin >= 62 && pin <= 69) // 8 pins
                port = &PORTK;
            else
                return;
            #endif

            #if 0
            // rearranged port configuration
            if (pin >= 22 && pin <= 29) // 8 pins
                port = &PORTA;
            else if (pin >= 30 && pin <= 37) // 8 pins
                port = &PORTC;
            else if (pin >= 42 && pin <= 49) // 8 pins
                port = &PORTL;
            else if (pin >= 54 && pin <= 61) // 8 pins
                port = &PORTF;
            else if (pin >= 62 && pin <= 69) // 8 pins
                port = &PORTK;
            else if ((pin >= 10 && pin <= 13) || (pin >= 50 && pin <= 53)) // 8 pins
                port = &PORTB;
            else if ((pin >= 6 && pin <= 9) || (pin == 16 || pin == 17)) // 6 pins
                port = &PORTH;
            else if ((pin >= 0 && pin <= 3) || pin == 5) // 5 pins
                port = &PORTE;
            else if ((pin >= 18 && pin <= 21) || pin == 38) // 5 pins
                port = &PORTD;
            else if ((pin >= 39 && pin <= 41) || pin == 4) // 4 pins
                port = &PORTG;
            else if (pin == 14 || pin == 15) // 2 pins
                port = &PORTJ;
            else
                return;
            #endif

            #if 0
            // simple mask configuration
            if (pin == 255)
                return;
            else if (pin == 0 || pin == 1) // 2 pins
                mask = SBI(pin);
            else if ((pin >= 2 && pin <= 3) || (pin >= 10 && pin <= 13) || (pin >= 22 && pin <= 29) || (pin >= 54 && pin <= 69)) // 30 pins
                mask = SBI((pin + 2) & 0b111);
            else if (pin == 4) // 1 pin
                mask = SBI(5);
            else if (pin == 5) // 1 pin
                mask = SBI(3);
            else if (pin >= 6 && pin <= 9) // 4 pins
                mask = SBI(pin - 3);
                // mask = SBI((pin + 5) & 0b111);
            else if (pin >= 14 && pin <= 17) // 4 pins
                mask = SBI((pin & 0x01) ? 0 : 1);
            else if ((pin >= 18 && pin <= 21) || (pin >= 30 && pin <= 37) || (pin >= 50 && pin <= 53)) // 16 pins
                // mask = SBI(21 - pin);
                // mask = SBI(37 - pin);
                // mask = SBI(53 - pin);
                mask = SBI(7 - ((pin + 2) & 0b111));
                // 7 - ((pin + 2) & 0b111) == 5 - (pin & 0b111)
            else if (pin == 38) // 1 pin
                mask = SBI(7);
            else if (pin >= 39 && pin <= 41) // 7, 0, 1 // +1: 0, 1, 2 // 2 - ((pin + 1) & 0b111) /// 3 pins
                mask = SBI(41 - pin);
            else if (pin >= 42 && pin <= 49) // 0b101010 - 0b110001 // 010 - 001 // 2, 3, 4, 5, 6, 7, 0, 1 // -2: 0, 1, 2, 3, 4, 5, 6, 7 // 8 pins
                mask = SBI(49 - pin);
                // mask = SBI(7 - ((pin - 2) & 0b111));
            else
                return;
            #endif

            #if 0
            // rearranged mask configuration
            if ((pin >= 2 && pin <= 3) || (pin >= 10 && pin <= 13) || (pin >= 22 && pin <= 29) || (pin >= 54 && pin <= 69)) // 30 pins
                mask = SBI((pin + 2) & 0b111);
            else if ((pin >= 18 && pin <= 21) || (pin >= 30 && pin <= 37) || (pin >= 50 && pin <= 53)) // 16 pins
                // mask = SBI(21 - pin);
                // mask = SBI(37 - pin);
                // mask = SBI(53 - pin);
                mask = SBI(7 - ((pin + 2) & 0b111));
                // 7 - ((pin + 2) & 0b111) == 5 - (pin & 0b111)
            else if (pin >= 42 && pin <= 49) // 0b101010 - 0b110001 // 010 - 001 // 2, 3, 4, 5, 6, 7, 0, 1 // -2: 0, 1, 2, 3, 4, 5, 6, 7 // 8 pins
                mask = SBI(49 - pin);
                // mask = SBI(7 - ((pin - 2) & 0b111));
            else if (pin >= 6 && pin <= 9) // 4 pins
                mask = SBI(pin - 3);
                // mask = SBI((pin + 5) & 0b111);
            else if (pin >= 14 && pin <= 17) // 4 pins
                mask = SBI((pin & 0x01) ? 0 : 1);
            else if (pin >= 39 && pin <= 41) // 7, 0, 1 // +1: 0, 1, 2 // 2 - ((pin + 1) & 0b111) /// 3 pins
                mask = SBI(41 - pin);
            else if (pin == 0 || pin == 1) // 2 pins
                mask = SBI(pin);
            else if (pin == 4) // 1 pin
                mask = SBI(5);
            else if (pin == 5) // 1 pin
                mask = SBI(3);
            else if (pin == 38) // 1 pin
                mask = SBI(7);
            else
                return;
            #endif

            uint8_t rubbish = 0;
            PIN_TO_TIMER(pin, TCCR, A, rubbish) &= (~((1 << PIN_TO_COMPARE(pin, COM, 1, 8)) & 0xff)); // clear pwm output
            state ? (PIN_TO_P(PORT, pin, rubbish) |= PIN_TO_BITMSK(pin)) : (PIN_TO_P(PORT, pin, rubbish) &= (~PIN_TO_BITMSK(pin))); // write low or high
        }
        int digital_read (const uint8_t pin) __attribute__((__always_inline__));
        int digital_read (const uint8_t pin) {
            uint8_t rubbish = 0;
            PIN_TO_TIMER(pin, TCCR, A, rubbish) &= (~((1 << PIN_TO_COMPARE(pin, COM, 1, 8)) & 0xff)); // clear pwm output
            return PIN_TO_P(PIN, pin, rubbish) & PIN_TO_BITMSK(pin) ? 1 : 0; // read 1 or 0
        }
        void analog_write (const uint8_t pin, const uint8_t val) __attribute__((__always_inline__));
        void analog_write (const uint8_t pin, const uint8_t val) {
            uint8_t rubbish = 0;
            PIN_TO_P(DDR, pin, rubbish) |= PIN_TO_BITMSK(pin); // set data direction to output
            if (val == 0) {
                PIN_TO_TIMER(pin, TCCR, A, rubbish) &= (~((1 << PIN_TO_COMPARE(pin, COM, 1, 8)) & 0xff)); // clear pwm output
                PIN_TO_P(PORT, pin, rubbish) &= (~PIN_TO_BITMSK(pin)); // write low
            } else if (val == 255) {
                PIN_TO_TIMER(pin, TCCR, A, rubbish) &= (~((1 << PIN_TO_COMPARE(pin, COM, 1, 8)) & 0xff)); // clear pwm output
                PIN_TO_P(PORT, pin, rubbish) |= PIN_TO_BITMSK(pin); // write high
            } else {
                PIN_TO_TIMER(pin, TCCR, A, rubbish) |= ((1 << PIN_TO_COMPARE(pin, COM, 1, 8)) & 0xff); // set pwm output
                PIN_TO_COMPARE(pin, OCR, , rubbish) = val; // set pwm duty-cycle
            }
        }
    }
}

#define pinMode(...) yh::rec::pin_mode(__VA_ARGS__)
#define digitalWrite(...) yh::rec::digital_write(__VA_ARGS__)
#define digitalRead(...) yh::rec::digital_read(__VA_ARGS__)
#define analogWrite(...) yh::rec::analog_write(__VA_ARGS__)

#endif // #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#endif // #ifndef CUSTOM_PINS_MEGA_H