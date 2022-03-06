#ifndef CUSTOM_LCD_CPP
#define CUSTOM_LCD_CPP __DATE__ ", " __TIME__

#include "Custom_lcd.h"

// sends the data in 4-bit mode
inline void send_data_in_4bit_mode (Custom_lcd *lcd, const uint8_t val);
// sends the data in 8-bit mode
inline void send_data_in_8bit_mode (Custom_lcd *lcd, const uint8_t val);
// sends the data in 4-bit mode
// this method is faster because it assigns the val parameter to the output port register with 4-bits together
inline void send_data_in_4bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val);
// sends the data in 8-bit mode
// this method is faster because it assigns the val parameter to the output port register directly
inline void send_data_in_8bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val);

void Custom_lcd::pulse_en_pin () {
    (*en_pin_output_reg) &= ~en_pin_mask; // digitalWrite(en_pin, LOW)
    delayMicroseconds(1);
    (*en_pin_output_reg) |= en_pin_mask; // digitalWrite(en_pin, HIGH)
    delayMicroseconds(1); // enable pulse must be longer than 450 ns
    (*en_pin_output_reg) &= ~en_pin_mask; // digitalWrite(en_pin, LOW)
    delayMicroseconds(100); // sending a command takes longer than 37 us to settle
}

inline void send_data_in_4bit_mode (Custom_lcd *lcd, const uint8_t val) {
    (val & 0x80) ? (*(lcd->data_pins_output_reg[3]) |= lcd->data_pins_mask[3]) : (*(lcd->data_pins_output_reg[3]) &= ~lcd->data_pins_mask[3]);
    (val & 0x40) ? (*(lcd->data_pins_output_reg[2]) |= lcd->data_pins_mask[2]) : (*(lcd->data_pins_output_reg[2]) &= ~lcd->data_pins_mask[2]);
    (val & 0x20) ? (*(lcd->data_pins_output_reg[1]) |= lcd->data_pins_mask[1]) : (*(lcd->data_pins_output_reg[1]) &= ~lcd->data_pins_mask[1]);
    (val & 0x10) ? (*(lcd->data_pins_output_reg[0]) |= lcd->data_pins_mask[0]) : (*(lcd->data_pins_output_reg[0]) &= ~lcd->data_pins_mask[0]);
    lcd->pulse_en_pin();
    (val & 0x08) ? (*(lcd->data_pins_output_reg[3]) |= lcd->data_pins_mask[3]) : (*(lcd->data_pins_output_reg[3]) &= ~lcd->data_pins_mask[3]);
    (val & 0x04) ? (*(lcd->data_pins_output_reg[2]) |= lcd->data_pins_mask[2]) : (*(lcd->data_pins_output_reg[2]) &= ~lcd->data_pins_mask[2]);
    (val & 0x02) ? (*(lcd->data_pins_output_reg[1]) |= lcd->data_pins_mask[1]) : (*(lcd->data_pins_output_reg[1]) &= ~lcd->data_pins_mask[1]);
    (val & 0x01) ? (*(lcd->data_pins_output_reg[0]) |= lcd->data_pins_mask[0]) : (*(lcd->data_pins_output_reg[0]) &= ~lcd->data_pins_mask[0]);
    lcd->pulse_en_pin();
}

inline void send_data_in_8bit_mode (Custom_lcd *lcd, const uint8_t val) {
    (val & 0x80) ? (*(lcd->data_pins_output_reg[7]) |= lcd->data_pins_mask[7]) : (*(lcd->data_pins_output_reg[7]) &= ~lcd->data_pins_mask[7]);
    (val & 0x40) ? (*(lcd->data_pins_output_reg[6]) |= lcd->data_pins_mask[6]) : (*(lcd->data_pins_output_reg[6]) &= ~lcd->data_pins_mask[6]);
    (val & 0x20) ? (*(lcd->data_pins_output_reg[5]) |= lcd->data_pins_mask[5]) : (*(lcd->data_pins_output_reg[5]) &= ~lcd->data_pins_mask[5]);
    (val & 0x10) ? (*(lcd->data_pins_output_reg[4]) |= lcd->data_pins_mask[4]) : (*(lcd->data_pins_output_reg[4]) &= ~lcd->data_pins_mask[4]);
    (val & 0x08) ? (*(lcd->data_pins_output_reg[3]) |= lcd->data_pins_mask[3]) : (*(lcd->data_pins_output_reg[3]) &= ~lcd->data_pins_mask[3]);
    (val & 0x04) ? (*(lcd->data_pins_output_reg[2]) |= lcd->data_pins_mask[2]) : (*(lcd->data_pins_output_reg[2]) &= ~lcd->data_pins_mask[2]);
    (val & 0x02) ? (*(lcd->data_pins_output_reg[1]) |= lcd->data_pins_mask[1]) : (*(lcd->data_pins_output_reg[1]) &= ~lcd->data_pins_mask[1]);
    (val & 0x01) ? (*(lcd->data_pins_output_reg[0]) |= lcd->data_pins_mask[0]) : (*(lcd->data_pins_output_reg[0]) &= ~lcd->data_pins_mask[0]);
    lcd->pulse_en_pin();
}

