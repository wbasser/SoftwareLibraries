//*****************************************************************************
//
// lm3s1811.h - LM3S1811 Register Definitions
//
// Copyright (c) 2009-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// This is part of revision 9453 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#ifndef __LM3S1811_H__
#define __LM3S1811_H__

//*****************************************************************************
//
// Watchdog Timer registers (WATCHDOG0)
//
//*****************************************************************************
#define WATCHDOG0_LOAD_R        (*((volatile unsigned long *)0x40000000))
#define WATCHDOG0_VALUE_R       (*((volatile unsigned long *)0x40000004))
#define WATCHDOG0_CTL_R         (*((volatile unsigned long *)0x40000008))
#define WATCHDOG0_ICR_R         (*((volatile unsigned long *)0x4000000C))
#define WATCHDOG0_RIS_R         (*((volatile unsigned long *)0x40000010))
#define WATCHDOG0_MIS_R         (*((volatile unsigned long *)0x40000014))
#define WATCHDOG0_TEST_R        (*((volatile unsigned long *)0x40000418))
#define WATCHDOG0_LOCK_R        (*((volatile unsigned long *)0x40000C00))

//*****************************************************************************
//
// Watchdog Timer registers (WATCHDOG1)
//
//*****************************************************************************
#define WATCHDOG1_LOAD_R        (*((volatile unsigned long *)0x40001000))
#define WATCHDOG1_VALUE_R       (*((volatile unsigned long *)0x40001004))
#define WATCHDOG1_CTL_R         (*((volatile unsigned long *)0x40001008))
#define WATCHDOG1_ICR_R         (*((volatile unsigned long *)0x4000100C))
#define WATCHDOG1_RIS_R         (*((volatile unsigned long *)0x40001010))
#define WATCHDOG1_MIS_R         (*((volatile unsigned long *)0x40001014))
#define WATCHDOG1_TEST_R        (*((volatile unsigned long *)0x40001418))
#define WATCHDOG1_LOCK_R        (*((volatile unsigned long *)0x40001C00))

//*****************************************************************************
//
// GPIO registers (PORTA)
//
//*****************************************************************************
#define GPIO_PORTA_DATA_BITS_R  ((volatile unsigned long *)0x40004000)
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_IS_R         (*((volatile unsigned long *)0x40004404))
#define GPIO_PORTA_IBE_R        (*((volatile unsigned long *)0x40004408))
#define GPIO_PORTA_IEV_R        (*((volatile unsigned long *)0x4000440C))
#define GPIO_PORTA_IM_R         (*((volatile unsigned long *)0x40004410))
#define GPIO_PORTA_RIS_R        (*((volatile unsigned long *)0x40004414))
#define GPIO_PORTA_MIS_R        (*((volatile unsigned long *)0x40004418))
#define GPIO_PORTA_ICR_R        (*((volatile unsigned long *)0x4000441C))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DR2R_R       (*((volatile unsigned long *)0x40004500))
#define GPIO_PORTA_DR4R_R       (*((volatile unsigned long *)0x40004504))
#define GPIO_PORTA_DR8R_R       (*((volatile unsigned long *)0x40004508))
#define GPIO_PORTA_ODR_R        (*((volatile unsigned long *)0x4000450C))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *)0x40004510))
#define GPIO_PORTA_PDR_R        (*((volatile unsigned long *)0x40004514))
#define GPIO_PORTA_SLR_R        (*((volatile unsigned long *)0x40004518))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *)0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *)0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))

//*****************************************************************************
//
// GPIO registers (PORTB)
//
//*****************************************************************************
#define GPIO_PORTB_DATA_BITS_R  ((volatile unsigned long *)0x40005000)
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_IS_R         (*((volatile unsigned long *)0x40005404))
#define GPIO_PORTB_IBE_R        (*((volatile unsigned long *)0x40005408))
#define GPIO_PORTB_IEV_R        (*((volatile unsigned long *)0x4000540C))
#define GPIO_PORTB_IM_R         (*((volatile unsigned long *)0x40005410))
#define GPIO_PORTB_RIS_R        (*((volatile unsigned long *)0x40005414))
#define GPIO_PORTB_MIS_R        (*((volatile unsigned long *)0x40005418))
#define GPIO_PORTB_ICR_R        (*((volatile unsigned long *)0x4000541C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DR2R_R       (*((volatile unsigned long *)0x40005500))
#define GPIO_PORTB_DR4R_R       (*((volatile unsigned long *)0x40005504))
#define GPIO_PORTB_DR8R_R       (*((volatile unsigned long *)0x40005508))
#define GPIO_PORTB_ODR_R        (*((volatile unsigned long *)0x4000550C))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))
#define GPIO_PORTB_SLR_R        (*((volatile unsigned long *)0x40005518))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *)0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *)0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

//*****************************************************************************
//
// GPIO registers (PORTC)
//
//*****************************************************************************
#define GPIO_PORTC_DATA_BITS_R  ((volatile unsigned long *)0x40006000)
#define GPIO_PORTC_DATA_R       (*((volatile unsigned long *)0x400063FC))
#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_IS_R         (*((volatile unsigned long *)0x40006404))
#define GPIO_PORTC_IBE_R        (*((volatile unsigned long *)0x40006408))
#define GPIO_PORTC_IEV_R        (*((volatile unsigned long *)0x4000640C))
#define GPIO_PORTC_IM_R         (*((volatile unsigned long *)0x40006410))
#define GPIO_PORTC_RIS_R        (*((volatile unsigned long *)0x40006414))
#define GPIO_PORTC_MIS_R        (*((volatile unsigned long *)0x40006418))
#define GPIO_PORTC_ICR_R        (*((volatile unsigned long *)0x4000641C))
#define GPIO_PORTC_AFSEL_R      (*((volatile unsigned long *)0x40006420))
#define GPIO_PORTC_DR2R_R       (*((volatile unsigned long *)0x40006500))
#define GPIO_PORTC_DR4R_R       (*((volatile unsigned long *)0x40006504))
#define GPIO_PORTC_DR8R_R       (*((volatile unsigned long *)0x40006508))
#define GPIO_PORTC_ODR_R        (*((volatile unsigned long *)0x4000650C))
#define GPIO_PORTC_PUR_R        (*((volatile unsigned long *)0x40006510))
#define GPIO_PORTC_PDR_R        (*((volatile unsigned long *)0x40006514))
#define GPIO_PORTC_SLR_R        (*((volatile unsigned long *)0x40006518))
#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))
#define GPIO_PORTC_LOCK_R       (*((volatile unsigned long *)0x40006520))
#define GPIO_PORTC_CR_R         (*((volatile unsigned long *)0x40006524))
#define GPIO_PORTC_AMSEL_R      (*((volatile unsigned long *)0x40006528))
#define GPIO_PORTC_PCTL_R       (*((volatile unsigned long *)0x4000652C))

//*****************************************************************************
//
// GPIO registers (PORTD)
//
//*****************************************************************************
#define GPIO_PORTD_DATA_BITS_R  ((volatile unsigned long *)0x40007000)
#define GPIO_PORTD_DATA_R       (*((volatile unsigned long *)0x400073FC))
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *)0x40007400))
#define GPIO_PORTD_IS_R         (*((volatile unsigned long *)0x40007404))
#define GPIO_PORTD_IBE_R        (*((volatile unsigned long *)0x40007408))
#define GPIO_PORTD_IEV_R        (*((volatile unsigned long *)0x4000740C))
#define GPIO_PORTD_IM_R         (*((volatile unsigned long *)0x40007410))
#define GPIO_PORTD_RIS_R        (*((volatile unsigned long *)0x40007414))
#define GPIO_PORTD_MIS_R        (*((volatile unsigned long *)0x40007418))
#define GPIO_PORTD_ICR_R        (*((volatile unsigned long *)0x4000741C))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *)0x40007420))
#define GPIO_PORTD_DR2R_R       (*((volatile unsigned long *)0x40007500))
#define GPIO_PORTD_DR4R_R       (*((volatile unsigned long *)0x40007504))
#define GPIO_PORTD_DR8R_R       (*((volatile unsigned long *)0x40007508))
#define GPIO_PORTD_ODR_R        (*((volatile unsigned long *)0x4000750C))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *)0x40007510))
#define GPIO_PORTD_PDR_R        (*((volatile unsigned long *)0x40007514))
#define GPIO_PORTD_SLR_R        (*((volatile unsigned long *)0x40007518))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *)0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *)0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *)0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *)0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *)0x4000752C))

//*****************************************************************************
//
// SSI registers (SSI0)
//
//*****************************************************************************
#define SSI0_CR0_R              (*((volatile unsigned long *)0x40008000))
#define SSI0_CR1_R              (*((volatile unsigned long *)0x40008004))
#define SSI0_DR_R               (*((volatile unsigned long *)0x40008008))
#define SSI0_SR_R               (*((volatile unsigned long *)0x4000800C))
#define SSI0_CPSR_R             (*((volatile unsigned long *)0x40008010))
#define SSI0_IM_R               (*((volatile unsigned long *)0x40008014))
#define SSI0_RIS_R              (*((volatile unsigned long *)0x40008018))
#define SSI0_MIS_R              (*((volatile unsigned long *)0x4000801C))
#define SSI0_ICR_R              (*((volatile unsigned long *)0x40008020))
#define SSI0_DMACTL_R           (*((volatile unsigned long *)0x40008024))

//*****************************************************************************
//
// SSI registers (SSI1)
//
//*****************************************************************************
#define SSI1_CR0_R              (*((volatile unsigned long *)0x40009000))
#define SSI1_CR1_R              (*((volatile unsigned long *)0x40009004))
#define SSI1_DR_R               (*((volatile unsigned long *)0x40009008))
#define SSI1_SR_R               (*((volatile unsigned long *)0x4000900C))
#define SSI1_CPSR_R             (*((volatile unsigned long *)0x40009010))
#define SSI1_IM_R               (*((volatile unsigned long *)0x40009014))
#define SSI1_RIS_R              (*((volatile unsigned long *)0x40009018))
#define SSI1_MIS_R              (*((volatile unsigned long *)0x4000901C))
#define SSI1_ICR_R              (*((volatile unsigned long *)0x40009020))
#define SSI1_DMACTL_R           (*((volatile unsigned long *)0x40009024))

//*****************************************************************************
//
// UART registers (UART0)
//
//*****************************************************************************
#define UART0_DR_R              (*((volatile unsigned long *)0x4000C000))
#define UART0_RSR_R             (*((volatile unsigned long *)0x4000C004))
#define UART0_ECR_R             (*((volatile unsigned long *)0x4000C004))
#define UART0_FR_R              (*((volatile unsigned long *)0x4000C018))
#define UART0_ILPR_R            (*((volatile unsigned long *)0x4000C020))
#define UART0_IBRD_R            (*((volatile unsigned long *)0x4000C024))
#define UART0_FBRD_R            (*((volatile unsigned long *)0x4000C028))
#define UART0_LCRH_R            (*((volatile unsigned long *)0x4000C02C))
#define UART0_CTL_R             (*((volatile unsigned long *)0x4000C030))
#define UART0_IFLS_R            (*((volatile unsigned long *)0x4000C034))
#define UART0_IM_R              (*((volatile unsigned long *)0x4000C038))
#define UART0_RIS_R             (*((volatile unsigned long *)0x4000C03C))
#define UART0_MIS_R             (*((volatile unsigned long *)0x4000C040))
#define UART0_ICR_R             (*((volatile unsigned long *)0x4000C044))
#define UART0_DMACTL_R          (*((volatile unsigned long *)0x4000C048))
#define UART0_LCTL_R            (*((volatile unsigned long *)0x4000C090))
#define UART0_LSS_R             (*((volatile unsigned long *)0x4000C094))
#define UART0_LTIM_R            (*((volatile unsigned long *)0x4000C098))

//*****************************************************************************
//
// UART registers (UART1)
//
//*****************************************************************************
#define UART1_DR_R              (*((volatile unsigned long *)0x4000D000))
#define UART1_RSR_R             (*((volatile unsigned long *)0x4000D004))
#define UART1_ECR_R             (*((volatile unsigned long *)0x4000D004))
#define UART1_FR_R              (*((volatile unsigned long *)0x4000D018))
#define UART1_ILPR_R            (*((volatile unsigned long *)0x4000D020))
#define UART1_IBRD_R            (*((volatile unsigned long *)0x4000D024))
#define UART1_FBRD_R            (*((volatile unsigned long *)0x4000D028))
#define UART1_LCRH_R            (*((volatile unsigned long *)0x4000D02C))
#define UART1_CTL_R             (*((volatile unsigned long *)0x4000D030))
#define UART1_IFLS_R            (*((volatile unsigned long *)0x4000D034))
#define UART1_IM_R              (*((volatile unsigned long *)0x4000D038))
#define UART1_RIS_R             (*((volatile unsigned long *)0x4000D03C))
#define UART1_MIS_R             (*((volatile unsigned long *)0x4000D040))
#define UART1_ICR_R             (*((volatile unsigned long *)0x4000D044))
#define UART1_DMACTL_R          (*((volatile unsigned long *)0x4000D048))
#define UART1_LCTL_R            (*((volatile unsigned long *)0x4000D090))
#define UART1_LSS_R             (*((volatile unsigned long *)0x4000D094))
#define UART1_LTIM_R            (*((volatile unsigned long *)0x4000D098))

//*****************************************************************************
//
// UART registers (UART2)
//
//*****************************************************************************
#define UART2_DR_R              (*((volatile unsigned long *)0x4000E000))
#define UART2_RSR_R             (*((volatile unsigned long *)0x4000E004))
#define UART2_ECR_R             (*((volatile unsigned long *)0x4000E004))
#define UART2_FR_R              (*((volatile unsigned long *)0x4000E018))
#define UART2_ILPR_R            (*((volatile unsigned long *)0x4000E020))
#define UART2_IBRD_R            (*((volatile unsigned long *)0x4000E024))
#define UART2_FBRD_R            (*((volatile unsigned long *)0x4000E028))
#define UART2_LCRH_R            (*((volatile unsigned long *)0x4000E02C))
#define UART2_CTL_R             (*((volatile unsigned long *)0x4000E030))
#define UART2_IFLS_R            (*((volatile unsigned long *)0x4000E034))
#define UART2_IM_R              (*((volatile unsigned long *)0x4000E038))
#define UART2_RIS_R             (*((volatile unsigned long *)0x4000E03C))
#define UART2_MIS_R             (*((volatile unsigned long *)0x4000E040))
#define UART2_ICR_R             (*((volatile unsigned long *)0x4000E044))
#define UART2_DMACTL_R          (*((volatile unsigned long *)0x4000E048))
#define UART2_LCTL_R            (*((volatile unsigned long *)0x4000E090))
#define UART2_LSS_R             (*((volatile unsigned long *)0x4000E094))
#define UART2_LTIM_R            (*((volatile unsigned long *)0x4000E098))

//*****************************************************************************
//
// I2C registers (I2C0 MASTER)
//
//*****************************************************************************
#define I2C0_MASTER_MSA_R       (*((volatile unsigned long *)0x40020000))
#define I2C0_MASTER_MCS_R       (*((volatile unsigned long *)0x40020004))
#define I2C0_MASTER_MDR_R       (*((volatile unsigned long *)0x40020008))
#define I2C0_MASTER_MTPR_R      (*((volatile unsigned long *)0x4002000C))
#define I2C0_MASTER_MIMR_R      (*((volatile unsigned long *)0x40020010))
#define I2C0_MASTER_MRIS_R      (*((volatile unsigned long *)0x40020014))
#define I2C0_MASTER_MMIS_R      (*((volatile unsigned long *)0x40020018))
#define I2C0_MASTER_MICR_R      (*((volatile unsigned long *)0x4002001C))
#define I2C0_MASTER_MCR_R       (*((volatile unsigned long *)0x40020020))

//*****************************************************************************
//
// I2C registers (I2C0 SLAVE)
//
//*****************************************************************************
#define I2C0_SLAVE_SOAR_R       (*((volatile unsigned long *)0x40020800))
#define I2C0_SLAVE_SCSR_R       (*((volatile unsigned long *)0x40020804))
#define I2C0_SLAVE_SDR_R        (*((volatile unsigned long *)0x40020808))
#define I2C0_SLAVE_SIMR_R       (*((volatile unsigned long *)0x4002080C))
#define I2C0_SLAVE_SRIS_R       (*((volatile unsigned long *)0x40020810))
#define I2C0_SLAVE_SMIS_R       (*((volatile unsigned long *)0x40020814))
#define I2C0_SLAVE_SICR_R       (*((volatile unsigned long *)0x40020818))

//*****************************************************************************
//
// I2C registers (I2C1 MASTER)
//
//*****************************************************************************
#define I2C1_MASTER_MSA_R       (*((volatile unsigned long *)0x40021000))
#define I2C1_MASTER_MCS_R       (*((volatile unsigned long *)0x40021004))
#define I2C1_MASTER_MDR_R       (*((volatile unsigned long *)0x40021008))
#define I2C1_MASTER_MTPR_R      (*((volatile unsigned long *)0x4002100C))
#define I2C1_MASTER_MIMR_R      (*((volatile unsigned long *)0x40021010))
#define I2C1_MASTER_MRIS_R      (*((volatile unsigned long *)0x40021014))
#define I2C1_MASTER_MMIS_R      (*((volatile unsigned long *)0x40021018))
#define I2C1_MASTER_MICR_R      (*((volatile unsigned long *)0x4002101C))
#define I2C1_MASTER_MCR_R       (*((volatile unsigned long *)0x40021020))

//*****************************************************************************
//
// I2C registers (I2C1 SLAVE)
//
//*****************************************************************************
#define I2C1_SLAVE_SOAR_R       (*((volatile unsigned long *)0x40021800))
#define I2C1_SLAVE_SCSR_R       (*((volatile unsigned long *)0x40021804))
#define I2C1_SLAVE_SDR_R        (*((volatile unsigned long *)0x40021808))
#define I2C1_SLAVE_SIMR_R       (*((volatile unsigned long *)0x4002180C))
#define I2C1_SLAVE_SRIS_R       (*((volatile unsigned long *)0x40021810))
#define I2C1_SLAVE_SMIS_R       (*((volatile unsigned long *)0x40021814))
#define I2C1_SLAVE_SICR_R       (*((volatile unsigned long *)0x40021818))

//*****************************************************************************
//
// GPIO registers (PORTE)
//
//*****************************************************************************
#define GPIO_PORTE_DATA_BITS_R  ((volatile unsigned long *)0x40024000)
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_IS_R         (*((volatile unsigned long *)0x40024404))
#define GPIO_PORTE_IBE_R        (*((volatile unsigned long *)0x40024408))
#define GPIO_PORTE_IEV_R        (*((volatile unsigned long *)0x4002440C))
#define GPIO_PORTE_IM_R         (*((volatile unsigned long *)0x40024410))
#define GPIO_PORTE_RIS_R        (*((volatile unsigned long *)0x40024414))
#define GPIO_PORTE_MIS_R        (*((volatile unsigned long *)0x40024418))
#define GPIO_PORTE_ICR_R        (*((volatile unsigned long *)0x4002441C))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DR2R_R       (*((volatile unsigned long *)0x40024500))
#define GPIO_PORTE_DR4R_R       (*((volatile unsigned long *)0x40024504))
#define GPIO_PORTE_DR8R_R       (*((volatile unsigned long *)0x40024508))
#define GPIO_PORTE_ODR_R        (*((volatile unsigned long *)0x4002450C))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_SLR_R        (*((volatile unsigned long *)0x40024518))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

//*****************************************************************************
//
// GPIO registers (PORTF)
//
//*****************************************************************************
#define GPIO_PORTF_DATA_BITS_R  ((volatile unsigned long *)0x40025000)
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_IS_R         (*((volatile unsigned long *)0x40025404))
#define GPIO_PORTF_IBE_R        (*((volatile unsigned long *)0x40025408))
#define GPIO_PORTF_IEV_R        (*((volatile unsigned long *)0x4002540C))
#define GPIO_PORTF_IM_R         (*((volatile unsigned long *)0x40025410))
#define GPIO_PORTF_RIS_R        (*((volatile unsigned long *)0x40025414))
#define GPIO_PORTF_MIS_R        (*((volatile unsigned long *)0x40025418))
#define GPIO_PORTF_ICR_R        (*((volatile unsigned long *)0x4002541C))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DR2R_R       (*((volatile unsigned long *)0x40025500))
#define GPIO_PORTF_DR4R_R       (*((volatile unsigned long *)0x40025504))
#define GPIO_PORTF_DR8R_R       (*((volatile unsigned long *)0x40025508))
#define GPIO_PORTF_ODR_R        (*((volatile unsigned long *)0x4002550C))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_PDR_R        (*((volatile unsigned long *)0x40025514))
#define GPIO_PORTF_SLR_R        (*((volatile unsigned long *)0x40025518))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))

//*****************************************************************************
//
// GPIO registers (PORTG)
//
//*****************************************************************************
#define GPIO_PORTG_DATA_BITS_R  ((volatile unsigned long *)0x40026000)
#define GPIO_PORTG_DATA_R       (*((volatile unsigned long *)0x400263FC))
#define GPIO_PORTG_DIR_R        (*((volatile unsigned long *)0x40026400))
#define GPIO_PORTG_IS_R         (*((volatile unsigned long *)0x40026404))
#define GPIO_PORTG_IBE_R        (*((volatile unsigned long *)0x40026408))
#define GPIO_PORTG_IEV_R        (*((volatile unsigned long *)0x4002640C))
#define GPIO_PORTG_IM_R         (*((volatile unsigned long *)0x40026410))
#define GPIO_PORTG_RIS_R        (*((volatile unsigned long *)0x40026414))
#define GPIO_PORTG_MIS_R        (*((volatile unsigned long *)0x40026418))
#define GPIO_PORTG_ICR_R        (*((volatile unsigned long *)0x4002641C))
#define GPIO_PORTG_AFSEL_R      (*((volatile unsigned long *)0x40026420))
#define GPIO_PORTG_DR2R_R       (*((volatile unsigned long *)0x40026500))
#define GPIO_PORTG_DR4R_R       (*((volatile unsigned long *)0x40026504))
#define GPIO_PORTG_DR8R_R       (*((volatile unsigned long *)0x40026508))
#define GPIO_PORTG_ODR_R        (*((volatile unsigned long *)0x4002650C))
#define GPIO_PORTG_PUR_R        (*((volatile unsigned long *)0x40026510))
#define GPIO_PORTG_PDR_R        (*((volatile unsigned long *)0x40026514))
#define GPIO_PORTG_SLR_R        (*((volatile unsigned long *)0x40026518))
#define GPIO_PORTG_DEN_R        (*((volatile unsigned long *)0x4002651C))
#define GPIO_PORTG_LOCK_R       (*((volatile unsigned long *)0x40026520))
#define GPIO_PORTG_CR_R         (*((volatile unsigned long *)0x40026524))
#define GPIO_PORTG_AMSEL_R      (*((volatile unsigned long *)0x40026528))
#define GPIO_PORTG_PCTL_R       (*((volatile unsigned long *)0x4002652C))

//*****************************************************************************
//
// GPIO registers (PORTH)
//
//*****************************************************************************
#define GPIO_PORTH_DATA_BITS_R  ((volatile unsigned long *)0x40027000)
#define GPIO_PORTH_DATA_R       (*((volatile unsigned long *)0x400273FC))
#define GPIO_PORTH_DIR_R        (*((volatile unsigned long *)0x40027400))
#define GPIO_PORTH_IS_R         (*((volatile unsigned long *)0x40027404))
#define GPIO_PORTH_IBE_R        (*((volatile unsigned long *)0x40027408))
#define GPIO_PORTH_IEV_R        (*((volatile unsigned long *)0x4002740C))
#define GPIO_PORTH_IM_R         (*((volatile unsigned long *)0x40027410))
#define GPIO_PORTH_RIS_R        (*((volatile unsigned long *)0x40027414))
#define GPIO_PORTH_MIS_R        (*((volatile unsigned long *)0x40027418))
#define GPIO_PORTH_ICR_R        (*((volatile unsigned long *)0x4002741C))
#define GPIO_PORTH_AFSEL_R      (*((volatile unsigned long *)0x40027420))
#define GPIO_PORTH_DR2R_R       (*((volatile unsigned long *)0x40027500))
#define GPIO_PORTH_DR4R_R       (*((volatile unsigned long *)0x40027504))
#define GPIO_PORTH_DR8R_R       (*((volatile unsigned long *)0x40027508))
#define GPIO_PORTH_ODR_R        (*((volatile unsigned long *)0x4002750C))
#define GPIO_PORTH_PUR_R        (*((volatile unsigned long *)0x40027510))
#define GPIO_PORTH_PDR_R        (*((volatile unsigned long *)0x40027514))
#define GPIO_PORTH_SLR_R        (*((volatile unsigned long *)0x40027518))
#define GPIO_PORTH_DEN_R        (*((volatile unsigned long *)0x4002751C))
#define GPIO_PORTH_LOCK_R       (*((volatile unsigned long *)0x40027520))
#define GPIO_PORTH_CR_R         (*((volatile unsigned long *)0x40027524))
#define GPIO_PORTH_AMSEL_R      (*((volatile unsigned long *)0x40027528))
#define GPIO_PORTH_PCTL_R       (*((volatile unsigned long *)0x4002752C))

//*****************************************************************************
//
// Timer registers (TIMER0)
//
//*****************************************************************************
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_TBMR_R           (*((volatile unsigned long *)0x40030008))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_RIS_R            (*((volatile unsigned long *)0x4003001C))
#define TIMER0_MIS_R            (*((volatile unsigned long *)0x40030020))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TBILR_R          (*((volatile unsigned long *)0x4003002C))
#define TIMER0_TAMATCHR_R       (*((volatile unsigned long *)0x40030030))
#define TIMER0_TBMATCHR_R       (*((volatile unsigned long *)0x40030034))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))
#define TIMER0_TBPR_R           (*((volatile unsigned long *)0x4003003C))
#define TIMER0_TAPMR_R          (*((volatile unsigned long *)0x40030040))
#define TIMER0_TBPMR_R          (*((volatile unsigned long *)0x40030044))
#define TIMER0_TAR_R            (*((volatile unsigned long *)0x40030048))
#define TIMER0_TBR_R            (*((volatile unsigned long *)0x4003004C))
#define TIMER0_TAV_R            (*((volatile unsigned long *)0x40030050))
#define TIMER0_TBV_R            (*((volatile unsigned long *)0x40030054))

//*****************************************************************************
//
// Timer registers (TIMER1)
//
//*****************************************************************************
#define TIMER1_CFG_R            (*((volatile unsigned long *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile unsigned long *)0x40031004))
#define TIMER1_TBMR_R           (*((volatile unsigned long *)0x40031008))
#define TIMER1_CTL_R            (*((volatile unsigned long *)0x4003100C))
#define TIMER1_IMR_R            (*((volatile unsigned long *)0x40031018))
#define TIMER1_RIS_R            (*((volatile unsigned long *)0x4003101C))
#define TIMER1_MIS_R            (*((volatile unsigned long *)0x40031020))
#define TIMER1_ICR_R            (*((volatile unsigned long *)0x40031024))
#define TIMER1_TAILR_R          (*((volatile unsigned long *)0x40031028))
#define TIMER1_TBILR_R          (*((volatile unsigned long *)0x4003102C))
#define TIMER1_TAMATCHR_R       (*((volatile unsigned long *)0x40031030))
#define TIMER1_TBMATCHR_R       (*((volatile unsigned long *)0x40031034))
#define TIMER1_TAPR_R           (*((volatile unsigned long *)0x40031038))
#define TIMER1_TBPR_R           (*((volatile unsigned long *)0x4003103C))
#define TIMER1_TAPMR_R          (*((volatile unsigned long *)0x40031040))
#define TIMER1_TBPMR_R          (*((volatile unsigned long *)0x40031044))
#define TIMER1_TAR_R            (*((volatile unsigned long *)0x40031048))
#define TIMER1_TBR_R            (*((volatile unsigned long *)0x4003104C))
#define TIMER1_TAV_R            (*((volatile unsigned long *)0x40031050))
#define TIMER1_TBV_R            (*((volatile unsigned long *)0x40031054))

//*****************************************************************************
//
// Timer registers (TIMER2)
//
//*****************************************************************************
#define TIMER2_CFG_R            (*((volatile unsigned long *)0x40032000))
#define TIMER2_TAMR_R           (*((volatile unsigned long *)0x40032004))
#define TIMER2_TBMR_R           (*((volatile unsigned long *)0x40032008))
#define TIMER2_CTL_R            (*((volatile unsigned long *)0x4003200C))
#define TIMER2_IMR_R            (*((volatile unsigned long *)0x40032018))
#define TIMER2_RIS_R            (*((volatile unsigned long *)0x4003201C))
#define TIMER2_MIS_R            (*((volatile unsigned long *)0x40032020))
#define TIMER2_ICR_R            (*((volatile unsigned long *)0x40032024))
#define TIMER2_TAILR_R          (*((volatile unsigned long *)0x40032028))
#define TIMER2_TBILR_R          (*((volatile unsigned long *)0x4003202C))
#define TIMER2_TAMATCHR_R       (*((volatile unsigned long *)0x40032030))
#define TIMER2_TBMATCHR_R       (*((volatile unsigned long *)0x40032034))
#define TIMER2_TAPR_R           (*((volatile unsigned long *)0x40032038))
#define TIMER2_TBPR_R           (*((volatile unsigned long *)0x4003203C))
#define TIMER2_TAPMR_R          (*((volatile unsigned long *)0x40032040))
#define TIMER2_TBPMR_R          (*((volatile unsigned long *)0x40032044))
#define TIMER2_TAR_R            (*((volatile unsigned long *)0x40032048))
#define TIMER2_TBR_R            (*((volatile unsigned long *)0x4003204C))
#define TIMER2_TAV_R            (*((volatile unsigned long *)0x40032050))
#define TIMER2_TBV_R            (*((volatile unsigned long *)0x40032054))

