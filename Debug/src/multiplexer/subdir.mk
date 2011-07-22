################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/multiplexer/AbstractMultiplexer.cpp \
../src/multiplexer/MP4BoxMultiplexer.cpp 

OBJS += \
./src/multiplexer/AbstractMultiplexer.o \
./src/multiplexer/MP4BoxMultiplexer.o 

CPP_DEPS += \
./src/multiplexer/AbstractMultiplexer.d \
./src/multiplexer/MP4BoxMultiplexer.d 


# Each subdirectory must supply rules for building sources it contributes
src/multiplexer/%.o: ../src/multiplexer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


