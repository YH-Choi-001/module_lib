// warning: This library disables the original pinMode and I/O functions on the pins.
// This library aims to provide a faster I/O communication with the pins.
// To increase speed, SRAM is sacrificed.
// <Arduino.h> chose SRAM to give up speed, and now, we choose speed to give up SRAM.

#ifndef CUSTOM_PINS_H
#define CUSTOM_PINS_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        namespace custom_pins {
            #define FUNCTION_PWM 0


            #ifndef sbi
            #define sbi(x,y) (x|=(1<<y))
            #endif
            #ifndef cbi
            #define cbi(x,y) (x&=(~(1<<y)))
            #endif


            #if FUNCTION_PWM

            // expanded analogWrite(...) functions
            // XXX fix needed for atmega8
            #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
            void set_COM00_PWM (const uint8_t val) __attribute__((__always_inline__));
            void set_COM00_PWM (const uint8_t val) {
                // connect pwm to pin on timer 0
                sbi(TCCR0, COM00);
                OCR0 = val; // set pwm duty
            }
            #endif

            #if defined(TCCR0A) && defined(COM0A1)
            inline void set_COM0A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 0, channel A
                sbi(TCCR0A, COM0A1);
                OCR0A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR0A) && defined(COM0B1)
            inline void set_COM0B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 0, channel B
                sbi(TCCR0A, COM0B1);
                OCR0B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1A1)
            inline void set_COM1A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 1, channel A
                sbi(TCCR1A, COM1A1);
                OCR1A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1B1)
            inline void set_COM1B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 1, channel B
                sbi(TCCR1A, COM1B1);
                OCR1B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1C1)
            inline void set_COM1C1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 1, channel C
                sbi(TCCR1A, COM1C1);
                OCR1C = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2) && defined(COM21)
            inline void set_COM21_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 2
                sbi(TCCR2, COM21);
                OCR2 = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2A) && defined(COM2A1)
            inline void set_COM2A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 2, channel A
                sbi(TCCR2A, COM2A1);
                OCR2A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2A) && defined(COM2B1)
            inline void set_COM2B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 2, channel B
                sbi(TCCR2A, COM2B1);
                OCR2B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3A1)
            inline void set_COM3A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 3, channel A
                sbi(TCCR3A, COM3A1);
                OCR3A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3B1)
            inline void set_COM3B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 3, channel B
                sbi(TCCR3A, COM3B1);
                OCR3B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3C1)
            inline void set_COM3C1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 3, channel C
                sbi(TCCR3A, COM3C1);
                OCR3C = val; // set pwm duty
            }
            #endif

            #if defined(TCCR4A)
            inline void set_COM4A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                //connect pwm to pin on timer 4, channel A
                sbi(TCCR4A, COM4A1);
                #if defined(COM4A0)		// only used on 32U4
                cbi(TCCR4A, COM4A0);
                #endif
                OCR4A = val;	// set pwm duty
            }
            #endif
            
            #if defined(TCCR4A) && defined(COM4B1)
            inline void set_COM4B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 4, channel B
                sbi(TCCR4A, COM4B1);
                OCR4B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR4A) && defined(COM4C1)
            inline void set_COM4C1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 4, channel C
                sbi(TCCR4A, COM4C1);
                OCR4C = val; // set pwm duty
            }
            #endif
                
            #if defined(TCCR4C) && defined(COM4D1)
            inline void set_COM4D1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 4, channel D
                sbi(TCCR4C, COM4D1);
                #if defined(COM4D0)		// only used on 32U4
                cbi(TCCR4C, COM4D0);
                #endif
                OCR4D = val;	// set pwm duty
            }
            #endif

                            
            #if defined(TCCR5A) && defined(COM5A1)
            inline void set_COM5A1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 5, channel A
                sbi(TCCR5A, COM5A1);
                OCR5A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR5A) && defined(COM5B1)
            inline void set_COM5B1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 5, channel B
                sbi(TCCR5A, COM5B1);
                OCR5B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR5A) && defined(COM5C1)
            inline void set_COM5C1_PWM (const uint8_t val) __attribute__((__always_inline__)) {
                // connect pwm to pin on timer 5, channel C
                sbi(TCCR5A, COM5C1);
                OCR5C = val; // set pwm duty
            }
            #endif


            #endif // #if FUNCTION_PWM



        }
        class Custom_pin {
            private:
                // the counter to count how many objects has been automatically created
                static uint8_t auto_object_counter;
                // output port register
                volatile uint8_t *const output_reg;
                // input port register
                volatile uint8_t *const input_reg;
                // mode register
                volatile uint8_t *const mode_reg;
                // I/O bit mask
                const uint8_t bit_mask;
                // timer
                const uint8_t timer;
                #if FUNCTION_PWM
                // pointer to the function to turn off PWM output
                void (*off_pwm)();
                // pointer to the function to write the PWM value for this pin
                void (*analog_write_ptr)(const uint8_t);
                #else
                // timer A register
                volatile uint8_t *timer_A_reg;
                // PWM channel enable bit mask
                uint8_t timer_pwm_bit_mask;
                // PWM value of the pin
                volatile uint8_t *compare_match_reg;
                #endif
                void timer_identification () {
                    #if FUNCTION_PWM
                    off_pwm = NULL;
                    switch(timer)
                    {
                        // XXX fix needed for atmega8
                        #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
                        case TIMER0A:
                            analog_write_ptr = Custom_pins::set_COM00_PWM;
                            break;
                        #endif

                        #if defined(TCCR0A) && defined(COM0A1)
                        case TIMER0A:
                            analog_write_ptr = Custom_pins::set_COM0A1_PWM;
                            break;
                        #endif

                        #if defined(TCCR0A) && defined(COM0B1)
                        case TIMER0B:
                            analog_write_ptr = Custom_pins::set_COM0B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1A1)
                        case TIMER1A:
                            analog_write_ptr = Custom_pins::set_COM1A1_PWM;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1B1)
                        case TIMER1B:
                            analog_write_ptr = Custom_pins::set_COM1B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1C1)
                        case TIMER1C:
                            analog_write_ptr = Custom_pins::set_COM1C1_PWM;
                            break;
                        #endif

                        #if defined(TCCR2) && defined(COM21)
                        case TIMER2:
                            analog_write_ptr = Custom_pins::set_COM21_PWM;
                            break;
                        #endif

                        #if defined(TCCR2A) && defined(COM2A1)
                        case TIMER2A:
                            analog_write_ptr = Custom_pins::set_COM2A1_PWM;
                            break;
                        #endif

                        #if defined(TCCR2A) && defined(COM2B1)
                        case TIMER2B:
                            analog_write_ptr = Custom_pins::set_COM2B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3A1)
                        case TIMER3A:
                            analog_write_ptr = Custom_pins::set_COM3A1_PWM;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3B1)
                        case TIMER3B:
                            analog_write_ptr = Custom_pins::set_COM3B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3C1)
                        case TIMER3C:
                            analog_write_ptr = Custom_pins::set_COM3C1_PWM;
                            break;
                        #endif

                        #if defined(TCCR4A)
                        case TIMER4A:
                            analog_write_ptr = Custom_pins::set_COM4A1_PWM;
                            break;
                        #endif
                        
                        #if defined(TCCR4A) && defined(COM4B1)
                        case TIMER4B:
                            analog_write_ptr = Custom_pins::set_COM4B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR4A) && defined(COM4C1)
                        case TIMER4C:
                            analog_write_ptr = Custom_pins::set_COM4C1_PWM;
                            break;
                        #endif
                            
                        #if defined(TCCR4C) && defined(COM4D1)
                        case TIMER4D:				
                            analog_write_ptr = Custom_pins::set_COM4D1_PWM;
                            break;
                        #endif

                                        
                        #if defined(TCCR5A) && defined(COM5A1)
                        case TIMER5A:
                            analog_write_ptr = Custom_pins::set_COM5A1_PWM;
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5B1)
                        case TIMER5B:
                            analog_write_ptr = Custom_pins::set_COM5B1_PWM;
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5C1)
                        case TIMER5C:
                            analog_write_ptr = Custom_pins::set_COM5C1_PWM;
                            break;
                        #endif

                        case NOT_ON_TIMER:
                        default:
                            analog_write_ptr = NULL;
                    }
                    #else
                    switch(timer)
                    {
                        // XXX fix needed for atmega8
                        #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
                        case TIMER0A:
                            timer_A_reg = &TCCR0;
                            timer_pwm_bit_mask = (1 << COM00);
                            compare_match_reg = &OCR0;
                            break;
                        #endif

                        #if defined(TCCR0A) && defined(COM0A1)
                        case TIMER0A:
                            timer_A_reg = &TCCR0A;
                            timer_pwm_bit_mask = (1 << COM0A1);
                            compare_match_reg = &OCR0A;
                            break;
                        #endif

                        #if defined(TCCR0A) && defined(COM0B1)
                        case TIMER0B:
                            timer_A_reg = &TCCR0A;
                            timer_pwm_bit_mask = (1 << COM0B1);
                            compare_match_reg = &OCR0B;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1A1)
                        case TIMER1A:
                            timer_A_reg = &TCCR1A;
                            timer_pwm_bit_mask = (1 << COM1A1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1A);
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1B1)
                        case TIMER1B:
                            timer_A_reg = &TCCR1A;
                            timer_pwm_bit_mask = (1 << COM1B1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1B);
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1C1)
                        case TIMER1C:
                            timer_A_reg = &TCCR1A;
                            timer_pwm_bit_mask = (1 << COM1C1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR1C);
                            break;
                        #endif

                        #if defined(TCCR2) && defined(COM21)
                        case TIMER2:
                            timer_A_reg = &TCCR2;
                            timer_pwm_bit_mask = (1 << COM21);
                            compare_match_reg = &OCR2;
                            break;
                        #endif

                        #if defined(TCCR2A) && defined(COM2A1)
                        case TIMER2A:
                            timer_A_reg = &TCCR2A;
                            timer_pwm_bit_mask = (1 << COM2A1);
                            compare_match_reg = &OCR2A;
                            break;
                        #endif

                        #if defined(TCCR2A) && defined(COM2B1)
                        case TIMER2B:
                            timer_A_reg = &TCCR2A;
                            timer_pwm_bit_mask = (1 << COM2B1);
                            compare_match_reg = &OCR2B;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3A1)
                        case TIMER3A:
                            timer_A_reg = &TCCR3A;
                            timer_pwm_bit_mask = (1 << COM3A1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3A);
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3B1)
                        case TIMER3B:
                            timer_A_reg = &TCCR3A;
                            timer_pwm_bit_mask = (1 << COM3B1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3B);
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3C1)
                        case TIMER3C:
                            timer_A_reg = &TCCR3A;
                            timer_pwm_bit_mask = (1 << COM3C1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR3C);
                            break;
                        #endif

                        #if defined(TCCR4A)
                        case TIMER4A:
                            #if defined(COM4A0)		// only used on 32U4
                            cbi(TCCR4A, COM4A0);
                            #endif
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4A1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4A);
                            break;
                        #endif
                        
                        #if defined(TCCR4A) && defined(COM4B1)
                        case TIMER4B:
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4B1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4B);
                            break;
                        #endif

                        #if defined(TCCR4A) && defined(COM4C1)
                        case TIMER4C:
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4C1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4C);
                            break;
                        #endif
                            
                        #if defined(TCCR4C) && defined(COM4D1)
                        case TIMER4D:
                            #if defined(COM4D0)		// only used on 32U4
                            cbi(TCCR4C, COM4D0);
                            #endif
                            timer_A_reg = &TCCR4C;
                            timer_pwm_bit_mask = (1 << COM4D1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR4D);
                            break;
                        #endif


                        #if defined(TCCR5A) && defined(COM5A1)
                        case TIMER5A:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5A1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5A);
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5B1)
                        case TIMER5B:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5B1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5B);
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5C1)
                        case TIMER5C:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5C1);
                            compare_match_reg = reinterpret_cast<volatile uint8_t *>(&OCR5C);
                            break;
                        #endif

                        case NOT_ON_TIMER:
                        default:
                            timer_A_reg = NULL;
                            timer_pwm_bit_mask = 0;
                            compare_match_reg = NULL;
                    }
                    #endif // #if !FUNCTION_PWM
                }
            public:
                // inits the pin number into the object
                Custom_pin (const uint8_t init_pin) :
                output_reg(   portOutputRegister( digitalPinToPort(init_pin) )   ),
                input_reg(   portInputRegister( digitalPinToPort(init_pin) )   ),
                mode_reg(   portModeRegister( digitalPinToPort(init_pin) )   ),
                bit_mask(digitalPinToBitMask(init_pin)),
                timer(digitalPinToTimer(init_pin))
                {
                    timer_identification();
                }
                // directly initialized on declaration
                Custom_pin () :
                output_reg(   portOutputRegister( digitalPinToPort(auto_object_counter) )   ),
                input_reg(   portInputRegister( digitalPinToPort(auto_object_counter) )   ),
                mode_reg(   portModeRegister( digitalPinToPort(auto_object_counter) )   ),
                bit_mask(digitalPinToBitMask(auto_object_counter)),
                timer(digitalPinToTimer(auto_object_counter++))
                {

                    timer_identification();
                }
                //
                void pin_mode (const uint8_t mode) {
                    if (mode == INPUT) {
                        uint8_t oldSREG = SREG;
                                cli();
                        *mode_reg &= ~bit_mask;
                        *output_reg &= ~bit_mask;
                        SREG = oldSREG;
                    } else if (mode == INPUT_PULLUP) {
                        uint8_t oldSREG = SREG;
                                cli();
                        *mode_reg &= ~bit_mask;
                        *output_reg |= bit_mask;
                        SREG = oldSREG;
                    } else {
                        uint8_t oldSREG = SREG;
                                cli();
                        *mode_reg |= bit_mask;
                        SREG = oldSREG;
                    }
                }
                //
                void digital_write (const bool val) __attribute__((__always_inline__))
                { val ? ((*output_reg) |= bit_mask) : ((*output_reg) &= ~bit_mask); }
                //
                void digital_write_HIGH ()          __attribute__((__always_inline__))
                { (*output_reg) |= bit_mask; }
                //
                void digital_write_LOW ()           __attribute__((__always_inline__))
                { (*output_reg) &= ~bit_mask; }
                //
                void digital_write_TOGGLE ()        __attribute__((__always_inline__))
                { (*output_reg) ^= bit_mask; }
                //
                bool digital_read_logical ()        __attribute__((__always_inline__))
                { return (*input_reg) & bit_mask; }
                //
                bool digital_read ()                __attribute__((__always_inline__))
                { return ((*input_reg) & bit_mask) ? 1 : 0; }
                //
                void analog_write (const uint8_t val) __attribute__((__always_inline__))
                {
                    #if FUNCTION_PWM
                    if (analog_write_ptr == NULL) {
                        if (val > 127) (*output_reg) |= bit_mask; // write HIGH
                        else (*output_reg) &= ~bit_mask;          // write LOW
                    } else {
                        if (!val) {
                            off_pwm();                               // off PWM
                            (*output_reg) &= ~bit_mask;              // write LOW
                        } else if (val == 255) {
                            off_pwm();                               // off PWM
                            (*output_reg) |= bit_mask;               // write HIGH
                        } else {
                            analog_write_ptr(val);                   // write PWM value
                        }
                    }
                    #else
                    if (compare_match_reg == NULL) {
                        if (val > 127) (*output_reg) |= bit_mask; // write HIGH
                        else (*output_reg) &= ~bit_mask;          // write LOW
                    } else {
                        if (!val) {
                            (*timer_A_reg) &= ~(timer_pwm_bit_mask); // off PWM
                            (*output_reg) &= ~bit_mask;              // write LOW
                        } else if (val == 255) {
                            (*timer_A_reg) &= ~(timer_pwm_bit_mask); // off PWM
                            (*output_reg) |= bit_mask;               // write HIGH
                        } else {
                            (*timer_A_reg) |= (timer_pwm_bit_mask);  // on PWM
                            (*compare_match_reg) = val;              // write PWM value
                        }
                    }
                    #endif // #if !FUNCTION_PWM
                }
        };
        extern Custom_pin pins [];
        namespace custom_pins {
            inline void pin_mode (const uint8_t pin, const uint8_t mode)    __attribute__((__always_inline__));
            inline void pin_mode (const uint8_t pin, const uint8_t mode)                                        { pins[pin].pin_mode(mode); }
            inline void digital_write (const uint8_t pin, const bool val)   __attribute__((__always_inline__));
            inline void digital_write (const uint8_t pin, const bool val)                                       { pins[pin].digital_write(val); }
            inline bool digital_read (const uint8_t pin)                    __attribute__((__always_inline__));
            inline bool digital_read (const uint8_t pin)                                                        { return pins[pin].digital_read(); }
            inline void analog_write (const uint8_t pin, const uint8_t val) __attribute__((__always_inline__));
            inline void analog_write (const uint8_t pin, const uint8_t val)                                     { pins[pin].analog_write(val); }
            inline void digital_write_HIGH   (const uint8_t pin)   __attribute__((__always_inline__));
            inline void digital_write_HIGH   (const uint8_t pin)                                       { pins[pin].digital_write_HIGH(); }
            inline void digital_write_LOW    (const uint8_t pin)   __attribute__((__always_inline__));
            inline void digital_write_LOW    (const uint8_t pin)                                       { pins[pin].digital_write_LOW(); }
            inline void digital_write_TOGGLE (const uint8_t pin)   __attribute__((__always_inline__));
            inline void digital_write_TOGGLE (const uint8_t pin)                                       { pins[pin].digital_write_TOGGLE(); }
        }
    }
}

#ifndef CUSTOM_PINS_CPP
// the macros below override the original <Arduino.h> defined functions excluding analogRead()
#define pinMode(pin,mode) yh::rec::pins[pin].pin_mode(mode)
#define digitalWrite(pin,val) yh::rec::pins[pin].digital_write(val)
#define digitalRead(pin) yh::rec::pins[pin].digital_read()
#define analogWrite(pin,val) yh::rec::pins[pin].analog_write(val)
// #define analogRead(...) yh::rec::custom_pins::analog_read(__VA_ARGS__) // current analogRead() function from <Arduino.h> is already the fastest when striving to preserve 10-bit resolution
#define digitalWriteHIGH(pin) yh::rec::pins[pin].digital_write_HIGH()
#define digitalWriteLOW(pin) yh::rec::pins[pin].digital_write_LOW()
#define digitalWriteTOGGLE(pin) yh::rec::pins[pin].digital_write_TOGGLE()
#endif // #ifndef CUSTOM_PINS_CPP

#endif // #ifndef CUSTOM_PINS_H