################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
./encoder/AbstractAudioEncoder.cpp \
./encoder/AbstractEncoder.cpp \
./encoder/AbstractVideoEncoder.cpp \
./encoder/EncoderFactory.cpp \
./encoder/ffmpegAACEncoder.cpp \
./encoder/vp8Encoder.cpp \
./encoder/x264Encoder.cpp 

OBJS += \
./encoder/AbstractAudioEncoder.o \
./encoder/AbstractEncoder.o \
./encoder/AbstractVideoEncoder.o \
./encoder/EncoderFactory.o \
./encoder/ffmpegAACEncoder.o \
./encoder/vp8Encoder.o \
./encoder/x264Encoder.o 

CPP_DEPS += \
./encoder/AbstractAudioEncoder.d \
./encoder/AbstractEncoder.d \
./encoder/AbstractVideoEncoder.d \
./encoder/EncoderFactory.d \
./encoder/ffmpegAACEncoder.d \
./encoder/vp8Encoder.d \
./encoder/x264Encoder.d 


# Each subdirectory must supply rules for building sources it contributes
encoder/%.o: ./encoder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


