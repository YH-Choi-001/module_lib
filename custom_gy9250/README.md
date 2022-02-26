# This library is still under development, thus is not available. Please do not use any code from this custom_gy9250 folder.
<!--
# You should use code only when you understand them  

You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named custom_gy521 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library is applicable to GY-521 (MPU6000 or MPU6050), GY-6500 (MPU6500), GY-9250 (MPU9250),
    just treat the word gy-521 as the module you are holding right now.  
2. This library has configured some parameters to the module by default values which are based on RCJ soccer purpose.
    (The resolution of gyroscope is default to 1000 degrees per second.)  
3. You are advised to check the who_am_i value of the chip before usage, since there are many reports that an MPU6500 chip was installed on an MPU9250 board. The appearance of the module may fake you. The check is to let you understand what is the model of the chip that is really inside the module you are holding.  
4. This library sets the I2C clock to 400 KHz frequency (fast mode) to achieve faster communication.
  
## How to use:  
1. Constructor of a GY-521 object ` Custom_gy521::Custom_gy521 ( I2C_address ) `  
Syntax: ` Custom_gy521 gy521 (0x68); ` or `Custom_gy521 gy521 (0x69);`  
The line above calls the constructor of a GY-521 object.  
You should declare an object of the Custom_gy521 outside any any functions.  
In the `()`, input the I2C address of the chip (which is 0x68 by default).  
If you connect the AD0 pin of the chip to a pin written HIGH, the I2C address of the chip will be changed to 0x69.  
Therefore, you should input `0x68` to the `()` unless the AD0 pin of the chip is written HIGH, then you should input `0x69`.  
  
2. `void Custom_gy521::begin ()` method  
Syntax: `gy521.begin();`  
This `begin()` function configures the I2C communication from Wire.h (which is a built-in library with Arduino IDE).  
You must call this begin() in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. `uint8_t Custom_gy521::who_am_i ()` method  
Syntax: `uint8_t who_am_i = gy521.who_am_i();`  
This `who_am_i()` function asks the chip to give a value to identify itself.  
The value returned by this method can tell you whether this chip is MPU6000, MPU6050, MPU6500 or MPU9250.  
If the value returned by this function is 0x68 (or 104 in decimal), then this chip is MPU6000 or MPU6050.  
If the value returned by this function is 0x70 (or 106 in decimal), then this chip is MPU6500.  
If the value returned by this function is 0x71 (or 107 in decimal), then this chip is MPU9250.  
  
4. `volatile double Custom_gy521::roll, Custom_gy521::pitch, Custom_gy521::yaw` members  
These 3 members with the data type of `double` stores the current roll, pitch and yaw values of chip facing at.  
If you want to reset these values or assign a specific value to them, just type `gy521.roll = 180;` or `gy521.yaw = 0;` and so on.  
Warning: By rotating the chip in more than 1 axis at the same time will cause these values to fail.  
  
5. `volatile double Custom_gy521::corr_roll, Custom_gy521::corr_pitch, Custom_gy521::corr_yaw` members  
These 3 members with the data type of `double` stores the correction of roll, pitch and yaw values to be added to the gyroscope data collected every time the `update_gyro()` (or specific axis like `update_yaw()`) method is called.  
They stores the calibration values generated by the `cal_gyro()` (or specific axis like `cal_roll_()`) method.  
You may assign them to 0 or a value that you have achieved by previous calibrations on the same chip.  
  
6. `void Custom_gy521::cal_gyro ( sampling_amount, updating_function )` method  
Syntax: `gy521.cal_gyro();` or `gy521.cal_gyro(100);` or `gy521.cal_gyro(100, print_new_line);`  
**ATTENTION: When gyroscope calibration is in progress, put the chip on a flat surface, hold still, until this function has exited.**  
This `cal_gyro()` function calibrates all 3-axes of gyroscope of the chip.  
This function repeats reading the gyroscope values of the 3 axes and gets a mean value to be subtracted from every time the programmer gets new gyroscope data.  
The argument `sampling_amount` tells the function how many times it should get data samples. The larger the value, the calibration is more accurate, but also more time-consuming. By default, the function will get 8192 data samples, so this argument is optional.  
The argument `updating_function` tells the function to execute the updating_function() after the gyroscope has sampled 1 time. This function should be as short and precise as possible. By default, this method will not do anything after the gyroscope has sampled 1 time, so this argument is optional.  
  
