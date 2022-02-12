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
    (The resolution of gyroscope is default to 1000 degrees per second)  
3. You are advised to check the who_am_i value of the chip before usage, since there are many reports that an MPU6500 chip was installed on an MPU9250 board. The appearance of the module may fake you. The check is to let you understand what is the model of the chip that is really inside the module you are holding.  
  
### How to use:  
1. Constructor of a GY-521 object ` Custom_gy521::Custom_gy521 ( I2C_address ) `  
Syntax: ` Custom_gy521 gy521 (0x68); `  
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

4. `void Custom_gy521::cal_gyro ( sampling_amount )` method  
Syntax: `gy521.cal_gyro();` or `gy521.cal_gyro(100);`  
This `cal_gyro()` function calibrates all 3-axes of gyroscope of the chip.  
This function repeats reading the gyroscope values of 3 axes and gets a mean value to be subtracted from every time the programmer gets new gyroscope data.  
The argument `sampling_amount` tells the function how many times it should get data samples. The larger the value, the calibration is more accurate, but also more time-consuming. By default, the funciton will get 8192 data samples, so this argument is optional.  
  
If you find any problems, please raise an issue or contact me at yhei.choi@gmail.com.  
  
**This repository is written and maintained by YH Choi,**  
**tested with hardware modules provided by CSWCSS REC**  
**and technically supported by CSWCSS REC hardware team.**