# You should use code only when you understand them  
  
**Written exclusively for Arduino Nano, Uno, Mega, Mega2560, Mega2560 pro**  
  
You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Us_016 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library has configured some parameters to the module by default values which are based on RCJ soccer purpose.
    (The resolution of the pwm is set to 8-bit by default.)  
  
## How to use:  
1. **Constructor of a Btn7971b object ` yh::rec::Btn7971b::Btn7971b ( pwm_pin, direction_pin ) `**  
Syntax: `yh::rec::Btn7971b motor (6, 7);`
The line above calls the constructor of a Btn7971b object.  
You should declare an object of the Btn7971b type outside of any functions.  
In the `()`, first input the pwm pin on the Arduino that is connected to 1 of the 2 IN pins on the Btn7971b, then input the digital pin on the Arduino that is connected to the another IN pin on the Btn7971b.  
  
2. **`void yh::rec::Btn7971b::begin ()` method**  
Syntax: `motor.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. **`void yh::rec::Us_016::set_spd ( input_speed )` method**  
Syntax: `us_016.set_spd(127);`  
This `set_spd()` function sets the speed of the motor in both forwards and backwards. `input_speed` ranges from -255 to +255.  
  
4. **`void yh::rec::Us_016::stop_motor ()` method**  
Syntax: `us_016.stop_motor();`  
This `stop_motor()` function stops the motor. It is a bit faster than using the `set_spd()` method.  
  
5. **`void yh::rec::Us_016::full_spd_forwards ()` method**  
Syntax: `us_016.stop_motor();`  
This `full_spd_forwards()` function sets the speed of the motor to full speed forwards. It is a bit faster than using the `set_spd()` method.  
  
6. **`void yh::rec::Us_016::full_spd_backwards ()` method**  
Syntax: `us_016.stop_motor();`  
This `full_spd_forwards()` function sets the speed of the motor to full speed backwards. It is a bit faster than using the `set_spd()` method.  
  
7. **`int16_t yh::rec::Us_016::get_spd ()` method**  
Syntax: `int16_t inputed_motor_speed = us_016.get_spd();`  
This `get_spd()` function returns the output speed to the motor. It may not be equal to the value you inputed in `set_spd(...)` function, because `set_spd(...)` is limited in +-255 only.  
  
8. **`int16_t yh::rec::Us_016::operator += ( speed_increased )` method**  
Syntax: `us_016 += 3;`  
This `operator += ()` function increases the speed of the motor by `speed_increased`. It is really suitable for motor with encoders, that allows fine tuning the speed of the motor with reference to feedbacks.  
  
9. **`int16_t yh::rec::Us_016::operator -= ( speed_decreased )` method**  
Syntax: `us_016 -= 3;`  
This `operator -= ()` function decreases the speed of the motor by `speed_decreased`. It is really suitable for motor with encoders, that allows fine tuning the speed of the motor with reference to feedbacks.  
  
10. **`int16_t yh::rec::Us_016::operator ++ ()` method**  
Syntax: `us_016++;`  
This `operator ++ ()` function increases the speed of the motor by 1. It is really suitable for motor with encoders, that allows fine tuning the speed of the motor with reference to feedbacks.  
  
10. **`int16_t yh::rec::Us_016::operator -- ()` method**  
Syntax: `us_016--;`  
This `operator -- ()` function decreases the speed of the motor by 1. It is really suitable for motor with encoders, that allows fine tuning the speed of the motor with reference to feedbacks.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**