//*****************************************************************************
//
// Timer registers (TIMER3)
//
//*****************************************************************************
#define TIMER3_CFG_R            (*((volatile unsigned long *)0x40033000))
#define TIMER3_TAMR_R           (*((volatile unsigned long *)0x40033004))
#define TIMER3_TBMR_R           (*((volatile unsigned long *)0x40033008))
#define TIMER3_CTL_R            (*((volatile unsigned long *)0x4003300C))
#define TIMER3_IMR_R            (*((volatile unsigned long *)0x40033018))
#define TIMER3_RIS_R            (*((volatile unsigned long *)0x4003301C))
#define TIMER3_MIS_R            (*((volatile unsigned long *)0x40033020))
#define TIMER3_ICR_R            (*((volatile unsigned long *)0x40033024))
#define TIMER3_TAILR_R          (*((volatile unsigned long *)0x40033028))
#define TIMER3_TBILR_R          (*((volatile unsigned long *)0x4003302C))
#define TIMER3_TAMATCHR_R       (*((volatile unsigned long *)0x40033030))
#define TIMER3_TBMATCHR_R       (*((volatile unsigned long *)0x40033034))
#define TIMER3_TAPR_R           (*((volatile unsigned long *)0x40033038))
#define TIMER3_TBPR_R           (*((volatile unsigned long *)0x4003303C))
#define TIMER3_TAPMR_R          (*((volatile unsigned long *)0x40033040))
#define TIMER3_TBPMR_R          (*((volatile unsigned long *)0x40033044))
#define TIMER3_TAR_R            (*((volatile unsigned long *)0x40033048))
#define TIMER3_TBR_R            (*((volatile unsigned long *)0x4003304C))
#define TIMER3_TAV_R            (*((volatile unsigned long *)0x40033050))
#define TIMER3_TBV_R            (*((volatile unsigned long *)0x40033054))

//*****************************************************************************
//
// ADC registers (ADC0)
//
//*****************************************************************************
#define ADC0_ACTSS_R            (*((volatile unsigned long *)0x40038000))
#define ADC0_RIS_R              (*((volatile unsigned long *)0x40038004))
#define ADC0_IM_R               (*((volatile unsigned long *)0x40038008))
#define ADC0_ISC_R              (*((volatile unsigned long *)0x4003800C))
#define ADC0_OSTAT_R            (*((volatile unsigned long *)0x40038010))
#define ADC0_EMUX_R             (*((volatile unsigned long *)0x40038014))
#define ADC0_USTAT_R            (*((volatile unsigned long *)0x40038018))
#define ADC0_SSPRI_R            (*((volatile unsigned long *)0x40038020))
#define ADC0_SPC_R              (*((volatile unsigned long *)0x40038024))
#define ADC0_PSSI_R             (*((volatile unsigned long *)0x40038028))
#define ADC0_SAC_R              (*((volatile unsigned long *)0x40038030))
#define ADC0_DCISC_R            (*((volatile unsigned long *)0x40038034))
#define ADC0_CTL_R              (*((volatile unsigned long *)0x40038038))
#define ADC0_SSMUX0_R           (*((volatile unsigned long *)0x40038040))
#define ADC0_SSCTL0_R           (*((volatile unsigned long *)0x40038044))
#define ADC0_SSFIFO0_R          (*((volatile unsigned long *)0x40038048))
#define ADC0_SSFSTAT0_R         (*((volatile unsigned long *)0x4003804C))
#define ADC0_SSOP0_R            (*((volatile unsigned long *)0x40038050))
#define ADC0_SSDC0_R            (*((volatile unsigned long *)0x40038054))
#define ADC0_SSMUX1_R           (*((volatile unsigned long *)0x40038060))
#define ADC0_SSCTL1_R           (*((volatile unsigned long *)0x40038064))
#define ADC0_SSFIFO1_R          (*((volatile unsigned long *)0x40038068))
#define ADC0_SSFSTAT1_R         (*((volatile unsigned long *)0x4003806C))
#define ADC0_SSOP1_R            (*((volatile unsigned long *)0x40038070))
#define ADC0_SSDC1_R            (*((volatile unsigned long *)0x40038074))
#define ADC0_SSMUX2_R           (*((volatile unsigned long *)0x40038080))
#define ADC0_SSCTL2_R           (*((volatile unsigned long *)0x40038084))
#define ADC0_SSFIFO2_R          (*((volatile unsigned long *)0x40038088))
#define ADC0_SSFSTAT2_R         (*((volatile unsigned long *)0x4003808C))
#define ADC0_SSOP2_R            (*((volatile unsigned long *)0x40038090))
#define ADC0_SSDC2_R            (*((volatile unsigned long *)0x40038094))
#define ADC0_SSMUX3_R           (*((volatile unsigned long *)0x400380A0))
#define ADC0_SSCTL3_R           (*((volatile unsigned long *)0x400380A4))
#define ADC0_SSFIFO3_R          (*((volatile unsigned long *)0x400380A8))
#define ADC0_SSFSTAT3_R         (*((volatile unsigned long *)0x400380AC))
#define ADC0_SSOP3_R            (*((volatile unsigned long *)0x400380B0))
#define ADC0_SSDC3_R            (*((volatile unsigned long *)0x400380B4))
#define ADC0_DCRIC_R            (*((volatile unsigned long *)0x40038D00))
#define ADC0_DCCTL0_R           (*((volatile unsigned long *)0x40038E00))
#define ADC0_DCCTL1_R           (*((volatile unsigned long *)0x40038E04))
#define ADC0_DCCTL2_R           (*((volatile unsigned long *)0x40038E08))
#define ADC0_DCCTL3_R           (*((volatile unsigned long *)0x40038E0C))
#define ADC0_DCCTL4_R           (*((volatile unsigned long *)0x40038E10))
#define ADC0_DCCTL5_R           (*((volatile unsigned long *)0x40038E14))
#define ADC0_DCCTL6_R           (*((volatile unsigned long *)0x40038E18))
#define ADC0_DCCTL7_R           (*((volatile unsigned long *)0x40038E1C))
#define ADC0_DCCMP0_R           (*((volatile unsigned long *)0x40038E40))
#define ADC0_DCCMP1_R           (*((volatile unsigned long *)0x40038E44))
#define ADC0_DCCMP2_R           (*((volatile unsigned long *)0x40038E48))
#define ADC0_DCCMP3_R           (*((volatile unsigned long *)0x40038E4C))
#define ADC0_DCCMP4_R           (*((volatile unsigned long *)0x40038E50))
#define ADC0_DCCMP5_R           (*((volatile unsigned long *)0x40038E54))
#define ADC0_DCCMP6_R           (*((volatile unsigned long *)0x40038E58))
#define ADC0_DCCMP7_R           (*((volatile unsigned long *)0x40038E5C))

//*****************************************************************************
//
// Comparator registers (COMP)
//
//*****************************************************************************
#define COMP_ACMIS_R            (*((volatile unsigned long *)0x4003C000))
#define COMP_ACRIS_R            (*((volatile unsigned long *)0x4003C004))
#define COMP_ACINTEN_R          (*((volatile unsigned long *)0x4003C008))
#define COMP_ACREFCTL_R         (*((volatile unsigned long *)0x4003C010))
#define COMP_ACSTAT0_R          (*((volatile unsigned long *)0x4003C020))
#define COMP_ACCTL0_R           (*((volatile unsigned long *)0x4003C024))
#define COMP_ACSTAT1_R          (*((volatile unsigned long *)0x4003C040))
#define COMP_ACCTL1_R           (*((volatile unsigned long *)0x4003C044))

//*****************************************************************************
//
// GPIO registers (PORTJ)
//
//*****************************************************************************
#define GPIO_PORTJ_DATA_BITS_R  ((volatile unsigned long *)0x4003D000)
#define GPIO_PORTJ_DATA_R       (*((volatile unsigned long *)0x4003D3FC))
#define GPIO_PORTJ_DIR_R        (*((volatile unsigned long *)0x4003D400))
#define GPIO_PORTJ_IS_R         (*((volatile unsigned long *)0x4003D404))
#define GPIO_PORTJ_IBE_R        (*((volatile unsigned long *)0x4003D408))
#define GPIO_PORTJ_IEV_R        (*((volatile unsigned long *)0x4003D40C))
#define GPIO_PORTJ_IM_R         (*((volatile unsigned long *)0x4003D410))
#define GPIO_PORTJ_RIS_R        (*((volatile unsigned long *)0x4003D414))
#define GPIO_PORTJ_MIS_R        (*((volatile unsigned long *)0x4003D418))
#define GPIO_PORTJ_ICR_R        (*((volatile unsigned long *)0x4003D41C))
#define GPIO_PORTJ_AFSEL_R      (*((volatile unsigned long *)0x4003D420))
#define GPIO_PORTJ_DR2R_R       (*((volatile unsigned long *)0x4003D500))
#define GPIO_PORTJ_DR4R_R       (*((volatile unsigned long *)0x4003D504))
#define GPIO_PORTJ_DR8R_R       (*((volatile unsigned long *)0x4003D508))
#define GPIO_PORTJ_ODR_R        (*((volatile unsigned long *)0x4003D50C))
#define GPIO_PORTJ_PUR_R        (*((volatile unsigned long *)0x4003D510))
#define GPIO_PORTJ_PDR_R        (*((volatile unsigned long *)0x4003D514))
#define GPIO_PORTJ_SLR_R        (*((volatile unsigned long *)0x4003D518))
#define GPIO_PORTJ_DEN_R        (*((volatile unsigned long *)0x4003D51C))
#define GPIO_PORTJ_LOCK_R       (*((volatile unsigned long *)0x4003D520))
#define GPIO_PORTJ_CR_R         (*((volatile unsigned long *)0x4003D524))
#define GPIO_PORTJ_AMSEL_R      (*((volatile unsigned long *)0x4003D528))
#define GPIO_PORTJ_PCTL_R       (*((volatile unsigned long *)0x4003D52C))

//*****************************************************************************
//
// GPIO registers (PORTA AHB)
//
//*****************************************************************************
#define GPIO_PORTA_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x40058000)
#define GPIO_PORTA_AHB_DATA_R   (*((volatile unsigned long *)0x400583FC))
#define GPIO_PORTA_AHB_DIR_R    (*((volatile unsigned long *)0x40058400))
#define GPIO_PORTA_AHB_IS_R     (*((volatile unsigned long *)0x40058404))
#define GPIO_PORTA_AHB_IBE_R    (*((volatile unsigned long *)0x40058408))
#define GPIO_PORTA_AHB_IEV_R    (*((volatile unsigned long *)0x4005840C))
#define GPIO_PORTA_AHB_IM_R     (*((volatile unsigned long *)0x40058410))
#define GPIO_PORTA_AHB_RIS_R    (*((volatile unsigned long *)0x40058414))
#define GPIO_PORTA_AHB_MIS_R    (*((volatile unsigned long *)0x40058418))
#define GPIO_PORTA_AHB_ICR_R    (*((volatile unsigned long *)0x4005841C))
#define GPIO_PORTA_AHB_AFSEL_R  (*((volatile unsigned long *)0x40058420))
#define GPIO_PORTA_AHB_DR2R_R   (*((volatile unsigned long *)0x40058500))
#define GPIO_PORTA_AHB_DR4R_R   (*((volatile unsigned long *)0x40058504))
#define GPIO_PORTA_AHB_DR8R_R   (*((volatile unsigned long *)0x40058508))
#define GPIO_PORTA_AHB_ODR_R    (*((volatile unsigned long *)0x4005850C))
#define GPIO_PORTA_AHB_PUR_R    (*((volatile unsigned long *)0x40058510))
#define GPIO_PORTA_AHB_PDR_R    (*((volatile unsigned long *)0x40058514))
#define GPIO_PORTA_AHB_SLR_R    (*((volatile unsigned long *)0x40058518))
#define GPIO_PORTA_AHB_DEN_R    (*((volatile unsigned long *)0x4005851C))
#define GPIO_PORTA_AHB_LOCK_R   (*((volatile unsigned long *)0x40058520))
#define GPIO_PORTA_AHB_CR_R     (*((volatile unsigned long *)0x40058524))
#define GPIO_PORTA_AHB_AMSEL_R  (*((volatile unsigned long *)0x40058528))
#define GPIO_PORTA_AHB_PCTL_R   (*((volatile unsigned long *)0x4005852C))

//*****************************************************************************
//
// GPIO registers (PORTB AHB)
//
//*****************************************************************************
#define GPIO_PORTB_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x40059000)
#define GPIO_PORTB_AHB_DATA_R   (*((volatile unsigned long *)0x400593FC))
#define GPIO_PORTB_AHB_DIR_R    (*((volatile unsigned long *)0x40059400))
#define GPIO_PORTB_AHB_IS_R     (*((volatile unsigned long *)0x40059404))
#define GPIO_PORTB_AHB_IBE_R    (*((volatile unsigned long *)0x40059408))
#define GPIO_PORTB_AHB_IEV_R    (*((volatile unsigned long *)0x4005940C))
#define GPIO_PORTB_AHB_IM_R     (*((volatile unsigned long *)0x40059410))
#define GPIO_PORTB_AHB_RIS_R    (*((volatile unsigned long *)0x40059414))
#define GPIO_PORTB_AHB_MIS_R    (*((volatile unsigned long *)0x40059418))
#define GPIO_PORTB_AHB_ICR_R    (*((volatile unsigned long *)0x4005941C))
#define GPIO_PORTB_AHB_AFSEL_R  (*((volatile unsigned long *)0x40059420))
#define GPIO_PORTB_AHB_DR2R_R   (*((volatile unsigned long *)0x40059500))
#define GPIO_PORTB_AHB_DR4R_R   (*((volatile unsigned long *)0x40059504))
#define GPIO_PORTB_AHB_DR8R_R   (*((volatile unsigned long *)0x40059508))
#define GPIO_PORTB_AHB_ODR_R    (*((volatile unsigned long *)0x4005950C))
#define GPIO_PORTB_AHB_PUR_R    (*((volatile unsigned long *)0x40059510))
#define GPIO_PORTB_AHB_PDR_R    (*((volatile unsigned long *)0x40059514))
#define GPIO_PORTB_AHB_SLR_R    (*((volatile unsigned long *)0x40059518))
#define GPIO_PORTB_AHB_DEN_R    (*((volatile unsigned long *)0x4005951C))
#define GPIO_PORTB_AHB_LOCK_R   (*((volatile unsigned long *)0x40059520))
#define GPIO_PORTB_AHB_CR_R     (*((volatile unsigned long *)0x40059524))
#define GPIO_PORTB_AHB_AMSEL_R  (*((volatile unsigned long *)0x40059528))
#define GPIO_PORTB_AHB_PCTL_R   (*((volatile unsigned long *)0x4005952C))

//*****************************************************************************
//
// GPIO registers (PORTC AHB)
//
//*****************************************************************************
#define GPIO_PORTC_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005A000)
#define GPIO_PORTC_AHB_DATA_R   (*((volatile unsigned long *)0x4005A3FC))
#define GPIO_PORTC_AHB_DIR_R    (*((volatile unsigned long *)0x4005A400))
#define GPIO_PORTC_AHB_IS_R     (*((volatile unsigned long *)0x4005A404))
#define GPIO_PORTC_AHB_IBE_R    (*((volatile unsigned long *)0x4005A408))
#define GPIO_PORTC_AHB_IEV_R    (*((volatile unsigned long *)0x4005A40C))
#define GPIO_PORTC_AHB_IM_R     (*((volatile unsigned long *)0x4005A410))
#define GPIO_PORTC_AHB_RIS_R    (*((volatile unsigned long *)0x4005A414))
#define GPIO_PORTC_AHB_MIS_R    (*((volatile unsigned long *)0x4005A418))
#define GPIO_PORTC_AHB_ICR_R    (*((volatile unsigned long *)0x4005A41C))
#define GPIO_PORTC_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005A420))
#define GPIO_PORTC_AHB_DR2R_R   (*((volatile unsigned long *)0x4005A500))
#define GPIO_PORTC_AHB_DR4R_R   (*((volatile unsigned long *)0x4005A504))
#define GPIO_PORTC_AHB_DR8R_R   (*((volatile unsigned long *)0x4005A508))
#define GPIO_PORTC_AHB_ODR_R    (*((volatile unsigned long *)0x4005A50C))
#define GPIO_PORTC_AHB_PUR_R    (*((volatile unsigned long *)0x4005A510))
#define GPIO_PORTC_AHB_PDR_R    (*((volatile unsigned long *)0x4005A514))
#define GPIO_PORTC_AHB_SLR_R    (*((volatile unsigned long *)0x4005A518))
#define GPIO_PORTC_AHB_DEN_R    (*((volatile unsigned long *)0x4005A51C))
#define GPIO_PORTC_AHB_LOCK_R   (*((volatile unsigned long *)0x4005A520))
#define GPIO_PORTC_AHB_CR_R     (*((volatile unsigned long *)0x4005A524))
#define GPIO_PORTC_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005A528))
#define GPIO_PORTC_AHB_PCTL_R   (*((volatile unsigned long *)0x4005A52C))

//*****************************************************************************
//
// GPIO registers (PORTD AHB)
//
//*****************************************************************************
#define GPIO_PORTD_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005B000)
#define GPIO_PORTD_AHB_DATA_R   (*((volatile unsigned long *)0x4005B3FC))
#define GPIO_PORTD_AHB_DIR_R    (*((volatile unsigned long *)0x4005B400))
#define GPIO_PORTD_AHB_IS_R     (*((volatile unsigned long *)0x4005B404))
#define GPIO_PORTD_AHB_IBE_R    (*((volatile unsigned long *)0x4005B408))
#define GPIO_PORTD_AHB_IEV_R    (*((volatile unsigned long *)0x4005B40C))
#define GPIO_PORTD_AHB_IM_R     (*((volatile unsigned long *)0x4005B410))
#define GPIO_PORTD_AHB_RIS_R    (*((volatile unsigned long *)0x4005B414))
#define GPIO_PORTD_AHB_MIS_R    (*((volatile unsigned long *)0x4005B418))
#define GPIO_PORTD_AHB_ICR_R    (*((volatile unsigned long *)0x4005B41C))
#define GPIO_PORTD_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005B420))
#define GPIO_PORTD_AHB_DR2R_R   (*((volatile unsigned long *)0x4005B500))
#define GPIO_PORTD_AHB_DR4R_R   (*((volatile unsigned long *)0x4005B504))
#define GPIO_PORTD_AHB_DR8R_R   (*((volatile unsigned long *)0x4005B508))
#define GPIO_PORTD_AHB_ODR_R    (*((volatile unsigned long *)0x4005B50C))
#define GPIO_PORTD_AHB_PUR_R    (*((volatile unsigned long *)0x4005B510))
#define GPIO_PORTD_AHB_PDR_R    (*((volatile unsigned long *)0x4005B514))
#define GPIO_PORTD_AHB_SLR_R    (*((volatile unsigned long *)0x4005B518))
#define GPIO_PORTD_AHB_DEN_R    (*((volatile unsigned long *)0x4005B51C))
#define GPIO_PORTD_AHB_LOCK_R   (*((volatile unsigned long *)0x4005B520))
#define GPIO_PORTD_AHB_CR_R     (*((volatile unsigned long *)0x4005B524))
#define GPIO_PORTD_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005B528))
#define GPIO_PORTD_AHB_PCTL_R   (*((volatile unsigned long *)0x4005B52C))

//*****************************************************************************
//
// GPIO registers (PORTE AHB)
//
//*****************************************************************************
#define GPIO_PORTE_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005C000)
#define GPIO_PORTE_AHB_DATA_R   (*((volatile unsigned long *)0x4005C3FC))
#define GPIO_PORTE_AHB_DIR_R    (*((volatile unsigned long *)0x4005C400))
#define GPIO_PORTE_AHB_IS_R     (*((volatile unsigned long *)0x4005C404))
#define GPIO_PORTE_AHB_IBE_R    (*((volatile unsigned long *)0x4005C408))
#define GPIO_PORTE_AHB_IEV_R    (*((volatile unsigned long *)0x4005C40C))
#define GPIO_PORTE_AHB_IM_R     (*((volatile unsigned long *)0x4005C410))
#define GPIO_PORTE_AHB_RIS_R    (*((volatile unsigned long *)0x4005C414))
#define GPIO_PORTE_AHB_MIS_R    (*((volatile unsigned long *)0x4005C418))
#define GPIO_PORTE_AHB_ICR_R    (*((volatile unsigned long *)0x4005C41C))
#define GPIO_PORTE_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005C420))
#define GPIO_PORTE_AHB_DR2R_R   (*((volatile unsigned long *)0x4005C500))
#define GPIO_PORTE_AHB_DR4R_R   (*((volatile unsigned long *)0x4005C504))
#define GPIO_PORTE_AHB_DR8R_R   (*((volatile unsigned long *)0x4005C508))
#define GPIO_PORTE_AHB_ODR_R    (*((volatile unsigned long *)0x4005C50C))
#define GPIO_PORTE_AHB_PUR_R    (*((volatile unsigned long *)0x4005C510))
#define GPIO_PORTE_AHB_PDR_R    (*((volatile unsigned long *)0x4005C514))
#define GPIO_PORTE_AHB_SLR_R    (*((volatile unsigned long *)0x4005C518))
#define GPIO_PORTE_AHB_DEN_R    (*((volatile unsigned long *)0x4005C51C))
#define GPIO_PORTE_AHB_LOCK_R   (*((volatile unsigned long *)0x4005C520))
#define GPIO_PORTE_AHB_CR_R     (*((volatile unsigned long *)0x4005C524))
#define GPIO_PORTE_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005C528))
#define GPIO_PORTE_AHB_PCTL_R   (*((volatile unsigned long *)0x4005C52C))

//*****************************************************************************
//
// GPIO registers (PORTF AHB)
//
//*****************************************************************************
#define GPIO_PORTF_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005D000)
#define GPIO_PORTF_AHB_DATA_R   (*((volatile unsigned long *)0x4005D3FC))
#define GPIO_PORTF_AHB_DIR_R    (*((volatile unsigned long *)0x4005D400))
#define GPIO_PORTF_AHB_IS_R     (*((volatile unsigned long *)0x4005D404))
#define GPIO_PORTF_AHB_IBE_R    (*((volatile unsigned long *)0x4005D408))
#define GPIO_PORTF_AHB_IEV_R    (*((volatile unsigned long *)0x4005D40C))
#define GPIO_PORTF_AHB_IM_R     (*((volatile unsigned long *)0x4005D410))
#define GPIO_PORTF_AHB_RIS_R    (*((volatile unsigned long *)0x4005D414))
#define GPIO_PORTF_AHB_MIS_R    (*((volatile unsigned long *)0x4005D418))
#define GPIO_PORTF_AHB_ICR_R    (*((volatile unsigned long *)0x4005D41C))
#define GPIO_PORTF_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005D420))
#define GPIO_PORTF_AHB_DR2R_R   (*((volatile unsigned long *)0x4005D500))
#define GPIO_PORTF_AHB_DR4R_R   (*((volatile unsigned long *)0x4005D504))
#define GPIO_PORTF_AHB_DR8R_R   (*((volatile unsigned long *)0x4005D508))
#define GPIO_PORTF_AHB_ODR_R    (*((volatile unsigned long *)0x4005D50C))
#define GPIO_PORTF_AHB_PUR_R    (*((volatile unsigned long *)0x4005D510))
#define GPIO_PORTF_AHB_PDR_R    (*((volatile unsigned long *)0x4005D514))
#define GPIO_PORTF_AHB_SLR_R    (*((volatile unsigned long *)0x4005D518))
#define GPIO_PORTF_AHB_DEN_R    (*((volatile unsigned long *)0x4005D51C))
#define GPIO_PORTF_AHB_LOCK_R   (*((volatile unsigned long *)0x4005D520))
#define GPIO_PORTF_AHB_CR_R     (*((volatile unsigned long *)0x4005D524))
#define GPIO_PORTF_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005D528))
#define GPIO_PORTF_AHB_PCTL_R   (*((volatile unsigned long *)0x4005D52C))

//*****************************************************************************
//
// GPIO registers (PORTG AHB)
//
//*****************************************************************************
#define GPIO_PORTG_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005E000)
#define GPIO_PORTG_AHB_DATA_R   (*((volatile unsigned long *)0x4005E3FC))
#define GPIO_PORTG_AHB_DIR_R    (*((volatile unsigned long *)0x4005E400))
#define GPIO_PORTG_AHB_IS_R     (*((volatile unsigned long *)0x4005E404))
#define GPIO_PORTG_AHB_IBE_R    (*((volatile unsigned long *)0x4005E408))
#define GPIO_PORTG_AHB_IEV_R    (*((volatile unsigned long *)0x4005E40C))
#define GPIO_PORTG_AHB_IM_R     (*((volatile unsigned long *)0x4005E410))
#define GPIO_PORTG_AHB_RIS_R    (*((volatile unsigned long *)0x4005E414))
#define GPIO_PORTG_AHB_MIS_R    (*((volatile unsigned long *)0x4005E418))
#define GPIO_PORTG_AHB_ICR_R    (*((volatile unsigned long *)0x4005E41C))
#define GPIO_PORTG_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005E420))
#define GPIO_PORTG_AHB_DR2R_R   (*((volatile unsigned long *)0x4005E500))
#define GPIO_PORTG_AHB_DR4R_R   (*((volatile unsigned long *)0x4005E504))
#define GPIO_PORTG_AHB_DR8R_R   (*((volatile unsigned long *)0x4005E508))
#define GPIO_PORTG_AHB_ODR_R    (*((volatile unsigned long *)0x4005E50C))
#define GPIO_PORTG_AHB_PUR_R    (*((volatile unsigned long *)0x4005E510))
#define GPIO_PORTG_AHB_PDR_R    (*((volatile unsigned long *)0x4005E514))
#define GPIO_PORTG_AHB_SLR_R    (*((volatile unsigned long *)0x4005E518))
#define GPIO_PORTG_AHB_DEN_R    (*((volatile unsigned long *)0x4005E51C))
#define GPIO_PORTG_AHB_LOCK_R   (*((volatile unsigned long *)0x4005E520))
#define GPIO_PORTG_AHB_CR_R     (*((volatile unsigned long *)0x4005E524))
#define GPIO_PORTG_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005E528))
#define GPIO_PORTG_AHB_PCTL_R   (*((volatile unsigned long *)0x4005E52C))

//*****************************************************************************
//
// GPIO registers (PORTH AHB)
//
//*****************************************************************************
#define GPIO_PORTH_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x4005F000)
#define GPIO_PORTH_AHB_DATA_R   (*((volatile unsigned long *)0x4005F3FC))
#define GPIO_PORTH_AHB_DIR_R    (*((volatile unsigned long *)0x4005F400))
#define GPIO_PORTH_AHB_IS_R     (*((volatile unsigned long *)0x4005F404))
#define GPIO_PORTH_AHB_IBE_R    (*((volatile unsigned long *)0x4005F408))
#define GPIO_PORTH_AHB_IEV_R    (*((volatile unsigned long *)0x4005F40C))
#define GPIO_PORTH_AHB_IM_R     (*((volatile unsigned long *)0x4005F410))
#define GPIO_PORTH_AHB_RIS_R    (*((volatile unsigned long *)0x4005F414))
#define GPIO_PORTH_AHB_MIS_R    (*((volatile unsigned long *)0x4005F418))
#define GPIO_PORTH_AHB_ICR_R    (*((volatile unsigned long *)0x4005F41C))
#define GPIO_PORTH_AHB_AFSEL_R  (*((volatile unsigned long *)0x4005F420))
#define GPIO_PORTH_AHB_DR2R_R   (*((volatile unsigned long *)0x4005F500))
#define GPIO_PORTH_AHB_DR4R_R   (*((volatile unsigned long *)0x4005F504))
#define GPIO_PORTH_AHB_DR8R_R   (*((volatile unsigned long *)0x4005F508))
#define GPIO_PORTH_AHB_ODR_R    (*((volatile unsigned long *)0x4005F50C))
#define GPIO_PORTH_AHB_PUR_R    (*((volatile unsigned long *)0x4005F510))
#define GPIO_PORTH_AHB_PDR_R    (*((volatile unsigned long *)0x4005F514))
#define GPIO_PORTH_AHB_SLR_R    (*((volatile unsigned long *)0x4005F518))
#define GPIO_PORTH_AHB_DEN_R    (*((volatile unsigned long *)0x4005F51C))
#define GPIO_PORTH_AHB_LOCK_R   (*((volatile unsigned long *)0x4005F520))
#define GPIO_PORTH_AHB_CR_R     (*((volatile unsigned long *)0x4005F524))
#define GPIO_PORTH_AHB_AMSEL_R  (*((volatile unsigned long *)0x4005F528))
#define GPIO_PORTH_AHB_PCTL_R   (*((volatile unsigned long *)0x4005F52C))