7. `void Custom_gy521::update_gyro ()` method  
Syntax: `gy521.update_gyro();`  
This `update_gyro()` function updates the roll, pitch and yaw in the object.  
  
<!--
**The following methods are the 1-axis version of `cal_gyro(...)` and `update_gyro()` methods.**  
**They have the same functionality as `cal_gyro(...)` and `update_gyro()` but are a bit faster since they only need to get data of 1-axis.**  
  
8. `void Custom_gy521::cal_roll ( sampling_amount )` method  
Syntax: `gy521.cal_roll();` or `gy521.cal_roll(100);`  
**ATTENTION: When gyroscope calibration is in progress, put the chip on a flat surface, hold still, until this function has exited.**  
This `cal_roll()` function **only calibrates the x-axis of gyroscope** of the chip.  
This function repeats reading the gyroscope values of the **x-axis** and gets a mean value to be subtracted from every time the programmer gets new gyroscope data.  
The argument `sampling_amount` tells the function how many times it should get data samples. The larger the value, the calibration is more accurate, but also more time-consuming. By default, the funciton will get 8192 data samples, so this argument is optional.  
  
9. `void Custom_gy521::update_roll ()` method  
Syntax: `gy521.update_roll();`  
This `update_roll()` function **only updates the member `roll`** in the object.  
  
10. `void Custom_gy521::cal_pitch ( sampling_amount )` method  
Syntax: `gy521.cal_pitch();` or `gy521.cal_pitch(100);`  
**ATTENTION: When gyroscope calibration is in progress, put the chip on a flat surface, hold still, until this function has exited.**  
This `cal_pitch()` function **only calibrates the y-axis of gyroscope** of the chip.  
This function repeats reading the gyroscope values of the **y-axis** and gets a mean value to be subtracted from every time the programmer gets new gyroscope data.  
The argument `sampling_amount` tells the function how many times it should get data samples. The larger the value, the calibration is more accurate, but also more time-consuming. By default, the funciton will get 8192 data samples, so this argument is optional.  
  
11. `void Custom_gy521::update_pitch ()` method  
Syntax: `gy521.update_pitch();`  
This `update_pitch()` function **only updates the member `pitch`** in the object.  
  
12. `void Custom_gy521::cal_yaw ( sampling_amount )` method  
Syntax: `gy521.cal_yaw();` or `gy521.cal_yaw(100);`  
**ATTENTION: When gyroscope calibration is in progress, put the chip on a flat surface, hold still, until this function has exited.**  
This `cal_yaw()` function **only calibrates the z-axis of gyroscope** of the chip.  
This function repeats reading the gyroscope values of the **z-axis** and gets a mean value to be subtracted from every time the programmer gets new gyroscope data.  
The argument `sampling_amount` tells the function how many times it should get data samples. The larger the value, the calibration is more accurate, but also more time-consuming. By default, the funciton will get 8192 data samples, so this argument is optional.  
  
13. `void Custom_gy521::update_yaw ()` method  
Syntax: `gy521.update_yaw();`  
This `update_yaw()` function **only updates the member `yaw`** in the object.  
-->
  
## Datasheets and Register Maps:  
The links below are the datasheets and register maps of the 3 types of chips mentioned above.  
This library is written based on the datasheets and register maps below.  
Note that some important information, such as the way to interpret raw data, are written in the register map instead of the datasheet.  
If you would like to write your own library, please read the whole document before writing any code in order to let you know the features and limits of the module.  
  
MPU-60X0 (for MPU-6050)  
- datasheet  
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf  
- register map  
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf  
  
MPU-6500  
- datasheet  
http://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2020/06/PS-MPU-6500A-01-v1.3.pdf  
- register map  
https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6500-Register-Map2.pdf  
  
MPU-9250  
- datasheet  
https://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf  
- register map  
https://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**