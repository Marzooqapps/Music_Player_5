/**
 * @file Dump.c
 * @author your name (you@domain.com), Jonathan Valvano, Matthew Yu
 *    <TA NAME and LAB SECTION # HERE>
 * @brief 
 *    A set of debugging functions that capture data for later inspection.
 *    There are two primary methods:
 *       - DebugCapture will record data and time.
 *       - JitterMeasure will measure real time jitter.
 * @version 0.2
 * @date 2022-09-01 <REPLACE WITH DATE OF LAST REVISION>
 *
 * @copyright Copyright (c) 2022
 */

/** File includes. */
#include <stdint.h>
#include "../inc/Dump.h"
#include "../inc/tm4c123gh6pm.h"

// Global variables
uint32_t DumpTimeBuffer[DUMPBUFSIZE];
uint32_t DumpDataBuf[DUMPBUFSIZE];
uint32_t DumpNum;
uint32_t MaxJitter;
uint32_t MinJitter;
uint8_t FirstCall;
uint32_t previousJitter =0;

void Timer1_Init(void) {
    volatile uint32_t delay;
    SYSCTL_RCGCTIMER_R    |= 0x02;                // 0) activate TIMER1
    delay                  = SYSCTL_RCGCTIMER_R;  // allow time to finish activating
    TIMER1_CTL_R           = 0x00000000;          // 1) disable TIMER1A during setup
    TIMER1_CFG_R           = 0x00000000;          // 2) configure for 32-bit mode
    TIMER1_TAMR_R          = 0x00000002;          // 3) configure for periodic mode, COUNT-DOWN settings
    TIMER1_TAILR_R         = 0xFFFFFFFF;          // 4) reload value
    TIMER1_TAPR_R          = 0;                   // 5) bus clock resolution
    TIMER1_CTL_R           = 0x00000001;          // 10) enable TIMER1A
}

void DumpInit(void){
    /* TODO (EE445L Lab 2): complete this. */
    
    // What should dump init do?
    // - reset the dump memory so we don't see old data.
    // - reset or start any timing required for our DumpTimeBuffer.
}

void DumpCapture(uint32_t data){
    /* TODO (EE445L Lab 2): complete this. */
    
    // Use TIMER1_TAR_R as current time.
    // 
    // Note: how do you deal with timer overflow?
	
	uint32_t currentTime = TIMER1_TAR_R;
	
	if(DumpNum < DUMPBUFSIZE && currentTime > DumpTimeBuffer[DumpNum-1]){
		DumpTimeBuffer[DumpNum] = currentTime;
		DumpDataBuf[DumpNum] = data;
		DumpNum++;
	}
	
	
}

uint32_t DumpCount(void){ 
    /* TODO (EE445L Lab 2): complete this. */
    return DumpNum;
}

uint32_t* DumpData(void){
    return DumpDataBuf;
}

uint32_t* DumpTime(void){ 
    return DumpTimeBuffer;
}

void JitterInit(void){
    /* TODO (EE445L Lab 2): complete this. */
    
    // Note that jitter is independent of dump and should not 
    // share the same data structures. Create and reset your own 
    // variables indicating the measured jitter.
	
		MaxJitter = 0;
		MinJitter = 0xFFFFFFFF;
		FirstCall = 1;
}

void JitterMeasure(void){
    /* TODO (EE445L Lab 2): complete this. */
    
    // We'll define the jitter we want to measure as "peak-to-peak" 
    // jitter or "cycle-to-cycle" jitter; this is the maximum time 
    // difference between any two periodic measurements.
    //
    // We determine this by measuring the time of successive calls:
    // on the first time called, JitterMeasure just measures current 
    // time. On subsequent calls, JitterMeasure measures the elapsed time
    // from previous calls.
    // 
    // Note: how do you deal with timer overflow?
	
		uint32_t currentJitter = TIMER1_TAR_R;
	
	//if(currentJitter > previousJitter){				// Checking for Overflow
		if(FirstCall==1){
			previousJitter = currentJitter;
			FirstCall=0;
		}
		else{
			uint32_t timeElapsed = currentJitter - previousJitter;
			previousJitter = currentJitter;
			
			if(timeElapsed > MaxJitter){
				MaxJitter = timeElapsed;
			}
			if(timeElapsed < MinJitter){
				MinJitter = timeElapsed;
			}			
		}
		
	//}
}

uint32_t JitterGet(void){
    /* TODO (EE445L Lab 2): complete this. */
	return (MaxJitter - MinJitter);
}