//*****************************************************************************
//
// GPIO registers (PORTJ AHB)
//
//*****************************************************************************
#define GPIO_PORTJ_AHB_DATA_BITS_R \
                                ((volatile unsigned long *)0x40060000)
#define GPIO_PORTJ_AHB_DATA_R   (*((volatile unsigned long *)0x400603FC))
#define GPIO_PORTJ_AHB_DIR_R    (*((volatile unsigned long *)0x40060400))
#define GPIO_PORTJ_AHB_IS_R     (*((volatile unsigned long *)0x40060404))
#define GPIO_PORTJ_AHB_IBE_R    (*((volatile unsigned long *)0x40060408))
#define GPIO_PORTJ_AHB_IEV_R    (*((volatile unsigned long *)0x4006040C))
#define GPIO_PORTJ_AHB_IM_R     (*((volatile unsigned long *)0x40060410))
#define GPIO_PORTJ_AHB_RIS_R    (*((volatile unsigned long *)0x40060414))
#define GPIO_PORTJ_AHB_MIS_R    (*((volatile unsigned long *)0x40060418))
#define GPIO_PORTJ_AHB_ICR_R    (*((volatile unsigned long *)0x4006041C))
#define GPIO_PORTJ_AHB_AFSEL_R  (*((volatile unsigned long *)0x40060420))
#define GPIO_PORTJ_AHB_DR2R_R   (*((volatile unsigned long *)0x40060500))
#define GPIO_PORTJ_AHB_DR4R_R   (*((volatile unsigned long *)0x40060504))
#define GPIO_PORTJ_AHB_DR8R_R   (*((volatile unsigned long *)0x40060508))
#define GPIO_PORTJ_AHB_ODR_R    (*((volatile unsigned long *)0x4006050C))
#define GPIO_PORTJ_AHB_PUR_R    (*((volatile unsigned long *)0x40060510))
#define GPIO_PORTJ_AHB_PDR_R    (*((volatile unsigned long *)0x40060514))
#define GPIO_PORTJ_AHB_SLR_R    (*((volatile unsigned long *)0x40060518))
#define GPIO_PORTJ_AHB_DEN_R    (*((volatile unsigned long *)0x4006051C))
#define GPIO_PORTJ_AHB_LOCK_R   (*((volatile unsigned long *)0x40060520))
#define GPIO_PORTJ_AHB_CR_R     (*((volatile unsigned long *)0x40060524))
#define GPIO_PORTJ_AHB_AMSEL_R  (*((volatile unsigned long *)0x40060528))
#define GPIO_PORTJ_AHB_PCTL_R   (*((volatile unsigned long *)0x4006052C))

//*****************************************************************************
//
// External Peripheral Interface registers (EPI0)
//
//*****************************************************************************
#define EPI0_CFG_R              (*((volatile unsigned long *)0x400D0000))
#define EPI0_BAUD_R             (*((volatile unsigned long *)0x400D0004))
#define EPI0_HB16CFG_R          (*((volatile unsigned long *)0x400D0010))
#define EPI0_GPCFG_R            (*((volatile unsigned long *)0x400D0010))
#define EPI0_SDRAMCFG_R         (*((volatile unsigned long *)0x400D0010))
#define EPI0_HB8CFG_R           (*((volatile unsigned long *)0x400D0010))
#define EPI0_HB8CFG2_R          (*((volatile unsigned long *)0x400D0014))
#define EPI0_HB16CFG2_R         (*((volatile unsigned long *)0x400D0014))
#define EPI0_GPCFG2_R           (*((volatile unsigned long *)0x400D0014))
#define EPI0_ADDRMAP_R          (*((volatile unsigned long *)0x400D001C))
#define EPI0_RSIZE0_R           (*((volatile unsigned long *)0x400D0020))
#define EPI0_RADDR0_R           (*((volatile unsigned long *)0x400D0024))
#define EPI0_RPSTD0_R           (*((volatile unsigned long *)0x400D0028))
#define EPI0_RSIZE1_R           (*((volatile unsigned long *)0x400D0030))
#define EPI0_RADDR1_R           (*((volatile unsigned long *)0x400D0034))
#define EPI0_RPSTD1_R           (*((volatile unsigned long *)0x400D0038))
#define EPI0_STAT_R             (*((volatile unsigned long *)0x400D0060))
#define EPI0_RFIFOCNT_R         (*((volatile unsigned long *)0x400D006C))
#define EPI0_READFIFO_R         (*((volatile unsigned long *)0x400D0070))
#define EPI0_READFIFO1_R        (*((volatile unsigned long *)0x400D0074))
#define EPI0_READFIFO2_R        (*((volatile unsigned long *)0x400D0078))
#define EPI0_READFIFO3_R        (*((volatile unsigned long *)0x400D007C))
#define EPI0_READFIFO4_R        (*((volatile unsigned long *)0x400D0080))
#define EPI0_READFIFO5_R        (*((volatile unsigned long *)0x400D0084))
#define EPI0_READFIFO6_R        (*((volatile unsigned long *)0x400D0088))
#define EPI0_READFIFO7_R        (*((volatile unsigned long *)0x400D008C))
#define EPI0_FIFOLVL_R          (*((volatile unsigned long *)0x400D0200))
#define EPI0_WFIFOCNT_R         (*((volatile unsigned long *)0x400D0204))
#define EPI0_IM_R               (*((volatile unsigned long *)0x400D0210))
#define EPI0_RIS_R              (*((volatile unsigned long *)0x400D0214))
#define EPI0_MIS_R              (*((volatile unsigned long *)0x400D0218))
#define EPI0_EISC_R             (*((volatile unsigned long *)0x400D021C))

//*****************************************************************************
//
// Hibernation module registers (HIB)
//
//*****************************************************************************
#define HIB_RTCC_R              (*((volatile unsigned long *)0x400FC000))
#define HIB_RTCM0_R             (*((volatile unsigned long *)0x400FC004))
#define HIB_RTCM1_R             (*((volatile unsigned long *)0x400FC008))
#define HIB_RTCLD_R             (*((volatile unsigned long *)0x400FC00C))
#define HIB_CTL_R               (*((volatile unsigned long *)0x400FC010))
#define HIB_IM_R                (*((volatile unsigned long *)0x400FC014))
#define HIB_RIS_R               (*((volatile unsigned long *)0x400FC018))
#define HIB_MIS_R               (*((volatile unsigned long *)0x400FC01C))
#define HIB_IC_R                (*((volatile unsigned long *)0x400FC020))
#define HIB_RTCT_R              (*((volatile unsigned long *)0x400FC024))
#define HIB_DATA_R              (*((volatile unsigned long *)0x400FC030))

//*****************************************************************************
//
// FLASH registers (FLASH CTRL)
//
//*****************************************************************************
#define FLASH_FMA_R             (*((volatile unsigned long *)0x400FD000))
#define FLASH_FMD_R             (*((volatile unsigned long *)0x400FD004))
#define FLASH_FMC_R             (*((volatile unsigned long *)0x400FD008))
#define FLASH_FCRIS_R           (*((volatile unsigned long *)0x400FD00C))
#define FLASH_FCIM_R            (*((volatile unsigned long *)0x400FD010))
#define FLASH_FCMISC_R          (*((volatile unsigned long *)0x400FD014))
#define FLASH_FMC2_R            (*((volatile unsigned long *)0x400FD020))
#define FLASH_FWBVAL_R          (*((volatile unsigned long *)0x400FD030))
#define FLASH_FCTL_R            (*((volatile unsigned long *)0x400FD0F8))
#define FLASH_FWBN_R            (*((volatile unsigned long *)0x400FD100))
#define FLASH_RMCTL_R           (*((volatile unsigned long *)0x400FE0F0))
#define FLASH_RMVER_R           (*((volatile unsigned long *)0x400FE0F4))
#define FLASH_USERDBG_R         (*((volatile unsigned long *)0x400FE1D0))
#define FLASH_BOOTCFG_R         (*((volatile unsigned long *)0x400FE1D0))
#define FLASH_USERREG0_R        (*((volatile unsigned long *)0x400FE1E0))
#define FLASH_USERREG1_R        (*((volatile unsigned long *)0x400FE1E4))
#define FLASH_USERREG2_R        (*((volatile unsigned long *)0x400FE1E8))
#define FLASH_USERREG3_R        (*((volatile unsigned long *)0x400FE1EC))
#define FLASH_FMPRE0_R          (*((volatile unsigned long *)0x400FE200))
#define FLASH_FMPRE1_R          (*((volatile unsigned long *)0x400FE204))
#define FLASH_FMPRE2_R          (*((volatile unsigned long *)0x400FE208))
#define FLASH_FMPRE3_R          (*((volatile unsigned long *)0x400FE20C))
#define FLASH_FMPPE0_R          (*((volatile unsigned long *)0x400FE400))
#define FLASH_FMPPE1_R          (*((volatile unsigned long *)0x400FE404))
#define FLASH_FMPPE2_R          (*((volatile unsigned long *)0x400FE408))
#define FLASH_FMPPE3_R          (*((volatile unsigned long *)0x400FE40C))

//*****************************************************************************
//
// System Control registers (SYSCTL)
//
//*****************************************************************************
#define SYSCTL_DID0_R           (*((volatile unsigned long *)0x400FE000))
#define SYSCTL_DID1_R           (*((volatile unsigned long *)0x400FE004))
#define SYSCTL_DC0_R            (*((volatile unsigned long *)0x400FE008))
#define SYSCTL_DC1_R            (*((volatile unsigned long *)0x400FE010))
#define SYSCTL_DC2_R            (*((volatile unsigned long *)0x400FE014))
#define SYSCTL_DC3_R            (*((volatile unsigned long *)0x400FE018))
#define SYSCTL_DC4_R            (*((volatile unsigned long *)0x400FE01C))
#define SYSCTL_DC5_R            (*((volatile unsigned long *)0x400FE020))
#define SYSCTL_DC6_R            (*((volatile unsigned long *)0x400FE024))
#define SYSCTL_DC7_R            (*((volatile unsigned long *)0x400FE028))
#define SYSCTL_DC8_R            (*((volatile unsigned long *)0x400FE02C))
#define SYSCTL_PBORCTL_R        (*((volatile unsigned long *)0x400FE030))
#define SYSCTL_SRCR0_R          (*((volatile unsigned long *)0x400FE040))
#define SYSCTL_SRCR1_R          (*((volatile unsigned long *)0x400FE044))
#define SYSCTL_SRCR2_R          (*((volatile unsigned long *)0x400FE048))
#define SYSCTL_RIS_R            (*((volatile unsigned long *)0x400FE050))
#define SYSCTL_IMC_R            (*((volatile unsigned long *)0x400FE054))
#define SYSCTL_MISC_R           (*((volatile unsigned long *)0x400FE058))
#define SYSCTL_RESC_R           (*((volatile unsigned long *)0x400FE05C))
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))
#define SYSCTL_PLLCFG_R         (*((volatile unsigned long *)0x400FE064))
#define SYSCTL_GPIOHBCTL_R      (*((volatile unsigned long *)0x400FE06C))
#define SYSCTL_RCC2_R           (*((volatile unsigned long *)0x400FE070))
#define SYSCTL_MOSCCTL_R        (*((volatile unsigned long *)0x400FE07C))
#define SYSCTL_RCGC0_R          (*((volatile unsigned long *)0x400FE100))
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_SCGC0_R          (*((volatile unsigned long *)0x400FE110))
#define SYSCTL_SCGC1_R          (*((volatile unsigned long *)0x400FE114))
#define SYSCTL_SCGC2_R          (*((volatile unsigned long *)0x400FE118))
#define SYSCTL_DCGC0_R          (*((volatile unsigned long *)0x400FE120))
#define SYSCTL_DCGC1_R          (*((volatile unsigned long *)0x400FE124))
#define SYSCTL_DCGC2_R          (*((volatile unsigned long *)0x400FE128))
#define SYSCTL_DSLPCLKCFG_R     (*((volatile unsigned long *)0x400FE144))
#define SYSCTL_PIOSCCAL_R       (*((volatile unsigned long *)0x400FE150))
#define SYSCTL_PIOSCSTAT_R      (*((volatile unsigned long *)0x400FE154))
#define SYSCTL_DC9_R            (*((volatile unsigned long *)0x400FE190))
#define SYSCTL_NVMSTAT_R        (*((volatile unsigned long *)0x400FE1A0))

//*****************************************************************************
//
// Micro Direct Memory Access registers (UDMA)
//
//*****************************************************************************
#define UDMA_STAT_R             (*((volatile unsigned long *)0x400FF000))
#define UDMA_CFG_R              (*((volatile unsigned long *)0x400FF004))
#define UDMA_CTLBASE_R          (*((volatile unsigned long *)0x400FF008))
#define UDMA_ALTBASE_R          (*((volatile unsigned long *)0x400FF00C))
#define UDMA_WAITSTAT_R         (*((volatile unsigned long *)0x400FF010))
#define UDMA_SWREQ_R            (*((volatile unsigned long *)0x400FF014))
#define UDMA_USEBURSTSET_R      (*((volatile unsigned long *)0x400FF018))
#define UDMA_USEBURSTCLR_R      (*((volatile unsigned long *)0x400FF01C))
#define UDMA_REQMASKSET_R       (*((volatile unsigned long *)0x400FF020))
#define UDMA_REQMASKCLR_R       (*((volatile unsigned long *)0x400FF024))
#define UDMA_ENASET_R           (*((volatile unsigned long *)0x400FF028))
#define UDMA_ENACLR_R           (*((volatile unsigned long *)0x400FF02C))
#define UDMA_ALTSET_R           (*((volatile unsigned long *)0x400FF030))
#define UDMA_ALTCLR_R           (*((volatile unsigned long *)0x400FF034))
#define UDMA_PRIOSET_R          (*((volatile unsigned long *)0x400FF038))
#define UDMA_PRIOCLR_R          (*((volatile unsigned long *)0x400FF03C))
#define UDMA_ERRCLR_R           (*((volatile unsigned long *)0x400FF04C))
#define UDMA_CHASGN_R           (*((volatile unsigned long *)0x400FF500))

//*****************************************************************************
//
// Micro Direct Memory Access (uDMA) offsets (UDMA)
//
//*****************************************************************************
#define UDMA_SRCENDP            0x00000000  // DMA Channel Source Address End
                                            // Pointer
#define UDMA_DSTENDP            0x00000004  // DMA Channel Destination Address
                                            // End Pointer
#define UDMA_CHCTL              0x00000008  // DMA Channel Control Word

//*****************************************************************************
//
// NVIC registers (NVIC)
//
//*****************************************************************************
#define NVIC_INT_TYPE_R         (*((volatile unsigned long *)0xE000E004))
#define NVIC_ACTLR_R            (*((volatile unsigned long *)0xE000E008))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CAL_R           (*((volatile unsigned long *)0xE000E01C))
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))
#define NVIC_EN1_R              (*((volatile unsigned long *)0xE000E104))
#define NVIC_DIS0_R             (*((volatile unsigned long *)0xE000E180))
#define NVIC_DIS1_R             (*((volatile unsigned long *)0xE000E184))
#define NVIC_PEND0_R            (*((volatile unsigned long *)0xE000E200))
#define NVIC_PEND1_R            (*((volatile unsigned long *)0xE000E204))
#define NVIC_UNPEND0_R          (*((volatile unsigned long *)0xE000E280))
#define NVIC_UNPEND1_R          (*((volatile unsigned long *)0xE000E284))
#define NVIC_ACTIVE0_R          (*((volatile unsigned long *)0xE000E300))
#define NVIC_ACTIVE1_R          (*((volatile unsigned long *)0xE000E304))
#define NVIC_PRI0_R             (*((volatile unsigned long *)0xE000E400))
#define NVIC_PRI1_R             (*((volatile unsigned long *)0xE000E404))
#define NVIC_PRI2_R             (*((volatile unsigned long *)0xE000E408))
#define NVIC_PRI3_R             (*((volatile unsigned long *)0xE000E40C))
#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))
#define NVIC_PRI5_R             (*((volatile unsigned long *)0xE000E414))
#define NVIC_PRI6_R             (*((volatile unsigned long *)0xE000E418))
#define NVIC_PRI7_R             (*((volatile unsigned long *)0xE000E41C))
#define NVIC_PRI8_R             (*((volatile unsigned long *)0xE000E420))
#define NVIC_PRI9_R             (*((volatile unsigned long *)0xE000E424))
#define NVIC_PRI10_R            (*((volatile unsigned long *)0xE000E428))
#define NVIC_PRI11_R            (*((volatile unsigned long *)0xE000E42C))
#define NVIC_PRI12_R            (*((volatile unsigned long *)0xE000E430))
#define NVIC_PRI13_R            (*((volatile unsigned long *)0xE000E434))
#define NVIC_CPUID_R            (*((volatile unsigned long *)0xE000ED00))
#define NVIC_INT_CTRL_R         (*((volatile unsigned long *)0xE000ED04))
#define NVIC_VTABLE_R           (*((volatile unsigned long *)0xE000ED08))
#define NVIC_APINT_R            (*((volatile unsigned long *)0xE000ED0C))
#define NVIC_SYS_CTRL_R         (*((volatile unsigned long *)0xE000ED10))
#define NVIC_CFG_CTRL_R         (*((volatile unsigned long *)0xE000ED14))
#define NVIC_SYS_PRI1_R         (*((volatile unsigned long *)0xE000ED18))
#define NVIC_SYS_PRI2_R         (*((volatile unsigned long *)0xE000ED1C))
#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))
#define NVIC_SYS_HND_CTRL_R     (*((volatile unsigned long *)0xE000ED24))
#define NVIC_FAULT_STAT_R       (*((volatile unsigned long *)0xE000ED28))
#define NVIC_HFAULT_STAT_R      (*((volatile unsigned long *)0xE000ED2C))
#define NVIC_DEBUG_STAT_R       (*((volatile unsigned long *)0xE000ED30))
#define NVIC_MM_ADDR_R          (*((volatile unsigned long *)0xE000ED34))
#define NVIC_FAULT_ADDR_R       (*((volatile unsigned long *)0xE000ED38))
#define NVIC_MPU_TYPE_R         (*((volatile unsigned long *)0xE000ED90))
#define NVIC_MPU_CTRL_R         (*((volatile unsigned long *)0xE000ED94))
#define NVIC_MPU_NUMBER_R       (*((volatile unsigned long *)0xE000ED98))
#define NVIC_MPU_BASE_R         (*((volatile unsigned long *)0xE000ED9C))
#define NVIC_MPU_ATTR_R         (*((volatile unsigned long *)0xE000EDA0))
#define NVIC_MPU_BASE1_R        (*((volatile unsigned long *)0xE000EDA4))
#define NVIC_MPU_ATTR1_R        (*((volatile unsigned long *)0xE000EDA8))
#define NVIC_MPU_BASE2_R        (*((volatile unsigned long *)0xE000EDAC))
#define NVIC_MPU_ATTR2_R        (*((volatile unsigned long *)0xE000EDB0))
#define NVIC_MPU_BASE3_R        (*((volatile unsigned long *)0xE000EDB4))
#define NVIC_MPU_ATTR3_R        (*((volatile unsigned long *)0xE000EDB8))
#define NVIC_DBG_CTRL_R         (*((volatile unsigned long *)0xE000EDF0))
#define NVIC_DBG_XFER_R         (*((volatile unsigned long *)0xE000EDF4))
#define NVIC_DBG_DATA_R         (*((volatile unsigned long *)0xE000EDF8))
#define NVIC_DBG_INT_R          (*((volatile unsigned long *)0xE000EDFC))
#define NVIC_SW_TRIG_R          (*((volatile unsigned long *)0xE000EF00))

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_LOAD register.
//
//*****************************************************************************
#define WDT_LOAD_M              0xFFFFFFFF  // Watchdog Load Value
#define WDT_LOAD_S              0

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_VALUE register.
//
//*****************************************************************************
#define WDT_VALUE_M             0xFFFFFFFF  // Watchdog Value
#define WDT_VALUE_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_CTL register.
//
//*****************************************************************************
#define WDT_CTL_WRC             0x80000000  // Write Complete
#define WDT_CTL_RESEN           0x00000002  // Watchdog Reset Enable
#define WDT_CTL_INTEN           0x00000001  // Watchdog Interrupt Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_ICR register.
//
//*****************************************************************************
#define WDT_ICR_M               0xFFFFFFFF  // Watchdog Interrupt Clear
#define WDT_ICR_S               0

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_RIS register.
//
//*****************************************************************************
#define WDT_RIS_WDTRIS          0x00000001  // Watchdog Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_MIS register.
//
//*****************************************************************************
#define WDT_MIS_WDTMIS          0x00000001  // Watchdog Masked Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_TEST register.
//
//*****************************************************************************
#define WDT_TEST_STALL          0x00000100  // Watchdog Stall Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the WDT_O_LOCK register.
//
//*****************************************************************************
#define WDT_LOCK_M              0xFFFFFFFF  // Watchdog Lock
#define WDT_LOCK_UNLOCKED       0x00000000  // Unlocked
#define WDT_LOCK_LOCKED         0x00000001  // Locked

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_IM register.
//
//*****************************************************************************
#define GPIO_IM_GPIO_M          0x000000FF  // GPIO Interrupt Mask Enable
#define GPIO_IM_GPIO_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_RIS register.
//
//*****************************************************************************
#define GPIO_RIS_GPIO_M         0x000000FF  // GPIO Interrupt Raw Status
#define GPIO_RIS_GPIO_S         0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_MIS register.
//
//*****************************************************************************
#define GPIO_MIS_GPIO_M         0x000000FF  // GPIO Masked Interrupt Status
#define GPIO_MIS_GPIO_S         0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_ICR register.
//
//*****************************************************************************
#define GPIO_ICR_GPIO_M         0x000000FF  // GPIO Interrupt Clear
#define GPIO_ICR_GPIO_S         0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_O_LOCK register.
//
//*****************************************************************************
#define GPIO_LOCK_M             0xFFFFFFFF  // GPIO Lock
#define GPIO_LOCK_UNLOCKED      0x00000000  // The GPIOCR register is unlocked
                                            // and may be modified
#define GPIO_LOCK_LOCKED        0x00000001  // The GPIOCR register is locked
                                            // and may not be modified
