# You should use code only when you understand them  
  
**Written exclusively for Arduino Nano, Uno, Mega, Mega2560, Mega2560 pro**  
  
You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Us_016 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library has configured some parameters to the module by default values which are based on RCJ soccer purpose.
    (The threshold of the brightness detected is set to half of the theoretical maximum value (which is 1023) by default.)  
<!--2. This library provides a fast type and a normal type of the class for the module, which the fast type does not store the readings of the sensor to member variables, while the normal type stores the readings of the sensor to member variables, and allows the programmer to read them without updating the sensors again.  -->
  
## How to use:  
1. **Constructor of an US-016 object ` yh::rec::Sen_1930::Sen_1930 ( signal_pin ) `**  
Syntax: `yh::rec::Sen_1930 sen_1930 (A0);`  
The line above calls the constructor of a SEN-1930 object.  
You should declare an object of the Sen_1930 type outside any any functions.  
In the `()`, input the analog pin on the Arduino that is connected to the signal pin on the SEN-1930.  
  
2. **`void yh::rec::Sen_1930::begin ()` method**  
Syntax: `sen_1930.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. **`void yh::rec::Sen_1930::set_threshold (  black_and_white_threshold  )` method**  
Syntax: `grayscale.set_threshold(700);`  
This `set_threshold()` function sets the threshold to determine whether the brightness detected is white or black. The brightness detected by the sensor is compared against the value of `black_and_white_threshold` inputed here to give boolean outputs in `touch_black()` and `touch_white()` methods. This threshold is set to 1023 / 2 = 511 by default.  
  
4. **`uint16_t yh::rec::Sen_1930::raw_read_gry_value ()` method**  
Syntax: `uint16_t brightness = grayscale.raw_read_gry_value();`  
This `raw_read_gry_value()` function returns the intensity of the brightness read, out of 1023 (10-bit resolution). It relies on the `analogRead()` function officially declared in `Arduino.h`.  
  
5. **`bool yh::rec::Sen_1930::touch_black ()` method**  
Syntax: `bool touching_black = grayscale.touch_black();`  
This `touch_black()` function checks if the grayscale sees black colour. For more details, see `set_threshold(...)` method.  
  
6. **`bool yh::rec::Sen_1930::touch_white ()` method**  
Syntax: `bool touching_white = grayscale.touch_white();`  
This `touch_white()` function checks if the grayscale sees white colour. For more details, see `set_threshold(...)` method.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**