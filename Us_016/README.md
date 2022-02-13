# You should use code only when you understand them  
  
**Written exclusively for Arduino Pro Mini, Nano, Uno, Mega, Mega2560, Mega2560 pro**  
  
You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Us_016 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library has configured some parameters to the module by default values which are based on RCJ soccer purpose.
    (The range of the sensor is default to 3069 millimeters with a resolution of 3mm.)  
2. This library provides a fast type and a normal type of the class for the module, where the fast type does not store the readings of the sensor to variables, while the normal type stores the readings of the sensor to variables, and allows the programmer to read them without updating the sensors again.  
3. When 3 or more US-016 modules are used on the same Arduino board, only 2 of them can read useful readings, while the other modules can only read garbage readings.  
  
## How to use:  
1. **Constructor of an US-016 object** ` yh::rec::Us_016::Us_016 ( digital_pin, analog_pin, range ) ` (also applicable to `yh::rec::Us_016_fast`)  
Syntax: `yh::rec::Us_016 us_016 (13, A0);` or `yh::rec::Us_016 us_016 (13, A0, 3);`  
The line above calls the constructor of an US-016 object.  
You should declare an object of the Us_016 type outside any any functions.  
In the `()`, first input the digital pin on the Arduino that is connected to the range pin on the US-016, then input the analog pin on the Arduino that is connected to the out pin on the US-016. Last, you can input the range of the US-016 module, either 1 for 1 meter or 3 for 3 meters. It is by default set to 3 meters, so it is optional. For more details, see `set_resolution(...)` method.  
  
2. `void yh::rec::Us_016::begin ()` **method** (also applicable to `yh::rec::Us_016_fast`)  
Syntax: `us_016.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module.  
You must call this begin() in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. `uint16_t yh::rec::Us_016::read_dist_mm ()` **method** (also applicable to `yh::rec::Us_016_fast`)  
Syntax: `uint16_t distance_read = us_016.read_dist_mm();`  
This `read_dist_mm()` function reads the distance between this ultrasound sensor and the obstacle in front of it in millimeters.  
  
4. `uint16_t yh::rec::Us_016::read_dist_cm ()` **method** (also applicable to `yh::rec::Us_016_fast`)  
Syntax: `uint16_t distance_read = us_016.read_dist_cm();`  
This `read_dist_mm()` function reads the distance between this ultrasound sensor and the obstacle in front of it in centimeters.  
  
5. `uint16_t yh::rec::Us_016::get_previous_dist_mm ()` **method** (**not** applicable to `yh::rec::Us_016_fast`)  
Syntax: `uint16_t distance_read = us_016.get_previous_dist_mm();`  
This `read_dist_mm()` function gets the previous distance read between this ultrasound sensor and the obstacle in front of it in millimeters.  
  
6. `uint16_t yh::rec::Us_016::get_previous_dist_cm ()` **method** (**not** applicable to `yh::rec::Us_016_fast`)  
Syntax: `uint16_t distance_read = us_016.get_previous_dist_cm();`  
This `read_dist_mm()` function gets the previous distance read between this ultrasound sensor and the obstacle in front of it in centimeters.  
  
7. `uint16_t yh::rec::Us_016::set_resolution ( resolution )` **method** (also applicable to `yh::rec::Us_016_fast`)  
Syntax: `us_016.set_resolution(1);` or `us_016.set_resolution(3);`  
This `set_resolution()` function sets the resolution of the module to either 1 millimeter or 3 millimeters.  
When the resolution is set to 1 millimeter, the detectable range of the module will be limited to 1 meter.  
When the resolution is set to 3 millimeters, the detectable range of the module will be limited to 3 meters.  
You should either input `1` or `3` to the argument `resolution`.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**