#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port A.
//
//*****************************************************************************
#define GPIO_PCTL_PA7_M         0xF0000000  // PA7 mask
#define GPIO_PCTL_PA7_I2C1SDA   0x10000000  // I2C1SDA on PA7
#define GPIO_PCTL_PA7_CCP4      0x20000000  // CCP4 on PA7
#define GPIO_PCTL_PA7_CCP3      0x70000000  // CCP3 on PA7
#define GPIO_PCTL_PA7_U1DCD     0x90000000  // U1DCD on PA7
#define GPIO_PCTL_PA6_M         0x0F000000  // PA6 mask
#define GPIO_PCTL_PA6_I2C1SCL   0x01000000  // I2C1SCL on PA6
#define GPIO_PCTL_PA6_CCP1      0x02000000  // CCP1 on PA6
#define GPIO_PCTL_PA6_U1CTS     0x09000000  // U1CTS on PA6
#define GPIO_PCTL_PA5_M         0x00F00000  // PA5 mask
#define GPIO_PCTL_PA5_SSI0TX    0x00100000  // SSI0TX on PA5
#define GPIO_PCTL_PA4_M         0x000F0000  // PA4 mask
#define GPIO_PCTL_PA4_SSI0RX    0x00010000  // SSI0RX on PA4
#define GPIO_PCTL_PA3_M         0x0000F000  // PA3 mask
#define GPIO_PCTL_PA3_SSI0FSS   0x00001000  // SSI0FSS on PA3
#define GPIO_PCTL_PA2_M         0x00000F00  // PA2 mask
#define GPIO_PCTL_PA2_SSI0CLK   0x00000100  // SSI0CLK on PA2
#define GPIO_PCTL_PA1_M         0x000000F0  // PA1 mask
#define GPIO_PCTL_PA1_U0TX      0x00000010  // U0TX on PA1
#define GPIO_PCTL_PA1_I2C1SDA   0x00000080  // I2C1SDA on PA1
#define GPIO_PCTL_PA1_U1TX      0x00000090  // U1TX on PA1
#define GPIO_PCTL_PA0_M         0x0000000F  // PA0 mask
#define GPIO_PCTL_PA0_U0RX      0x00000001  // U0RX on PA0
#define GPIO_PCTL_PA0_I2C1SCL   0x00000008  // I2C1SCL on PA0
#define GPIO_PCTL_PA0_U1RX      0x00000009  // U1RX on PA0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port B.
//
//*****************************************************************************
#define GPIO_PCTL_PB7_M         0xF0000000  // PB7 mask
#define GPIO_PCTL_PB7_NMI       0x40000000  // NMI on PB7
#define GPIO_PCTL_PB6_M         0x0F000000  // PB6 mask
#define GPIO_PCTL_PB6_CCP1      0x01000000  // CCP1 on PB6
#define GPIO_PCTL_PB6_CCP7      0x02000000  // CCP7 on PB6
#define GPIO_PCTL_PB6_C0O       0x03000000  // C0O on PB6
#define GPIO_PCTL_PB6_CCP5      0x06000000  // CCP5 on PB6
#define GPIO_PCTL_PB5_M         0x00F00000  // PB5 mask
#define GPIO_PCTL_PB5_C0O       0x00100000  // C0O on PB5
#define GPIO_PCTL_PB5_CCP5      0x00200000  // CCP5 on PB5
#define GPIO_PCTL_PB5_CCP6      0x00300000  // CCP6 on PB5
#define GPIO_PCTL_PB5_CCP0      0x00400000  // CCP0 on PB5
#define GPIO_PCTL_PB5_CCP2      0x00600000  // CCP2 on PB5
#define GPIO_PCTL_PB5_U1TX      0x00700000  // U1TX on PB5
#define GPIO_PCTL_PB5_EPI0S22   0x00800000  // EPI0S22 on PB5
#define GPIO_PCTL_PB4_M         0x000F0000  // PB4 mask
#define GPIO_PCTL_PB4_U2RX      0x00040000  // U2RX on PB4
#define GPIO_PCTL_PB4_U1RX      0x00070000  // U1RX on PB4
#define GPIO_PCTL_PB4_EPI0S23   0x00080000  // EPI0S23 on PB4
#define GPIO_PCTL_PB3_M         0x0000F000  // PB3 mask
#define GPIO_PCTL_PB3_I2C0SDA   0x00001000  // I2C0SDA on PB3
#define GPIO_PCTL_PB2_M         0x00000F00  // PB2 mask
#define GPIO_PCTL_PB2_I2C0SCL   0x00000100  // I2C0SCL on PB2
#define GPIO_PCTL_PB2_CCP3      0x00000400  // CCP3 on PB2
#define GPIO_PCTL_PB2_CCP0      0x00000500  // CCP0 on PB2
#define GPIO_PCTL_PB1_M         0x000000F0  // PB1 mask
#define GPIO_PCTL_PB1_CCP2      0x00000010  // CCP2 on PB1
#define GPIO_PCTL_PB1_CCP1      0x00000040  // CCP1 on PB1
#define GPIO_PCTL_PB1_U1TX      0x00000050  // U1TX on PB1
#define GPIO_PCTL_PB0_M         0x0000000F  // PB0 mask
#define GPIO_PCTL_PB0_CCP0      0x00000001  // CCP0 on PB0
#define GPIO_PCTL_PB0_U1RX      0x00000005  // U1RX on PB0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port C.
//
//*****************************************************************************
#define GPIO_PCTL_PC7_M         0xF0000000  // PC7 mask
#define GPIO_PCTL_PC7_CCP4      0x10000000  // CCP4 on PC7
#define GPIO_PCTL_PC7_CCP0      0x40000000  // CCP0 on PC7
#define GPIO_PCTL_PC7_U1TX      0x50000000  // U1TX on PC7
#define GPIO_PCTL_PC7_C1O       0x70000000  // C1O on PC7
#define GPIO_PCTL_PC7_EPI0S5    0x80000000  // EPI0S5 on PC7
#define GPIO_PCTL_PC6_M         0x0F000000  // PC6 mask
#define GPIO_PCTL_PC6_CCP3      0x01000000  // CCP3 on PC6
#define GPIO_PCTL_PC6_U1RX      0x05000000  // U1RX on PC6
#define GPIO_PCTL_PC6_CCP0      0x06000000  // CCP0 on PC6
#define GPIO_PCTL_PC6_EPI0S4    0x08000000  // EPI0S4 on PC6
#define GPIO_PCTL_PC5_M         0x00F00000  // PC5 mask
#define GPIO_PCTL_PC5_CCP1      0x00100000  // CCP1 on PC5
#define GPIO_PCTL_PC5_C1O       0x00200000  // C1O on PC5
#define GPIO_PCTL_PC5_C0O       0x00300000  // C0O on PC5
#define GPIO_PCTL_PC5_CCP3      0x00500000  // CCP3 on PC5
#define GPIO_PCTL_PC5_EPI0S3    0x00800000  // EPI0S3 on PC5
#define GPIO_PCTL_PC4_M         0x000F0000  // PC4 mask
#define GPIO_PCTL_PC4_CCP5      0x00010000  // CCP5 on PC4
#define GPIO_PCTL_PC4_CCP2      0x00050000  // CCP2 on PC4
#define GPIO_PCTL_PC4_CCP4      0x00060000  // CCP4 on PC4
#define GPIO_PCTL_PC4_EPI0S2    0x00080000  // EPI0S2 on PC4
#define GPIO_PCTL_PC4_CCP1      0x00090000  // CCP1 on PC4
#define GPIO_PCTL_PC3_M         0x0000F000  // PC3 mask
#define GPIO_PCTL_PC3_TDO       0x00003000  // TDO on PC3
#define GPIO_PCTL_PC2_M         0x00000F00  // PC2 mask
#define GPIO_PCTL_PC2_TDI       0x00000300  // TDI on PC2
#define GPIO_PCTL_PC1_M         0x000000F0  // PC1 mask
#define GPIO_PCTL_PC1_TMS       0x00000030  // TMS on PC1
#define GPIO_PCTL_PC0_M         0x0000000F  // PC0 mask
#define GPIO_PCTL_PC0_TCK       0x00000003  // TCK on PC0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port D.
//
//*****************************************************************************
#define GPIO_PCTL_PD7_M         0xF0000000  // PD7 mask
#define GPIO_PCTL_PD7_C0O       0x20000000  // C0O on PD7
#define GPIO_PCTL_PD7_CCP1      0x30000000  // CCP1 on PD7
#define GPIO_PCTL_PD7_U1DTR     0x90000000  // U1DTR on PD7
#define GPIO_PCTL_PD7_EPI0S30   0xA0000000  // EPI0S30 on PD7
#define GPIO_PCTL_PD6_M         0x0F000000  // PD6 mask
#define GPIO_PCTL_PD6_U2TX      0x09000000  // U2TX on PD6
#define GPIO_PCTL_PD6_EPI0S29   0x0A000000  // EPI0S29 on PD6
#define GPIO_PCTL_PD5_M         0x00F00000  // PD5 mask
#define GPIO_PCTL_PD5_CCP2      0x00100000  // CCP2 on PD5
#define GPIO_PCTL_PD5_CCP4      0x00200000  // CCP4 on PD5
#define GPIO_PCTL_PD5_U2RX      0x00900000  // U2RX on PD5
#define GPIO_PCTL_PD5_EPI0S28   0x00A00000  // EPI0S28 on PD5
#define GPIO_PCTL_PD4_M         0x000F0000  // PD4 mask
#define GPIO_PCTL_PD4_CCP0      0x00010000  // CCP0 on PD4
#define GPIO_PCTL_PD4_CCP3      0x00020000  // CCP3 on PD4
#define GPIO_PCTL_PD4_U1RI      0x00090000  // U1RI on PD4
#define GPIO_PCTL_PD4_EPI0S19   0x000A0000  // EPI0S19 on PD4
#define GPIO_PCTL_PD3_M         0x0000F000  // PD3 mask
#define GPIO_PCTL_PD3_U1TX      0x00001000  // U1TX on PD3
#define GPIO_PCTL_PD3_CCP7      0x00002000  // CCP7 on PD3
#define GPIO_PCTL_PD3_CCP0      0x00004000  // CCP0 on PD3
#define GPIO_PCTL_PD3_EPI0S21   0x00008000  // EPI0S21 on PD3
#define GPIO_PCTL_PD2_M         0x00000F00  // PD2 mask
#define GPIO_PCTL_PD2_U1RX      0x00000100  // U1RX on PD2
#define GPIO_PCTL_PD2_CCP6      0x00000200  // CCP6 on PD2
#define GPIO_PCTL_PD2_CCP5      0x00000400  // CCP5 on PD2
#define GPIO_PCTL_PD2_EPI0S20   0x00000800  // EPI0S20 on PD2
#define GPIO_PCTL_PD1_M         0x000000F0  // PD1 mask
#define GPIO_PCTL_PD1_U2TX      0x00000040  // U2TX on PD1
#define GPIO_PCTL_PD1_U1TX      0x00000050  // U1TX on PD1
#define GPIO_PCTL_PD1_CCP7      0x00000060  // CCP7 on PD1
#define GPIO_PCTL_PD1_U1DCD     0x00000090  // U1DCD on PD1
#define GPIO_PCTL_PD1_CCP2      0x000000A0  // CCP2 on PD1
#define GPIO_PCTL_PD0_M         0x0000000F  // PD0 mask
#define GPIO_PCTL_PD0_U2RX      0x00000004  // U2RX on PD0
#define GPIO_PCTL_PD0_U1RX      0x00000005  // U1RX on PD0
#define GPIO_PCTL_PD0_CCP6      0x00000006  // CCP6 on PD0
#define GPIO_PCTL_PD0_U1CTS     0x00000009  // U1CTS on PD0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port E.
//
//*****************************************************************************
#define GPIO_PCTL_PE7_M         0xF0000000  // PE7 mask
#define GPIO_PCTL_PE7_U1DCD     0x90000000  // U1DCD on PE7
#define GPIO_PCTL_PE6_M         0x0F000000  // PE6 mask
#define GPIO_PCTL_PE6_C1O       0x02000000  // C1O on PE6
#define GPIO_PCTL_PE6_U1CTS     0x09000000  // U1CTS on PE6
#define GPIO_PCTL_PE5_M         0x00F00000  // PE5 mask
#define GPIO_PCTL_PE5_CCP5      0x00100000  // CCP5 on PE5
#define GPIO_PCTL_PE4_M         0x000F0000  // PE4 mask
#define GPIO_PCTL_PE4_CCP3      0x00010000  // CCP3 on PE4
#define GPIO_PCTL_PE4_U2TX      0x00050000  // U2TX on PE4
#define GPIO_PCTL_PE4_CCP2      0x00060000  // CCP2 on PE4
#define GPIO_PCTL_PE3_M         0x0000F000  // PE3 mask
#define GPIO_PCTL_PE3_CCP1      0x00001000  // CCP1 on PE3
#define GPIO_PCTL_PE3_SSI1TX    0x00002000  // SSI1TX on PE3
#define GPIO_PCTL_PE3_CCP7      0x00005000  // CCP7 on PE3
#define GPIO_PCTL_PE3_EPI0S25   0x00008000  // EPI0S25 on PE3
#define GPIO_PCTL_PE2_M         0x00000F00  // PE2 mask
#define GPIO_PCTL_PE2_CCP4      0x00000100  // CCP4 on PE2
#define GPIO_PCTL_PE2_SSI1RX    0x00000200  // SSI1RX on PE2
#define GPIO_PCTL_PE2_CCP2      0x00000500  // CCP2 on PE2
#define GPIO_PCTL_PE2_EPI0S24   0x00000800  // EPI0S24 on PE2
#define GPIO_PCTL_PE1_M         0x000000F0  // PE1 mask
#define GPIO_PCTL_PE1_SSI1FSS   0x00000020  // SSI1FSS on PE1
#define GPIO_PCTL_PE1_CCP2      0x00000040  // CCP2 on PE1
#define GPIO_PCTL_PE1_CCP6      0x00000050  // CCP6 on PE1
#define GPIO_PCTL_PE1_EPI0S9    0x00000080  // EPI0S9 on PE1
#define GPIO_PCTL_PE0_M         0x0000000F  // PE0 mask
#define GPIO_PCTL_PE0_SSI1CLK   0x00000002  // SSI1CLK on PE0
#define GPIO_PCTL_PE0_CCP3      0x00000003  // CCP3 on PE0
#define GPIO_PCTL_PE0_EPI0S8    0x00000008  // EPI0S8 on PE0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port F.
//
//*****************************************************************************
#define GPIO_PCTL_PF7_M         0xF0000000  // PF7 mask
#define GPIO_PCTL_PF7_CCP4      0x10000000  // CCP4 on PF7
#define GPIO_PCTL_PF7_EPI0S12   0x80000000  // EPI0S12 on PF7
#define GPIO_PCTL_PF6_M         0x0F000000  // PF6 mask
#define GPIO_PCTL_PF6_CCP1      0x01000000  // CCP1 on PF6
#define GPIO_PCTL_PF6_U1RTS     0x0A000000  // U1RTS on PF6
#define GPIO_PCTL_PF5_M         0x00F00000  // PF5 mask
#define GPIO_PCTL_PF5_CCP2      0x00100000  // CCP2 on PF5
#define GPIO_PCTL_PF5_C1O       0x00200000  // C1O on PF5
#define GPIO_PCTL_PF5_EPI0S15   0x00800000  // EPI0S15 on PF5
#define GPIO_PCTL_PF5_SSI1TX    0x00900000  // SSI1TX on PF5
#define GPIO_PCTL_PF4_M         0x000F0000  // PF4 mask
#define GPIO_PCTL_PF4_CCP0      0x00010000  // CCP0 on PF4
#define GPIO_PCTL_PF4_C0O       0x00020000  // C0O on PF4
#define GPIO_PCTL_PF4_EPI0S12   0x00080000  // EPI0S12 on PF4
#define GPIO_PCTL_PF4_SSI1RX    0x00090000  // SSI1RX on PF4
#define GPIO_PCTL_PF3_M         0x0000F000  // PF3 mask
#define GPIO_PCTL_PF3_SSI1FSS   0x00009000  // SSI1FSS on PF3
#define GPIO_PCTL_PF2_M         0x00000F00  // PF2 mask
#define GPIO_PCTL_PF2_SSI1CLK   0x00000900  // SSI1CLK on PF2
#define GPIO_PCTL_PF1_M         0x000000F0  // PF1 mask
#define GPIO_PCTL_PF1_U1RTS     0x00000090  // U1RTS on PF1
#define GPIO_PCTL_PF1_CCP3      0x000000A0  // CCP3 on PF1
#define GPIO_PCTL_PF0_M         0x0000000F  // PF0 mask
#define GPIO_PCTL_PF0_U1DSR     0x00000009  // U1DSR on PF0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port G.
//
//*****************************************************************************
#define GPIO_PCTL_PG7_M         0xF0000000  // PG7 mask
#define GPIO_PCTL_PG7_CCP5      0x80000000  // CCP5 on PG7
#define GPIO_PCTL_PG7_EPI0S31   0x90000000  // EPI0S31 on PG7
#define GPIO_PCTL_PG6_M         0x0F000000  // PG6 mask
#define GPIO_PCTL_PG6_U1RI      0x0A000000  // U1RI on PG6
#define GPIO_PCTL_PG5_M         0x00F00000  // PG5 mask
#define GPIO_PCTL_PG5_CCP5      0x00100000  // CCP5 on PG5
#define GPIO_PCTL_PG5_U1DTR     0x00A00000  // U1DTR on PG5
#define GPIO_PCTL_PG4_M         0x000F0000  // PG4 mask
#define GPIO_PCTL_PG4_CCP3      0x00010000  // CCP3 on PG4
#define GPIO_PCTL_PG4_EPI0S15   0x00080000  // EPI0S15 on PG4
#define GPIO_PCTL_PG4_U1RI      0x000A0000  // U1RI on PG4
#define GPIO_PCTL_PG1_M         0x000000F0  // PG1 mask
#define GPIO_PCTL_PG1_U2TX      0x00000010  // U2TX on PG1
#define GPIO_PCTL_PG1_I2C1SDA   0x00000030  // I2C1SDA on PG1
#define GPIO_PCTL_PG1_EPI0S14   0x00000080  // EPI0S14 on PG1
#define GPIO_PCTL_PG0_M         0x0000000F  // PG0 mask
#define GPIO_PCTL_PG0_U2RX      0x00000001  // U2RX on PG0
#define GPIO_PCTL_PG0_I2C1SCL   0x00000003  // I2C1SCL on PG0
#define GPIO_PCTL_PG0_EPI0S13   0x00000008  // EPI0S13 on PG0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port H.
//
//*****************************************************************************
#define GPIO_PCTL_PH7_M         0xF0000000  // PH7 mask
#define GPIO_PCTL_PH7_EPI0S27   0x80000000  // EPI0S27 on PH7
#define GPIO_PCTL_PH7_SSI1TX    0xB0000000  // SSI1TX on PH7
#define GPIO_PCTL_PH6_M         0x0F000000  // PH6 mask
#define GPIO_PCTL_PH6_EPI0S26   0x08000000  // EPI0S26 on PH6
#define GPIO_PCTL_PH6_SSI1RX    0x0B000000  // SSI1RX on PH6
#define GPIO_PCTL_PH5_M         0x00F00000  // PH5 mask
#define GPIO_PCTL_PH5_EPI0S11   0x00800000  // EPI0S11 on PH5
#define GPIO_PCTL_PH5_SSI1FSS   0x00B00000  // SSI1FSS on PH5
#define GPIO_PCTL_PH4_M         0x000F0000  // PH4 mask
#define GPIO_PCTL_PH4_EPI0S10   0x00080000  // EPI0S10 on PH4
#define GPIO_PCTL_PH4_SSI1CLK   0x000B0000  // SSI1CLK on PH4
#define GPIO_PCTL_PH3_M         0x0000F000  // PH3 mask
#define GPIO_PCTL_PH3_EPI0S0    0x00008000  // EPI0S0 on PH3
#define GPIO_PCTL_PH2_M         0x00000F00  // PH2 mask
#define GPIO_PCTL_PH2_C1O       0x00000200  // C1O on PH2
#define GPIO_PCTL_PH2_EPI0S1    0x00000800  // EPI0S1 on PH2
#define GPIO_PCTL_PH1_M         0x000000F0  // PH1 mask
#define GPIO_PCTL_PH1_CCP7      0x00000010  // CCP7 on PH1
#define GPIO_PCTL_PH1_EPI0S7    0x00000080  // EPI0S7 on PH1
#define GPIO_PCTL_PH0_M         0x0000000F  // PH0 mask
#define GPIO_PCTL_PH0_CCP6      0x00000001  // CCP6 on PH0
#define GPIO_PCTL_PH0_EPI0S6    0x00000008  // EPI0S6 on PH0

//*****************************************************************************
//
// The following are defines for the bit fields in the GPIO_PCTL register for
// port J.
//
//*****************************************************************************
#define GPIO_PCTL_PJ2_M         0x00000F00  // PJ2 mask
#define GPIO_PCTL_PJ2_EPI0S18   0x00000800  // EPI0S18 on PJ2
#define GPIO_PCTL_PJ2_CCP0      0x00000900  // CCP0 on PJ2
#define GPIO_PCTL_PJ1_M         0x000000F0  // PJ1 mask
#define GPIO_PCTL_PJ1_EPI0S17   0x00000080  // EPI0S17 on PJ1
#define GPIO_PCTL_PJ1_I2C1SDA   0x000000B0  // I2C1SDA on PJ1
#define GPIO_PCTL_PJ0_M         0x0000000F  // PJ0 mask
#define GPIO_PCTL_PJ0_EPI0S16   0x00000008  // EPI0S16 on PJ0
#define GPIO_PCTL_PJ0_I2C1SCL   0x0000000B  // I2C1SCL on PJ0

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_CR0 register.
//
//*****************************************************************************
#define SSI_CR0_SCR_M           0x0000FF00  // SSI Serial Clock Rate
#define SSI_CR0_SPH             0x00000080  // SSI Serial Clock Phase
#define SSI_CR0_SPO             0x00000040  // SSI Serial Clock Polarity
#define SSI_CR0_FRF_M           0x00000030  // SSI Frame Format Select
#define SSI_CR0_FRF_MOTO        0x00000000  // Freescale SPI Frame Format
#define SSI_CR0_FRF_TI          0x00000010  // Texas Instruments Synchronous
                                            // Serial Frame Format
#define SSI_CR0_FRF_NMW         0x00000020  // MICROWIRE Frame Format
#define SSI_CR0_DSS_M           0x0000000F  // SSI Data Size Select
#define SSI_CR0_DSS_4           0x00000003  // 4-bit data
#define SSI_CR0_DSS_5           0x00000004  // 5-bit data
#define SSI_CR0_DSS_6           0x00000005  // 6-bit data
#define SSI_CR0_DSS_7           0x00000006  // 7-bit data
#define SSI_CR0_DSS_8           0x00000007  // 8-bit data
#define SSI_CR0_DSS_9           0x00000008  // 9-bit data
#define SSI_CR0_DSS_10          0x00000009  // 10-bit data
#define SSI_CR0_DSS_11          0x0000000A  // 11-bit data
#define SSI_CR0_DSS_12          0x0000000B  // 12-bit data
#define SSI_CR0_DSS_13          0x0000000C  // 13-bit data
#define SSI_CR0_DSS_14          0x0000000D  // 14-bit data
#define SSI_CR0_DSS_15          0x0000000E  // 15-bit data
#define SSI_CR0_DSS_16          0x0000000F  // 16-bit data
#define SSI_CR0_SCR_S           8

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_CR1 register.
//
//*****************************************************************************
#define SSI_CR1_EOT             0x00000010  // End of Transmission
#define SSI_CR1_SOD             0x00000008  // SSI Slave Mode Output Disable
#define SSI_CR1_MS              0x00000004  // SSI Master/Slave Select
#define SSI_CR1_SSE             0x00000002  // SSI Synchronous Serial Port
                                            // Enable
#define SSI_CR1_LBM             0x00000001  // SSI Loopback Mode

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_DR register.
//
//*****************************************************************************
#define SSI_DR_DATA_M           0x0000FFFF  // SSI Receive/Transmit Data
#define SSI_DR_DATA_S           0

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_SR register.
//
//*****************************************************************************
#define SSI_SR_BSY              0x00000010  // SSI Busy Bit
#define SSI_SR_RFF              0x00000008  // SSI Receive FIFO Full
#define SSI_SR_RNE              0x00000004  // SSI Receive FIFO Not Empty
#define SSI_SR_TNF              0x00000002  // SSI Transmit FIFO Not Full
#define SSI_SR_TFE              0x00000001  // SSI Transmit FIFO Empty

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_CPSR register.
//
//*****************************************************************************
#define SSI_CPSR_CPSDVSR_M      0x000000FF  // SSI Clock Prescale Divisor
#define SSI_CPSR_CPSDVSR_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_IM register.
//
//*****************************************************************************
#define SSI_IM_TXIM             0x00000008  // SSI Transmit FIFO Interrupt Mask
#define SSI_IM_RXIM             0x00000004  // SSI Receive FIFO Interrupt Mask
#define SSI_IM_RTIM             0x00000002  // SSI Receive Time-Out Interrupt
                                            // Mask
#define SSI_IM_RORIM            0x00000001  // SSI Receive Overrun Interrupt
                                            // Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_RIS register.
//
//*****************************************************************************
#define SSI_RIS_TXRIS           0x00000008  // SSI Transmit FIFO Raw Interrupt
                                            // Status
#define SSI_RIS_RXRIS           0x00000004  // SSI Receive FIFO Raw Interrupt
                                            // Status
#define SSI_RIS_RTRIS           0x00000002  // SSI Receive Time-Out Raw
                                            // Interrupt Status
#define SSI_RIS_RORRIS          0x00000001  // SSI Receive Overrun Raw
                                            // Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_MIS register.
//
//*****************************************************************************
#define SSI_MIS_TXMIS           0x00000008  // SSI Transmit FIFO Masked
                                            // Interrupt Status
#define SSI_MIS_RXMIS           0x00000004  // SSI Receive FIFO Masked
                                            // Interrupt Status
#define SSI_MIS_RTMIS           0x00000002  // SSI Receive Time-Out Masked
                                            // Interrupt Status
#define SSI_MIS_RORMIS          0x00000001  // SSI Receive Overrun Masked
                                            // Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_ICR register.
//
//*****************************************************************************
#define SSI_ICR_RTIC            0x00000002  // SSI Receive Time-Out Interrupt
                                            // Clear
#define SSI_ICR_RORIC           0x00000001  // SSI Receive Overrun Interrupt
                                            // Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the SSI_O_DMACTL register.
//
//*****************************************************************************
#define SSI_DMACTL_TXDMAE       0x00000002  // Transmit DMA Enable
#define SSI_DMACTL_RXDMAE       0x00000001  // Receive DMA Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_DR register.
//
//*****************************************************************************
#define UART_DR_OE              0x00000800  // UART Overrun Error
#define UART_DR_BE              0x00000400  // UART Break Error
#define UART_DR_PE              0x00000200  // UART Parity Error
#define UART_DR_FE              0x00000100  // UART Framing Error
#define UART_DR_DATA_M          0x000000FF  // Data Transmitted or Received
#define UART_DR_DATA_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_RSR register.
//
//*****************************************************************************
#define UART_RSR_OE             0x00000008  // UART Overrun Error
#define UART_RSR_BE             0x00000004  // UART Break Error
#define UART_RSR_PE             0x00000002  // UART Parity Error
#define UART_RSR_FE             0x00000001  // UART Framing Error

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_ECR register.
//
//*****************************************************************************
#define UART_ECR_DATA_M         0x000000FF  // Error Clear
#define UART_ECR_DATA_S         0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_FR register.
//
//*****************************************************************************
#define UART_FR_RI              0x00000100  // Ring Indicator
#define UART_FR_TXFE            0x00000080  // UART Transmit FIFO Empty
#define UART_FR_RXFF            0x00000040  // UART Receive FIFO Full
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY            0x00000008  // UART Busy
#define UART_FR_DCD             0x00000004  // Data Carrier Detect
#define UART_FR_DSR             0x00000002  // Data Set Ready
#define UART_FR_CTS             0x00000001  // Clear To Send

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_ILPR register.
//
//*****************************************************************************
#define UART_ILPR_ILPDVSR_M     0x000000FF  // IrDA Low-Power Divisor
#define UART_ILPR_ILPDVSR_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_IBRD register.
//
//*****************************************************************************
#define UART_IBRD_DIVINT_M      0x0000FFFF  // Integer Baud-Rate Divisor
#define UART_IBRD_DIVINT_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_FBRD register.
//
//*****************************************************************************
#define UART_FBRD_DIVFRAC_M     0x0000003F  // Fractional Baud-Rate Divisor
#define UART_FBRD_DIVFRAC_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LCRH register.
//
//*****************************************************************************
#define UART_LCRH_SPS           0x00000080  // UART Stick Parity Select
#define UART_LCRH_WLEN_M        0x00000060  // UART Word Length
#define UART_LCRH_WLEN_5        0x00000000  // 5 bits (default)
#define UART_LCRH_WLEN_6        0x00000020  // 6 bits
#define UART_LCRH_WLEN_7        0x00000040  // 7 bits
#define UART_LCRH_WLEN_8        0x00000060  // 8 bits
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_LCRH_STP2          0x00000008  // UART Two Stop Bits Select
#define UART_LCRH_EPS           0x00000004  // UART Even Parity Select
#define UART_LCRH_PEN           0x00000002  // UART Parity Enable
#define UART_LCRH_BRK           0x00000001  // UART Send Break

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_CTL register.
//
//*****************************************************************************
#define UART_CTL_CTSEN          0x00008000  // Enable Clear To Send
#define UART_CTL_RTSEN          0x00004000  // Enable Request to Send
#define UART_CTL_RTS            0x00000800  // Request to Send
#define UART_CTL_DTR            0x00000400  // Data Terminal Ready
#define UART_CTL_RXE            0x00000200  // UART Receive Enable
#define UART_CTL_TXE            0x00000100  // UART Transmit Enable
#define UART_CTL_LBE            0x00000080  // UART Loop Back Enable
#define UART_CTL_LIN            0x00000040  // LIN Mode Enable
#define UART_CTL_HSE            0x00000020  // High-Speed Enable
#define UART_CTL_EOT            0x00000010  // End of Transmission
#define UART_CTL_SMART          0x00000008  // ISO 7816 Smart Card Support
#define UART_CTL_SIRLP          0x00000004  // UART SIR Low-Power Mode
#define UART_CTL_SIREN          0x00000002  // UART SIR Enable
#define UART_CTL_UARTEN         0x00000001  // UART Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_IFLS register.
//
//*****************************************************************************
#define UART_IFLS_RX_M          0x00000038  // UART Receive Interrupt FIFO
                                            // Level Select
#define UART_IFLS_RX1_8         0x00000000  // RX FIFO >= 1/8 full
#define UART_IFLS_RX2_8         0x00000008  // RX FIFO >= 1/4 full
#define UART_IFLS_RX4_8         0x00000010  // RX FIFO >= 1/2 full (default)
#define UART_IFLS_RX6_8         0x00000018  // RX FIFO >= 3/4 full
#define UART_IFLS_RX7_8         0x00000020  // RX FIFO >= 7/8 full
#define UART_IFLS_TX_M          0x00000007  // UART Transmit Interrupt FIFO
                                            // Level Select
#define UART_IFLS_TX1_8         0x00000000  // TX FIFO <= 1/8 full
#define UART_IFLS_TX2_8         0x00000001  // TX FIFO <= 1/4 full
#define UART_IFLS_TX4_8         0x00000002  // TX FIFO <= 1/2 full (default)
#define UART_IFLS_TX6_8         0x00000003  // TX FIFO <= 3/4 full
#define UART_IFLS_TX7_8         0x00000004  // TX FIFO <= 7/8 full

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_IM register.
//
//*****************************************************************************
#define UART_IM_LME5IM          0x00008000  // LIN Mode Edge 5 Interrupt Mask
#define UART_IM_LME1IM          0x00004000  // LIN Mode Edge 1 Interrupt Mask
#define UART_IM_LMSBIM          0x00002000  // LIN Mode Sync Break Interrupt
                                            // Mask
#define UART_IM_OEIM            0x00000400  // UART Overrun Error Interrupt
                                            // Mask
#define UART_IM_BEIM            0x00000200  // UART Break Error Interrupt Mask
#define UART_IM_PEIM            0x00000100  // UART Parity Error Interrupt Mask
#define UART_IM_FEIM            0x00000080  // UART Framing Error Interrupt
                                            // Mask
#define UART_IM_RTIM            0x00000040  // UART Receive Time-Out Interrupt
                                            // Mask
#define UART_IM_TXIM            0x00000020  // UART Transmit Interrupt Mask
#define UART_IM_RXIM            0x00000010  // UART Receive Interrupt Mask
#define UART_IM_DSRMIM          0x00000008  // UART Data Set Ready Modem
                                            // Interrupt Mask
#define UART_IM_DCDMIM          0x00000004  // UART Data Carrier Detect Modem
                                            // Interrupt Mask
#define UART_IM_CTSMIM          0x00000002  // UART Clear to Send Modem
                                            // Interrupt Mask
#define UART_IM_RIMIM           0x00000001  // UART Ring Indicator Modem
                                            // Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_RIS register.
//
//*****************************************************************************
#define UART_RIS_LME5RIS        0x00008000  // LIN Mode Edge 5 Raw Interrupt
                                            // Status
#define UART_RIS_LME1RIS        0x00004000  // LIN Mode Edge 1 Raw Interrupt
                                            // Status
#define UART_RIS_LMSBRIS        0x00002000  // LIN Mode Sync Break Raw
                                            // Interrupt Status
#define UART_RIS_OERIS          0x00000400  // UART Overrun Error Raw Interrupt
                                            // Status
#define UART_RIS_BERIS          0x00000200  // UART Break Error Raw Interrupt
                                            // Status
#define UART_RIS_PERIS          0x00000100  // UART Parity Error Raw Interrupt
                                            // Status
#define UART_RIS_FERIS          0x00000080  // UART Framing Error Raw Interrupt
                                            // Status
#define UART_RIS_RTRIS          0x00000040  // UART Receive Time-Out Raw
                                            // Interrupt Status
#define UART_RIS_TXRIS          0x00000020  // UART Transmit Raw Interrupt
                                            // Status
#define UART_RIS_RXRIS          0x00000010  // UART Receive Raw Interrupt
                                            // Status
#define UART_RIS_DSRRIS         0x00000008  // UART Data Set Ready Modem Raw
                                            // Interrupt Status
#define UART_RIS_DCDRIS         0x00000004  // UART Data Carrier Detect Modem
                                            // Raw Interrupt Status
#define UART_RIS_CTSRIS         0x00000002  // UART Clear to Send Modem Raw
                                            // Interrupt Status
#define UART_RIS_RIRIS          0x00000001  // UART Ring Indicator Modem Raw
                                            // Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_MIS register.
//
//*****************************************************************************
#define UART_MIS_LME5MIS        0x00008000  // LIN Mode Edge 5 Masked Interrupt
                                            // Status
#define UART_MIS_LME1MIS        0x00004000  // LIN Mode Edge 1 Masked Interrupt
                                            // Status
#define UART_MIS_LMSBMIS        0x00002000  // LIN Mode Sync Break Masked
                                            // Interrupt Status
#define UART_MIS_OEMIS          0x00000400  // UART Overrun Error Masked
                                            // Interrupt Status
#define UART_MIS_BEMIS          0x00000200  // UART Break Error Masked
                                            // Interrupt Status
#define UART_MIS_PEMIS          0x00000100  // UART Parity Error Masked
                                            // Interrupt Status
#define UART_MIS_FEMIS          0x00000080  // UART Framing Error Masked
                                            // Interrupt Status
#define UART_MIS_RTMIS          0x00000040  // UART Receive Time-Out Masked
                                            // Interrupt Status
#define UART_MIS_TXMIS          0x00000020  // UART Transmit Masked Interrupt
                                            // Status
#define UART_MIS_RXMIS          0x00000010  // UART Receive Masked Interrupt
                                            // Status
#define UART_MIS_DSRMIS         0x00000008  // UART Data Set Ready Modem Masked
                                            // Interrupt Status
#define UART_MIS_DCDMIS         0x00000004  // UART Data Carrier Detect Modem
                                            // Masked Interrupt Status
#define UART_MIS_CTSMIS         0x00000002  // UART Clear to Send Modem Masked
                                            // Interrupt Status
#define UART_MIS_RIMIS          0x00000001  // UART Ring Indicator Modem Masked
                                            // Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_ICR register.
