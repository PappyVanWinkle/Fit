################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/abreast_dm.obj: ../src/abreast_dm.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/abreast_dm.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/consume.obj: ../src/consume.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/consume.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/dm_hash.obj: ../src/dm_hash.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/dm_hash.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/fit_aes.obj: ../src/fit_aes.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/fit_aes.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/fit_debug.obj: ../src/fit_debug.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/fit_debug.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/fit_rsa.obj: ../src/fit_rsa.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/fit_rsa.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/fit_version.obj: ../src/fit_version.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/fit_version.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/get_info.obj: ../src/get_info.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/get_info.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/internal.obj: ../src/internal.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/internal.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/mem_read.obj: ../src/mem_read.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/mem_read.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/node_locking.obj: ../src/node_locking.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/node_locking.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/parser.obj: ../src/parser.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/parser.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/validate.obj: ../src/validate.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/Users/Gemalto/git/fit/fitgood/inc" --include_path="C:/Users/Gemalto/git/fit/fitgood/mbedtls-2.2.1/include" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.7/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=DEBUG --define=UART_BUFFERED --define=FIT_DEBUG_MSG --define=FIT_USE_NODE_LOCKING --define=FIT_USE_CLOCK --define=FIT_USE_E2 --define=TARGET_IS_TM4C129_RA0 --define=PART_TM4C1292NCPDT --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="src/validate.pp" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


