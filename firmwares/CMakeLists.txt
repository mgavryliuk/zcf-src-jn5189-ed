set(APP_TARGET ${TARGET})
set(SDK_TARGET SDK)
set(PRE_BUILD_TARGET ZPS_PDUM_lib)
set(VENV_DIR $ENV{VENV_DIR})

set(JENNIC_CHIP JN5189)
set(JENNIC_CHIP_FAMILY JN518x)
set(JENNIC_STACK "MAC")
set(JENNIC_MAC "MiniMac")

set(SDK_PREFIX ${PROJECT_SOURCE_DIR}/sdk)
set(SDK_LINKER_SCRIPT_DIR ${SDK_PREFIX}/middleware/wireless/zigbee/platform/K32W0/libs)
set(ZPS_CONFIG "${SDK_PREFIX}/middleware/wireless/zigbee/tools/ZPSConfig/Source/ZPSConfig.py")
set(PDUM_CONFIG "${SDK_PREFIX}/middleware/wireless/zigbee/tools/PDUMConfig/Source/PDUMConfig.py")
set(ZPSCFG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${APP_TARGET}/app.zpscfg")
set(DK6_IMAGE_TOOL "${SDK_PREFIX}/tools/imagetool/dk6_image_tool.py")

# ============= START - PDUMConfig / ZPSConfig library =============
add_custom_command(
    OUTPUT
        pdum_gen.c
        pdum_gen.h
    COMMAND uv run --directory ${VENV_DIR}
            ${PDUM_CONFIG}
            -z ${APP_TARGET}
            -e LITTLE_ENDIAN_PROCESSOR
            -f ${ZPSCFG_FILE}
            -o ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(
    OUTPUT
        zps_gen.c
        zps_gen.h
    COMMAND uv run --directory ${VENV_DIR}
             ${ZPS_CONFIG}
            -n ${APP_TARGET}
            -t ${JENNIC_CHIP}
            -l ${SDK_LINKER_SCRIPT_DIR}/libZPSNWK_ZED.a
            -a ${SDK_LINKER_SCRIPT_DIR}/libZPSAPL.a
            -e LITTLE_ENDIAN_PROCESSOR
            -f ${ZPSCFG_FILE}
            -o ${CMAKE_CURRENT_BINARY_DIR}
            -c ${TOOLCHAIN_ABS_PATH}
)

add_library(${PRE_BUILD_TARGET} STATIC
    zps_gen.c
    pdum_gen.c
)
target_include_directories(${PRE_BUILD_TARGET} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

# ============= END - PDUMConfig / ZPSConfig library =============
add_compile_definitions(
    ${JENNIC_CHIP}=5189
    ${JENNIC_CHIP_FAMILY}=5180
    JENNIC_CHIP=${JENNIC_CHIP}
    JENNIC_CHIP_${JENNIC_CHIP}
    JENNIC_CHIP_NAME=_${JENNIC_CHIP}
    JENNIC_CHIP_FAMILY=${JENNIC_CHIP_FAMILY}
    JENNIC_CHIP_FAMILY_${JENNIC_CHIP_FAMILY}
    JENNIC_CHIP_FAMILY_NAME=_${JENNIC_CHIP_FAMILY}

    HEAP_SIZE=4000
    CPU_JN5189HN
    EndDevice
    ZBPRO_DEVICE_TYPE_ZED
    cPWR_FullPowerDownMode
    FSL_OSA_BM_TIMER_CONFIG=FSL_OSA_BM_TIMER_SYSTICK
    ENABLE_RAM_VECTOR_TABLE
    gEepromType_d=gEepromDevice_InternalFlash_c
    gEepromParams_bufferedWrite_c=1
    SINGLE_CHANNEL=12
    ZIGBEE_USE_FRAMEWORK
    gSupportBle=0
    PDM_NO_RTOS
    CPU_JN518X
    SUPPORT_FOR_15_4=1
    SUPPORT_FOR_BLE=0

    DBG_ENABLE
    SDK_DEBUGCONSOLE=1
    PRINTF_ADVANCED_ENABLE=1
)

set(SDK_INCLUDE_FOLDERS
    ${SDK_PREFIX}/middleware/wireless/framework/Common
    ${SDK_PREFIX}/middleware/wireless/framework/PDM/Include
    ${SDK_PREFIX}/middleware/wireless/framework/PDUM/Include
    ${SDK_PREFIX}/middleware/wireless/framework/PWRM/Include
    ${SDK_PREFIX}/middleware/wireless/framework/PDM/Include
    ${SDK_PREFIX}/middleware/wireless/framework/Lists
    ${SDK_PREFIX}/middleware/wireless/zigbee/ZigbeeCommon/Include
    ${SDK_PREFIX}/middleware/wireless/zigbee/platform/
    ${SDK_PREFIX}/middleware/wireless/zigbee/ZPSAPL/Include
    ${SDK_PREFIX}/middleware/wireless/zigbee/ZPSNWK/Include
    ${SDK_PREFIX}/middleware/wireless/zigbee/ZPSTSV/Include
    ${SDK_PREFIX}/middleware/wireless/ieee-802.15.4/Include
    ${SDK_PREFIX}/middleware/wireless/zigbee/ZPSMAC/Include
    ${SDK_PREFIX}/middleware/wireless/ieee-802.15.4/mMac/Include
    ${SDK_PREFIX}/middleware/wireless/ieee-802.15.4/uMac/Include
    ${SDK_PREFIX}/devices/JN5189/utilities/str
    ${SDK_PREFIX}/devices/JN5189/utilities/debug_console
    ${SDK_PREFIX}/devices/JN5189/drivers
    ${SDK_PREFIX}/devices/JN5189
    ${SDK_PREFIX}/CMSIS/Include
    ${SDK_PREFIX}/components/serial_manager
    ${SDK_PREFIX}/components/uart
    ${SDK_PREFIX}/middleware/wireless/framework/LowPower/Interface/jn5189dk6
    ${SDK_PREFIX}/middleware/wireless/framework/OSAbstraction/Interface
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDK_INCLUDE_FOLDERS}
    ${PROJECT_SOURCE_DIR}/firmwares
    ${PROJECT_SOURCE_DIR}/firmwares/common/include
    ${PROJECT_SOURCE_DIR}/firmwares/${APP_TARGET}
    ${PROJECT_SOURCE_DIR}/firmwares/${APP_TARGET}/board
)

link_directories(
    ${SDK_PREFIX}/devices/${JENNIC_CHIP}/gcc/
    ${SDK_PREFIX}/middleware/wireless/ieee-802.15.4/lib
    ${SDK_PREFIX}/middleware/wireless/framework/PDM/Library
    ${SDK_PREFIX}/middleware/wireless/framework/PDUM/Library
    ${SDK_PREFIX}/middleware/wireless/framework/PWRM/Library
    ${SDK_PREFIX}/middleware/wireless/framework/XCVR/lib
    ${SDK_LINKER_SCRIPT_DIR}
)

# ============= START - SDK library =============
set(SDK_SRC
    ${SDK_PREFIX}/middleware/wireless/framework/OSAbstraction/Source/fsl_os_abstraction_bm.c
    ${SDK_PREFIX}/devices/JN5189/gcc/startup_JN5189.c
    ${SDK_PREFIX}/devices/JN5189/system_JN5189.c

    ${SDK_PREFIX}/devices/JN5189/utilities/debug_console/fsl_debug_console.c
    ${SDK_PREFIX}/devices/JN5189/utilities/str/fsl_str.c
    ${SDK_PREFIX}/devices/JN5189/utilities/fsl_assert.c
    ${SDK_PREFIX}/components/serial_manager/serial_manager.c
    ${SDK_PREFIX}/components/serial_manager/serial_port_uart.c
    ${SDK_PREFIX}/components/uart/usart_adapter.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_adc.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_clock.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_gpio.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_reset.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_power.c

    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_aes.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_common.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_usart.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_ctimer.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_dma.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_flash.c
    ${SDK_PREFIX}/devices/JN5189/drivers/fsl_flexcomm.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_fmeas.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_gint.c
    # ${SDK_PREFIX}/devices/JN5189/drivers/fsl_gpio.c
)
add_library(${SDK_TARGET} STATIC ${SDK_SRC})
target_link_libraries(${SDK_TARGET} PUBLIC ${PRE_BUILD_TARGET})
# ============= END - SDK library =============

set(LDLIBS
    PWRM
    PDUM
    Radio
    PDM
    ZPSAPL
    ZPSGP_ZED
    ZPSIPAN_ZED
    ZPSMAC_Mini_SOC
    ZPSNWK_ZED
    ZPSTSV
    MiniMac
)

set(APP_SRC
    common/src/main.c
    ${APP_TARGET}/board/board.c
    ${APP_TARGET}/board/board_utility.c
    ${APP_TARGET}/board/clock_config.c
    ${APP_TARGET}/board/pin_mux.c
)
add_executable(${APP_TARGET} ${APP_SRC})

target_compile_definitions(${APP_TARGET} PRIVATE
    # DEBUG_ENTRYPOINT
    # DEBUG_BASIC_EP
    # DEBUG_BATTERY
    # DEBUG_BUTTON
    # DEBUG_MAIN
    # DEBUG_NODE
    # DEBUG_EVENTS
    # DEBUG_NODE_AF_CB
    # DEBUG_NODE_BDB
    # DEBUG_NODE_ZDO
    # DEBUG_ON_OFF_EP
    # DEBUG_POLL
    # DEBUG_REPORT
)

target_link_libraries(${APP_TARGET}
    -Wl,--start-group
    ${SDK_TARGET}
    ${LDLIBS}
    -Wl,--end-group
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -TAppBuildMinRetention_JN5189.ld")

MESSAGE(STATUS "CMAKE_SIZE: ${CMAKE_SIZE}")

set_target_properties(${APP_TARGET} PROPERTIES SUFFIX ".axf")
add_custom_command(TARGET ${APP_TARGET} POST_BUILD COMMAND ${CMAKE_SIZE} ${APP_TARGET}.axf)
add_custom_target(${APP_TARGET}.bin ALL
    DEPENDS ${APP_TARGET}
    COMMAND uv run --directory ${VENV_DIR} ${DK6_IMAGE_TOOL} -s 294912 ${CMAKE_CURRENT_BINARY_DIR}/${APP_TARGET}.axf
    COMMAND ${CMAKE_COMMAND} -E echo "Generating binary ..."
    COMMAND ${CMAKE_OBJCOPY} -v -O binary ${APP_TARGET}.axf ${APP_TARGET}.bin
)