//
//*****************************************************************************
#define UART_ICR_LME5IC         0x00008000  // LIN Mode Edge 5 Interrupt Clear
#define UART_ICR_LME5MIC        0x00008000  // LIN Mode Edge 5 Interrupt Clear
#define UART_ICR_LME1MIC        0x00004000  // LIN Mode Edge 1 Interrupt Clear
#define UART_ICR_LME1IC         0x00004000  // LIN Mode Edge 1 Interrupt Clear
#define UART_ICR_LMSBMIC        0x00002000  // LIN Mode Sync Break Interrupt
                                            // Clear
#define UART_ICR_LMSBIC         0x00002000  // LIN Mode Sync Break Interrupt
                                            // Clear
#define UART_ICR_OEIC           0x00000400  // Overrun Error Interrupt Clear
#define UART_ICR_BEIC           0x00000200  // Break Error Interrupt Clear
#define UART_ICR_PEIC           0x00000100  // Parity Error Interrupt Clear
#define UART_ICR_FEIC           0x00000080  // Framing Error Interrupt Clear
#define UART_ICR_RTIC           0x00000040  // Receive Time-Out Interrupt Clear
#define UART_ICR_TXIC           0x00000020  // Transmit Interrupt Clear
#define UART_ICR_RXIC           0x00000010  // Receive Interrupt Clear
#define UART_ICR_DSRMIC         0x00000008  // UART Data Set Ready Modem
                                            // Interrupt Clear
#define UART_ICR_DCDMIC         0x00000004  // UART Data Carrier Detect Modem
                                            // Interrupt Clear
#define UART_ICR_CTSMIC         0x00000002  // UART Clear to Send Modem
                                            // Interrupt Clear
#define UART_ICR_RIMIC          0x00000001  // UART Ring Indicator Modem
                                            // Interrupt Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_DMACTL register.
//
//*****************************************************************************
#define UART_DMACTL_DMAERR      0x00000004  // DMA on Error
#define UART_DMACTL_TXDMAE      0x00000002  // Transmit DMA Enable
#define UART_DMACTL_RXDMAE      0x00000001  // Receive DMA Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LCTL register.
//
//*****************************************************************************
#define UART_LCTL_BLEN_M        0x00000030  // Sync Break Length
#define UART_LCTL_BLEN_13T      0x00000000  // Sync break length is 13T bits
                                            // (default)
#define UART_LCTL_BLEN_14T      0x00000010  // Sync break length is 14T bits
#define UART_LCTL_BLEN_15T      0x00000020  // Sync break length is 15T bits
#define UART_LCTL_BLEN_16T      0x00000030  // Sync break length is 16T bits
#define UART_LCTL_MASTER        0x00000001  // LIN Master Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LSS register.
//
//*****************************************************************************
#define UART_LSS_TSS_M          0x0000FFFF  // Timer Snap Shot
#define UART_LSS_TSS_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the UART_O_LTIM register.
//
//*****************************************************************************
#define UART_LTIM_TIMER_M       0x0000FFFF  // Timer Value
#define UART_LTIM_TIMER_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MSA register.
//
//*****************************************************************************
#define I2C_MSA_SA_M            0x000000FE  // I2C Slave Address
#define I2C_MSA_RS              0x00000001  // Receive not send
#define I2C_MSA_SA_S            1

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SOAR register.
//
//*****************************************************************************
#define I2C_SOAR_OAR_M          0x0000007F  // I2C Slave Own Address
#define I2C_SOAR_OAR_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SCSR register.
//
//*****************************************************************************
#define I2C_SCSR_FBR            0x00000004  // First Byte Received
#define I2C_SCSR_TREQ           0x00000002  // Transmit Request
#define I2C_SCSR_DA             0x00000001  // Device Active
#define I2C_SCSR_RREQ           0x00000001  // Receive Request

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MCS register.
//
//*****************************************************************************
#define I2C_MCS_BUSBSY          0x00000040  // Bus Busy
#define I2C_MCS_IDLE            0x00000020  // I2C Idle
#define I2C_MCS_ARBLST          0x00000010  // Arbitration Lost
#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SDR register.
//
//*****************************************************************************
#define I2C_SDR_DATA_M          0x000000FF  // Data for Transfer
#define I2C_SDR_DATA_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MDR register.
//
//*****************************************************************************
#define I2C_MDR_DATA_M          0x000000FF  // Data Transferred
#define I2C_MDR_DATA_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MTPR register.
//
//*****************************************************************************
#define I2C_MTPR_TPR_M          0x0000007F  // SCL Clock Period
#define I2C_MTPR_TPR_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SIMR register.
//
//*****************************************************************************
#define I2C_SIMR_STOPIM         0x00000004  // Stop Condition Interrupt Mask
#define I2C_SIMR_STARTIM        0x00000002  // Start Condition Interrupt Mask
#define I2C_SIMR_DATAIM         0x00000001  // Data Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SRIS register.
//
//*****************************************************************************
#define I2C_SRIS_STOPRIS        0x00000004  // Stop Condition Raw Interrupt
                                            // Status
#define I2C_SRIS_STARTRIS       0x00000002  // Start Condition Raw Interrupt
                                            // Status
#define I2C_SRIS_DATARIS        0x00000001  // Data Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MIMR register.
//
//*****************************************************************************
#define I2C_MIMR_IM             0x00000001  // Master Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MRIS register.
//
//*****************************************************************************
#define I2C_MRIS_RIS            0x00000001  // Master Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SMIS register.
//
//*****************************************************************************
#define I2C_SMIS_STOPMIS        0x00000004  // Stop Condition Masked Interrupt
                                            // Status
#define I2C_SMIS_STARTMIS       0x00000002  // Start Condition Masked Interrupt
                                            // Status
#define I2C_SMIS_DATAMIS        0x00000001  // Data Masked Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_SICR register.
//
//*****************************************************************************
#define I2C_SICR_STOPIC         0x00000004  // Stop Condition Interrupt Clear
#define I2C_SICR_STARTIC        0x00000002  // Start Condition Interrupt Clear
#define I2C_SICR_DATAIC         0x00000001  // Data Interrupt Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MMIS register.
//
//*****************************************************************************
#define I2C_MMIS_MIS            0x00000001  // Masked Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MICR register.
//
//*****************************************************************************
#define I2C_MICR_IC             0x00000001  // Master Interrupt Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the I2C_O_MCR register.
//
//*****************************************************************************
#define I2C_MCR_SFE             0x00000020  // I2C Slave Function Enable
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable
#define I2C_MCR_LPBK            0x00000001  // I2C Loopback

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_CFG register.
//
//*****************************************************************************
#define TIMER_CFG_M             0x00000007  // GPTM Configuration
#define TIMER_CFG_32_BIT_TIMER  0x00000000  // 32-bit timer configuration
#define TIMER_CFG_32_BIT_RTC    0x00000001  // 32-bit real-time clock (RTC)
                                            // counter configuration
#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration. The
                                            // function is controlled by bits
                                            // 1:0 of GPTMTAMR and GPTMTBMR

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAMR register.
//
//*****************************************************************************
#define TIMER_TAMR_TASNAPS      0x00000080  // GPTM Timer A Snap-Shot Mode
#define TIMER_TAMR_TAWOT        0x00000040  // GPTM Timer A Wait-on-Trigger
#define TIMER_TAMR_TAMIE        0x00000020  // GPTM Timer A Match Interrupt
                                            // Enable
#define TIMER_TAMR_TACDIR       0x00000010  // GPTM Timer A Count Direction
#define TIMER_TAMR_TAAMS        0x00000008  // GPTM Timer A Alternate Mode
                                            // Select
#define TIMER_TAMR_TACMR        0x00000004  // GPTM Timer A Capture Mode
#define TIMER_TAMR_TAMR_M       0x00000003  // GPTM Timer A Mode
#define TIMER_TAMR_TAMR_1_SHOT  0x00000001  // One-Shot Timer mode
#define TIMER_TAMR_TAMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_TAMR_TAMR_CAP     0x00000003  // Capture mode

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBMR register.
//
//*****************************************************************************
#define TIMER_TBMR_TBSNAPS      0x00000080  // GPTM Timer B Snap-Shot Mode
#define TIMER_TBMR_TBWOT        0x00000040  // GPTM Timer B Wait-on-Trigger
#define TIMER_TBMR_TBMIE        0x00000020  // GPTM Timer B Match Interrupt
                                            // Enable
#define TIMER_TBMR_TBCDIR       0x00000010  // GPTM Timer B Count Direction
#define TIMER_TBMR_TBAMS        0x00000008  // GPTM Timer B Alternate Mode
                                            // Select
#define TIMER_TBMR_TBCMR        0x00000004  // GPTM Timer B Capture Mode
#define TIMER_TBMR_TBMR_M       0x00000003  // GPTM Timer B Mode
#define TIMER_TBMR_TBMR_1_SHOT  0x00000001  // One-Shot Timer mode
#define TIMER_TBMR_TBMR_PERIOD  0x00000002  // Periodic Timer mode
#define TIMER_TBMR_TBMR_CAP     0x00000003  // Capture mode

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_CTL register.
//
//*****************************************************************************
#define TIMER_CTL_TBPWML        0x00004000  // GPTM Timer B PWM Output Level
#define TIMER_CTL_TBOTE         0x00002000  // GPTM Timer B Output Trigger
                                            // Enable
#define TIMER_CTL_TBEVENT_M     0x00000C00  // GPTM Timer B Event Mode
#define TIMER_CTL_TBEVENT_POS   0x00000000  // Positive edge
#define TIMER_CTL_TBEVENT_NEG   0x00000400  // Negative edge
#define TIMER_CTL_TBEVENT_BOTH  0x00000C00  // Both edges
#define TIMER_CTL_TBSTALL       0x00000200  // GPTM Timer B Stall Enable
#define TIMER_CTL_TBEN          0x00000100  // GPTM Timer B Enable
#define TIMER_CTL_TAPWML        0x00000040  // GPTM Timer A PWM Output Level
#define TIMER_CTL_TAOTE         0x00000020  // GPTM Timer A Output Trigger
                                            // Enable
#define TIMER_CTL_RTCEN         0x00000010  // GPTM RTC Stall Enable
#define TIMER_CTL_TAEVENT_M     0x0000000C  // GPTM Timer A Event Mode
#define TIMER_CTL_TAEVENT_POS   0x00000000  // Positive edge
#define TIMER_CTL_TAEVENT_NEG   0x00000004  // Negative edge
#define TIMER_CTL_TAEVENT_BOTH  0x0000000C  // Both edges
#define TIMER_CTL_TASTALL       0x00000002  // GPTM Timer A Stall Enable
#define TIMER_CTL_TAEN          0x00000001  // GPTM Timer A Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_IMR register.
//
//*****************************************************************************
#define TIMER_IMR_TBMIM         0x00000800  // GPTM Timer B Match Interrupt
                                            // Mask
#define TIMER_IMR_CBEIM         0x00000400  // GPTM Timer B Capture Mode Event
                                            // Interrupt Mask
#define TIMER_IMR_CBMIM         0x00000200  // GPTM Timer B Capture Mode Match
                                            // Interrupt Mask
#define TIMER_IMR_TBTOIM        0x00000100  // GPTM Timer B Time-Out Interrupt
                                            // Mask
#define TIMER_IMR_TAMIM         0x00000010  // GPTM Timer A Match Interrupt
                                            // Mask
#define TIMER_IMR_RTCIM         0x00000008  // GPTM RTC Interrupt Mask
#define TIMER_IMR_CAEIM         0x00000004  // GPTM Timer A Capture Mode Event
                                            // Interrupt Mask
#define TIMER_IMR_CAMIM         0x00000002  // GPTM Timer A Capture Mode Match
                                            // Interrupt Mask
#define TIMER_IMR_TATOIM        0x00000001  // GPTM Timer A Time-Out Interrupt
                                            // Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_RIS register.
//
//*****************************************************************************
#define TIMER_RIS_TBMRIS        0x00000800  // GPTM Timer B Match Raw Interrupt
#define TIMER_RIS_CBERIS        0x00000400  // GPTM Timer B Capture Mode Event
                                            // Raw Interrupt
#define TIMER_RIS_CBMRIS        0x00000200  // GPTM Timer B Capture Mode Match
                                            // Raw Interrupt
#define TIMER_RIS_TBTORIS       0x00000100  // GPTM Timer B Time-Out Raw
                                            // Interrupt
#define TIMER_RIS_TAMRIS        0x00000010  // GPTM Timer A Match Raw Interrupt
#define TIMER_RIS_RTCRIS        0x00000008  // GPTM RTC Raw Interrupt
#define TIMER_RIS_CAERIS        0x00000004  // GPTM Timer A Capture Mode Event
                                            // Raw Interrupt
#define TIMER_RIS_CAMRIS        0x00000002  // GPTM Timer A Capture Mode Match
                                            // Raw Interrupt
#define TIMER_RIS_TATORIS       0x00000001  // GPTM Timer A Time-Out Raw
                                            // Interrupt

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_MIS register.
//
//*****************************************************************************
#define TIMER_MIS_TBMMIS        0x00000800  // GPTM Timer B Match Masked
                                            // Interrupt
#define TIMER_MIS_CBEMIS        0x00000400  // GPTM Timer B Capture Mode Event
                                            // Masked Interrupt
#define TIMER_MIS_CBMMIS        0x00000200  // GPTM Timer B Capture Mode Match
                                            // Masked Interrupt
#define TIMER_MIS_TBTOMIS       0x00000100  // GPTM Timer B Time-Out Masked
                                            // Interrupt
#define TIMER_MIS_TAMMIS        0x00000010  // GPTM Timer A Match Masked
                                            // Interrupt
#define TIMER_MIS_RTCMIS        0x00000008  // GPTM RTC Masked Interrupt
#define TIMER_MIS_CAEMIS        0x00000004  // GPTM Timer A Capture Mode Event
                                            // Masked Interrupt
#define TIMER_MIS_CAMMIS        0x00000002  // GPTM Timer A Capture Mode Match
                                            // Masked Interrupt
#define TIMER_MIS_TATOMIS       0x00000001  // GPTM Timer A Time-Out Masked
                                            // Interrupt

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_ICR register.
//
//*****************************************************************************
#define TIMER_ICR_TBMCINT       0x00000800  // GPTM Timer B Match Interrupt
                                            // Clear
#define TIMER_ICR_CBECINT       0x00000400  // GPTM Timer B Capture Mode Event
                                            // Interrupt Clear
#define TIMER_ICR_CBMCINT       0x00000200  // GPTM Timer B Capture Mode Match
                                            // Interrupt Clear
#define TIMER_ICR_TBTOCINT      0x00000100  // GPTM Timer B Time-Out Interrupt
                                            // Clear
#define TIMER_ICR_TAMCINT       0x00000010  // GPTM Timer A Match Interrupt
                                            // Clear
#define TIMER_ICR_RTCCINT       0x00000008  // GPTM RTC Interrupt Clear
#define TIMER_ICR_CAECINT       0x00000004  // GPTM Timer A Capture Mode Event
                                            // Interrupt Clear
#define TIMER_ICR_CAMCINT       0x00000002  // GPTM Timer A Capture Mode Match
                                            // Interrupt Clear
#define TIMER_ICR_TATOCINT      0x00000001  // GPTM Timer A Time-Out Raw
                                            // Interrupt

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAILR register.
//
//*****************************************************************************
#define TIMER_TAILR_M           0xFFFFFFFF  // GPTM Timer A Interval Load
                                            // Register
#define TIMER_TAILR_TAILRH_M    0xFFFF0000  // GPTM Timer A Interval Load
                                            // Register High
#define TIMER_TAILR_TAILRL_M    0x0000FFFF  // GPTM Timer A Interval Load
                                            // Register Low
#define TIMER_TAILR_TAILRH_S    16
#define TIMER_TAILR_TAILRL_S    0
#define TIMER_TAILR_S           0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBILR register.
//
//*****************************************************************************
#define TIMER_TBILR_M           0xFFFFFFFF  // GPTM Timer B Interval Load
                                            // Register
#define TIMER_TBILR_TBILRL_M    0x0000FFFF  // GPTM Timer B Interval Load
                                            // Register
#define TIMER_TBILR_TBILRL_S    0
#define TIMER_TBILR_S           0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAMATCHR
// register.
//
//*****************************************************************************
#define TIMER_TAMATCHR_TAMR_M   0xFFFFFFFF  // GPTM Timer A Match Register
#define TIMER_TAMATCHR_TAMRH_M  0xFFFF0000  // GPTM Timer A Match Register High
#define TIMER_TAMATCHR_TAMRL_M  0x0000FFFF  // GPTM Timer A Match Register Low
#define TIMER_TAMATCHR_TAMRH_S  16
#define TIMER_TAMATCHR_TAMRL_S  0
#define TIMER_TAMATCHR_TAMR_S   0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBMATCHR
// register.
//
//*****************************************************************************
#define TIMER_TBMATCHR_TBMR_M   0xFFFFFFFF  // GPTM Timer B Match Register
#define TIMER_TBMATCHR_TBMRL_M  0x0000FFFF  // GPTM Timer B Match Register Low
#define TIMER_TBMATCHR_TBMR_S   0
#define TIMER_TBMATCHR_TBMRL_S  0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAPR register.
//
//*****************************************************************************
#define TIMER_TAPR_TAPSR_M      0x000000FF  // GPTM Timer A Prescale
#define TIMER_TAPR_TAPSR_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBPR register.
//
//*****************************************************************************
#define TIMER_TBPR_TBPSR_M      0x000000FF  // GPTM Timer B Prescale
#define TIMER_TBPR_TBPSR_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAPMR register.
//
//*****************************************************************************
#define TIMER_TAPMR_TAPSMR_M    0x000000FF  // GPTM TimerA Prescale Match
#define TIMER_TAPMR_TAPSMR_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBPMR register.
//
//*****************************************************************************
#define TIMER_TBPMR_TBPSMR_M    0x000000FF  // GPTM TimerB Prescale Match
#define TIMER_TBPMR_TBPSMR_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAR register.
//
//*****************************************************************************
#define TIMER_TAR_M             0xFFFFFFFF  // GPTM Timer A Register
#define TIMER_TAR_TARH_M        0xFFFF0000  // GPTM Timer A Register High
#define TIMER_TAR_TARL_M        0x0000FFFF  // GPTM Timer A Register Low
#define TIMER_TAR_TARH_S        16
#define TIMER_TAR_TARL_S        0
#define TIMER_TAR_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBR register.
//
//*****************************************************************************
#define TIMER_TBR_M             0xFFFFFFFF  // GPTM Timer B Register
#define TIMER_TBR_TBRL_M        0x00FFFFFF  // GPTM Timer B
#define TIMER_TBR_TBRL_S        0
#define TIMER_TBR_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TAV register.
//
//*****************************************************************************
#define TIMER_TAV_M             0xFFFFFFFF  // GPTM Timer A Value
#define TIMER_TAV_TAVH_M        0xFFFF0000  // GPTM Timer A Value High
#define TIMER_TAV_TAVL_M        0x0000FFFF  // GPTM Timer A Register Low
#define TIMER_TAV_TAVH_S        16
#define TIMER_TAV_TAVL_S        0
#define TIMER_TAV_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the TIMER_O_TBV register.
//
//*****************************************************************************
#define TIMER_TBV_M             0xFFFFFFFF  // GPTM Timer B Value
#define TIMER_TBV_TBVL_M        0x0000FFFF  // GPTM Timer B Register
#define TIMER_TBV_TBVL_S        0
#define TIMER_TBV_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_ACTSS register.
//
//*****************************************************************************
#define ADC_ACTSS_ASEN3         0x00000008  // ADC SS3 Enable
#define ADC_ACTSS_ASEN2         0x00000004  // ADC SS2 Enable
#define ADC_ACTSS_ASEN1         0x00000002  // ADC SS1 Enable
#define ADC_ACTSS_ASEN0         0x00000001  // ADC SS0 Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_RIS register.
//
//*****************************************************************************
#define ADC_RIS_INRDC           0x00010000  // Digital Comparator Raw Interrupt
                                            // Status
#define ADC_RIS_INR3            0x00000008  // SS3 Raw Interrupt Status
#define ADC_RIS_INR2            0x00000004  // SS2 Raw Interrupt Status
#define ADC_RIS_INR1            0x00000002  // SS1 Raw Interrupt Status
#define ADC_RIS_INR0            0x00000001  // SS0 Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_IM register.
//
//*****************************************************************************
#define ADC_IM_DCONSS3          0x00080000  // Digital Comparator Interrupt on
                                            // SS3
#define ADC_IM_DCONSS2          0x00040000  // Digital Comparator Interrupt on
                                            // SS2
#define ADC_IM_DCONSS1          0x00020000  // Digital Comparator Interrupt on
                                            // SS1
#define ADC_IM_DCONSS0          0x00010000  // Digital Comparator Interrupt on
                                            // SS0
#define ADC_IM_MASK3            0x00000008  // SS3 Interrupt Mask
#define ADC_IM_MASK2            0x00000004  // SS2 Interrupt Mask
#define ADC_IM_MASK1            0x00000002  // SS1 Interrupt Mask
#define ADC_IM_MASK0            0x00000001  // SS0 Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_ISC register.
//
//*****************************************************************************
#define ADC_ISC_DCINSS3         0x00080000  // Digital Comparator Interrupt
                                            // Status on SS3
#define ADC_ISC_DCINSS2         0x00040000  // Digital Comparator Interrupt
                                            // Status on SS2
#define ADC_ISC_DCINSS1         0x00020000  // Digital Comparator Interrupt
                                            // Status on SS1
#define ADC_ISC_DCINSS0         0x00010000  // Digital Comparator Interrupt
                                            // Status on SS0
#define ADC_ISC_IN3             0x00000008  // SS3 Interrupt Status and Clear
#define ADC_ISC_IN2             0x00000004  // SS2 Interrupt Status and Clear
#define ADC_ISC_IN1             0x00000002  // SS1 Interrupt Status and Clear
#define ADC_ISC_IN0             0x00000001  // SS0 Interrupt Status and Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_OSTAT register.
//
//*****************************************************************************
#define ADC_OSTAT_OV3           0x00000008  // SS3 FIFO Overflow
#define ADC_OSTAT_OV2           0x00000004  // SS2 FIFO Overflow
#define ADC_OSTAT_OV1           0x00000002  // SS1 FIFO Overflow
#define ADC_OSTAT_OV0           0x00000001  // SS0 FIFO Overflow

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_EMUX register.
//
//*****************************************************************************
#define ADC_EMUX_EM3_M          0x0000F000  // SS3 Trigger Select
#define ADC_EMUX_EM3_PROCESSOR  0x00000000  // Processor (default)
#define ADC_EMUX_EM3_COMP0      0x00001000  // Analog Comparator 0
#define ADC_EMUX_EM3_COMP1      0x00002000  // Analog Comparator 1
#define ADC_EMUX_EM3_EXTERNAL   0x00004000  // External (GPIO PB4)
#define ADC_EMUX_EM3_TIMER      0x00005000  // Timer
#define ADC_EMUX_EM3_ALWAYS     0x0000F000  // Always (continuously sample)
#define ADC_EMUX_EM2_M          0x00000F00  // SS2 Trigger Select
#define ADC_EMUX_EM2_PROCESSOR  0x00000000  // Processor (default)
#define ADC_EMUX_EM2_COMP0      0x00000100  // Analog Comparator 0
#define ADC_EMUX_EM2_COMP1      0x00000200  // Analog Comparator 1
#define ADC_EMUX_EM2_EXTERNAL   0x00000400  // External (GPIO PB4)
#define ADC_EMUX_EM2_TIMER      0x00000500  // Timer
#define ADC_EMUX_EM2_ALWAYS     0x00000F00  // Always (continuously sample)
#define ADC_EMUX_EM1_M          0x000000F0  // SS1 Trigger Select
#define ADC_EMUX_EM1_PROCESSOR  0x00000000  // Processor (default)
#define ADC_EMUX_EM1_COMP0      0x00000010  // Analog Comparator 0
#define ADC_EMUX_EM1_COMP1      0x00000020  // Analog Comparator 1
#define ADC_EMUX_EM1_EXTERNAL   0x00000040  // External (GPIO PB4)
#define ADC_EMUX_EM1_TIMER      0x00000050  // Timer
#define ADC_EMUX_EM1_ALWAYS     0x000000F0  // Always (continuously sample)
#define ADC_EMUX_EM0_M          0x0000000F  // SS0 Trigger Select
#define ADC_EMUX_EM0_PROCESSOR  0x00000000  // Processor (default)
#define ADC_EMUX_EM0_COMP0      0x00000001  // Analog Comparator 0
#define ADC_EMUX_EM0_COMP1      0x00000002  // Analog Comparator 1
#define ADC_EMUX_EM0_EXTERNAL   0x00000004  // External (GPIO PB4)
#define ADC_EMUX_EM0_TIMER      0x00000005  // Timer
#define ADC_EMUX_EM0_ALWAYS     0x0000000F  // Always (continuously sample)

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_USTAT register.
//
//*****************************************************************************
#define ADC_USTAT_UV3           0x00000008  // SS3 FIFO Underflow
#define ADC_USTAT_UV2           0x00000004  // SS2 FIFO Underflow
#define ADC_USTAT_UV1           0x00000002  // SS1 FIFO Underflow
#define ADC_USTAT_UV0           0x00000001  // SS0 FIFO Underflow

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSPRI register.
//
//*****************************************************************************
#define ADC_SSPRI_SS3_M         0x00003000  // SS3 Priority
#define ADC_SSPRI_SS3_1ST       0x00000000  // First priority
#define ADC_SSPRI_SS3_2ND       0x00001000  // Second priority
#define ADC_SSPRI_SS3_3RD       0x00002000  // Third priority
#define ADC_SSPRI_SS3_4TH       0x00003000  // Fourth priority
#define ADC_SSPRI_SS2_M         0x00000300  // SS2 Priority
#define ADC_SSPRI_SS2_1ST       0x00000000  // First priority
#define ADC_SSPRI_SS2_2ND       0x00000100  // Second priority
#define ADC_SSPRI_SS2_3RD       0x00000200  // Third priority
#define ADC_SSPRI_SS2_4TH       0x00000300  // Fourth priority
#define ADC_SSPRI_SS1_M         0x00000030  // SS1 Priority
#define ADC_SSPRI_SS1_1ST       0x00000000  // First priority
#define ADC_SSPRI_SS1_2ND       0x00000010  // Second priority
#define ADC_SSPRI_SS1_3RD       0x00000020  // Third priority
#define ADC_SSPRI_SS1_4TH       0x00000030  // Fourth priority
#define ADC_SSPRI_SS0_M         0x00000003  // SS0 Priority
#define ADC_SSPRI_SS0_1ST       0x00000000  // First priority
#define ADC_SSPRI_SS0_2ND       0x00000001  // Second priority
#define ADC_SSPRI_SS0_3RD       0x00000002  // Third priority
#define ADC_SSPRI_SS0_4TH       0x00000003  // Fourth priority

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SPC register.
//
//*****************************************************************************
#define ADC_SPC_PHASE_M         0x0000000F  // Phase Difference
#define ADC_SPC_PHASE_0         0x00000000  // ADC sample lags by 0.0
#define ADC_SPC_PHASE_22_5      0x00000001  // ADC sample lags by 22.5
#define ADC_SPC_PHASE_45        0x00000002  // ADC sample lags by 45.0
#define ADC_SPC_PHASE_67_5      0x00000003  // ADC sample lags by 67.5
#define ADC_SPC_PHASE_90        0x00000004  // ADC sample lags by 90.0
#define ADC_SPC_PHASE_112_5     0x00000005  // ADC sample lags by 112.5
#define ADC_SPC_PHASE_135       0x00000006  // ADC sample lags by 135.0
#define ADC_SPC_PHASE_157_5     0x00000007  // ADC sample lags by 157.5
#define ADC_SPC_PHASE_180       0x00000008  // ADC sample lags by 180.0
#define ADC_SPC_PHASE_202_5     0x00000009  // ADC sample lags by 202.5
#define ADC_SPC_PHASE_225       0x0000000A  // ADC sample lags by 225.0
#define ADC_SPC_PHASE_247_5     0x0000000B  // ADC sample lags by 247.5
#define ADC_SPC_PHASE_270       0x0000000C  // ADC sample lags by 270.0
#define ADC_SPC_PHASE_292_5     0x0000000D  // ADC sample lags by 292.5
#define ADC_SPC_PHASE_315       0x0000000E  // ADC sample lags by 315.0
#define ADC_SPC_PHASE_337_5     0x0000000F  // ADC sample lags by 337.5

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_PSSI register.
//
//*****************************************************************************
#define ADC_PSSI_SS3            0x00000008  // SS3 Initiate
#define ADC_PSSI_SS2            0x00000004  // SS2 Initiate
#define ADC_PSSI_SS1            0x00000002  // SS1 Initiate
#define ADC_PSSI_SS0            0x00000001  // SS0 Initiate

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SAC register.
//
//*****************************************************************************
#define ADC_SAC_AVG_M           0x00000007  // Hardware Averaging Control
#define ADC_SAC_AVG_OFF         0x00000000  // No hardware oversampling
#define ADC_SAC_AVG_2X          0x00000001  // 2x hardware oversampling
#define ADC_SAC_AVG_4X          0x00000002  // 4x hardware oversampling
#define ADC_SAC_AVG_8X          0x00000003  // 8x hardware oversampling
#define ADC_SAC_AVG_16X         0x00000004  // 16x hardware oversampling
#define ADC_SAC_AVG_32X         0x00000005  // 32x hardware oversampling
#define ADC_SAC_AVG_64X         0x00000006  // 64x hardware oversampling

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCISC register.
//
//*****************************************************************************
#define ADC_DCISC_DCINT7        0x00000080  // Digital Comparator 7 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT6        0x00000040  // Digital Comparator 6 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT5        0x00000020  // Digital Comparator 5 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT4        0x00000010  // Digital Comparator 4 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT3        0x00000008  // Digital Comparator 3 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT2        0x00000004  // Digital Comparator 2 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT1        0x00000002  // Digital Comparator 1 Interrupt
                                            // Status and Clear
