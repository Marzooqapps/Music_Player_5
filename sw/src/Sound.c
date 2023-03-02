//Sound.c
//Generates sounds and outputs it to a DAC
//Marzooq Shah
//Feb 28, 2023

#include <stdint.h>
#include <stdbool.h>
#include "Sound.h"
#include "inc/SysTickInts.h"
#include "inc/DAC.h"
#include "inc/EdgeInterruptPortF.h"

//Global Variables

uint16_t BeatIndex;
uint16_t NoteIndex;
uint16_t HowManyTimesEachNotePlays;
bool Play;


#define MAX_NOTES 25
#define MAX_BEATS 64
#define TIME_NOTE_PLAYS 40
//Instruments

const unsigned short Flute[64] = {  	
  1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,	
  1219,1092,1007,913,890,833,847,810,777,744,674,	
  598,551,509,476,495,533,589,659,758,876, 1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,	
  1219,1092,1007,913,890,833,847,810,777,744,674,	
  598,551,509,476,495,533,589,659,758,876	
};  	

const unsigned short Horn[64] = {  	
  1063,1082,1119,1275,1678,1748,1275,755,661,661,703,	
  731,769,845,1039,1134,1209,1332,1465,1545,1427,1588,	
  1370,1086,708,519,448,490,566,684,802,992,
	1063,1082,1119,1275,1678,1748,1275,755,661,661,703,	
  731,769,845,1039,1134,1209,1332,1465,1545,1427,1588,	
  1370,1086,708,519,448,490,566,684,802,992		
};  	

const unsigned short Trumpet[MAX_BEATS] = {  	
  1007,1088,1156,1194,1067,789,303,99,789,1510,1476,	
  1173,1067,1037,1084,1062,1011,1015,1045,1062,1011,1011,	
  1058,1113,1084,1075,1079,1105,1088,1049,1015,1045,
	1007,1088,1156,1194,1067,789,303,99,789,1510,1476,	
  1173,1067,1037,1084,1062,1011,1015,1045,1062,1011,1011,	
  1058,1113,1084,1075,1079,1105,1088,1049,1015,1045		
}; 

const unsigned short Wave[MAX_BEATS] = {  
  1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
  562,534,524,534,562,608,670,746,833,926,
	1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
  562,534,524,534,562,608,670,746,833,926
};  



//note song[MAX_NOTES] = {{C_2, Trumpet},{C_2, Trumpet},{C_2, Trumpet},{C_2, Trumpet},{C_2, Trumpet},{C_2, Trumpet},{C_2, Trumpet},{F0, Trumpet},{D_1, Trumpet},{A0, Horn},{A1, Flute},{C_2, Trumpet},{F0, Trumpet},{D_1, Trumpet},{A0, Horn},{A1, Flute}};

note song[MAX_NOTES] = {{G, Flute},{G, Flute},{A, Flute},{G, Flute},{C, Flute},{B, Flute},{G, Flute},{G, Flute},{A, Flute},{G, Flute},{D, Flute},{C, Flute},{G, Flute},{G, Flute},{G, Flute},{E, Flute}, {C, Flute},{B, Flute},{A, Flute},{F, Flute},{F, Flute},{E,Flute},{C, Flute},{D, Flute},{C, Flute}};


// **************Sound_Init*********************
// Initialize all the global variables
// Initialize SysTick and DAC

void Sound_Init(void){
	
	BeatIndex = 0;
	NoteIndex = 0;
	HowManyTimesEachNotePlays =0;
	Play = true;
	
	EdgePortF_Init(Sound_PlayPause, Sound_Rewind);
	SysTick_Init(song[0].frequency, Sound_Out);
	//SysTick_Init(D0, Sound_Out);
	
	dac_init();
	
}

//Test to be removed later

void Test_Sound_Out(void){

	dac_output(Flute[BeatIndex]);
	BeatIndex = (BeatIndex + 1) % MAX_BEATS;
}





//Outputs current value to DAC
//Changes BeatIndex and NoteIndex
void Sound_Out(void){
	
	dac_output(song[NoteIndex].instrument[BeatIndex]);
	
	if(BeatIndex >= MAX_BEATS - 1){
		BeatIndex = 0;
		//Play each note 20 times before switching as a test
		HowManyTimesEachNotePlays++;
		
		if(HowManyTimesEachNotePlays == TIME_NOTE_PLAYS){
			NoteIndex = (NoteIndex+1) % MAX_NOTES;
		
			//Change SysTick Frequency
			changeSysTick_period(song[NoteIndex].frequency);	
			HowManyTimesEachNotePlays =0;
		}
		
		
	}
	
	else{
		BeatIndex++;
	}

}

//Rewinds the sound by clearing BeatIndex and NoteIndex
//Changes the period of Systick.
void Sound_Rewind(void){
	
	BeatIndex = 0;
	NoteIndex = 0;
	HowManyTimesEachNotePlays =0;
	changeSysTick_period(song[NoteIndex].frequency);
}

//Plays sound if sound was paused
//and Pauses sound if sound was playing
void Sound_PlayPause(void){
	
	if(Play){
		SysTick_Disarm();
	}
	else{
		SysTick_Arm();
	}
	
	Play = !Play;
}