################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src\HID.cpp 

LINK_OBJ += \
.\libraries\HID\src\HID.cpp.o 

CPP_DEPS += \
.\libraries\HID\src\HID.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\HID\src\HID.cpp.o: C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src\HID.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\tools\avr-gcc\5.4.0-atmel3.6.1-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR     -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\cores\arduino" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\variants\standard" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\ArduinoThread\2.1.1" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.23\libraries\HID\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\Keypad\3.1.1\src" -I"C:\Users\TAARISI3\sloeber\arduinoPlugin\libraries\AnalogMatrixKeypad\src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