#define ADC_DCISC_DCINT0        0x00000001  // Digital Comparator 0 Interrupt
                                            // Status and Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_CTL register.
//
//*****************************************************************************
#define ADC_CTL_VREF            0x00000001  // Voltage Reference Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSMUX0 register.
//
//*****************************************************************************
#define ADC_SSMUX0_MUX7_M       0x70000000  // 8th Sample Input Select
#define ADC_SSMUX0_MUX6_M       0x07000000  // 7th Sample Input Select
#define ADC_SSMUX0_MUX5_M       0x00700000  // 6th Sample Input Select
#define ADC_SSMUX0_MUX4_M       0x00070000  // 5th Sample Input Select
#define ADC_SSMUX0_MUX3_M       0x00007000  // 4th Sample Input Select
#define ADC_SSMUX0_MUX2_M       0x00000700  // 3rd Sample Input Select
#define ADC_SSMUX0_MUX1_M       0x00000070  // 2nd Sample Input Select
#define ADC_SSMUX0_MUX0_M       0x00000007  // 1st Sample Input Select
#define ADC_SSMUX0_MUX7_S       28
#define ADC_SSMUX0_MUX6_S       24
#define ADC_SSMUX0_MUX5_S       20
#define ADC_SSMUX0_MUX4_S       16
#define ADC_SSMUX0_MUX3_S       12
#define ADC_SSMUX0_MUX2_S       8
#define ADC_SSMUX0_MUX1_S       4
#define ADC_SSMUX0_MUX0_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSCTL0 register.
//
//*****************************************************************************
#define ADC_SSCTL0_TS7          0x80000000  // 8th Sample Temp Sensor Select
#define ADC_SSCTL0_IE7          0x40000000  // 8th Sample Interrupt Enable
#define ADC_SSCTL0_END7         0x20000000  // 8th Sample is End of Sequence
#define ADC_SSCTL0_D7           0x10000000  // 8th Sample Diff Input Select
#define ADC_SSCTL0_TS6          0x08000000  // 7th Sample Temp Sensor Select
#define ADC_SSCTL0_IE6          0x04000000  // 7th Sample Interrupt Enable
#define ADC_SSCTL0_END6         0x02000000  // 7th Sample is End of Sequence
#define ADC_SSCTL0_D6           0x01000000  // 7th Sample Diff Input Select
#define ADC_SSCTL0_TS5          0x00800000  // 6th Sample Temp Sensor Select
#define ADC_SSCTL0_IE5          0x00400000  // 6th Sample Interrupt Enable
#define ADC_SSCTL0_END5         0x00200000  // 6th Sample is End of Sequence
#define ADC_SSCTL0_D5           0x00100000  // 6th Sample Diff Input Select
#define ADC_SSCTL0_TS4          0x00080000  // 5th Sample Temp Sensor Select
#define ADC_SSCTL0_IE4          0x00040000  // 5th Sample Interrupt Enable
#define ADC_SSCTL0_END4         0x00020000  // 5th Sample is End of Sequence
#define ADC_SSCTL0_D4           0x00010000  // 5th Sample Diff Input Select
#define ADC_SSCTL0_TS3          0x00008000  // 4th Sample Temp Sensor Select
#define ADC_SSCTL0_IE3          0x00004000  // 4th Sample Interrupt Enable
#define ADC_SSCTL0_END3         0x00002000  // 4th Sample is End of Sequence
#define ADC_SSCTL0_D3           0x00001000  // 4th Sample Diff Input Select
#define ADC_SSCTL0_TS2          0x00000800  // 3rd Sample Temp Sensor Select
#define ADC_SSCTL0_IE2          0x00000400  // 3rd Sample Interrupt Enable
#define ADC_SSCTL0_END2         0x00000200  // 3rd Sample is End of Sequence
#define ADC_SSCTL0_D2           0x00000100  // 3rd Sample Diff Input Select
#define ADC_SSCTL0_TS1          0x00000080  // 2nd Sample Temp Sensor Select
#define ADC_SSCTL0_IE1          0x00000040  // 2nd Sample Interrupt Enable
#define ADC_SSCTL0_END1         0x00000020  // 2nd Sample is End of Sequence
#define ADC_SSCTL0_D1           0x00000010  // 2nd Sample Diff Input Select
#define ADC_SSCTL0_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL0_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL0_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL0_D0           0x00000001  // 1st Sample Diff Input Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFIFO0 register.
//
//*****************************************************************************
#define ADC_SSFIFO0_DATA_M      0x000003FF  // Conversion Result Data
#define ADC_SSFIFO0_DATA_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFSTAT0 register.
//
//*****************************************************************************
#define ADC_SSFSTAT0_FULL       0x00001000  // FIFO Full
#define ADC_SSFSTAT0_EMPTY      0x00000100  // FIFO Empty
#define ADC_SSFSTAT0_HPTR_M     0x000000F0  // FIFO Head Pointer
#define ADC_SSFSTAT0_TPTR_M     0x0000000F  // FIFO Tail Pointer
#define ADC_SSFSTAT0_HPTR_S     4
#define ADC_SSFSTAT0_TPTR_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSOP0 register.
//
//*****************************************************************************
#define ADC_SSOP0_S7DCOP        0x10000000  // Sample 7 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S6DCOP        0x01000000  // Sample 6 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S5DCOP        0x00100000  // Sample 5 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S4DCOP        0x00010000  // Sample 4 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S3DCOP        0x00001000  // Sample 3 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S2DCOP        0x00000100  // Sample 2 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S1DCOP        0x00000010  // Sample 1 Digital Comparator
                                            // Operation
#define ADC_SSOP0_S0DCOP        0x00000001  // Sample 0 Digital Comparator
                                            // Operation

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSDC0 register.
//
//*****************************************************************************
#define ADC_SSDC0_S7DCSEL_M     0xF0000000  // Sample 7 Digital Comparator
                                            // Select
#define ADC_SSDC0_S6DCSEL_M     0x0F000000  // Sample 6 Digital Comparator
                                            // Select
#define ADC_SSDC0_S5DCSEL_M     0x00F00000  // Sample 5 Digital Comparator
                                            // Select
#define ADC_SSDC0_S4DCSEL_M     0x000F0000  // Sample 4 Digital Comparator
                                            // Select
#define ADC_SSDC0_S3DCSEL_M     0x0000F000  // Sample 3 Digital Comparator
                                            // Select
#define ADC_SSDC0_S2DCSEL_M     0x00000F00  // Sample 2 Digital Comparator
                                            // Select
#define ADC_SSDC0_S1DCSEL_M     0x000000F0  // Sample 1 Digital Comparator
                                            // Select
#define ADC_SSDC0_S0DCSEL_M     0x0000000F  // Sample 0 Digital Comparator
                                            // Select
#define ADC_SSDC0_S6DCSEL_S     24
#define ADC_SSDC0_S5DCSEL_S     20
#define ADC_SSDC0_S4DCSEL_S     16
#define ADC_SSDC0_S3DCSEL_S     12
#define ADC_SSDC0_S2DCSEL_S     8
#define ADC_SSDC0_S1DCSEL_S     4
#define ADC_SSDC0_S0DCSEL_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSMUX1 register.
//
//*****************************************************************************
#define ADC_SSMUX1_MUX3_M       0x00007000  // 4th Sample Input Select
#define ADC_SSMUX1_MUX2_M       0x00000700  // 3rd Sample Input Select
#define ADC_SSMUX1_MUX1_M       0x00000070  // 2nd Sample Input Select
#define ADC_SSMUX1_MUX0_M       0x00000007  // 1st Sample Input Select
#define ADC_SSMUX1_MUX3_S       12
#define ADC_SSMUX1_MUX2_S       8
#define ADC_SSMUX1_MUX1_S       4
#define ADC_SSMUX1_MUX0_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSCTL1 register.
//
//*****************************************************************************
#define ADC_SSCTL1_TS3          0x00008000  // 4th Sample Temp Sensor Select
#define ADC_SSCTL1_IE3          0x00004000  // 4th Sample Interrupt Enable
#define ADC_SSCTL1_END3         0x00002000  // 4th Sample is End of Sequence
#define ADC_SSCTL1_D3           0x00001000  // 4th Sample Diff Input Select
#define ADC_SSCTL1_TS2          0x00000800  // 3rd Sample Temp Sensor Select
#define ADC_SSCTL1_IE2          0x00000400  // 3rd Sample Interrupt Enable
#define ADC_SSCTL1_END2         0x00000200  // 3rd Sample is End of Sequence
#define ADC_SSCTL1_D2           0x00000100  // 3rd Sample Diff Input Select
#define ADC_SSCTL1_TS1          0x00000080  // 2nd Sample Temp Sensor Select
#define ADC_SSCTL1_IE1          0x00000040  // 2nd Sample Interrupt Enable
#define ADC_SSCTL1_END1         0x00000020  // 2nd Sample is End of Sequence
#define ADC_SSCTL1_D1           0x00000010  // 2nd Sample Diff Input Select
#define ADC_SSCTL1_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL1_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL1_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL1_D0           0x00000001  // 1st Sample Diff Input Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFIFO1 register.
//
//*****************************************************************************
#define ADC_SSFIFO1_DATA_M      0x000003FF  // Conversion Result Data
#define ADC_SSFIFO1_DATA_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFSTAT1 register.
//
//*****************************************************************************
#define ADC_SSFSTAT1_FULL       0x00001000  // FIFO Full
#define ADC_SSFSTAT1_EMPTY      0x00000100  // FIFO Empty
#define ADC_SSFSTAT1_HPTR_M     0x000000F0  // FIFO Head Pointer
#define ADC_SSFSTAT1_TPTR_M     0x0000000F  // FIFO Tail Pointer
#define ADC_SSFSTAT1_HPTR_S     4
#define ADC_SSFSTAT1_TPTR_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSOP1 register.
//
//*****************************************************************************
#define ADC_SSOP1_S3DCOP        0x00001000  // Sample 3 Digital Comparator
                                            // Operation
#define ADC_SSOP1_S2DCOP        0x00000100  // Sample 2 Digital Comparator
                                            // Operation
#define ADC_SSOP1_S1DCOP        0x00000010  // Sample 1 Digital Comparator
                                            // Operation
#define ADC_SSOP1_S0DCOP        0x00000001  // Sample 0 Digital Comparator
                                            // Operation

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSDC1 register.
//
//*****************************************************************************
#define ADC_SSDC1_S3DCSEL_M     0x0000F000  // Sample 3 Digital Comparator
                                            // Select
#define ADC_SSDC1_S2DCSEL_M     0x00000F00  // Sample 2 Digital Comparator
                                            // Select
#define ADC_SSDC1_S1DCSEL_M     0x000000F0  // Sample 1 Digital Comparator
                                            // Select
#define ADC_SSDC1_S0DCSEL_M     0x0000000F  // Sample 0 Digital Comparator
                                            // Select
#define ADC_SSDC1_S2DCSEL_S     8
#define ADC_SSDC1_S1DCSEL_S     4
#define ADC_SSDC1_S0DCSEL_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSMUX2 register.
//
//*****************************************************************************
#define ADC_SSMUX2_MUX3_M       0x00007000  // 4th Sample Input Select
#define ADC_SSMUX2_MUX2_M       0x00000700  // 3rd Sample Input Select
#define ADC_SSMUX2_MUX1_M       0x00000070  // 2nd Sample Input Select
#define ADC_SSMUX2_MUX0_M       0x00000007  // 1st Sample Input Select
#define ADC_SSMUX2_MUX3_S       12
#define ADC_SSMUX2_MUX2_S       8
#define ADC_SSMUX2_MUX1_S       4
#define ADC_SSMUX2_MUX0_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSCTL2 register.
//
//*****************************************************************************
#define ADC_SSCTL2_TS3          0x00008000  // 4th Sample Temp Sensor Select
#define ADC_SSCTL2_IE3          0x00004000  // 4th Sample Interrupt Enable
#define ADC_SSCTL2_END3         0x00002000  // 4th Sample is End of Sequence
#define ADC_SSCTL2_D3           0x00001000  // 4th Sample Diff Input Select
#define ADC_SSCTL2_TS2          0x00000800  // 3rd Sample Temp Sensor Select
#define ADC_SSCTL2_IE2          0x00000400  // 3rd Sample Interrupt Enable
#define ADC_SSCTL2_END2         0x00000200  // 3rd Sample is End of Sequence
#define ADC_SSCTL2_D2           0x00000100  // 3rd Sample Diff Input Select
#define ADC_SSCTL2_TS1          0x00000080  // 2nd Sample Temp Sensor Select
#define ADC_SSCTL2_IE1          0x00000040  // 2nd Sample Interrupt Enable
#define ADC_SSCTL2_END1         0x00000020  // 2nd Sample is End of Sequence
#define ADC_SSCTL2_D1           0x00000010  // 2nd Sample Diff Input Select
#define ADC_SSCTL2_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL2_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL2_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL2_D0           0x00000001  // 1st Sample Diff Input Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFIFO2 register.
//
//*****************************************************************************
#define ADC_SSFIFO2_DATA_M      0x000003FF  // Conversion Result Data
#define ADC_SSFIFO2_DATA_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFSTAT2 register.
//
//*****************************************************************************
#define ADC_SSFSTAT2_FULL       0x00001000  // FIFO Full
#define ADC_SSFSTAT2_EMPTY      0x00000100  // FIFO Empty
#define ADC_SSFSTAT2_HPTR_M     0x000000F0  // FIFO Head Pointer
#define ADC_SSFSTAT2_TPTR_M     0x0000000F  // FIFO Tail Pointer
#define ADC_SSFSTAT2_HPTR_S     4
#define ADC_SSFSTAT2_TPTR_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSOP2 register.
//
//*****************************************************************************
#define ADC_SSOP2_S3DCOP        0x00001000  // Sample 3 Digital Comparator
                                            // Operation
#define ADC_SSOP2_S2DCOP        0x00000100  // Sample 2 Digital Comparator
                                            // Operation
#define ADC_SSOP2_S1DCOP        0x00000010  // Sample 1 Digital Comparator
                                            // Operation
#define ADC_SSOP2_S0DCOP        0x00000001  // Sample 0 Digital Comparator
                                            // Operation

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSDC2 register.
//
//*****************************************************************************
#define ADC_SSDC2_S3DCSEL_M     0x0000F000  // Sample 3 Digital Comparator
                                            // Select
#define ADC_SSDC2_S2DCSEL_M     0x00000F00  // Sample 2 Digital Comparator
                                            // Select
#define ADC_SSDC2_S1DCSEL_M     0x000000F0  // Sample 1 Digital Comparator
                                            // Select
#define ADC_SSDC2_S0DCSEL_M     0x0000000F  // Sample 0 Digital Comparator
                                            // Select
#define ADC_SSDC2_S2DCSEL_S     8
#define ADC_SSDC2_S1DCSEL_S     4
#define ADC_SSDC2_S0DCSEL_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSMUX3 register.
//
//*****************************************************************************
#define ADC_SSMUX3_MUX0_M       0x00000007  // 1st Sample Input Select
#define ADC_SSMUX3_MUX0_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSCTL3 register.
//
//*****************************************************************************
#define ADC_SSCTL3_TS0          0x00000008  // 1st Sample Temp Sensor Select
#define ADC_SSCTL3_IE0          0x00000004  // 1st Sample Interrupt Enable
#define ADC_SSCTL3_END0         0x00000002  // 1st Sample is End of Sequence
#define ADC_SSCTL3_D0           0x00000001  // 1st Sample Diff Input Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFIFO3 register.
//
//*****************************************************************************
#define ADC_SSFIFO3_DATA_M      0x000003FF  // Conversion Result Data
#define ADC_SSFIFO3_DATA_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSFSTAT3 register.
//
//*****************************************************************************
#define ADC_SSFSTAT3_FULL       0x00001000  // FIFO Full
#define ADC_SSFSTAT3_EMPTY      0x00000100  // FIFO Empty
#define ADC_SSFSTAT3_HPTR_M     0x000000F0  // FIFO Head Pointer
#define ADC_SSFSTAT3_TPTR_M     0x0000000F  // FIFO Tail Pointer
#define ADC_SSFSTAT3_HPTR_S     4
#define ADC_SSFSTAT3_TPTR_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSOP3 register.
//
//*****************************************************************************
#define ADC_SSOP3_S0DCOP        0x00000001  // Sample 0 Digital Comparator
                                            // Operation

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_SSDC3 register.
//
//*****************************************************************************
#define ADC_SSDC3_S0DCSEL_M     0x0000000F  // Sample 0 Digital Comparator
                                            // Select

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCRIC register.
//
//*****************************************************************************
#define ADC_DCRIC_DCTRIG7       0x00800000  // Digital Comparator Trigger 7
#define ADC_DCRIC_DCTRIG6       0x00400000  // Digital Comparator Trigger 6
#define ADC_DCRIC_DCTRIG5       0x00200000  // Digital Comparator Trigger 5
#define ADC_DCRIC_DCTRIG4       0x00100000  // Digital Comparator Trigger 4
#define ADC_DCRIC_DCTRIG3       0x00080000  // Digital Comparator Trigger 3
#define ADC_DCRIC_DCTRIG2       0x00040000  // Digital Comparator Trigger 2
#define ADC_DCRIC_DCTRIG1       0x00020000  // Digital Comparator Trigger 1
#define ADC_DCRIC_DCTRIG0       0x00010000  // Digital Comparator Trigger 0
#define ADC_DCRIC_DCINT7        0x00000080  // Digital Comparator Interrupt 7
#define ADC_DCRIC_DCINT6        0x00000040  // Digital Comparator Interrupt 6
#define ADC_DCRIC_DCINT5        0x00000020  // Digital Comparator Interrupt 5
#define ADC_DCRIC_DCINT4        0x00000010  // Digital Comparator Interrupt 4
#define ADC_DCRIC_DCINT3        0x00000008  // Digital Comparator Interrupt 3
#define ADC_DCRIC_DCINT2        0x00000004  // Digital Comparator Interrupt 2
#define ADC_DCRIC_DCINT1        0x00000002  // Digital Comparator Interrupt 1
#define ADC_DCRIC_DCINT0        0x00000001  // Digital Comparator Interrupt 0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL0 register.
//
//*****************************************************************************
#define ADC_DCCTL0_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL0_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL0_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL0_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL0_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL0_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL0_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL0_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL0_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL0_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL0_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL0_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL0_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL0_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL0_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL0_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL0_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL0_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL0_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL0_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL1 register.
//
//*****************************************************************************
#define ADC_DCCTL1_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL1_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL1_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL1_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL1_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL1_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL1_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL1_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL1_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL1_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL1_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL1_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL1_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL1_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL1_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL1_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL1_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL1_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL1_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL1_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL2 register.
//
//*****************************************************************************
#define ADC_DCCTL2_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL2_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL2_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL2_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL2_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL2_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL2_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL2_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL2_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL2_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL2_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL2_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL2_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL2_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL2_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL2_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL2_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL2_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL2_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL2_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL3 register.
//
//*****************************************************************************
#define ADC_DCCTL3_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL3_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL3_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL3_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL3_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL3_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL3_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL3_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL3_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL3_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL3_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL3_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL3_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL3_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL3_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL3_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL3_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL3_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL3_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL3_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL4 register.
//
//*****************************************************************************
#define ADC_DCCTL4_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL4_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL4_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL4_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL4_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL4_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL4_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL4_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL4_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL4_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL4_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL4_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL4_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL4_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL4_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL4_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL4_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL4_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL4_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL4_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL5 register.
//
//*****************************************************************************
#define ADC_DCCTL5_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL5_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL5_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL5_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL5_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL5_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL5_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL5_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL5_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL5_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL5_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL5_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL5_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL5_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL5_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL5_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL5_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL5_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL5_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL5_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL6 register.
//
//*****************************************************************************
#define ADC_DCCTL6_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL6_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL6_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL6_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL6_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL6_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL6_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL6_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL6_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL6_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL6_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL6_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL6_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL6_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL6_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL6_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL6_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL6_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL6_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL6_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCTL7 register.
//
//*****************************************************************************
#define ADC_DCCTL7_CTE          0x00001000  // Comparison Trigger Enable
#define ADC_DCCTL7_CTC_M        0x00000C00  // Comparison Trigger Condition
#define ADC_DCCTL7_CTC_LOW      0x00000000  // Low Band
#define ADC_DCCTL7_CTC_MID      0x00000400  // Mid Band
#define ADC_DCCTL7_CTC_HIGH     0x00000C00  // High Band
#define ADC_DCCTL7_CTM_M        0x00000300  // Comparison Trigger Mode
#define ADC_DCCTL7_CTM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL7_CTM_ONCE     0x00000100  // Once
#define ADC_DCCTL7_CTM_HALWAYS  0x00000200  // Hysteresis Always
#define ADC_DCCTL7_CTM_HONCE    0x00000300  // Hysteresis Once
#define ADC_DCCTL7_CIE          0x00000010  // Comparison Interrupt Enable
#define ADC_DCCTL7_CIC_M        0x0000000C  // Comparison Interrupt Condition
#define ADC_DCCTL7_CIC_LOW      0x00000000  // Low Band
#define ADC_DCCTL7_CIC_MID      0x00000004  // Mid Band
#define ADC_DCCTL7_CIC_HIGH     0x0000000C  // High Band
#define ADC_DCCTL7_CIM_M        0x00000003  // Comparison Interrupt Mode
#define ADC_DCCTL7_CIM_ALWAYS   0x00000000  // Always
#define ADC_DCCTL7_CIM_ONCE     0x00000001  // Once
#define ADC_DCCTL7_CIM_HALWAYS  0x00000002  // Hysteresis Always
#define ADC_DCCTL7_CIM_HONCE    0x00000003  // Hysteresis Once

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP0 register.
//
//*****************************************************************************
#define ADC_DCCMP0_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP0_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP0_COMP1_S      16
#define ADC_DCCMP0_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP1 register.
//
//*****************************************************************************
#define ADC_DCCMP1_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP1_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP1_COMP1_S      16
#define ADC_DCCMP1_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP2 register.
//
//*****************************************************************************
#define ADC_DCCMP2_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP2_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP2_COMP1_S      16
#define ADC_DCCMP2_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP3 register.
//
//*****************************************************************************
#define ADC_DCCMP3_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP3_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP3_COMP1_S      16
#define ADC_DCCMP3_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP4 register.
//
//*****************************************************************************
#define ADC_DCCMP4_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP4_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP4_COMP1_S      16
#define ADC_DCCMP4_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP5 register.
//
//*****************************************************************************
#define ADC_DCCMP5_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP5_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP5_COMP1_S      16
#define ADC_DCCMP5_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP6 register.
//
//*****************************************************************************
#define ADC_DCCMP6_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP6_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP6_COMP1_S      16
#define ADC_DCCMP6_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the ADC_O_DCCMP7 register.
//
//*****************************************************************************
#define ADC_DCCMP7_COMP1_M      0x03FF0000  // Compare 1
#define ADC_DCCMP7_COMP0_M      0x000003FF  // Compare 0
#define ADC_DCCMP7_COMP1_S      16
#define ADC_DCCMP7_COMP0_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACMIS register.
//
//*****************************************************************************
#define COMP_ACMIS_IN1          0x00000002  // Comparator 1 Masked Interrupt
                                            // Status
#define COMP_ACMIS_IN0          0x00000001  // Comparator 0 Masked Interrupt
                                            // Status

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACRIS register.
//
//*****************************************************************************
#define COMP_ACRIS_IN1          0x00000002  // Comparator 1 Interrupt Status
#define COMP_ACRIS_IN0          0x00000001  // Comparator 0 Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACINTEN register.
//
//*****************************************************************************
#define COMP_ACINTEN_IN1        0x00000002  // Comparator 1 Interrupt Enable
#define COMP_ACINTEN_IN0        0x00000001  // Comparator 0 Interrupt Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACREFCTL
// register.
//
//*****************************************************************************
#define COMP_ACREFCTL_EN        0x00000200  // Resistor Ladder Enable
#define COMP_ACREFCTL_RNG       0x00000100  // Resistor Ladder Range
#define COMP_ACREFCTL_VREF_M    0x0000000F  // Resistor Ladder Voltage Ref
#define COMP_ACREFCTL_VREF_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACSTAT0 register.
//
//*****************************************************************************
#define COMP_ACSTAT0_OVAL       0x00000002  // Comparator Output Value

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACCTL0 register.
//
//*****************************************************************************
#define COMP_ACCTL0_TOEN        0x00000800  // Trigger Output Enable
#define COMP_ACCTL0_ASRCP_M     0x00000600  // Analog Source Positive
#define COMP_ACCTL0_ASRCP_PIN   0x00000000  // Pin value of Cn+
#define COMP_ACCTL0_ASRCP_PIN0  0x00000200  // Pin value of C0+
#define COMP_ACCTL0_ASRCP_REF   0x00000400  // Internal voltage reference
#define COMP_ACCTL0_TSLVAL      0x00000080  // Trigger Sense Level Value
#define COMP_ACCTL0_TSEN_M      0x00000060  // Trigger Sense
#define COMP_ACCTL0_TSEN_LEVEL  0x00000000  // Level sense, see TSLVAL
#define COMP_ACCTL0_TSEN_FALL   0x00000020  // Falling edge
#define COMP_ACCTL0_TSEN_RISE   0x00000040  // Rising edge
#define COMP_ACCTL0_TSEN_BOTH   0x00000060  // Either edge
#define COMP_ACCTL0_ISLVAL      0x00000010  // Interrupt Sense Level Value
#define COMP_ACCTL0_ISEN_M      0x0000000C  // Interrupt Sense
#define COMP_ACCTL0_ISEN_LEVEL  0x00000000  // Level sense, see ISLVAL
#define COMP_ACCTL0_ISEN_FALL   0x00000004  // Falling edge
#define COMP_ACCTL0_ISEN_RISE   0x00000008  // Rising edge
#define COMP_ACCTL0_ISEN_BOTH   0x0000000C  // Either edge
#define COMP_ACCTL0_CINV        0x00000002  // Comparator Output Invert

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACSTAT1 register.
//
//*****************************************************************************
#define COMP_ACSTAT1_OVAL       0x00000002  // Comparator Output Value

//*****************************************************************************
//
// The following are defines for the bit fields in the COMP_O_ACCTL1 register.
//
//*****************************************************************************
#define COMP_ACCTL1_TOEN        0x00000800  // Trigger Output Enable
#define COMP_ACCTL1_ASRCP_M     0x00000600  // Analog Source Positive
#define COMP_ACCTL1_ASRCP_PIN   0x00000000  // Pin value of Cn+
#define COMP_ACCTL1_ASRCP_PIN0  0x00000200  // Pin value of C0+
#define COMP_ACCTL1_ASRCP_REF   0x00000400  // Internal voltage reference
                                            // (VIREF)
