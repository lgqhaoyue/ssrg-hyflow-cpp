################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/util/parser/ConfigFile.cpp 

OBJS += \
./src/util/parser/ConfigFile.o 

CPP_DEPS += \
./src/util/parser/ConfigFile.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/parser/%.o: ../src/util/parser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


