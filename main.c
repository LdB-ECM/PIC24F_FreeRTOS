/*
 * File:   main.c
 * Author: ldb
 *
 * Created on 19 March 2019, 11:10 PM
 */

// System Includes
#include <xc.h>             // PIC header
#include <p24FJ128GA705.h>  // PIC header for 24FJ128GA705 CPU
#include <stdbool.h>		// C standard unit needed for bool and true/false
#include <stdint.h>			// C standard unit needed for uint8_t, uint32_t, etc
#include <stdio.h>

#define FOSC   32000000L
#define FCY    (FOSC/2)
#define BAUD_RATE_UART1    9600L


/* Task scheduler includes. */
#include "FreeRTOS/include/FreeRTOS.h"
#include "FreeRTOS/include/task.h"


// FOSCSEL
#pragma config FNOSC = FRCPLL // Oscillator Source Selection (Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) )
#pragma config PLLMODE = PLL4X // PLL Mode Selection (4x PLL selected)
#pragma config IESO = OFF // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)
 
// FOSC
#pragma config POSCMD = NONE // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFCN = ON // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config SOSCSEL = ON // SOSC Power Selection Configuration bits (SOSC is used in crystal (SOSCI/SOSCO) mode)
#pragma config PLLSS = PLL_FRC // PLL Secondary Selection Configuration bit (PLL is fed by the on-chip Fast RC (FRC) oscillator)
#pragma config IOL1WAY = OFF // Peripheral pin select configuration bit (Allow multiple reconfigurations)
#pragma config FCKSM = CSDCMD // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)
 
// FWDT
#pragma config WDTPS = PS32768 // Watchdog Timer Postscaler bits (1:32,768)
#pragma config FWPSA = PR128 // Watchdog Timer Prescaler bit (1:128)
#pragma config FWDTEN = OFF // Watchdog Timer Enable bits (WDT and SWDTEN disabled)
#pragma config WINDIS = OFF // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config WDTWIN = WIN25 // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config WDTCMX = WDTCLK // WDT MUX Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC // WDT Clock Source Select bits (WDT uses LPRC)
 
// FPOR
#pragma config BOREN = ON // Brown Out Enable bit (Brown Out Enable Bit)
#pragma config LPCFG = ON // Low power regulator control (Retention Sleep controlled by RETEN)
#pragma config DNVPEN = ENABLE // Downside Voltage Protection Enable bit (Downside protection enabled using ZPBOR when BOR is inactive)
 
// FICD
#pragma config ICS = PGD1 // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF // JTAG Enable bit (JTAG is disabled)
 
// FDEVOPT1
#pragma config ALTCMPI = DISABLE // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config SOSCHP = ON // SOSC High Power Enable bit (valid only when SOSCSEL = 1 (Enable SOSC high power mode (default))
#pragma config ALTI2C1 = ALTI2CEN // Alternate I2C pin Location (SDA1 and SCL1 on RB9 and RB8)
 

// Peripheral Pin Select I/O Lock Macro's

#define CLEAR_IOLOCK()  __builtin_write_OSCCONL(OSCCON & ~0x40)
#define SET_IOLOCK()    __builtin_write_OSCCONL(OSCCON | 0x40)


static void PIC_init(void)
{
    // 4x PLL increases internal 8 MHz oscillator to 32 MHz
     _PLLEN = 1;
     _RCDIV = 0;
    
     CLEAR_IOLOCK();
     
    //  PIC24FJ128GA705: 0 = Digital IO
    ANSA = 0;
    ANSB = 0;
    ANSC = 0;
    
    
    SET_IOLOCK();
 
}

#define HEART_BEAT(a)  (_LATC3 = (a & 0x1));

volatile int tick = 0;

void task1 (void *pParam) {
	while (1) 
	{
        HEART_BEAT(0);
		vTaskDelay(configTICK_RATE_HZ);
        tick++;
        HEART_BEAT(1);
        vTaskDelay(configTICK_RATE_HZ);
        tick++;
	}
}

int main(void) {
    PIC_init();    
   
    xTaskCreate(task1, "HEARTBEAT", 200, NULL, 3, NULL);
    vTaskStartScheduler();
    
    /* should never reach here*/
    while (1){};    
    return 0;
}