#define COMP_ACCTL1_TSLVAL      0x00000080  // Trigger Sense Level Value
#define COMP_ACCTL1_TSEN_M      0x00000060  // Trigger Sense
#define COMP_ACCTL1_TSEN_LEVEL  0x00000000  // Level sense, see TSLVAL
#define COMP_ACCTL1_TSEN_FALL   0x00000020  // Falling edge
#define COMP_ACCTL1_TSEN_RISE   0x00000040  // Rising edge
#define COMP_ACCTL1_TSEN_BOTH   0x00000060  // Either edge
#define COMP_ACCTL1_ISLVAL      0x00000010  // Interrupt Sense Level Value
#define COMP_ACCTL1_ISEN_M      0x0000000C  // Interrupt Sense
#define COMP_ACCTL1_ISEN_LEVEL  0x00000000  // Level sense, see ISLVAL
#define COMP_ACCTL1_ISEN_FALL   0x00000004  // Falling edge
#define COMP_ACCTL1_ISEN_RISE   0x00000008  // Rising edge
#define COMP_ACCTL1_ISEN_BOTH   0x0000000C  // Either edge
#define COMP_ACCTL1_CINV        0x00000002  // Comparator Output Invert

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_CFG register.
//
//*****************************************************************************
#define EPI_CFG_BLKEN           0x00000010  // Block Enable
#define EPI_CFG_MODE_M          0x0000000F  // Mode Select
#define EPI_CFG_MODE_NONE       0x00000000  // General Purpose
#define EPI_CFG_MODE_SDRAM      0x00000001  // SDRAM
#define EPI_CFG_MODE_HB8        0x00000002  // 8-Bit Host-Bus (HB8)
#define EPI_CFG_MODE_HB16       0x00000003  // 16-Bit Host-Bus (HB16)

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_BAUD register.
//
//*****************************************************************************
#define EPI_BAUD_COUNT1_M       0xFFFF0000  // Baud Rate Counter 1
#define EPI_BAUD_COUNT0_M       0x0000FFFF  // Baud Rate Counter 0
#define EPI_BAUD_COUNT1_S       16
#define EPI_BAUD_COUNT0_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_HB16CFG register.
//
//*****************************************************************************
#define EPI_HB16CFG_XFFEN       0x00800000  // External FIFO FULL Enable
#define EPI_HB16CFG_XFEEN       0x00400000  // External FIFO EMPTY Enable
#define EPI_HB16CFG_WRHIGH      0x00200000  // WRITE Strobe Polarity
#define EPI_HB16CFG_RDHIGH      0x00100000  // READ Strobe Polarity
#define EPI_HB16CFG_MAXWAIT_M   0x0000FF00  // Maximum Wait
#define EPI_HB16CFG_WRWS_M      0x000000C0  // Write Wait States
#define EPI_HB16CFG_WRWS_0      0x00000000  // No wait states
#define EPI_HB16CFG_WRWS_1      0x00000040  // 1 wait state
#define EPI_HB16CFG_WRWS_2      0x00000080  // 2 wait states
#define EPI_HB16CFG_WRWS_3      0x000000C0  // 3 wait states
#define EPI_HB16CFG_RDWS_M      0x00000030  // Read Wait States
#define EPI_HB16CFG_RDWS_0      0x00000000  // No wait states
#define EPI_HB16CFG_RDWS_1      0x00000010  // 1 wait state
#define EPI_HB16CFG_RDWS_2      0x00000020  // 2 wait states
#define EPI_HB16CFG_RDWS_3      0x00000030  // 3 wait states
#define EPI_HB16CFG_BSEL        0x00000004  // Byte Select Configuration
#define EPI_HB16CFG_MODE_M      0x00000003  // Host Bus Sub-Mode
#define EPI_HB16CFG_MODE_ADMUX  0x00000000  // ADMUX - AD[15:0]
#define EPI_HB16CFG_MODE_ADNMUX 0x00000001  // ADNONMUX - D[15:0]
#define EPI_HB16CFG_MODE_SRAM   0x00000002  // Continuous Read - D[15:0]
#define EPI_HB16CFG_MODE_XFIFO  0x00000003  // XFIFO - D[15:0]
#define EPI_HB16CFG_MAXWAIT_S   8

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_GPCFG register.
//
//*****************************************************************************
#define EPI_GPCFG_CLKPIN        0x80000000  // Clock Pin
#define EPI_GPCFG_CLKGATE       0x40000000  // Clock Gated
#define EPI_GPCFG_RDYEN         0x10000000  // Ready Enable
#define EPI_GPCFG_FRMPIN        0x08000000  // Framing Pin
#define EPI_GPCFG_FRM50         0x04000000  // 50/50 Frame
#define EPI_GPCFG_FRMCNT_M      0x03C00000  // Frame Count
#define EPI_GPCFG_RW            0x00200000  // Read and Write
#define EPI_GPCFG_WR2CYC        0x00080000  // 2-Cycle Writes
#define EPI_GPCFG_RD2CYC        0x00040000  // 2-Cycle Reads
#define EPI_GPCFG_MAXWAIT_M     0x0000FF00  // Maximum Wait
#define EPI_GPCFG_ASIZE_M       0x00000030  // Address Bus Size
#define EPI_GPCFG_ASIZE_NONE    0x00000000  // No address
#define EPI_GPCFG_ASIZE_4BIT    0x00000010  // Up to 4 bits wide
#define EPI_GPCFG_ASIZE_12BIT   0x00000020  // Up to 12 bits wide. This size
                                            // cannot be used with 24-bit data
#define EPI_GPCFG_ASIZE_20BIT   0x00000030  // Up to 20 bits wide. This size
                                            // cannot be used with data sizes
                                            // other than 8
#define EPI_GPCFG_DSIZE_M       0x00000003  // Size of Data Bus
#define EPI_GPCFG_DSIZE_4BIT    0x00000000  // 8 Bits Wide (EPI0S0 to EPI0S7)
#define EPI_GPCFG_DSIZE_16BIT   0x00000001  // 16 Bits Wide (EPI0S0 to EPI0S15)
#define EPI_GPCFG_DSIZE_24BIT   0x00000002  // 24 Bits Wide (EPI0S0 to EPI0S23)
#define EPI_GPCFG_DSIZE_32BIT   0x00000003  // 32 Bits Wide (EPI0S0 to EPI0S31)
#define EPI_GPCFG_FRMCNT_S      22
#define EPI_GPCFG_MAXWAIT_S     8

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_SDRAMCFG register.
//
//*****************************************************************************
#define EPI_SDRAMCFG_FREQ_M     0xC0000000  // EPI Frequency Range
#define EPI_SDRAMCFG_FREQ_NONE  0x00000000  // 0 - 15 MHz
#define EPI_SDRAMCFG_FREQ_15MHZ 0x40000000  // 15 - 30 MHz
#define EPI_SDRAMCFG_FREQ_30MHZ 0x80000000  // 30 - 50 MHz
#define EPI_SDRAMCFG_RFSH_M     0x07FF0000  // Refresh Counter
#define EPI_SDRAMCFG_SLEEP      0x00000200  // Sleep Mode
#define EPI_SDRAMCFG_SIZE_M     0x00000003  // Size of SDRAM
#define EPI_SDRAMCFG_SIZE_8MB   0x00000000  // 64 megabits (8MB)
#define EPI_SDRAMCFG_SIZE_16MB  0x00000001  // 128 megabits (16MB)
#define EPI_SDRAMCFG_SIZE_32MB  0x00000002  // 256 megabits (32MB)
#define EPI_SDRAMCFG_SIZE_64MB  0x00000003  // 512 megabits (64MB)
#define EPI_SDRAMCFG_RFSH_S     16

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_HB8CFG register.
//
//*****************************************************************************
#define EPI_HB8CFG_XFFEN        0x00800000  // External FIFO FULL Enable
#define EPI_HB8CFG_XFEEN        0x00400000  // External FIFO EMPTY Enable
#define EPI_HB8CFG_WRHIGH       0x00200000  // WRITE Strobe Polarity
#define EPI_HB8CFG_RDHIGH       0x00100000  // READ Strobe Polarity
#define EPI_HB8CFG_MAXWAIT_M    0x0000FF00  // Maximum Wait
#define EPI_HB8CFG_WRWS_M       0x000000C0  // Write Wait States
#define EPI_HB8CFG_WRWS_0       0x00000000  // No wait states
#define EPI_HB8CFG_WRWS_1       0x00000040  // 1 wait state
#define EPI_HB8CFG_WRWS_2       0x00000080  // 2 wait states
#define EPI_HB8CFG_WRWS_3       0x000000C0  // 3 wait states
#define EPI_HB8CFG_RDWS_M       0x00000030  // Read Wait States
#define EPI_HB8CFG_RDWS_0       0x00000000  // No wait states
#define EPI_HB8CFG_RDWS_1       0x00000010  // 1 wait state
#define EPI_HB8CFG_RDWS_2       0x00000020  // 2 wait states
#define EPI_HB8CFG_RDWS_3       0x00000030  // 3 wait states
#define EPI_HB8CFG_MODE_M       0x00000003  // Host Bus Sub-Mode
#define EPI_HB8CFG_MODE_MUX     0x00000000  // ADMUX - AD[7:0]
#define EPI_HB8CFG_MODE_NMUX    0x00000001  // ADNONMUX - D[7:0]
#define EPI_HB8CFG_MODE_SRAM    0x00000002  // Continuous Read - D[7:0]
#define EPI_HB8CFG_MODE_FIFO    0x00000003  // XFIFO - D[7:0]
#define EPI_HB8CFG_MAXWAIT_S    8

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_HB8CFG2 register.
//
//*****************************************************************************
#define EPI_HB8CFG2_WORD        0x80000000  // Word Access Mode
#define EPI_HB8CFG2_CSBAUD      0x04000000  // Chip Select Baud Rate
#define EPI_HB8CFG2_CSCFG_M     0x03000000  // Chip Select Configuration
#define EPI_HB8CFG2_CSCFG_ALE   0x00000000  // ALE Configuration
#define EPI_HB8CFG2_CSCFG_CS    0x01000000  // CSn Configuration
#define EPI_HB8CFG2_CSCFG_DCS   0x02000000  // Dual CSn Configuration
#define EPI_HB8CFG2_CSCFG_ADCS  0x03000000  // ALE with Dual CSn Configuration

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_HB16CFG2 register.
//
//*****************************************************************************
#define EPI_HB16CFG2_WORD       0x80000000  // Word Access Mode
#define EPI_HB16CFG2_CSBAUD     0x04000000  // Chip Select Baud Rate
#define EPI_HB16CFG2_CSCFG_M    0x03000000  // Chip Select Configuration
#define EPI_HB16CFG2_CSCFG_ALE  0x00000000  // ALE Configuration
#define EPI_HB16CFG2_CSCFG_CS   0x01000000  // CSn Configuration
#define EPI_HB16CFG2_CSCFG_DCS  0x02000000  // Dual CSn Configuration
#define EPI_HB16CFG2_CSCFG_ADCS 0x03000000  // ALE with Dual CSn Configuration

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_GPCFG2 register.
//
//*****************************************************************************
#define EPI_GPCFG2_WORD         0x80000000  // Word Access Mode

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_ADDRMAP register.
//
//*****************************************************************************
#define EPI_ADDRMAP_EPSZ_M      0x000000C0  // External Peripheral Size
#define EPI_ADDRMAP_EPSZ_256B   0x00000000  // 256 bytes; lower address range:
                                            // 0x00 to 0xFF
#define EPI_ADDRMAP_EPSZ_64KB   0x00000040  // 64 KB; lower address range:
                                            // 0x0000 to 0xFFFF
#define EPI_ADDRMAP_EPSZ_16MB   0x00000080  // 16 MB; lower address range:
                                            // 0x00.0000 to 0xFF.FFFF
#define EPI_ADDRMAP_EPSZ_512MB  0x000000C0  // 512 MB; lower address range:
                                            // 0x000.0000 to 0x1FFF.FFFF
#define EPI_ADDRMAP_EPADR_M     0x00000030  // External Peripheral Address
#define EPI_ADDRMAP_EPADR_NONE  0x00000000  // Not mapped
#define EPI_ADDRMAP_EPADR_A000  0x00000010  // At 0xA000.0000
#define EPI_ADDRMAP_EPADR_C000  0x00000020  // At 0xC000.0000
#define EPI_ADDRMAP_ERSZ_M      0x0000000C  // External RAM Size
#define EPI_ADDRMAP_ERSZ_256B   0x00000000  // 256 bytes; lower address range:
                                            // 0x00 to 0xFF
#define EPI_ADDRMAP_ERSZ_64KB   0x00000004  // 64 KB; lower address range:
                                            // 0x0000 to 0xFFFF
#define EPI_ADDRMAP_ERSZ_16MB   0x00000008  // 16 MB; lower address range:
                                            // 0x00.0000 to 0xFF.FFFF
#define EPI_ADDRMAP_ERSZ_512MB  0x0000000C  // 512 MB; lower address range:
                                            // 0x000.0000 to 0x1FFF.FFFF
#define EPI_ADDRMAP_ERADR_M     0x00000003  // External RAM Address
#define EPI_ADDRMAP_ERADR_NONE  0x00000000  // Not mapped
#define EPI_ADDRMAP_ERADR_6000  0x00000001  // At 0x6000.0000
#define EPI_ADDRMAP_ERADR_8000  0x00000002  // At 0x8000.0000

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RSIZE0 register.
//
//*****************************************************************************
#define EPI_RSIZE0_SIZE_M       0x00000003  // Current Size
#define EPI_RSIZE0_SIZE_8BIT    0x00000001  // Byte (8 bits)
#define EPI_RSIZE0_SIZE_16BIT   0x00000002  // Half-word (16 bits)
#define EPI_RSIZE0_SIZE_32BIT   0x00000003  // Word (32 bits)

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RADDR0 register.
//
//*****************************************************************************
#define EPI_RADDR0_ADDR_M       0x1FFFFFFF  // Current Address
#define EPI_RADDR0_ADDR_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RPSTD0 register.
//
//*****************************************************************************
#define EPI_RPSTD0_POSTCNT_M    0x00001FFF  // Post Count
#define EPI_RPSTD0_POSTCNT_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RSIZE1 register.
//
//*****************************************************************************
#define EPI_RSIZE1_SIZE_M       0x00000003  // Current Size
#define EPI_RSIZE1_SIZE_8BIT    0x00000001  // Byte (8 bits)
#define EPI_RSIZE1_SIZE_16BIT   0x00000002  // Half-word (16 bits)
#define EPI_RSIZE1_SIZE_32BIT   0x00000003  // Word (32 bits)

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RADDR1 register.
//
//*****************************************************************************
#define EPI_RADDR1_ADDR_M       0x1FFFFFFF  // Current Address
#define EPI_RADDR1_ADDR_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RPSTD1 register.
//
//*****************************************************************************
#define EPI_RPSTD1_POSTCNT_M    0x00001FFF  // Post Count
#define EPI_RPSTD1_POSTCNT_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_STAT register.
//
//*****************************************************************************
#define EPI_STAT_CELOW          0x00000200  // Clock Enable Low
#define EPI_STAT_XFFULL         0x00000100  // External FIFO Full
#define EPI_STAT_XFEMPTY        0x00000080  // External FIFO Empty
#define EPI_STAT_INITSEQ        0x00000040  // Initialization Sequence
#define EPI_STAT_WBUSY          0x00000020  // Write Busy
#define EPI_STAT_NBRBUSY        0x00000010  // Non-Blocking Read Busy
#define EPI_STAT_ACTIVE         0x00000001  // Register Active

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RFIFOCNT register.
//
//*****************************************************************************
#define EPI_RFIFOCNT_COUNT_M    0x0000000F  // FIFO Count
#define EPI_RFIFOCNT_COUNT_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO register.
//
//*****************************************************************************
#define EPI_READFIFO_DATA_M     0xFFFFFFFF  // Reads Data
#define EPI_READFIFO_DATA_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO1
// register.
//
//*****************************************************************************
#define EPI_READFIFO1_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO1_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO2
// register.
//
//*****************************************************************************
#define EPI_READFIFO2_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO2_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO3
// register.
//
//*****************************************************************************
#define EPI_READFIFO3_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO3_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO4
// register.
//
//*****************************************************************************
#define EPI_READFIFO4_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO4_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO5
// register.
//
//*****************************************************************************
#define EPI_READFIFO5_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO5_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO6
// register.
//
//*****************************************************************************
#define EPI_READFIFO6_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO6_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_READFIFO7
// register.
//
//*****************************************************************************
#define EPI_READFIFO7_DATA_M    0xFFFFFFFF  // Reads Data
#define EPI_READFIFO7_DATA_S    0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_FIFOLVL register.
//
//*****************************************************************************
#define EPI_FIFOLVL_WFERR       0x00020000  // Write Full Error
#define EPI_FIFOLVL_RSERR       0x00010000  // Read Stall Error
#define EPI_FIFOLVL_WRFIFO_M    0x00000070  // Write FIFO
#define EPI_FIFOLVL_WRFIFO_EMPT 0x00000000  // Trigger when there are any
                                            // spaces available in the WFIFO
#define EPI_FIFOLVL_WRFIFO_1_4  0x00000020  // Trigger when there are up to 3
                                            // spaces available in the WFIFO
#define EPI_FIFOLVL_WRFIFO_1_2  0x00000030  // Trigger when there are up to 2
                                            // spaces available in the WFIFO
#define EPI_FIFOLVL_WRFIFO_3_4  0x00000040  // Trigger when there is 1 space
                                            // available in the WFIFO
#define EPI_FIFOLVL_RDFIFO_M    0x00000007  // Read FIFO
#define EPI_FIFOLVL_RDFIFO_EMPT 0x00000000  // Empty
#define EPI_FIFOLVL_RDFIFO_1_8  0x00000001  // Trigger when there are 1 or more
                                            // entries in the NBRFIFO
#define EPI_FIFOLVL_RDFIFO_1_4  0x00000002  // Trigger when there are 2 or more
                                            // entries in the NBRFIFO
#define EPI_FIFOLVL_RDFIFO_1_2  0x00000003  // Trigger when there are 4 or more
                                            // entries in the NBRFIFO
#define EPI_FIFOLVL_RDFIFO_3_4  0x00000004  // Trigger when there are 6 or more
                                            // entries in the NBRFIFO
#define EPI_FIFOLVL_RDFIFO_7_8  0x00000005  // Trigger when there are 7 or more
                                            // entries in the NBRFIFO
#define EPI_FIFOLVL_RDFIFO_FULL 0x00000006  // Trigger when there are 8 entries
                                            // in the NBRFIFO

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_WFIFOCNT register.
//
//*****************************************************************************
#define EPI_WFIFOCNT_WTAV_M     0x00000007  // Available Write Transactions
#define EPI_WFIFOCNT_WTAV_S     0

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_IM register.
//
//*****************************************************************************
#define EPI_IM_WRIM             0x00000004  // Write FIFO Empty Interrupt Mask
#define EPI_IM_RDIM             0x00000002  // Read FIFO Full Interrupt Mask
#define EPI_IM_ERRIM            0x00000001  // Error Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_RIS register.
//
//*****************************************************************************
#define EPI_RIS_WRRIS           0x00000004  // Write Raw Interrupt Status
#define EPI_RIS_RDRIS           0x00000002  // Read Raw Interrupt Status
#define EPI_RIS_ERRRIS          0x00000001  // Error Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_MIS register.
//
//*****************************************************************************
#define EPI_MIS_WRMIS           0x00000004  // Write Masked Interrupt Status
#define EPI_MIS_RDMIS           0x00000002  // Read Masked Interrupt Status
#define EPI_MIS_ERRMIS          0x00000001  // Error Masked Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the EPI_O_EISC register.
//
//*****************************************************************************
#define EPI_EISC_WTFULL         0x00000004  // Write FIFO Full Error
#define EPI_EISC_RSTALL         0x00000002  // Read Stalled Error
#define EPI_EISC_TOUT           0x00000001  // Timeout Error

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RTCC register.
//
//*****************************************************************************
#define HIB_RTCC_M              0xFFFFFFFF  // RTC Counter
#define HIB_RTCC_S              0

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RTCM0 register.
//
//*****************************************************************************
#define HIB_RTCM0_M             0xFFFFFFFF  // RTC Match 0
#define HIB_RTCM0_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RTCM1 register.
//
//*****************************************************************************
#define HIB_RTCM1_M             0xFFFFFFFF  // RTC Match 1
#define HIB_RTCM1_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RTCLD register.
//
//*****************************************************************************
#define HIB_RTCLD_M             0xFFFFFFFF  // RTC Load
#define HIB_RTCLD_S             0

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_CTL register.
//
//*****************************************************************************
#define HIB_CTL_WRC             0x80000000  // Write Complete/Capable
#define HIB_CTL_VDD3ON          0x00000100  // VDD Powered
#define HIB_CTL_VABORT          0x00000080  // Power Cut Abort Enable
#define HIB_CTL_CLK32EN         0x00000040  // Clocking Enable
#define HIB_CTL_LOWBATEN        0x00000020  // Low Battery Monitoring Enable
#define HIB_CTL_PINWEN          0x00000010  // External WAKE Pin Enable
#define HIB_CTL_RTCWEN          0x00000008  // RTC Wake-up Enable
#define HIB_CTL_CLKSEL          0x00000004  // Hibernation Module Clock Select
#define HIB_CTL_HIBREQ          0x00000002  // Hibernation Request
#define HIB_CTL_RTCEN           0x00000001  // RTC Timer Enable

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_IM register.
//
//*****************************************************************************
#define HIB_IM_EXTW             0x00000008  // External Wake-Up Interrupt Mask
#define HIB_IM_LOWBAT           0x00000004  // Low Battery Voltage Interrupt
                                            // Mask
#define HIB_IM_RTCALT1          0x00000002  // RTC Alert 1 Interrupt Mask
#define HIB_IM_RTCALT0          0x00000001  // RTC Alert 0 Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RIS register.
//
//*****************************************************************************
#define HIB_RIS_EXTW            0x00000008  // External Wake-Up Raw Interrupt
                                            // Status
#define HIB_RIS_LOWBAT          0x00000004  // Low Battery Voltage Raw
                                            // Interrupt Status
#define HIB_RIS_RTCALT1         0x00000002  // RTC Alert 1 Raw Interrupt Status
#define HIB_RIS_RTCALT0         0x00000001  // RTC Alert 0 Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_MIS register.
//
//*****************************************************************************
#define HIB_MIS_EXTW            0x00000008  // External Wake-Up Masked
                                            // Interrupt Status
#define HIB_MIS_LOWBAT          0x00000004  // Low Battery Voltage Masked
                                            // Interrupt Status
#define HIB_MIS_RTCALT1         0x00000002  // RTC Alert 1 Masked Interrupt
                                            // Status
#define HIB_MIS_RTCALT0         0x00000001  // RTC Alert 0 Masked Interrupt
                                            // Status

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_IC register.
//
//*****************************************************************************
#define HIB_IC_EXTW             0x00000008  // External Wake-Up Masked
                                            // Interrupt Clear
#define HIB_IC_LOWBAT           0x00000004  // Low Battery Voltage Masked
                                            // Interrupt Clear
#define HIB_IC_RTCALT1          0x00000002  // RTC Alert1 Masked Interrupt
                                            // Clear
#define HIB_IC_RTCALT0          0x00000001  // RTC Alert0 Masked Interrupt
                                            // Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_RTCT register.
//
//*****************************************************************************
#define HIB_RTCT_TRIM_M         0x0000FFFF  // RTC Trim Value
#define HIB_RTCT_TRIM_S         0

//*****************************************************************************
//
// The following are defines for the bit fields in the HIB_DATA register.
//
//*****************************************************************************
#define HIB_DATA_RTD_M          0xFFFFFFFF  // Hibernation Module NV Data
#define HIB_DATA_RTD_S          0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FMA register.
//
//*****************************************************************************
#define FLASH_FMA_OFFSET_M      0x0003FFFF  // Address Offset
#define FLASH_FMA_OFFSET_S      0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FMD register.
//
//*****************************************************************************
#define FLASH_FMD_DATA_M        0xFFFFFFFF  // Data Value
#define FLASH_FMD_DATA_S        0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FMC register.
//
//*****************************************************************************
#define FLASH_FMC_WRKEY         0xA4420000  // FLASH write key
#define FLASH_FMC_COMT          0x00000008  // Commit Register Value
#define FLASH_FMC_MERASE        0x00000004  // Mass Erase Flash Memory
#define FLASH_FMC_ERASE         0x00000002  // Erase a Page of Flash Memory
#define FLASH_FMC_WRITE         0x00000001  // Write a Word into Flash Memory

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FCRIS register.
//
//*****************************************************************************
#define FLASH_FCRIS_PRIS        0x00000002  // Programming Raw Interrupt Status
#define FLASH_FCRIS_ARIS        0x00000001  // Access Raw Interrupt Status

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FCIM register.
//
//*****************************************************************************
#define FLASH_FCIM_PMASK        0x00000002  // Programming Interrupt Mask
#define FLASH_FCIM_AMASK        0x00000001  // Access Interrupt Mask

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FCMISC register.
//
//*****************************************************************************
#define FLASH_FCMISC_PMISC      0x00000002  // Programming Masked Interrupt
                                            // Status and Clear
#define FLASH_FCMISC_AMISC      0x00000001  // Access Masked Interrupt Status
                                            // and Clear

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FMC2 register.
//
//*****************************************************************************
#define FLASH_FMC2_WRKEY        0xA4420000  // FLASH write key
#define FLASH_FMC2_WRBUF        0x00000001  // Buffered Flash Memory Write

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FWBVAL register.
//
//*****************************************************************************
#define FLASH_FWBVAL_FWB_M      0xFFFFFFFF  // Flash Memory Write Buffer

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FCTL register.
//
//*****************************************************************************
#define FLASH_FCTL_USDACK       0x00000002  // User Shut Down Acknowledge
#define FLASH_FCTL_USDREQ       0x00000001  // User Shut Down Request

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_FWBN register.
//
//*****************************************************************************
#define FLASH_FWBN_DATA_M       0xFFFFFFFF  // Data

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_RMCTL register.
//
//*****************************************************************************
#define FLASH_RMCTL_BA          0x00000001  // Boot Alias

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_RMVER register.
//
//*****************************************************************************
#define FLASH_RMVER_CONT_M      0xFF000000  // ROM Contents
#define FLASH_RMVER_CONT_LM_AES2 \
                                0x05000000  // Stellaris Boot Loader &
                                            // DriverLib with AES
#define FLASH_RMVER_VER_M       0x0000FF00  // ROM Version
#define FLASH_RMVER_REV_M       0x000000FF  // ROM Revision
#define FLASH_RMVER_VER_S       8
#define FLASH_RMVER_REV_S       0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_USERDBG register.
//
//*****************************************************************************
#define FLASH_USERDBG_NW        0x80000000  // User Debug Not Written
#define FLASH_USERDBG_DATA_M    0x7FFFFFFC  // User Data
#define FLASH_USERDBG_DBG1      0x00000002  // Debug Control 1
#define FLASH_USERDBG_DBG0      0x00000001  // Debug Control 0
#define FLASH_USERDBG_DATA_S    2

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_BOOTCFG register.
//
//*****************************************************************************
#define FLASH_BOOTCFG_NW        0x80000000  // Not Written
#define FLASH_BOOTCFG_PORT_M    0x0000E000  // Boot GPIO Port
#define FLASH_BOOTCFG_PORT_A    0x00000000  // Port A
#define FLASH_BOOTCFG_PORT_B    0x00002000  // Port B
#define FLASH_BOOTCFG_PORT_C    0x00004000  // Port C
#define FLASH_BOOTCFG_PORT_D    0x00006000  // Port D
#define FLASH_BOOTCFG_PORT_E    0x00008000  // Port E
#define FLASH_BOOTCFG_PORT_F    0x0000A000  // Port F
#define FLASH_BOOTCFG_PORT_G    0x0000C000  // Port G
#define FLASH_BOOTCFG_PORT_H    0x0000E000  // Port H
#define FLASH_BOOTCFG_PIN_M     0x00001C00  // Boot GPIO Pin
#define FLASH_BOOTCFG_PIN_0     0x00000000  // Pin 0
#define FLASH_BOOTCFG_PIN_1     0x00000400  // Pin 1
#define FLASH_BOOTCFG_PIN_2     0x00000800  // Pin 2
#define FLASH_BOOTCFG_PIN_3     0x00000C00  // Pin 3
#define FLASH_BOOTCFG_PIN_4     0x00001000  // Pin 4
#define FLASH_BOOTCFG_PIN_5     0x00001400  // Pin 5
#define FLASH_BOOTCFG_PIN_6     0x00001800  // Pin 6
#define FLASH_BOOTCFG_PIN_7     0x00001C00  // Pin 7
#define FLASH_BOOTCFG_POL       0x00000200  // Boot GPIO Polarity
#define FLASH_BOOTCFG_EN        0x00000100  // Boot GPIO Enable
#define FLASH_BOOTCFG_DBG1      0x00000002  // Debug Control 1
#define FLASH_BOOTCFG_DBG0      0x00000001  // Debug Control 0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_USERREG0 register.
//
//*****************************************************************************
#define FLASH_USERREG0_NW       0x80000000  // Not Written
#define FLASH_USERREG0_DATA_M   0x7FFFFFFF  // User Data
#define FLASH_USERREG0_DATA_S   0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_USERREG1 register.
//
//*****************************************************************************
#define FLASH_USERREG1_NW       0x80000000  // Not Written
#define FLASH_USERREG1_DATA_M   0x7FFFFFFF  // User Data
#define FLASH_USERREG1_DATA_S   0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_USERREG2 register.
//
//*****************************************************************************
#define FLASH_USERREG2_NW       0x80000000  // Not Written
#define FLASH_USERREG2_DATA_M   0x7FFFFFFF  // User Data
#define FLASH_USERREG2_DATA_S   0

//*****************************************************************************
//
// The following are defines for the bit fields in the FLASH_USERREG3 register.
//
//*****************************************************************************
#define FLASH_USERREG3_NW       0x80000000  // Not Written
#define FLASH_USERREG3_DATA_M   0x7FFFFFFF  // User Data
#define FLASH_USERREG3_DATA_S   0

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DID0 register.
//
//*****************************************************************************
#define SYSCTL_DID0_VER_M       0x70000000  // DID0 Version
#define SYSCTL_DID0_VER_1       0x10000000  // Second version of the DID0
                                            // register format
#define SYSCTL_DID0_CLASS_M     0x00FF0000  // Device Class
#define SYSCTL_DID0_CLASS_TEMPEST \
                                0x00040000  // Stellaris(R) Tempest-class
                                            // microcontrollers
#define SYSCTL_DID0_MAJ_M       0x0000FF00  // Major Revision
#define SYSCTL_DID0_MAJ_REVA    0x00000000  // Revision A (initial device)
#define SYSCTL_DID0_MAJ_REVB    0x00000100  // Revision B (first base layer
                                            // revision)
#define SYSCTL_DID0_MAJ_REVC    0x00000200  // Revision C (second base layer
                                            // revision)
#define SYSCTL_DID0_MIN_M       0x000000FF  // Minor Revision
#define SYSCTL_DID0_MIN_0       0x00000000  // Initial device, or a major
                                            // revision update
#define SYSCTL_DID0_MIN_1       0x00000001  // First metal layer change
#define SYSCTL_DID0_MIN_2       0x00000002  // Second metal layer change

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DID1 register.
//
//*****************************************************************************
#define SYSCTL_DID1_VER_M       0xF0000000  // DID1 Version
#define SYSCTL_DID1_VER_1       0x10000000  // Second version of the DID1
                                            // register format
