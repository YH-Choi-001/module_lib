# You should use code only when you understand them  
  
**Written exclusively for Arduino Nano, Uno, Mega, Mega2560, Mega2560 pro**  
  
You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Hc_sr04 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library currently does not support muxing trig pin and echo pin of HC-SR04 together. It will be developed shortly on request of hardware team due to shortage of pins.  
2. The theoretical maximum measuring range of HC-SR04 is 400 cm, but most clone boards that are popular on the market are unable to achieve that distance.  
  
## How to use:  
  
#### Hc_sr04 (normal version)  
  
1. **Constructor of a Hc_sr04 object ` yh::rec::Hc_sr04::Hc_sr04 ( trig_pin, echo_pin ) `**  
Syntax: `yh::rec::Hc_sr04 hc_sr04 (2, 3);`  
The line above calls the constructor of a Hc_sr04 object.  
You should declare an object of the Hc_sr04 type outside of any functions.  
In the `()`, first input the digital pin on the Arduino that is connected to the trigger pin on the HC-SR04, then input the digital pin on the Arduino that is connected to the echo pin on the HC-SR04.  
  
2. **`void yh::rec::Hc_sr04::begin ()` method**  
Syntax: `hc_sr04.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. **`void yh::rec::Hc_sr04::set_max_waiting_time_in_us ( maximum_waiting_time )` method**  
Syntax: `hc_sr04.set_max_waiting_time_in_us(13000);`  
This `set_max_waiting_time_in_us()` function sets the maximum time to wait for the returning ultrasonic wave to return, which is so called timeout. If the returning ultrasonic wave is not detected before timeout, the `read_dist_mm()` and `read_dist_cm()` methods will return a distance-undefined value.  
  
4. **`void yh::rec::Hc_sr04::set_max_range_in_mm ( maximum_range )` method**
Syntax: `hc_sr04.set_max_range_in_mm(1200);`  
This `set_max_range_in_mm()` function converts the inputed maximum range in millimeters into maximum time to wait for the returning ultrasonic wave to return, then sets this as the timeout. If the returning ultrasonic wave is not detected before timeout, the `read_dist_mm()` and `read_dist_cm()` methods will return a distance-undefined value.  
  
5. **`void yh::rec::Hc_sr04::set_max_range_in_cm ( maximum_range )` method**
Syntax: `hc_sr04.set_max_range_in_cm(70);`  
This `set_max_range_in_cm()` function converts the inputed maximum range in centimeters into maximum time to wait for the returning ultrasonic wave to return, then sets this as the timeout. If the returning ultrasonic wave is not detected before timeout, the `read_dist_mm()` and `read_dist_cm()` methods will return a distance-undefined value.  
  
6. **`uint16_t yh::rec::Hc_sr04::read_dist_mm ()` method**  
Syntax: `uint16_t distance_read = hc_sr04.read_dist_mm();`  
This `read_dist_mm()` function reads the distance between this ultrasound sensor and the obstacle in front of it in millimeters. It returns a distance-undefined value, which is 8888, when the returning ultrasonic wave is not detected before timeout. It relies on the `pulseIn()` function officially declared in `Arduino.h`, which has better performance under no-interrupt-environments.  
  
7. **`uint16_t yh::rec::Hc_sr04::read_dist_cm ()` method**  
Syntax: `uint16_t distance_read = hc_sr04.read_dist_cm();`  
This `read_dist_cm()` function reads the distance between this ultrasound sensor and the obstacle in front of it in centimeters. It returns a distance-undefined value, which is 888, when the returning ultrasonic wave is not detected before timeout. It relies on the `pulseIn()` function officially declared in `Arduino.h`, which has better performance under no-interrupt-environments.  
  
<!--
8. **other classes: will be written to this file shortly**  
-->
#### Hc_sr04 (timer-interrpt version)  
  
8. **Constructor of a Hc_sr04_timer_int object ` yh::rec::Hc_sr04_timer_int::Hc_sr04_timer_int ( trig_pin, echo_pin ) `**  
Syntax: `yh::rec::Hc_sr04_timer_int hc_sr04 (2, 3);`  
The line above calls the constructor of a Hc_sr04_timer_int object.  
You should declare an object of the Hc_sr04_timer_int type outside of any functions.  
In the `()`, first input the digital pin on the Arduino that is connected to the trigger pin on the HC-SR04, then input the digital pin on the Arduino that is connected to the echo pin on the HC-SR04.  
  
9. **`void yh::rec::Hc_sr04_timer_int::begin ()` method**  
Syntax: `hc_sr04.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module, and kicks off the initial measurement.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
10. **`void yh::rec::Hc_sr04_timer_int::set_time_interval_between_interrupts ( time_interval_between_interrpts )` method**  
Syntax: `hc_sr04.set_time_interval_between_interrupts(59);`  
This `set_time_interval_between_interrupts()` function only tells the library the time interval between each `isr_individual_sensor_routine()` method called, so as to let the `read_previous_dist_cm()` method convert ticks into time then distance.  
You may call this method while the program is running, as sometimes the scanning frequency of echo pin may be changed by software while the program is running.  
**This method does not automatically attach Hc_sr04_timer_int::isr_individual_sensor_routine() method to a timer interrupt.**  
  
11. **`bool yh::rec::Hc_sr04_timer_int::is_data_ready ()` method**  
Syntax: `bool is_data_ready = hc_sr04.is_data_ready();`  
This `is_data_ready()` function returns a boolean to indicate whether the distance measurement has ended.  
If the distance measurement has ended, the method returns true.  
If the distance measurement has not ended, the method returns false.  
  
