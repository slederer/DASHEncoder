################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./mpdgenerator/MPDGenerator.cpp 

OBJS += \
./mpdgenerator/MPDGenerator.o 

CPP_DEPS += \
./mpdgenerator/MPDGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
mpdgenerator/%.o: ./mpdgenerator/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


