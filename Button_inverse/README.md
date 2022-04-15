# You should use code only when you understand them  
  
**Written exclusively for Arduino Nano, Uno, Mega, Mega2560, Mega2560 pro**  
  
You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Button_inverse that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library is only suitable for buttons that ground the signal pin when pressed.  
2. This library does not set the pin mode of the digital pin on Arduino that is connected to the signal pin of the button to input-pullup, but sets it to input.  
  
## How to use:  
1. **Constructor of a Button_inverse object ` yh::rec::Button_inverse::Button_inverse ( button_signal_pin ) `**  
Syntax: `yh::rec::Button_inverse button (3);`  
The line above calls the constructor of a Button_inverse object.  
You should declare an object of the Sen_1930 type outside of any functions.  
In the `()`, input the digital pin on the Arduino that is connected to the signal pin of the button.  
  
2. **`void yh::rec::Button_inverse::begin ()` method**  
Syntax: `button.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. **`bool yh::rec::Button_inverse::pressed_down ()` method**  
Syntax: `bool is_button_pressed = button.pressed_down();`  
This `pressed_down()` function checks if the button is currently pressed down. It returns true when the button is pressed, returns false when the button is released.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**