inline void send_data_in_4bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val) {
    ( *(lcd->data_pins_output_reg[0]) ) =
    (   ( *(lcd->data_pins_output_reg[0]) ) & ( ~(0x0f << lcd->_4bit_mode_of_same_port_l_shift_val) )   ) | // turn off the 4 bits connecting lcd only
    ( (val & 0xf0) >> (4 - lcd->_4bit_mode_of_same_port_l_shift_val) ); // turn on the bits connecting lcd according to the value
    lcd->pulse_en_pin();
    ( *(lcd->data_pins_output_reg[0]) ) =
    (   ( *(lcd->data_pins_output_reg[0]) ) & ( ~(0x0f << lcd->_4bit_mode_of_same_port_l_shift_val) )   ) | // turn off the 4 bits connecting lcd only
    ( (val & 0x0f) << lcd->_4bit_mode_of_same_port_l_shift_val ); // turn on the bits connecting lcd according to the value
    lcd->pulse_en_pin();
}

inline void send_data_in_8bit_mode_of_same_port (Custom_lcd *lcd, const uint8_t val) {
    *(lcd->data_pins_output_reg[0]) = val;
    lcd->pulse_en_pin();
}

void Custom_lcd::auto_select_best_data_sending_method () {
    if (lcd_function & LCD_8BITMODE) {
        // the object is connected in 8-bit mode
        bool use_same_port_8bit_send_data_method = true;
        for (uint8_t i = 0; i < 8; i++) {
            if ( (data_pins_output_reg[i] != data_pins_output_reg[0]) || (data_pins_mask[i] != (0x01 << i)) ) {
                // 8-bit send data method cannot be used
                use_same_port_8bit_send_data_method = false;
                send_data = send_data_in_8bit_mode;
                break;
            }
        }
        if (use_same_port_8bit_send_data_method)
            send_data = send_data_in_8bit_mode_of_same_port;
    } else {
        // the object is connected in 4-bit mode
        bool use_same_port_4bit_send_data_method = true;
        for (uint8_t i = 0; i < 4; i++) {
            if ( (data_pins_output_reg[i] != data_pins_output_reg[0]) || (data_pins_mask[i] != (data_pins_mask[0] << i)) ) {
                // 8-bit send data method cannot be used
                use_same_port_4bit_send_data_method = false;
                send_data = send_data_in_4bit_mode;
                break;
            }
        }
        if (use_same_port_4bit_send_data_method) {
            send_data = send_data_in_4bit_mode_of_same_port;
            for (uint8_t i = 0; i < 5; i++) {
                if (data_pins_mask[0] == (0x01 << i)) {
                    _4bit_mode_of_same_port_l_shift_val = i;
                    break;
                }
            }
        }
    }
}

// ==================================================================================================== low level control

inline void Custom_lcd::write_command (const uint8_t cmd) {
    (*rs_pin_output_reg) &= ~rs_pin_mask; // digitalWrite(rs_pin, LOW)
    send_data(this, cmd);
}

inline void Custom_lcd::write_data (const uint8_t data) {
    (*rs_pin_output_reg) |= rs_pin_mask; // digitalWrite(rs_pin, HIGH)
    send_data(this, data);
}

// ==================================================================================================== commands not public to programmer

void Custom_lcd::setRowOffsets (int row0, int row1, int row2, int row3) {
    offset_of_rows[0] = row0;
    offset_of_rows[1] = row1;
    offset_of_rows[2] = row2;
    offset_of_rows[3] = row3;
}

// ==================================================================================================== constructors

#define CONFIG_OUTPUT_REG_N_MASKS(x) \
    x##_pin_output_reg=portOutputRegister(digitalPinToPort(x##_pin)); x##_pin_mask=digitalPinToBitMask(x##_pin);

