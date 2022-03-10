#ifndef CUSTOM_PRINT_H
#define CUSTOM_PRINT_H __DATE__ ", " __TIME__

// This library does not contain really complicated maths when dealing number-to-string conversion for higher speed performance.
// For best number-to-string conversion, see YH::Lib::Func::texts::base_10_to_base_n(...) function from YHtextLib.
class Custom_print {
    public:
        // the only method to print a character onto the lcd
        // you must define print_single_char(const char) in your class
        // you will encounter a compile error if you don't re-define me in your inherited class
        virtual void print_single_char (const char char_to_be_printed);
        // prints the char to the destination as an ASCII character
        // @param char the character to be printed
        void print (const char char_to_be_printed);
        // prints the string to the destination as an ASCII string
        // @param str the char [] (or string) to be printed
        void print (const char *str, const char terminating_char = '\0');
        // prints the unsigned integer to the destination as an ASCII string
        // @param val the value to be printed
        void
            print (const uint8_t val),
            print (const uint16_t val),
            print (const uint32_t val);
        // prints the signed integer to the destination as an ASCII string
        // @param val the value to be printed
        void
            print (const int8_t val),
            print (const int16_t val),
            print (const int32_t val);
        //
        // prints the double to the destination as an ASCII string
        // @param val the value to be printed
        // @param dp tells the method how many decimal places do you want to correct it to
        void print (const double val, const uint8_t dp = 2);
};

#endif // #ifndef CUSTOM_PRINT_H