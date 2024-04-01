#ifndef MBED_H
#include "mbed.h" // Just for code highlight when opened alone
#endif
#pragma once 
// Src2
typedef struct {                                /*!< (@ 0x40000000) APPROTECT Structure                                        */
  __IM  uint32_t  RESERVED[340];
  __IOM uint32_t  FORCEPROTECT;                 /*!< (@ 0x00000550) Software force enable APPROTECT mechanism until
                                                                    next reset. This register can only be written
                                                                    once.                                                      */
  __IM  uint32_t  RESERVED1;
  __IOM uint32_t  DISABLE;                      /*!< (@ 0x00000558) Software disable APPROTECT mechanism                       */
} NRF_APPROTECT_Type;                           /*!< Size = 1372 (0x55c)                                                       */

#define NRF_APPROTECT_BASE          0x40000000UL
#define NRF_APPROTECT               ((NRF_APPROTECT_Type*)     NRF_APPROTECT_BASE)
// Src3
#define UICR_APPROTECT_PALL_HwDisabled (0x5AUL) /*!< Hardware disable of access port protection for devices where access port protection is controlled by hardware and software */

// Src1
inline void nrf52_disable_approtect() {
    if ((NRF_UICR->APPROTECT & UICR_APPROTECT_PALL_Msk) !=
        (UICR_APPROTECT_PALL_HwDisabled << UICR_APPROTECT_PALL_Pos)) {

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}

        NRF_UICR->APPROTECT = ((NRF_UICR->APPROTECT & ~((uint32_t)UICR_APPROTECT_PALL_Msk)) |
            (UICR_APPROTECT_PALL_HwDisabled << UICR_APPROTECT_PALL_Pos));

        NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
        while (NRF_NVMC->READY == NVMC_READY_READY_Busy){}
    }

    // Src4
    /* Load APPROTECT soft branch from UICR.
       If UICR->APPROTECT is disabled, POWER->APPROTECT will be disabled. */
    NRF_APPROTECT->DISABLE = NRF_UICR->APPROTECT;
}
