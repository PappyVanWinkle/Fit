################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./src/abreast_dm.obj" \
"./src/consume.obj" \
"./src/dm_hash.obj" \
"./src/fit_aes.obj" \
"./src/fit_debug.obj" \
"./src/fit_rsa.obj" \
"./src/fit_version.obj" \
"./src/get_info.obj" \
"./src/internal.obj" \
"./src/mem_read.obj" \
"./src/node_locking.obj" \
"./src/parser.obj" \
"./src/validate.obj" \
"./sample/demo_getinfo.obj" \
"./sample/fit_demo.obj" \
"./mbedtls-2.2.1/library/asn1parse.obj" \
"./mbedtls-2.2.1/library/base64.obj" \
"./mbedtls-2.2.1/library/bignum.obj" \
"./mbedtls-2.2.1/library/md.obj" \
"./mbedtls-2.2.1/library/md5.obj" \
"./mbedtls-2.2.1/library/md_wrap.obj" \
"./mbedtls-2.2.1/library/oid.obj" \
"./mbedtls-2.2.1/library/pem.obj" \
"./mbedtls-2.2.1/library/pk.obj" \
"./mbedtls-2.2.1/library/pk_wrap.obj" \
"./mbedtls-2.2.1/library/pkparse.obj" \
"./mbedtls-2.2.1/library/rsa.obj" \
"./mbedtls-2.2.1/library/sha256.obj" \
"./hwdep/tm4c1294xl/board_setup.obj" \
"./hwdep/tm4c1294xl/delay.obj" \
"./hwdep/tm4c1294xl/eeprom_mem.obj" \
"./hwdep/tm4c1294xl/fingerprint.obj" \
"./hwdep/tm4c1294xl/flash_mem.obj" \
"./hwdep/tm4c1294xl/get_time.obj" \
"./hwdep/tm4c1294xl/led.obj" \
"./hwdep/tm4c1294xl/memory.obj" \
"./hwdep/tm4c1294xl/startup_ccs.obj" \
"./hwdep/tm4c1294xl/uart.obj" \
"./hwdep/tm4c1294xl/uartstdio.obj" \
"../hwdep/tm4c1294xl/fit_sample_ccs.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"C:\ti\TivaWare_C_Series-2.1.2.111\driverlib\ccs\Debug\driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir_vars.mk
-include sample/subdir_vars.mk
-include mbedtls-2.2.1/library/subdir_vars.mk
-include hwdep/tm4c1294xl/subdir_vars.mk
-include src/subdir_rules.mk
-include sample/subdir_rules.mk
-include mbedtls-2.2.1/library/subdir_rules.mk
-include hwdep/tm4c1294xl/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
fitgood.out \

EXE_OUTPUTS__QUOTED += \
"fitgood.out" \

BIN_OUTPUTS += \
fitgood.hex \

BIN_OUTPUTS__QUOTED += \
"fitgood.hex" \


# All Target
all: fitgood.out

# Tool invocations
fitgood.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off -z -m"fitgood.map" --stack_size=16384 --heap_size=16384 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --reread_libs --warn_sections --diag_wrap=off --display_error_number --xml_link_info="fitgood_linkInfo.xml" --rom_model -o "fitgood.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

fitgood.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armhex"  -o "fitgood.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "src\abreast_dm.pp" "src\consume.pp" "src\dm_hash.pp" "src\fit_aes.pp" "src\fit_debug.pp" "src\fit_rsa.pp" "src\fit_version.pp" "src\get_info.pp" "src\internal.pp" "src\mem_read.pp" "src\node_locking.pp" "src\parser.pp" "src\validate.pp" "sample\demo_getinfo.pp" "sample\fit_demo.pp" "mbedtls-2.2.1\library\asn1parse.pp" "mbedtls-2.2.1\library\base64.pp" "mbedtls-2.2.1\library\bignum.pp" "mbedtls-2.2.1\library\md.pp" "mbedtls-2.2.1\library\md5.pp" "mbedtls-2.2.1\library\md_wrap.pp" "mbedtls-2.2.1\library\oid.pp" "mbedtls-2.2.1\library\pem.pp" "mbedtls-2.2.1\library\pk.pp" "mbedtls-2.2.1\library\pk_wrap.pp" "mbedtls-2.2.1\library\pkparse.pp" "mbedtls-2.2.1\library\rsa.pp" "mbedtls-2.2.1\library\sha256.pp" "hwdep\tm4c1294xl\board_setup.pp" "hwdep\tm4c1294xl\delay.pp" "hwdep\tm4c1294xl\eeprom_mem.pp" "hwdep\tm4c1294xl\fingerprint.pp" "hwdep\tm4c1294xl\flash_mem.pp" "hwdep\tm4c1294xl\get_time.pp" "hwdep\tm4c1294xl\led.pp" "hwdep\tm4c1294xl\memory.pp" "hwdep\tm4c1294xl\startup_ccs.pp" "hwdep\tm4c1294xl\uart.pp" 
	-$(RM) "hwdep\tm4c1294xl\uartstdio.pp" 
	-$(RM) "src\abreast_dm.obj" "src\consume.obj" "src\dm_hash.obj" "src\fit_aes.obj" "src\fit_debug.obj" "src\fit_rsa.obj" "src\fit_version.obj" "src\get_info.obj" "src\internal.obj" "src\mem_read.obj" "src\node_locking.obj" "src\parser.obj" "src\validate.obj" "sample\demo_getinfo.obj" "sample\fit_demo.obj" "mbedtls-2.2.1\library\asn1parse.obj" "mbedtls-2.2.1\library\base64.obj" "mbedtls-2.2.1\library\bignum.obj" "mbedtls-2.2.1\library\md.obj" "mbedtls-2.2.1\library\md5.obj" "mbedtls-2.2.1\library\md_wrap.obj" "mbedtls-2.2.1\library\oid.obj" "mbedtls-2.2.1\library\pem.obj" "mbedtls-2.2.1\library\pk.obj" "mbedtls-2.2.1\library\pk_wrap.obj" "mbedtls-2.2.1\library\pkparse.obj" "mbedtls-2.2.1\library\rsa.obj" "mbedtls-2.2.1\library\sha256.obj" "hwdep\tm4c1294xl\board_setup.obj" "hwdep\tm4c1294xl\delay.obj" "hwdep\tm4c1294xl\eeprom_mem.obj" "hwdep\tm4c1294xl\fingerprint.obj" "hwdep\tm4c1294xl\flash_mem.obj" "hwdep\tm4c1294xl\get_time.obj" "hwdep\tm4c1294xl\led.obj" "hwdep\tm4c1294xl\memory.obj" "hwdep\tm4c1294xl\startup_ccs.obj" 
	-$(RM) "hwdep\tm4c1294xl\uart.obj" "hwdep\tm4c1294xl\uartstdio.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

