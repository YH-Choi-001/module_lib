#ifndef CUSTOM_LCD_H
#define CUSTOM_LCD_H __DATE__ ", " __TIME__

#if defined(ARDUINO) && !defined(Arduino_h)
#include <Arduino.h>
#endif // #if defined(ARDUINO) && !defined(Arduino_h)

#include "../Custom_print/Custom_print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// just connect RW pin to ground, because we will be writing data to lcd only
class Custom_lcd : public Custom_print {
    private:
        //
    protected:
        // pins that cannot be changed:
        // rs (or known as D/C) pin tells the lcd whether the bits being transmitted are commands or characters in ASCII
        const uint8_t rs_pin;
        volatile uint8_t *rs_pin_output_reg;
        uint8_t rs_pin_mask;
        // enable pin tells the lcd to read its data pins to receive transmitted data
        const uint8_t en_pin;
        volatile uint8_t *en_pin_output_reg;
        uint8_t en_pin_mask;
        // data pins are the pins on the arduino that are connected to D0 to D7 (or D3) of the lcd
        const uint8_t data_pins [8];
        volatile uint8_t *data_pins_output_reg [8];
        uint8_t data_pins_mask [8];

        //
        uint8_t _4bit_mode_of_same_port_l_shift_val;
        //

        // low level control

        // D0 for LSB, D7 (or D3) for MSB
        void (*send_data)(Custom_lcd *, const uint8_t);
        friend inline void send_data_in_4bit_mode (Custom_lcd *lcd, const uint8_t val);
        friend inline void send_data_in_8bit_mode (Custom_lcd *lcd, const uint8_t val);
        friend inline void send_data_in_4bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val);
        friend inline void send_data_in_8bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val);
        //
        uint8_t lcd_function,
                lcd_control,
                lcd_mode;
        //
        uint8_t number_of_rows;
        // used to send the absolute coordinate to lcd in setCursor() method, since lcd uses 1-dimension coordinate
        uint8_t offset_of_rows[4];
        // sends a pulse to the enable pin to notify lcd for the update
        inline void pulse_en_pin ();
        // checks if the data
        virtual void auto_select_best_data_sending_method ();

        // medium level control

        //
        inline void write_command (const uint8_t cmd);
        //
        inline void write_data (const uint8_t data);

        // commands not public to programmer
        //
        void setRowOffsets (int row1, int row2, int row3, int row4);
    public:
        //
        Custom_lcd (
            const uint8_t init_rs_pin,
            const uint8_t init_en_pin,
            const uint8_t d0_pin,
            const uint8_t d1_pin,
            const uint8_t d2_pin,
            const uint8_t d3_pin,
            const uint8_t d4_pin,
            const uint8_t d5_pin,
            const uint8_t d6_pin,
            const uint8_t d7_pin
        );
        //
        Custom_lcd (
            const uint8_t init_rs_pin,
            const uint8_t init_en_pin,
            const uint8_t d0_pin,
            const uint8_t d1_pin,
            const uint8_t d2_pin,
            const uint8_t d3_pin
        );
        void begin (const uint8_t cols, const uint8_t rows, const uint8_t dotsize = LCD_5x8DOTS);

        // high level control

        // commands
        // or you may say they are some cool features that an rcj soccer robot does not need
        void
            clear (),
            home (),
            noDisplay (),
            display (),
            noBlink (),
            blink (),
            noCursor (),
            cursor (),
            scrollDisplayLeft (),
            scrollDisplayRight (),
            leftToRight (),
            rightToLeft (),
            autoscroll (),
            noAutoscroll (),
            createChar (uint8_t location, uint8_t charmap[]);

        // commands
        void setCursor (const uint8_t x, uint8_t y);

        // high level control for programmer use
        void print_single_char (const char char_to_be_printed);
};

#endif // #ifndef CUSTOM_LCD_H