#ifndef CUSTOM_PINS_H
#define CUSTOM_PINS_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

namespace yh {
    namespace rec {
        namespace custom_pins {
            // YOU MUST CALL ME IN void setup () FUNCTION TO USE THIS OBJECT PROPERLY
            // kicks off the first ADC conversion, and let it run on
            void begin_analog_read ();
            // @param pin the analog pin to be read (or channel to be read)
            // @return the analog voltage value reference to AREF pin (or 5V if not connected)
            uint16_t analog_read (uint8_t pin_or_channel);
            //
            // expanded analogWrite(...) functions
            // XXX fix needed for atmega8
            #if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
            void set_COM00_PWM (const uint8_t val) {
                // connect pwm to pin on timer 0
                sbi(TCCR0, COM00);
                OCR0 = val; // set pwm duty
            }
            #endif

            #if defined(TCCR0A) && defined(COM0A1)
            void set_COM0A1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 0, channel A
                sbi(TCCR0A, COM0A1);
                OCR0A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR0A) && defined(COM0B1)
            void set_COM0B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 0, channel B
                sbi(TCCR0A, COM0B1);
                OCR0B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1A1)
            void set_COM1A1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 1, channel A
                sbi(TCCR1A, COM1A1);
                OCR1A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1B1)
            void set_COM1B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 1, channel B
                sbi(TCCR1A, COM1B1);
                OCR1B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR1A) && defined(COM1C1)
            void set_COM1C1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 1, channel C
                sbi(TCCR1A, COM1C1);
                OCR1C = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2) && defined(COM21)
            void set_COM21_PWM (const uint8_t val) {
                // connect pwm to pin on timer 2
                sbi(TCCR2, COM21);
                OCR2 = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2A) && defined(COM2A1)
            void set_COM2A1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 2, channel A
                sbi(TCCR2A, COM2A1);
                OCR2A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR2A) && defined(COM2B1)
            void set_COM2B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 2, channel B
                sbi(TCCR2A, COM2B1);
                OCR2B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3A1)
            void set_COM3A1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 3, channel A
                sbi(TCCR3A, COM3A1);
                OCR3A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3B1)
            void set_COM3B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 3, channel B
                sbi(TCCR3A, COM3B1);
                OCR3B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR3A) && defined(COM3C1)
            void set_COM3C1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 3, channel C
                sbi(TCCR3A, COM3C1);
                OCR3C = val; // set pwm duty
            }
            #endif

            #if defined(TCCR4A)
            void set_COM4A1_PWM (const uint8_t val) {
                //connect pwm to pin on timer 4, channel A
                sbi(TCCR4A, COM4A1);
                #if defined(COM4A0)		// only used on 32U4
                cbi(TCCR4A, COM4A0);
                #endif
                OCR4A = val;	// set pwm duty
            }
            #endif
            
            #if defined(TCCR4A) && defined(COM4B1)
            void set_COM4B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 4, channel B
                sbi(TCCR4A, COM4B1);
                OCR4B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR4A) && defined(COM4C1)
            void set_COM4C1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 4, channel C
                sbi(TCCR4A, COM4C1);
                OCR4C = val; // set pwm duty
            }
            #endif
                
            #if defined(TCCR4C) && defined(COM4D1)
            void set_COM4D1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 4, channel D
                sbi(TCCR4C, COM4D1);
                #if defined(COM4D0)		// only used on 32U4
                cbi(TCCR4C, COM4D0);
                #endif
                OCR4D = val;	// set pwm duty
            }
            #endif

                            
            #if defined(TCCR5A) && defined(COM5A1)
            void set_COM5A1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 5, channel A
                sbi(TCCR5A, COM5A1);
                OCR5A = val; // set pwm duty
            }
            #endif

            #if defined(TCCR5A) && defined(COM5B1)
            void set_COM5B1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 5, channel B
                sbi(TCCR5A, COM5B1);
                OCR5B = val; // set pwm duty
            }
            #endif

            #if defined(TCCR5A) && defined(COM5C1)
            void set_COM5C1_PWM (const uint8_t val) {
                // connect pwm to pin on timer 5, channel C
                sbi(TCCR5A, COM5C1);
                OCR5C = val; // set pwm duty
            }
            #endif
        }
        class Custom_pin {
            private:
                // pins that cannot be changed:
                // the pin that this object belongs to
                const uint8_t pin;
                // output port register
                volatile uint8_t *output_reg;
                // input port register
                volatile uint8_t *input_reg;
                // mode register
                volatile uint8_t *mode_reg;
                // I/O bit mask
                uint8_t bit_mask;
                // timer
                uint8_t timer;
                // timer A register
                volatile uint8_t *timer_A_reg;
                // PWM channel bit mask
                uint8_t timer_pwm_bit_mask;
                // PWM value of the pin
                volatile uint8_t *compare_match_reg;
                #if 0
                // pointer to the function to write the PWM value for this pin
                void (*analog_write_ptr)(const uint8_t);
                #endif
            public:
                // inits the pin number into the object
                Custom_pin (const uint8_t init_pin) :
                pin(init_pin),
                output_reg(portOutputRegister(digitalPinToPort(pin))),
                input_reg(portInputRegister(digitalPinToPort(pin))),
                mode_reg(portModeRegister(digitalPinToPort(pin))),
                bit_mask(digitalPinToBitMask(pin)),
                timer(digitalPinToTimer(pin))
                {
                    #if 0
                    //
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
                    #endif
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
                            compare_match_reg = &OCR1A;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1B1)
                        case TIMER1B:
                            timer_A_reg = &TCCR1A;
                            timer_pwm_bit_mask = (1 << COM1B1);
                            compare_match_reg = &OCR1B;
                            break;
                        #endif

                        #if defined(TCCR1A) && defined(COM1C1)
                        case TIMER1C:
                            timer_A_reg = &TCCR1A;
                            timer_pwm_bit_mask = (1 << COM1C1);
                            compare_match_reg = &OCR1C;
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
                            compare_match_reg = &OCR3A;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3B1)
                        case TIMER3B:
                            timer_A_reg = &TCCR3A;
                            timer_pwm_bit_mask = (1 << COM3B1);
                            compare_match_reg = &OCR3B;
                            break;
                        #endif

                        #if defined(TCCR3A) && defined(COM3C1)
                        case TIMER3C:
                            timer_A_reg = &TCCR3A;
                            timer_pwm_bit_mask = (1 << COM3C1);
                            compare_match_reg = &OCR3C;
                            break;
                        #endif

                        #if defined(TCCR4A)
                        case TIMER4A:
                            #if defined(COM4A0)		// only used on 32U4
                            cbi(TCCR4A, COM4A0);
                            #endif
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4A1);
                            compare_match_reg = &OCR4A;
                            break;
                        #endif
                        
                        #if defined(TCCR4A) && defined(COM4B1)
                        case TIMER4B:
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4B1);
                            compare_match_reg = &OCR4B;
                            break;
                        #endif

                        #if defined(TCCR4A) && defined(COM4C1)
                        case TIMER4C:
                            timer_A_reg = &TCCR4A;
                            timer_pwm_bit_mask = (1 << COM4C1);
                            compare_match_reg = &OCR4C;
                            break;
                        #endif
                            
                        #if defined(TCCR4C) && defined(COM4D1)
                        case TIMER4D:
                            #if defined(COM4D0)		// only used on 32U4
                            cbi(TCCR4C, COM4D0);
                            #endif
                            timer_A_reg = &TCCR4C;
                            timer_pwm_bit_mask = (1 << COM4D1);
                            compare_match_reg = &OCR4D;
                            break;
                        #endif


                        #if defined(TCCR5A) && defined(COM5A1)
                        case TIMER5A:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5A1);
                            compare_match_reg = &OCR5A;
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5B1)
                        case TIMER5B:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5B1);
                            compare_match_reg = &OCR5B;
                            break;
                        #endif

                        #if defined(TCCR5A) && defined(COM5C1)
                        case TIMER5C:
                            timer_A_reg = &TCCR5A;
                            timer_pwm_bit_mask = (1 << COM5C1);
                            compare_match_reg = &OCR5C;
                            break;
                        #endif

                        case NOT_ON_TIMER:
                        default:
                            timer_A_reg = NULL;
                            timer_pwm_bit_mask = 0;
                            compare_match_reg = NULL;
                    }
                }
                //
                inline void pin_mode (const uint8_t mode) {
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
                inline void digital_write (const bool val) { val ? ((*output_reg) |= bit_mask) : ((*output_reg) &= ~bit_mask); }
                //
                inline void digital_write_HIGH () { (*output_reg) |= bit_mask; }
                //
                inline void digital_write_LOW () { (*output_reg) &= ~bit_mask; }
                //
                inline void digital_write_TOGGLE () { (*output_reg) ^= bit_mask; }
                //
                inline bool digital_read_logical () { return (*input_reg) & bit_mask; }
                //
                inline bool digital_read () { return ((*input_reg) & bit_mask) ? 1 : 0; }
                //
                inline void analog_write (const uint8_t val) {
                    if (compare_match_reg == NULL) {
                        if (val > 127) digital_write_HIGH();
                        else digital_write_LOW();
                    } else {
                        if (!val) {
                            (*timer_A_reg) &= ~(timer_pwm_bit_mask);
                            digital_write_LOW();
                        } else if (val == 255) {
                            (*timer_A_reg) &= ~(timer_pwm_bit_mask);
                            digital_write_HIGH();
                        } else {
                            (*timer_A_reg) |= (timer_pwm_bit_mask);
                            (*compare_match_reg) = val;
                        }
                    }
                }
                //
                inline uint16_t analog_read () {
                    if (pin >= A0 && pin < (A0 + NUM_ANALOG_INPUTS)) return custom_pins::analog_read(pin);
                    else return digital_read_logical() ? 1023 : 0;
                }
        };
        namespace custom_pins {
            inline void pin_mode (Custom_pin &pin, const uint8_t mode) { pin.pin_mode(mode); }
            inline void digital_write (Custom_pin &pin, const bool val) { pin.digital_write(val); }
            inline bool digital_read (Custom_pin &pin) { return pin.digital_read(); }
        }
    }
}

#ifndef CUSTOM_PINS_CPP
#define analogRead(...) yh::rec::custom_pins::analog_read(__VA_ARGS__)
#endif // #ifndef CUSTOM_PINS_CPP

#endif // #ifndef CUSTOM_PINS_H