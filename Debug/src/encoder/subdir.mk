################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/encoder/AbstractAudioEncoder.cpp \
../src/encoder/AbstractEncoder.cpp \
../src/encoder/AbstractVideoEncoder.cpp \
../src/encoder/EncoderFactory.cpp \
../src/encoder/ffmpegAACEncoder.cpp \
../src/encoder/vp8Encoder.cpp \
../src/encoder/x264Encoder.cpp 

OBJS += \
./src/encoder/AbstractAudioEncoder.o \
./src/encoder/AbstractEncoder.o \
./src/encoder/AbstractVideoEncoder.o \
./src/encoder/EncoderFactory.o \
./src/encoder/ffmpegAACEncoder.o \
./src/encoder/vp8Encoder.o \
./src/encoder/x264Encoder.o 

CPP_DEPS += \
./src/encoder/AbstractAudioEncoder.d \
./src/encoder/AbstractEncoder.d \
./src/encoder/AbstractVideoEncoder.d \
./src/encoder/EncoderFactory.d \
./src/encoder/ffmpegAACEncoder.d \
./src/encoder/vp8Encoder.d \
./src/encoder/x264Encoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/encoder/%.o: ../src/encoder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


