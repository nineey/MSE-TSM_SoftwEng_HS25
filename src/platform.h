/*
 * Copyright (c) 2022-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PL_CONFIG_USE_PICO_W            (1) /* if using Pico W board */
  /* Note:
   * if using Pico-W:
   * a) enable 'library pico_cyw43_arch_lwip_sys_freertos' in CMakeLists.txt of src folder
   * b) Enable set(PICO_BOARD pico_w) in CMakeLists.txt in project root
   */
#define PL_CONFIG_USE_UNIT_TESTS        (1 && defined(ENABLE_UNIT_TESTS) && ENABLE_UNIT_TESTS==1) /* if using unit tests. ENABLE_UNIT_TESTS is set by the CMake file */
#if PL_CONFIG_USE_UNIT_TESTS && !defined(UNITY_OUTPUT_CHAR)
   #error "needs to be defined in IncludeMcuLibConfig.h!"
#endif

/* platform configuration macros: set to 1 to enable functionality */
#define PL_CONFIG_USE_SHELL             (0)
#define PL_CONFIG_USE_LEDS              (1) /* if using LEDs */
#define PL_CONFIG_USE_TUD_CDC           (1) /* tinyUSB CDC device with McuShellCdcDevice */
#define PL_CONFIG_USE_SHELL_CDC         (1 && PL_CONFIG_USE_TUD_CDC) /* if using CDC as shell interface */
#define PL_CONFIG_USE_SEMIHOSTING       (0) /* if using semihosting for standard I/O. Note that SEGGER has issues with it? */
#define PL_CONFIG_USE_RTT               (1) /* if using SEGGER RTT */
#define PL_CONFIG_USE_SAMPLE            (1) /* sample example used for the scientific debugging part */
#define PL_CONFIG_USE_GCOV              (0 && McuRdimon_CONFIG_IS_ENABLED) /* if using gcov */

/*!
 * \brief Module initialization
 */
void PL_Init(void);

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif /* PLATFORM_H_ */