#define SYSCTL_DID1_FAM_M       0x0F000000  // Family
#define SYSCTL_DID1_FAM_STELLARIS \
                                0x00000000  // Stellaris family of
                                            // microcontollers, that is, all
                                            // devices with external part
                                            // numbers starting with LM3S
#define SYSCTL_DID1_PRTNO_M     0x00FF0000  // Part Number
#define SYSCTL_DID1_PRTNO_1811  0x00160000  // LM3S1811
#define SYSCTL_DID1_PINCNT_M    0x0000E000  // Package Pin Count
#define SYSCTL_DID1_PINCNT_100  0x00004000  // 100-pin package
#define SYSCTL_DID1_TEMP_M      0x000000E0  // Temperature Range
#define SYSCTL_DID1_TEMP_C      0x00000000  // Commercial temperature range (0C
                                            // to 70C)
#define SYSCTL_DID1_TEMP_I      0x00000020  // Industrial temperature range
                                            // (-40C to 85C)
#define SYSCTL_DID1_TEMP_E      0x00000040  // Extended temperature range (-40C
                                            // to 105C)
#define SYSCTL_DID1_PKG_M       0x00000018  // Package Type
#define SYSCTL_DID1_PKG_SOIC    0x00000000  // SOIC package
#define SYSCTL_DID1_PKG_QFP     0x00000008  // LQFP package
#define SYSCTL_DID1_PKG_BGA     0x00000010  // BGA package
#define SYSCTL_DID1_ROHS        0x00000004  // RoHS-Compliance
#define SYSCTL_DID1_QUAL_M      0x00000003  // Qualification Status
#define SYSCTL_DID1_QUAL_ES     0x00000000  // Engineering Sample (unqualified)
#define SYSCTL_DID1_QUAL_PP     0x00000001  // Pilot Production (unqualified)
#define SYSCTL_DID1_QUAL_FQ     0x00000002  // Fully Qualified

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC0 register.
//
//*****************************************************************************
#define SYSCTL_DC0_SRAMSZ_M     0xFFFF0000  // SRAM Size
#define SYSCTL_DC0_SRAMSZ_32KB  0x007F0000  // 32 KB of SRAM
#define SYSCTL_DC0_FLASHSZ_M    0x0000FFFF  // Flash Size
#define SYSCTL_DC0_FLASHSZ_256K 0x0000007F  // 256 KB of Flash
#define SYSCTL_DC0_SRAMSZ_S     16          // SRAM size shift
#define SYSCTL_DC0_FLASHSZ_S    0           // Flash size shift

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC1 register.
//
//*****************************************************************************
#define SYSCTL_DC1_WDT1         0x10000000  // Watchdog Timer1 Present
#define SYSCTL_DC1_ADC0         0x00010000  // ADC Module 0 Present
#define SYSCTL_DC1_MINSYSDIV_M  0x0000F000  // System Clock Divider
#define SYSCTL_DC1_MINSYSDIV_100 \
                                0x00001000  // Divide VCO (400MHZ) by 5 minimum
#define SYSCTL_DC1_MINSYSDIV_66 0x00002000  // Divide VCO (400MHZ) by 2*2 + 2 =
                                            // 6 minimum
#define SYSCTL_DC1_MINSYSDIV_50 0x00003000  // Specifies a 50-MHz CPU clock
                                            // with a PLL divider of 4
#define SYSCTL_DC1_MINSYSDIV_25 0x00007000  // Specifies a 25-MHz clock with a
                                            // PLL divider of 8
#define SYSCTL_DC1_MINSYSDIV_20 0x00009000  // Specifies a 20-MHz clock with a
                                            // PLL divider of 10
#define SYSCTL_DC1_ADC0SPD_M    0x00000300  // Max ADC0 Speed
#define SYSCTL_DC1_ADC0SPD_1M   0x00000300  // 1M samples/second
#define SYSCTL_DC1_MPU          0x00000080  // MPU Present
#define SYSCTL_DC1_HIB          0x00000040  // Hibernation Module Present
#define SYSCTL_DC1_TEMP         0x00000020  // Temp Sensor Present
#define SYSCTL_DC1_PLL          0x00000010  // PLL Present
#define SYSCTL_DC1_WDT0         0x00000008  // Watchdog Timer 0 Present
#define SYSCTL_DC1_SWO          0x00000004  // SWO Trace Port Present
#define SYSCTL_DC1_SWD          0x00000002  // SWD Present
#define SYSCTL_DC1_JTAG         0x00000001  // JTAG Present

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC2 register.
//
//*****************************************************************************
#define SYSCTL_DC2_EPI0         0x40000000  // EPI Module 0 Present
#define SYSCTL_DC2_COMP1        0x02000000  // Analog Comparator 1 Present
#define SYSCTL_DC2_COMP0        0x01000000  // Analog Comparator 0 Present
#define SYSCTL_DC2_TIMER3       0x00080000  // Timer Module 3 Present
#define SYSCTL_DC2_TIMER2       0x00040000  // Timer Module 2 Present
#define SYSCTL_DC2_TIMER1       0x00020000  // Timer Module 1 Present
#define SYSCTL_DC2_TIMER0       0x00010000  // Timer Module 0 Present
#define SYSCTL_DC2_I2C1         0x00004000  // I2C Module 1 Present
#define SYSCTL_DC2_I2C0         0x00001000  // I2C Module 0 Present
#define SYSCTL_DC2_SSI1         0x00000020  // SSI Module 1 Present
#define SYSCTL_DC2_SSI0         0x00000010  // SSI Module 0 Present
#define SYSCTL_DC2_UART2        0x00000004  // UART Module 2 Present
#define SYSCTL_DC2_UART1        0x00000002  // UART Module 1 Present
#define SYSCTL_DC2_UART0        0x00000001  // UART Module 0 Present

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC3 register.
//
//*****************************************************************************
#define SYSCTL_DC3_32KHZ        0x80000000  // 32KHz Input Clock Available
#define SYSCTL_DC3_CCP5         0x20000000  // CCP5 Pin Present
#define SYSCTL_DC3_CCP4         0x10000000  // CCP4 Pin Present
#define SYSCTL_DC3_CCP3         0x08000000  // CCP3 Pin Present
#define SYSCTL_DC3_CCP2         0x04000000  // CCP2 Pin Present
#define SYSCTL_DC3_CCP1         0x02000000  // CCP1 Pin Present
#define SYSCTL_DC3_CCP0         0x01000000  // CCP0 Pin Present
#define SYSCTL_DC3_ADC0AIN7     0x00800000  // ADC Module 0 AIN7 Pin Present
#define SYSCTL_DC3_ADC0AIN6     0x00400000  // ADC Module 0 AIN6 Pin Present
#define SYSCTL_DC3_ADC0AIN5     0x00200000  // ADC Module 0 AIN5 Pin Present
#define SYSCTL_DC3_ADC0AIN4     0x00100000  // ADC Module 0 AIN4 Pin Present
#define SYSCTL_DC3_ADC0AIN3     0x00080000  // ADC Module 0 AIN3 Pin Present
#define SYSCTL_DC3_ADC0AIN2     0x00040000  // ADC Module 0 AIN2 Pin Present
#define SYSCTL_DC3_ADC0AIN1     0x00020000  // ADC Module 0 AIN1 Pin Present
#define SYSCTL_DC3_ADC0AIN0     0x00010000  // ADC Module 0 AIN0 Pin Present
#define SYSCTL_DC3_C1O          0x00000800  // C1o Pin Present
#define SYSCTL_DC3_C1PLUS       0x00000400  // C1+ Pin Present
#define SYSCTL_DC3_C1MINUS      0x00000200  // C1- Pin Present
#define SYSCTL_DC3_C0O          0x00000100  // C0o Pin Present
#define SYSCTL_DC3_C0PLUS       0x00000080  // C0+ Pin Present
#define SYSCTL_DC3_C0MINUS      0x00000040  // C0- Pin Present

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC4 register.
//
//*****************************************************************************
#define SYSCTL_DC4_PICAL        0x00040000  // PIOSC Calibrate
#define SYSCTL_DC4_CCP7         0x00008000  // CCP7 Pin Present
#define SYSCTL_DC4_CCP6         0x00004000  // CCP6 Pin Present
#define SYSCTL_DC4_UDMA         0x00002000  // Micro-DMA Module Present
#define SYSCTL_DC4_ROM          0x00001000  // Internal Code ROM Present
#define SYSCTL_DC4_GPIOJ        0x00000100  // GPIO Port J Present
#define SYSCTL_DC4_GPIOH        0x00000080  // GPIO Port H Present
#define SYSCTL_DC4_GPIOG        0x00000040  // GPIO Port G Present
#define SYSCTL_DC4_GPIOF        0x00000020  // GPIO Port F Present
#define SYSCTL_DC4_GPIOE        0x00000010  // GPIO Port E Present
#define SYSCTL_DC4_GPIOD        0x00000008  // GPIO Port D Present
#define SYSCTL_DC4_GPIOC        0x00000004  // GPIO Port C Present
#define SYSCTL_DC4_GPIOB        0x00000002  // GPIO Port B Present
#define SYSCTL_DC4_GPIOA        0x00000001  // GPIO Port A Present

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC7 register.
//
//*****************************************************************************
#define SYSCTL_DC7_DMACH30      0x40000000  // SW
#define SYSCTL_DC7_DMACH29      0x20000000  // I2S0_TX / CAN1_TX
#define SYSCTL_DC7_DMACH28      0x10000000  // I2S0_RX / CAN1_RX
#define SYSCTL_DC7_DMACH27      0x08000000  // CAN1_TX / ADC1_SS3
#define SYSCTL_DC7_DMACH26      0x04000000  // CAN1_RX / ADC1_SS2
#define SYSCTL_DC7_DMACH25      0x02000000  // SSI1_TX / ADC1_SS1
#define SYSCTL_DC7_DMACH24      0x01000000  // SSI1_RX / ADC1_SS0
#define SYSCTL_DC7_DMACH23      0x00800000  // UART1_TX / CAN2_TX
#define SYSCTL_DC7_DMACH22      0x00400000  // UART1_RX / CAN2_RX
#define SYSCTL_DC7_DMACH21      0x00200000  // Timer1B / EPI0_WFIFO
#define SYSCTL_DC7_DMACH20      0x00100000  // Timer1A / EPI0_NBRFIFO
#define SYSCTL_DC7_DMACH19      0x00080000  // Timer0B / Timer1B
#define SYSCTL_DC7_DMACH18      0x00040000  // Timer0A / Timer1A
#define SYSCTL_DC7_DMACH17      0x00020000  // ADC0_SS3
#define SYSCTL_DC7_DMACH16      0x00010000  // ADC0_SS2
#define SYSCTL_DC7_DMACH15      0x00008000  // ADC0_SS1 / Timer2B
#define SYSCTL_DC7_DMACH14      0x00004000  // ADC0_SS0 / Timer2A
#define SYSCTL_DC7_DMACH13      0x00002000  // CAN0_TX / UART2_TX
#define SYSCTL_DC7_DMACH12      0x00001000  // CAN0_RX / UART2_RX
#define SYSCTL_DC7_DMACH11      0x00000800  // SSI0_TX / SSI1_TX
#define SYSCTL_DC7_DMACH10      0x00000400  // SSI0_RX / SSI1_RX
#define SYSCTL_DC7_DMACH9       0x00000200  // UART0_TX / UART1_TX
#define SYSCTL_DC7_DMACH8       0x00000100  // UART0_RX / UART1_RX
#define SYSCTL_DC7_DMACH7       0x00000080  // ETH_TX / Timer2B
#define SYSCTL_DC7_DMACH6       0x00000040  // ETH_RX / Timer2A
#define SYSCTL_DC7_DMACH5       0x00000020  // USB_EP3_TX / Timer2B
#define SYSCTL_DC7_DMACH4       0x00000010  // USB_EP3_RX / Timer2A
#define SYSCTL_DC7_DMACH3       0x00000008  // USB_EP2_TX / Timer3B
#define SYSCTL_DC7_DMACH2       0x00000004  // USB_EP2_RX / Timer3A
#define SYSCTL_DC7_DMACH1       0x00000002  // USB_EP1_TX / UART2_TX
#define SYSCTL_DC7_DMACH0       0x00000001  // USB_EP1_RX / UART2_RX

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_DC8 register.
//
//*****************************************************************************
#define SYSCTL_DC8_ADC0AIN7     0x00000080  // ADC Module 0 AIN7 Pin Present
#define SYSCTL_DC8_ADC0AIN6     0x00000040  // ADC Module 0 AIN6 Pin Present
#define SYSCTL_DC8_ADC0AIN5     0x00000020  // ADC Module 0 AIN5 Pin Present
#define SYSCTL_DC8_ADC0AIN4     0x00000010  // ADC Module 0 AIN4 Pin Present
#define SYSCTL_DC8_ADC0AIN3     0x00000008  // ADC Module 0 AIN3 Pin Present
#define SYSCTL_DC8_ADC0AIN2     0x00000004  // ADC Module 0 AIN2 Pin Present
#define SYSCTL_DC8_ADC0AIN1     0x00000002  // ADC Module 0 AIN1 Pin Present
#define SYSCTL_DC8_ADC0AIN0     0x00000001  // ADC Module 0 AIN0 Pin Present

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_PBORCTL register.
//
//*****************************************************************************
#define SYSCTL_PBORCTL_BORIOR   0x00000002  // BOR Interrupt or Reset

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_SRCR0 register.
//
//*****************************************************************************
#define SYSCTL_SRCR0_WDT1       0x10000000  // WDT1 Reset Control
#define SYSCTL_SRCR0_ADC0       0x00010000  // ADC0 Reset Control
#define SYSCTL_SRCR0_HIB        0x00000040  // HIB Reset Control
#define SYSCTL_SRCR0_WDT0       0x00000008  // WDT0 Reset Control

//*****************************************************************************
//
// The following are defines for the bit fields in the SYSCTL_SRCR1 register.
//
//***********************AMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    Â1 9 3 E A  î8 F C D . t   m p Â4 6 4 3 5  îD 8 D 0 7 0   A 2 ÂF S P 4 D  î1 2 2 F 8 C   7 C ÂSP4D1~1TMP   ı≤„@oA ı≤„@é∑E8  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP  MºÜoAoA  ΩÜoA      Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP  MºÜoAoA ΩÜoAí∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F     )ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇµˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ )ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇµˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP  MºÜoAoA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP  MºÜoAoA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP  MºÜoAoA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP   ı≤„@oA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    C1 6 A 4 E  nD 6 F 1 . t   m p 7 3 4 0 0  nC B 9 A F A   0 B F S P F E  n9 F 1 4 1 8   D 4 FSPFE9~1TMP   ı≤„@oA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    Â1 6 A 4 E  nD 6 F 1 . t   m p Â7 3 4 0 0  nC B 9 A F A   0 B ÂF S P F E  n9 F 1 4 1 8   D 4 ÂSPFE9~1TMP   ı≤„@oA ı≤„@í∑_†  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    CB 3 A E B  hF 0 7 7 . t   m p E 9 4 5 1  h9 8 8 1 F 6   0 A F S P 8 7  hA D E 9 3 D   D 3 FSP87A~1TMP  \ºÜoAoA  ΩÜoA      Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F    ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ‘ ®ñ‘ ®ñ‘            êô
             ê˘†!      ∞›	      ˇ    =#P|ñ‘ êô
 ¥I îñ‘ @>#P   ‘ò‘ w{ w{       Dñ‘ ‰G tß!Pw{ w{ ó‘         ó‘         ó‘                                                                                                                                                        ‰G Ã˘†   Ä    w{ w{     ¿ó‘ Ãó‘ Ãó‘ º˘†º˘†‘˘†º˘†‘˘†‰˘†>#P‘ó‘     »ò‘ ‘ò‘ ‘ò‘            pô
             ˙†!     |0             =#Pò‘ pô
 I ò‘            Äô
             <˙†!      ∞›	      ˇ    =#PPò‘ Äô
 `I ò‘ hò‘            ∞ô
             d˙†!     »›	            =#P†ò‘ ∞ô
 \J ò‘ hò‘ ∏ò‘ @>#P       w{ w{       Ãó‘ ‰G           |S5     $ô‘ w{ d0  &È  $              ®ñ‘     tß!Pw{ w{ tô‘         tô‘        tô‘                            úÓ” ‘°  µ                                                                                                                  4ô‘     w{         !    É d0              !    É d0  8ö‘              d0  8ö‘  ö‘                8ö‘ Pö‘ Pö‘                  ‘Pœ  ô     õ‘ w{ §´œ 'È  ! 0    tDÀ       à                       ô             å˙†   Ù`; ï‘ T™œ     Ãí‘     |S5 ¯ò‘ d0      ®ñ‘            ÿö‘ tß!Px{ x{ Xõ‘         Xõ‘         ¸ú‘                        ¸õ‘ ¥Ó” ¿Ó” ‹Ó” ËÓ” ÙÓ” ®†  Ñ» »°  0ß  ‘°  »°  »°  x†  x†  »°  »°  ê» ‡®  †» ‘°  ‘°  ‘°  ÿ†  »°  ∏» »®  –» »°  ‡» l†  ¯» Ú–            ®j… ‘Pœ tö‘ ¿æœ x{ |ú‘ l†  » l†   » l†  8» ‘°  ‘°  ÿ†  »°  H» »°  ‘°  l†  X» ‘°  ‘°  ‡°  p» Ù¶  |» T†  å» »°  ‘°  ‡°  ú» h°  »°  ®» Ï®  ¸ú‘ ∏» ÿ†  –» ‡®  ‡» ‘°  » ®†  ¸» º°  »°  ¥†  » ‘°  ‡°   » ú†  ,» 8» H» T†  X» \¢  h» »°  x» »°  ‘°  ‰†  à» l†      ò» ‘°  ‡°  ®» º°  »°  »°  ¥» º®  ƒ» ‘°  †  ¥†  ‹» `†  Ù¶  »°  Ù» ¥†  » »°  ‘°  ‘°  ‘°  ‡°  » ú†   Ô” ∞¢  Ô”     tö‘ õ‘ pë‘ tß!Pç{ ç{ »ù‘         »ù‘         »ù‘                                                                                                                                                        ‰G »˙†   Ä    ç{ ç{     pû‘ tû‘ tû‘ ∏˙†>#P|û‘     ‘û‘ ÿû‘ ÿû‘            êô
             ˙†!      ∞›	      ˇ    =#P¨û‘ êô
 ¥I ƒû‘ @>#P   °‘ ç{ ç{       tû‘ ‰G tß!Pç{ ç{ <ü‘         <ü‘         <ü‘                                                                                                                                                        ‰G ,˚†   Ä    ç{ ç{     ü‘ ¸ü‘ ¸ü‘ ˚†˚†4˚†˚†4˚†D˚†>#PAMP        <^l?π@ 9∏*@     AT y c o    bˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTYCO        âDú?ä@ UòH@Mf    Am a x s t  ˚r e a m   ˇˇ  ˇˇˇˇMAXSTR~1    }Njú?ä@ 8∏*@Íh    AP u l s e    ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇPULSE       K¸R#@÷@ UòH@◊l    AZ i l o g  Ä  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇZILOG       ΩëS&@ä@ :∏*@£p    AW h i r l  ºp o o l   ˇˇ  ˇˇˇˇWHIRLP~1    1<^l?ä@ =^l?
    AP a c k a  rg e s   ˇˇˇˇ  ˇˇˇˇPACKAGES    ´;å;@ä@ ∆ë=@ß•    Br e s s    wˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇF r o n t  wP a n e l E   x p FRONTP~1    |ççB@ä@ UòH@Í≠    AC U I - S  At a c k   ˇˇ  ˇˇˇˇCUI-ST~1    á\pT@Ω@ «aV@I∞    AP a n a s  Ço n i c   ˇˇ  ˇˇˇˇPANASO~1    Å∏É[@û@ 4s@≥    AM i c r o  xc h i p   ˇˇ  ˇˇˇˇMICROC~2    ÖËn\@ä@ .´]@{≥    ECS         √”Ä\@ä@ .´]@7π    AA b r a c  Ão m   ˇˇˇˇˇˇ  ˇˇˇˇABRACOM     ö'Å\@ä@ .´]@åπ    AS u l l i  cn s   ˇˇˇˇˇˇ  ˇˇˇˇSULLINS     ≠·Å\@°@ .´]@¡π    AC o i l t  2r o n i c s     ˇˇCOILTR~1    t…Ef@û@ 4s@x∫    AO s r a m  ƒ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOSRAM       ?Ii@ä@ 4s@√∫    AK o b e t  ƒo n e   ˇˇˇˇ  ˇˇˇˇKOBETONE    1¸xs@ä@ µs@˘⁄    AO m e g a  ê  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMEGA       Ppvë@ë@ qvë@"…    AP o l o l  wu   ˇˇˇˇˇˇˇˇ  ˇˇˇˇPOLOLU      gì@ì@ gì@E    SHARP      ®ÓE~@ä@ ÔE~@e¯    D6 0 9 6 b  ï. p d f   ˇˇ  ˇˇˇˇ- 9 1 0 2  ï- 7 9 f 8 3   5 7 7 4 9 - 0  ïc a 9 - 4 9   2 2 E l e k t  ïo r - 0 a 3   8 4 ELEKTO~1PDF  {zdp@-A {dp@ﬁ´g AS i l i c  o n   L a b   s   SILICO~1    ÜZv?ä@ Zv?1=    ROBOCLAW   <7fì@î@ 8fì@D    AO m r o n  ﬂ  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇOMRON       ùÔPª@ª@ ÛPª@"    AI n v e n  bs e n s e     ˇˇˇˇINVENS~1    EÈ@Í@ EÈ@     AI n f i n  *e o n   ˇˇˇˇ  ˇˇˇˇINFINEON    "”ÑÍ@Î@ ‘ÑÍ@√     Da a 3 5 5  }. d a t   ˇˇ  ˇˇˇˇ- 8 6 7 1  }- 0 4 8 d 1   6 e c b d - 6  }7 2 c - 4 5   9 0 S y n c T  }o y _ 0 b d   b c SYNCTO~2DAT ·xÎ@-A ‚xÎ@ƒ    AC o m c h  ∂i p   ˇˇˇˇˇˇ  ˇˇˇˇCOMCHIP     4ÄÎ@Î@ 8ÄÎ@«     AT o t a l  åP h a s e     ˇˇˇˇTOTALP~1    ô`NAA /mA–    CMSIS       √•aA*A ÆaAΩÖ    AS V N & T  +o r t o i s   e   SVN&TO~1    cöYA*A ˘aASè    GNS         J≠6A;A ó≠6A%‰    AP e r i c  ™o m   ˇˇˇˇˇˇ  ˇˇˇˇPERICOM     %+s{?ä@ –≠*@ø@    AA n d r o  fi d   ˇˇˇˇˇˇ  ˇˇˇˇANDROID     \Ô≤A*A Û≤AFÉ    AC   S t a  ‚n d a r d s     ˇˇCSTAND~1    íobA*A tbA°Ñ    D3 6 b 6 8  ]. d a t   ˇˇ  ˇˇˇˇ- 9 7 6 2  ]- 3 1 0 7 c   8 1 3 2 5 - 9  ]9 c 8 - 4 e   3 c S y n c T  ]o y _ c 0 3   d 2 SYNCTO~3DAT Q¢-A-A ¢-A◊   AL S   R e  Ns e a r c h     ˇˇLSRESE~1    ∞Òç0A;A ˘ç0ANæ    AT e l i t  •  ˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇTELIT       opÆ6A;A zÆ6A£Ê    TRIMBLE    E[µ7A;A íµ7AúÁ    Bn i n g S  `y s t e m     ˇˇˇˇG l o b a  `l P o s i t   i o GLOBAL~1    2å;ATA å;A©    Bg m   ˇˇˇˇ ”ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇV i s u a  ”l   P a r a   d i VISUAL~1    |UBATA ÒUBA^¯    Be c   ˇˇˇˇ –ˇˇˇˇˇˇˇˇˇˇˇˇ  ˇˇˇˇB o s c h  –- S e n s o   r t BOSCH-~1    ´ŸyWAWA zWAÅ    Aw x W i d  ÿg e t s   ˇˇ  ˇˇˇˇWXWIDG~1    huÜoAoA vÜoAÓú    ÂF S P 2 C  \9 6 C 9 E 1   A 9 ÂSP2C9~1TMP  §õ<bAdA ú<bA/Ÿs AU L   S t  †a n d a r d   s   ULSTAN~1    ∫—=XAZA Ÿ=XAOH    CB 3 A E B  hF 0 7 7 . t   m p E 9 4 5 1  h9 8 8 1 F 6   0 A F S P 8 7  hA D E 9 3 D   D 3 FSP87A~1TMP  \ºÜoAoA ΩÜoAù∑"  Â3 D 4 1 1  ¿4 8 D C 8 F   C B ÂF S P 8 7  ¿B 5 E E B C   5 D ÂSP87B~1TMP  ∞0¶á?ä@ nTQ<®Cu
 ÂE 0 0 7 4  ”E E 8 F . t   m p Â6 C 4 0 D  ”5 B 2 A 4 F   E E ÂF S P 6 1  ”2 8 B 0 7 1   B 0 ÂSP612~1TMP  ∂0¶á?ä@ Ú^Y<JDŸ2 Â8 7 6 D 4  E 0 4 3 . t   m p Â5 A 4 0 1  C B D B 7 7   0 F ÂF S P 4 0  1 B 2 C 4 B   1 4 ÂSP401~1TMP  #iaö?ä@ jaö?ŒDW ÂE C 8 9 E  Ñ2 0 F 6 . t   m p Â0 2 4 6 3  Ñ7 9 7 5 F 0   B 3 ÂF S P 4 0  ÑA 1 7 9 A 5   D 8 ÂSP40A~1TMP  Q¢aö?ä@ £aö?†E}è  AB o u r n  e s   ˇˇˇˇˇˇ  ˇˇˇˇBOURNES     .7Dú?‹@ ∆ë=@©E    ÂF C 1 C 9  ∏9 B A C . t   m p Â1 4 4 5 8  ∏7 9 A A D 2   2 C ÂF S P 0 9  ∏D F D 4 F E   A 8 ÂSP09D~1TMP  `7Dú?ä@ uzï?™En† ÂD 1 C 1 B  "5 4 1 1 . t   m p Â9 2 4 A 3  "C A C 1 8 F   B 6 ÂF S P 7 E  "B 9 8 3 D 0   9 F ÂSP7EB~1TMP  rEDú?ä@ 9âí?ÂE
Ë JST         uMDú?∑@ ¨¥N@§F    AM i c r o  »n   ˇˇˇˇˇˇˇˇ  ˇˇˇˇMICRON      -PDú?ä@ 6∏*@”F     2ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇF@†Ç ?¿(2  H  ¿     0P       Ñ                                                                                         ® —        \®Ω	     ˛  ˇ ,hFâ   p   
           ˇ    ®   ‰  †  †   ‰    †               ‰   
                 ö      J      I     _             ·            ˇ	    6ˇ                 ˇ
      ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ 2ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇF@†Ç ?¿(2  H  ¿     0P       Ñ                                                                                         ® —        \®Ω	     ˛  ˇ ,hFâ   p   
           ˇ    ®   ‰  †  †   ‰    †               ‰   
                 ö      J      I     _             ·            ˇ	    6ˇ                 ˇ
      ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ‡» Ù¶  Ï» T†  ¸» \¢  » »°  » l†  ,» ‘°  ‡°  <» Ù¶  H» T†  X» ‰†  h» ‡°  x» Ù¶  Ñ» T†  î» ‡°  §» ê‘ ∞» T†  ¿» ‰†  –» ‰†  ‡» ‡°  ¯» º°  »°  » »°  » l†  $» ‘°  ‘°  ‡°  4» ú†  @» ú†  L» ú†  X» d» T†  ¥†  t» ‡°      å» º°  »°  ò» »°  ®» »°  ‘°  ‘°  ‘°  ‡°  ∏» ú†  ÑÓ” ∞¢  êÓ”                                                             Ëã‘ åå‘ »Å‘ tß!Pw{ w{ ºë‘         ºë‘         ºë‘                                                                                                                                                        ‰G ¯†   Ä    w{ w{     dí‘ hí‘ hí‘ ¯˜†>#Ppí‘     »í‘ Ãí‘ Ãí‘            êô
             0¯†!      ∞›	      ˇ    =#P†í‘ êô
 ¥I ∏í‘ @>#P   ¯î‘ w{ w{       hí‘ ‰G tß!Pw{ w{ 0ì‘         0ì‘         0ì‘                                                                                                                                                        ‰G l¯†   Ä    w{ w{     ‰ì‘ ì‘ ì‘ \¯†\¯†t¯†\¯†t¯†Ñ¯†>#P¯ì‘     Ïî‘ ¯î‘ ¯î‘            pô
             ¥¯†!     |0             =#P(î‘ pô
 I @î‘            Äô
             ‹¯†!      ∞›	      ˇ    =#Ptî‘ Äô
 `I @î‘ åî‘            ∞ô
             ˘†!     »›	            =#Pƒî‘ ∞ô
 \J @î‘ åî‘ ‹î‘ @>#P       w{ w{       ì‘ ‰G           Ù`; ô‘ Hï‘ w{ T™œ %È  $              Ãí‘     tß!Pw{ w{ òï‘         òï‘         òï‘                                                                                                                                                        ‰G h˘†   Ä    w{ w{     @ñ‘ Dñ‘ Dñ‘ X˘†>#PLñ‘     §ñ