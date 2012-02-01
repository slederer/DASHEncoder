

RM := rm -rf

USER_OBJS :=

LIBS :=

SUBDIRS := \
multiplexer \
mpdgenerator \
encoder \

-include subdir.mk
-include multiplexer/subdir.mk
-include mpdgenerator/subdir.mk
-include encoder/subdir.mk


# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: DASHEncoder

# Tool invocations
DASHEncoder: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o"DASHEncoder" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) DASHEncoder
	-@echo ' '