// 8-bit mode constructor
Custom_lcd::Custom_lcd (
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
) :
    rs_pin(init_rs_pin),
    en_pin(init_en_pin),
    data_pins{d0_pin, d1_pin, d2_pin, d3_pin, d4_pin, d5_pin, d6_pin, d7_pin}
{
    CONFIG_OUTPUT_REG_N_MASKS(rs);
    CONFIG_OUTPUT_REG_N_MASKS(en);
    CONFIG_OUTPUT_REG_N_MASKS(d0);
    CONFIG_OUTPUT_REG_N_MASKS(d1);
    CONFIG_OUTPUT_REG_N_MASKS(d2);
    CONFIG_OUTPUT_REG_N_MASKS(d3);
    CONFIG_OUTPUT_REG_N_MASKS(d4);
    CONFIG_OUTPUT_REG_N_MASKS(d5);
    CONFIG_OUTPUT_REG_N_MASKS(d6);
    CONFIG_OUTPUT_REG_N_MASKS(d7);
    lcd_function = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

// 4-bit mode constructor
Custom_lcd::Custom_lcd (
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
) :
    rs_pin(init_rs_pin),
    en_pin(init_en_pin),
    data_pins{d0_pin, d1_pin, d2_pin, d3_pin, 0, 0, 0, 0}
{
    CONFIG_OUTPUT_REG_N_MASKS(rs);
    CONFIG_OUTPUT_REG_N_MASKS(en);
    CONFIG_OUTPUT_REG_N_MASKS(d0);
    CONFIG_OUTPUT_REG_N_MASKS(d1);
    CONFIG_OUTPUT_REG_N_MASKS(d2);
    CONFIG_OUTPUT_REG_N_MASKS(d3);
    lcd_function = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
}

// ==================================================================================================== begin()

void Custom_lcd::begin (const uint8_t cols, const uint8_t rows, const uint8_t dotsize) {
    if (rows > 1) {
        lcd_function |= LCD_2LINE;
    }
    number_of_rows = rows;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != LCD_5x8DOTS) && (rows == 1)) {
        lcd_function |= LCD_5x10DOTS;
    }

    pinMode(rs_pin, OUTPUT);
    pinMode(en_pin, OUTPUT);
    // pinMode(rw_pin, OUTPUT);

    for (uint8_t i = 0; i < ( (lcd_function & LCD_8BITMODE) ? 8 : 4 ); i++) {
        pinMode(data_pins[i], OUTPUT);
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    delayMicroseconds(50000);
    // Now we pull both RS and R/W low to begin commands
    (*rs_pin_output_reg) &= ~rs_pin_mask; // digitalWrite(rs_pin, LOW);
    (*en_pin_output_reg) &= ~en_pin_mask; // digitalWrite(en_pin, LOW);
    // digitalWrite(rw_pin, LOW);
    
    //put the LCD into 4 bit or 8 bit mode
    if (lcd_function & LCD_8BITMODE) {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        write_command(LCD_FUNCTIONSET | lcd_function);
        delayMicroseconds(4500);  // wait more than 4.1ms

        // second try
        write_command(LCD_FUNCTIONSET | lcd_function);
        delayMicroseconds(150);

        // third go
        write_command(LCD_FUNCTIONSET | lcd_function);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        // write4bits(0x03);
        (*(data_pins_output_reg[3]) &= ~data_pins_mask[3]);
        (*(data_pins_output_reg[2]) &= ~data_pins_mask[2]);
        (*(data_pins_output_reg[1]) |= data_pins_mask[1]);
        (*(data_pins_output_reg[0]) |= data_pins_mask[0]);
        lcd->pulse_en_pin();
        delayMicroseconds(4500); // wait min 4.1ms

        // second try
        // write4bits(0x03);
        (*(data_pins_output_reg[3]) &= ~data_pins_mask[3]);
        (*(data_pins_output_reg[2]) &= ~data_pins_mask[2]);
        (*(data_pins_output_reg[1]) |= data_pins_mask[1]);
        (*(data_pins_output_reg[0]) |= data_pins_mask[0]);
        lcd->pulse_en_pin();
        delayMicroseconds(4500); // wait min 4.1ms
        
        // third go!
        // write4bits(0x03);
        (*(data_pins_output_reg[3]) &= ~data_pins_mask[3]);
        (*(data_pins_output_reg[2]) &= ~data_pins_mask[2]);
        (*(data_pins_output_reg[1]) |= data_pins_mask[1]);
        (*(data_pins_output_reg[0]) |= data_pins_mask[0]);
        lcd->pulse_en_pin();
        delayMicroseconds(150);

        // finally, set to 4-bit interface
        // write4bits(0x02);
        (*(data_pins_output_reg[3]) &= ~data_pins_mask[3]);
        (*(data_pins_output_reg[2]) &= ~data_pins_mask[2]);
        (*(data_pins_output_reg[1]) |= data_pins_mask[1]);
        (*(data_pins_output_reg[0]) &= ~data_pins_mask[0]);
        lcd->pulse_en_pin();
    }

    // finally, set # lines, font size, etc.
    write_command(LCD_FUNCTIONSET | lcd_function);

    // turn the display on with no cursor or blinking default
    lcd_control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    lcd_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    write_command(LCD_ENTRYMODESET | lcd_mode);

    auto_select_best_data_sending_method();
}

// ==================================================================================================== commands you don't need in rcj soccer

/********** high level commands, for the user! */
void Custom_lcd::clear () {
    write_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero++++++++++++
    delayMicroseconds(2000);  // this command takes a long time!
}

