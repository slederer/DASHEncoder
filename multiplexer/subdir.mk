################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./multiplexer/AbstractMultiplexer.cpp \
./multiplexer/MP4BoxMultiplexer.cpp 

OBJS += \
./multiplexer/AbstractMultiplexer.o \
./multiplexer/MP4BoxMultiplexer.o 

CPP_DEPS += \
./multiplexer/AbstractMultiplexer.d \
./multiplexer/MP4BoxMultiplexer.d 


# Each subdirectory must supply rules for building sources it contributes
multiplexer/%.o: ./multiplexer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -W -Wall -Wextra -pedantic -std=c++11 -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


