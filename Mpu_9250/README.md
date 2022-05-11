<!--# This library is still under development, thus is not available. Please do not use any code from this custom_gy9250 folder.  
  
## The code of this library are tested and proved to be successful, but this documentation is still being written. Please do not use any part of this library until this documentation is finished.  

-->
# You should use code only when you understand them  

You may download this folder to ANYWHERE YOU LIKE in your local disk, and then open your Arduino IDE.  
Select from the IDE's toolbar: Sketch -> Include Library -> Add .ZIP library... -> and select the folder named Mpu_9250 that you have just downloaded.  
Then you can use the library of this module.  
You may like to see the examples of this library to get a better understanding on how to use this library.  
To do so, select from the IDE's toolbar: File -> Examples -> (Scroll to the bottom) and hover the library name of the module you have added to Arduino IDE just now, then you can take a look or even try uploading the examples to your Arduino board.
  
## Remarks:  
1. This library uses some of the code in Mpu_6050 library as they have a lot in common. You must download the Mpu_6050 library too in order to let this library to work.  
2. This library has configured some parameters to the module by default values which are based on RCJ soccer purpose.
    (The resolution of gyroscope is default to 2000 degrees per second.)  
3. You are advised to check the who_am_i value of the chip before usage, since there are many reports that an MPU6500 chip was installed on an MPU9250 board. The appearance of the module may fake you. The check is to let you understand what is the model of the chip that is really inside the module you are holding.  
4. This library sets the I2C clock to 400 KHz frequency (fast mode) to achieve faster communication.
  
## How to use:  
1. **Constructor of a `yh::rec::Mpu_9250` object `yh::rec::Mpu_9250::Mpu_9250 ( I2C_address )`**  
Syntax: `yh::rec::Mpu_9250 gy9250 (0x68);` or `yh::rec::Mpu_9250 gy9250 (0x69);`  
The line above calls the constructor of a `yh::rec::Mpu_9250` object.  
You should declare an object of the `yh::rec::Mpu_9250` outside of any functions.  
In the `()`, input the I2C address of the chip (which is 0x68 by default).  
If you connect the AD0 pin of the chip to a pin written HIGH, the I2C address of the chip will be changed to 0x69.  
Therefore, you should input `0x68` to the `()` unless the AD0 pin of the chip is written HIGH, then you should input `0x69`.  
  
2. **`void yh::rec::Mpu_9250::begin ()` method**  
Syntax: `gy9250.begin();`  
This `begin()` function configures the I2C communication from Wire.h (which is a built-in library with Arduino IDE).  
You must call this begin() in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
  
3. **other MPU-6050 identical methods**  
See the documentation for Mpu_6050 library.  
  
4. **`yh::rec::Ak_8963 yh::rec::Mpu_9250::mag` object**  
Syntax: `gy9250.mag;`  
This `mag` object is automatically initialized by the constructor of `yh::rec::Mpu_9250`. If you wish to use the AK-8963 magnetometer on board of the MPU-9250 chip through methods of this `Ak_8963`, you should call `void yh::rec::Mpu_6050::enable_ext_i2c_slave_sensors ()` method before calling any methods of the `Ak_8963` object, including the `void yh::rec::Ak_8963::begin()` method, as `void yh::rec::Ak_8963::begin()` makes I2C communication with the ak8963 chip to configure some settings behind the scenes.  
  
5. **Constructor of a `yh::rec::Ak_8963` object `yh::rec::Ak_8963::Ak_8963 ( I2C_address )`**  
Syntax: `yh::rec::Ak_8963 mag (0x0C);`  
The line above calls the constructor of a yh::rec::Ak_8963 object.  
You should declare an object of the yh::rec::Ak_8963 outside of any functions.  
In the `()`, input the I2C address of the chip (which is 0x0C by default).  
This constructor is automatically called within the constructor of `yh::rec::Mpu_9250` when an object of `yh::rec::Ak_Mpu_9250` is declared.  
  
6. **`void yh::rec::Ak_8963::begin ()` method**  
Syntax: `mag.begin();` (or `gy9250.mag.begin();` for GY-9250 users)  
This `begin()` function configures the settings of the I2C bus and the chip.  
You must call this begin() in void setup () function to use this object properly.  
No arguments should be inputed when calling this function.  
If you are using the object as a member of Custom_gy9250, only call this method after you called `void yh::rec::Mpu_6050::enable_ext_i2c_slave_sensors ()` method.  
  
7. **`uint8_t yh::rec::Ak_8963::who_i_am ()` method**  
Syntax: `uint8_t wia = mag.who_i_am();` (or `uint8_t wia = gy9250.mag.who_i_am();` for GY-9250 users)  
This `who_i_am()` function asks the chip to give a value to identify itself.  
The value returned by this method can tell you whether this chip is AK-8963 or not.  
If the value returned by this function is 0x48 (or 72 in decimal), then this chip is AK-8963.  
  
8. **`void yh::rec::Ak_8963::single_calibrate ()` method**  
Syntax: `mag.single_calibrate();` (or `gy9250.mag.single_calibrate();` for GY-9250 users)  
This `single_calibrate()` function calibrates the magnetometer for 1 time.  
You should call this method in a for-loop for hundreds or thousands times, depending on your preference for the precision required.  
The calibrated data only helps give a relatively-zero value to each axis for direction-finding when using AK-8963 as a compass.  
These data do not do anything if you intended to collect the strength of magnetic field only.  
  
9. **`void yh::rec::Ak_8963::reset_heading ()` method**  
Syntax: `mag.reset_heading();` (or `gy9250.mag.reset_heading();` for GY-9250 users)  
This `reset_heading()` function sets the instantaneous heading as a relative-zero.  
The `get_heading()` method will return the heading of the chip with respect to this relative-zero set.  
It is advised to call this method before every kick-off in a RCJ-soccer tournament.  
  
10. **`double yh::rec::Ak_8963::get_heading ()` method**  
Syntax: `double heading = mag.get_heading();` (or `double heading = gy9250.mag.get_heading();` for GY-9250 users)  
This `get_heading()` function gets the heading of the chip, taking the relative-zero direction as 0.  
The value returned ranges from 0 to 359.99999999 degrees, and increases clockwisely.  
  
11. **`double yh::rec::Ak_8963::rz_heading` member**  
Syntax: `mag.rz_heading = 351.12;` (or `gy9250.mag.rz_heading = 351.12;` for GY-9250 users)  
The `get_heading()` method takes the direction indicated by `rz_heading` as 0.  
You may write `0` to this member to get the true heading of the world.  
  
  
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
- AK8963 datasheet and register map  
https://cdn.datasheetspdf.com/pdf-down/A/K/8/AK8963-AsahiKaseiMicrosystems.pdf  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**