void Custom_lcd::home () {
    write_command(LCD_RETURNHOME);  // set cursor position to zero
    delayMicroseconds(2000);  // this command takes a long time!
}

// Turn the display on/off (quickly)
void Custom_lcd::noDisplay () {
    lcd_control &= ~LCD_DISPLAYON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}
void Custom_lcd::display () {
    lcd_control |= LCD_DISPLAYON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}

// Turns the underline cursor on/off
void Custom_lcd::noCursor () {
    lcd_control &= ~LCD_CURSORON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}
void Custom_lcd::cursor () {
    lcd_control |= LCD_CURSORON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}

// Turn on and off the blinking cursor
void Custom_lcd::noBlink () {
    lcd_control &= ~LCD_BLINKON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}
void Custom_lcd::blink () {
    lcd_control |= LCD_BLINKON;
    write_command(LCD_DISPLAYCONTROL | lcd_control);
}

// These commands scroll the display without changing the RAM
void Custom_lcd::scrollDisplayLeft () {
    write_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Custom_lcd::scrollDisplayRight () {
    write_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void Custom_lcd::leftToRight () {
    lcd_mode |= LCD_ENTRYLEFT;
    write_command(LCD_ENTRYMODESET | lcd_mode);
}

// This is for text that flows Right to Left
void Custom_lcd::rightToLeft () {
    lcd_mode &= ~LCD_ENTRYLEFT;
    write_command(LCD_ENTRYMODESET | lcd_mode);
}

// This will 'right justify' text from the cursor
void Custom_lcd::autoscroll () {
    lcd_mode |= LCD_ENTRYSHIFTINCREMENT;
    write_command(LCD_ENTRYMODESET | lcd_mode);
}

// This will 'left justify' text from the cursor
void Custom_lcd::noAutoscroll () {
    lcd_mode &= ~LCD_ENTRYSHIFTINCREMENT;
    write_command(LCD_ENTRYMODESET | lcd_mode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void Custom_lcd::createChar (uint8_t location, uint8_t charmap[]) {
    if (location > 7) location = 7;
    write_command(LCD_SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write_data(charmap[i]);
    }
}

// ==================================================================================================== commands

void Custom_lcd::setCursor (const uint8_t x, const uint8_t y) {
    const size_t max_lines = sizeof(offset_of_rows) / sizeof(offset_of_rows[0]);
    if ( y >= max_lines ) {
        y = max_lines - 1;    // we count rows starting with 0
    }
    if ( y >= number_of_rows ) {
        y = number_of_rows - 1;    // we count rows starting with 0
    }
    write_command(LCD_SETDDRAMADDR | (x + _row_offsets[y]));
}

void Custom_lcd::print (const char char_to_be_printed) {
    write_data(char_to_be_printed);
}

void Custom_lcd::print (const char *str) {
    while (*str != NULL) {
        print(*str);
        str++;
    }
}

uint32_t pow_base_10 (const uint8_t exp) {
    uint32_t to_be_returned = 1;
    for (uint8_t i = 0; i < exp; to_be_returned *= 10, i++) { }
    return to_be_returned;
}

void Custom_lcd::print (const uint8_t val) {
    bool began_printing = false;
    for (uint8_t base = 100U; base; base /= 10U) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const uint16_t val) {
    bool began_printing = false;
    for (uint16_t base = 10000U; base; base /= 10U) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const uint32_t val) {
    bool began_printing = false;
    for (uint32_t base = 1000000000UL; base; base /= 10UL) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const int8_t val) {
    if (val < 0) print('-');
    bool began_printing = false;
    for (int8_t base = 100; base; base /= 10) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const int16_t val) {
    if (val < 0) print('-');
    bool began_printing = false;
    for (int16_t base = 10000; base; base /= 10) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const int32_t val) {
    if (val < 0) print('-');
    bool began_printing = false;
    for (int32_t base = 1000000000L; base; base /= 10L) {
        const uint8_t to_be_printed = val / base;
        if (began_printing || to_be_printed) {
            print('0' + to_be_printed);
            began_printing = true;
        }
    }
}

void Custom_lcd::print (const float val) {
    print(static_cast<double>(val));
}

void Custom_lcd::print (const double val, const uint8_t dp) {
    double pcs_val;
    if (val < 0) {
        print('-');
        pcs_val = -val;
    } else {
        pcs_val = val;
    }
    print(static_cast<uint32_t>(pcs_val));
    pcs_val -= static_cast<uint32_t>(pcs_val);
    pcs_val += 0.5 / pow_base_10(dp);
    for (uint8_t i = 0; i < dp; i++) {
        print('0' + static_cast<char>(pcs_val *= 10));
    }
}

#endif // #ifndef CUSTOM_LCD_CPP