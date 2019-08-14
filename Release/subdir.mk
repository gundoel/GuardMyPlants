################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
..\Sensor.cpp \
..\SoilMoistureSensor.cpp \
..\WaterLevelSensor.cpp \
..\Waterpump.cpp \
..\sloeber.ino.cpp 

LINK_OBJ += \
.\Sensor.cpp.o \
.\SoilMoistureSensor.cpp.o \
.\WaterLevelSensor.cpp.o \
.\Waterpump.cpp.o \
.\sloeber.ino.cpp.o 

CPP_DEPS += \
.\Sensor.cpp.d \
.\SoilMoistureSensor.cpp.d \
.\WaterLevelSensor.cpp.d \
.\Waterpump.cpp.d \
.\sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Sensor.cpp.o: ..\Sensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

SoilMoistureSensor.cpp.o: ..\SoilMoistureSensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

WaterLevelSensor.cpp.o: ..\WaterLevelSensor.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Waterpump.cpp.o: ..\Waterpump.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ..\sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"

	@echo 'Finished building: $<'
	@echo ' '