12. **`bool yh::rec::Hc_sr04_timer_int::is_data_not_ready ()` method**  
Syntax: `bool is_data_not_ready = hc_sr04.is_data_not_ready();`  
This `is_data_not_ready()` function returns a boolean to indicate whether the distance measurement has ended.  
If the distance measurement has ended, the method returns false.  
If the distance measurement has not ended, the method returns true.  
  
13. **`double yh::rec::Hc_sr04_timer_int::read_previous_dist_cm ()` method**  
Syntax: `double distance_read = hc_sr04.read_previous_dist_cm();`  
This `read_previous_dist_cm()` function returns the distance measured in the previous measurement in centimeters.  
It returns a distance-undefined value, which is 888, when the returning ultrasonic wave does not return.  
  
14. **`void yh::rec::Hc_sr04_timer_int::simple_update ()` method**  
Syntax: `hc_sr04.simple_update();`  
This `simple_update()` function checks if the current measurement has ended to start a new measurement.  
Always and only call this method in `void loop ()` function unconditionally to ensure continuing measurements.  
  
15. **`double yh::rec::Hc_sr04_timer_int::polling_update ()` method**  
Syntax: `double distance_read = hc_sr04.polling_update();`  
This `polling_update()` function combines `read_previous_dist_cm()` and `simple_update()` methods, which starts a new measurement if the current measurement has ended, and returns the previous distance measured in centimeters.  
It returns a distance-undefined value, which is 888, when the returning ultrasonic wave does not return.  
  
16. **`void yh::rec::Hc_sr04_timer_int::isr_individual_sensor_routine ()` method**  
Syntax: `hc_sr04.isr_individual_sensor_routine();`  
This `isr_individual_sensor_routine()` function performs checks on the state of the echo pins to measure the distance.  
You must always and only call this method in a timer interrupt whose frequency matches with the value you passed as argument in `set_time_interval_between_interrupts(...)` method.  
  
#### Hc_sr04 (hardware-interrupt and pin-change-interrupt version)  
  
17. **Constructor of a Hc_sr04_ext_int object ` yh::rec::Hc_sr04_ext_int::Hc_sr04_ext_int ( trig_pin, echo_pin ) `**  
Syntax: `yh::rec::Hc_sr04_ext_int hc_sr04 (2, 3);`  
The line above calls the constructor of a Hc_sr04_ext_int object.  
You should declare an object of the Hc_sr04_ext_int type outside of any functions.  
In the `()`, first input the digital pin on the Arduino that is connected to the trigger pin on the HC-SR04, then input the digital pin on the Arduino that is connected to the echo pin on the HC-SR04.  
  
18. **`void yh::rec::Hc_sr04_ext_int::begin ()` method**  
Syntax: `hc_sr04.begin();`  
This `begin()` function configures the pin modes of the pins occupied by this individual module, and kicks off the initial measurement.  
You must call this function in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
19. **`bool yh::rec::Hc_sr04_ext_int::is_data_ready ()` method**  
Syntax: `bool is_data_ready = hc_sr04.is_data_ready();`  
This `is_data_ready()` function returns a boolean to indicate whether the distance measurement has ended.  
If the distance measurement has ended, the method returns true.  
If the distance measurement has not ended, the method returns false.  
  
20. **`bool yh::rec::Hc_sr04_ext_int::is_data_not_ready ()` method**  
Syntax: `bool is_data_not_ready = hc_sr04.is_data_not_ready();`  
This `is_data_not_ready()` function returns a boolean to indicate whether the distance measurement has ended.  
If the distance measurement has ended, the method returns false.  
If the distance measurement has not ended, the method returns true.  
  
21. **`double yh::rec::Hc_sr04_ext_int::read_previous_dist_mm ()` method**  
Syntax: `double distance_read = hc_sr04.read_previous_dist_mm();`  
This `read_previous_dist_mm()` function returns the distance measured in the previous measurement in millimeters.  
It returns a distance-undefined value, which is 8888, when the returning ultrasonic wave does not return.  
  
22. **`double yh::rec::Hc_sr04_ext_int::read_previous_dist_cm ()` method**  
Syntax: `double distance_read = hc_sr04.read_previous_dist_cm();`  
This `read_previous_dist_cm()` function returns the distance measured in the previous measurement in centimeters.  
It returns a distance-undefined value, which is 888, when the returning ultrasonic wave does not return.  
  
23. **`void yh::rec::Hc_sr04_ext_int::simple_update ()` method**  
Syntax: `hc_sr04.simple_update();`  
This `simple_update()` function checks if the current measurement has ended to start a new measurement.  
Always and only call this method in `void loop ()` function unconditionally to ensure continuing measurements.  
  
24. **`double yh::rec::Hc_sr04_ext_int::polling_update ()` method**  
Syntax: `double distance_read = hc_sr04.polling_update();`  
This `polling_update()` function combines `read_previous_dist_cm()` and `simple_update()` methods, which starts a new measurement if the current measurement has ended, and returns the previous distance measured in centimeters.  
It returns a distance-undefined value, which is 888, when the returning ultrasonic wave does not return.  
  
25. **`void yh::rec::Hc_sr04_ext_int::isr_individual_sensor_routine ()` method**  
Syntax: `hc_sr04.isr_individual_sensor_routine();`  
This `isr_individual_sensor_routine()` function performs checks on the state of the echo pins to measure the distance.  
You must always and only call this method in an hardware interrupt in CHANGE mode, or a pin-change interrupt. Never call this method when the echo pin of this module has not changed its state.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**