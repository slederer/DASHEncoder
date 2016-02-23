################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./AnyOption.cpp \
./DASHEncoder.cpp 

OBJS += \
./AnyOption.o \
./DASHOption.o \
./DASHEncoder.o 

CPP_DEPS += \
./AnyOption.d \
./DASHOption.d \
./DASHEncoder.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -W -Wall -Wextra -pedantic -std=c++11 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


