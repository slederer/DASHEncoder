################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/mpdgenerator/MPDGenerator.cpp 

OBJS += \
./src/mpdgenerator/MPDGenerator.o 

CPP_DEPS += \
./src/mpdgenerator/MPDGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
src/mpdgenerator/%.o: ../src/mpdgenerator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


