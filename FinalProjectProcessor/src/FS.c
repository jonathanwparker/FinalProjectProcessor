#include "cmsis_os.h"  // CMSIS RTOS header file
#include "Board_LED.h"
#include "UART_driver.h"
#include "stdint.h"                     // data type definitions
#include "stdio.h"                      // file I/O functions
#include "rl_usb.h"                     // Keil.MDK-Pro::USB:CORE
#include "rl_fs.h"                      // Keil.MDK-Pro::File System:CORE
#include "stm32f4xx_hal.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_audio.h"
#include "math.h"
#include "arm_math.h"
#include <stdio.h>
#include <stdlib.h>

//HI TERRY HOW IT GOING???

#define LED_Green   0
#define LED_Orange  1
#define LED_Red     2
#define LED_Blue    3

// Commands to Blue_Blink_Thread
enum Blue_Blink_Commands{ 
  Blue_Blink_Enable,
  Blue_Blink_Disable,
  Blue_Blink_Ex
};

////////////////////////////////////////////////
// State Machine definitions
enum state{
  NoState,
  Init,
  Idle,
  Play,
  Pause
};

enum Triggers{
  Show_Files,
  Files_Indexed,
  End_Stream,
  Play_Pressed,
  Resume_Pressed,
  Pause_Pressed,
  Stop_Pressed

};

enum Buffers{
  Buffer1,
  Buffer2

};

enum Actions{
	NoAction,
	List,
	PlayAction,
	PauseAction,
	ResumeAction,
	StopAction

};

// WAVE file header format
typedef struct WAVHEADER {
	unsigned char riff[4];						// RIFF string
	uint32_t overall_size;				// overall size of file in bytes
	unsigned char wave[4];						// WAVE string
	unsigned char fmt_chunk_marker[4];		// fmt string with trailing null char
	uint32_t length_of_fmt;					// length of the format data
	uint16_t format_type;					// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	uint16_t channels;						// no.of channels
	uint32_t sample_rate;					// sampling rate (blocks per second)
	uint32_t byterate;						// SampleRate * NumChannels * BitsPerSample/8
	uint16_t block_align;					// NumChannels * BitsPerSample/8
	uint16_t bits_per_sample;				// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned char data_chunk_header [4];		// DATA string or FLLR string
	uint32_t data_size;						// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
} WAVHEADER;

// Receive characters from the VB GUI 
#define Show_Files_char "1"
#define Send_File_char "4"
#define Stop_File_char "5"
#define Pause_File_char "6"
#define Resume_File_char "7"
char *StartFileList_msg = "2\n";
char *EndFileList_msg = "3\n";
char *StartFileData_msg = "4\n";
char *EndFileData_msg = "5\n";
char *Slide_num = "1\n";
char name[256];

osSemaphoreDef (SEM0);
osSemaphoreId  (SEM0_ID);

//////////////////////////////////////////////////////////
void Control (void const *argument); // thread function
osThreadId tid_Control; // thread id
osThreadDef (Control, osPriorityNormal, 1, 0); // thread object

void FS (void const *argument);
osThreadId tid_FS;
osThreadDef (FS,osPriorityNormal,1,0);

// Command queue from Rx_Command to Controller
osMessageQId mid_CMDQueue; // message queue for commands to Thread
osMessageQDef (CMDQueue, 1, uint32_t); // message queue object

// Command queue from Controller Blue_Blink_Thread
osMessageQId mid_list_files; // message queue for commands to Thread
osMessageQDef (list_files_queue, 1, uint32_t); // message queue object

osMessageQId mid_buffer_queue; // message queue for commands to Thread
osMessageQDef (buffer_queue, 1, uint32_t); // message queue object

osMessageQId mid_fs; // message queue for commands to Thread
osMessageQDef (fs, 1, uint32_t); // message queue object

// UART receive thread
void Rx_Command (void const *argument);  // thread function
osThreadId tid_RX_Command;  // thread id
osThreadDef (Rx_Command, osPriorityNormal, 1, 0); // thread object
                 // thread object

int Process_Event(uint16_t event){
  osEvent evt;
  static uint16_t   Current_State = NoState; // Current state of the SM
  switch(Current_State){
    case NoState:
      // Next State
      Current_State = Init;
    
      break;
    case Init:
      if(event == Files_Indexed){
        Current_State = Idle;
        return NoAction;
      }
      break;
    case Idle:
	  if(event == Play_Pressed){
		  Current_State = Play;
		  return PlayAction;
	  }
	  else{
	      return NoAction;
	  }
      break;
    case Play:
      if(event==Stop_Pressed){
    	  Current_State = Idle;
    	  return StopAction;
      }
      else if(event==Pause_Pressed){
    	  Current_State = Pause;
    	  return PauseAction;

      }
      else{
    	return NoAction;
      }
      break;
    case Pause:
    	if(event==Resume_Pressed){
    		Current_State=Play;
    		return ResumeAction;
    	}
    	else if(event==Stop_Pressed){
    		Current_State=Idle;
    		return StopAction;
    	}
    	else{
    		return NoAction;
    	}
      break;
    default:
      break;
  } // end case(Current_State)
  return NoAction;
} // Process_Event


void Init_Thread (void) {
   LED_Initialize(); // Initialize the LEDs
   UART_Init(); // Initialize the UART
   mid_CMDQueue = osMessageCreate (osMessageQ(CMDQueue), NULL);  // create msg queue
  if (!mid_CMDQueue)return; // Message Queue object not created, handle failure
  mid_list_files = osMessageCreate (osMessageQ(list_files_queue), NULL);  // create msg queue
  if (!mid_list_files)return; // Message Queue object not created, handle failure
  mid_buffer_queue = osMessageCreate (osMessageQ(buffer_queue), NULL);  // create msg queue
    if (!mid_buffer_queue)return; // Message Queue object not created, handle failure
  mid_fs = osMessageCreate (osMessageQ(fs), NULL);  // create msg queue
	if (!mid_fs)return; // Message Queue object not created, handle failure

  SEM0_ID = osSemaphoreCreate(osSemaphore(SEM0), 0);

  // Create threads
   tid_RX_Command = osThreadCreate (osThread(Rx_Command), NULL);
  if (!tid_RX_Command) return;
   tid_Control = osThreadCreate (osThread(Control), NULL);
  if (!tid_Control) return;
  tid_FS = osThreadCreate (osThread(FS), NULL);
  if (!tid_FS) return;
}

// Thread function
void Control(void const *arg){
  osEvent evt; // Receive message object
  Process_Event(0); // Initialize the State Machine
  int action;
   while(1){
    evt = osMessageGet (mid_CMDQueue, osWaitForever); // receive command
      if (evt.status == osEventMessage) { // check for valid message
    	  if(evt.value.v==Show_Files){
    		  osMessagePut(mid_list_files,List,osWaitForever);
    	  }
    	  else {
    	      action = Process_Event(evt.value.v); // Process event
    	      if(action==PlayAction||action==PauseAction||action==ResumeAction||action==StopAction){
    	    	  osMessagePut(mid_fs,action,osWaitForever);
    	      }
    	  }
    }
   }
}

void Rx_Command (void const *argument){
   char rx_char[2]={0,0};
   int i=0;
   while(1){
      UART_receive(rx_char, 1); // Wait for command from PC GUI
    // Check for the type of character received
      if(!strcmp(rx_char,Show_Files_char)){
         // Trigger1 received
         osMessagePut (mid_CMDQueue, Show_Files, osWaitForever);
      }
      if(!strcmp(rx_char,Send_File_char)){
    	  UART_receivestring(name,256);
    	  osMessagePut(mid_CMDQueue, Play_Pressed,osWaitForever);
      }
      if(!strcmp(rx_char,Pause_File_char)){
    	  osMessagePut(mid_CMDQueue,Pause_Pressed,osWaitForever);
      }
      if(!strcmp(rx_char,Resume_File_char)){
    	  osMessagePut(mid_CMDQueue,Resume_Pressed,osWaitForever);
      }
      if(!strcmp(rx_char,Stop_File_char)){
    	  osMessagePut(mid_CMDQueue,Stop_Pressed,osWaitForever);
      }
   }
} // end Rx_Command

#define NUM_CHAN	2 // number of audio channels
#define NUM_POINTS 1024 // number of points per channel
#define BUF_LEN NUM_CHAN*NUM_POINTS // length of the audio buffer
/* buffer used for audio play */
int16_t Audio_Buffer[BUF_LEN];
int16_t Audio_Buffer2[BUF_LEN];
float32_t cnt = 0.0f; // time index
float32_t freq = 200.0; // signal frequency in Hz
float32_t tmp; // factor
int endStream = 0;
int num=0;

void FS (void const *argument) {
	int totalCount = 1;
	usbStatus ustatus; // USB driver status variable
			uint8_t drivenum = 0; // Using U0: drive number
			char *drive_name = "U0:"; // USB drive name
			char *file_len;
			char *sampleRate;
			fsFileInfo info;
			fsStatus fstatus; // file system status variable
			WAVHEADER header;
			size_t rd;
			uint32_t i;
			static uint8_t rtrn = 0;
			uint8_t rdnum = 1; // read buffer number
			osEvent evt;

			uint32_t Fs = 44100.0; // sample frequency
			char* length[15];
			char* samRate[15];


			static FILE *f;

			ustatus = USBH_Initialize (drivenum); // initialize the USB Host
			if (ustatus == usbOK){
				// loop until the device is OK, may be delay from Initialize
				ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
				while(ustatus != usbOK){
					ustatus = USBH_Device_GetStatus (drivenum); // get the status of the USB device
				}
				// initialize the drive
				fstatus = finit (drive_name);
				if (fstatus != fsOK){
					// handle the error, finit didn't work
				} // end if
				// Mount the drive
				fstatus = fmount (drive_name);
				if (fstatus != fsOK){
					// handle the error, fmount didn't work
				} // end if
				// file system and drive are good to go
				f = fopen ("Kalimba.wav","r");// open a file on the USB device
				if (f != NULL) {
					fread((void *)&header, sizeof(header), 1, f);
				} // end if file opened
			} // end if USBH_Initialize

			while(evt.value.v!=List){
				evt = osMessageGet(mid_list_files,osWaitForever);
			}
			info.fileID = 0;
			UART_send(StartFileList_msg,2); // Send start string
			while(ffind("*.wav",&info)==fsOK){
				UART_send(info.name, strlen(info.name));
				UART_send("\n",1); // this is the VB string terminator "\n"
			}
			UART_send(EndFileList_msg,2); // Send start string
			osMessagePut (mid_CMDQueue, Files_Indexed, osWaitForever);



	tmp = 6.28f*freq/Fs; // only calc this factor once
	// initialize the audio output
	rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, 44100);
	if (rtrn != AUDIO_OK)return;

	// generate data for the audio buffer
		fread(&Audio_Buffer[2*i],sizeof(Audio_Buffer),1,f); // Left channel
		Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel

	// Start the audio player, size is number of bytes so mult. by 2
  BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer, BUF_LEN*2);
  fread(&Audio_Buffer2[2*i],sizeof(Audio_Buffer2),1,f); // Left channel
  		Audio_Buffer2[2*i+1] = Audio_Buffer2[2*i]; // Right channel
	osMessagePut (mid_buffer_queue, Buffer2,osWaitForever);
  int BuffNum=2;
  endStream = 0;
  while(1){
	  evt = osMessageGet(mid_fs,osWaitForever);
	        if (evt.status == osEventMessage) { // check for valid message
	        	switch(evt.value.v){
	        	case PlayAction:
	        		f = fopen (name,"r");// open a file on the USB device
	        		uint32_t sec;
					if (f != NULL) {
						fread((void *)&header, sizeof(header), 1, f);
						endStream=0;
//						snprintf(length, sizeof length, "%lu", (unsigned long)header.overall_size);
//						snprintf(samRate, sizeof samRate, "%lu", (unsigned long)header.sample_rate);
						UART_send(StartFileData_msg,2);
						UART_send((char *)(&header.overall_size),sizeof((char *)(&header.overall_size)));
						UART_send((char *)(&header.sample_rate),sizeof((char *)(&header.sample_rate)));
						UART_send(EndFileData_msg,2);

						BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
						rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, header.sample_rate);
							if (rtrn != AUDIO_OK)return;

						totalCount=fread(&Audio_Buffer[2*i],sizeof(Audio_Buffer),1,f); // Left channel
						Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel

						BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer, BUF_LEN*2);
						BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
					}

	        	case ResumeAction:
	        		if(evt.value.v==ResumeAction){
	        			BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_OFF);
						BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer, BUF_LEN);
						BuffNum=2;
						endStream=0;
	        		}
	        		else if(evt.value.v==StopAction){
	        			fclose(f);
	        		}
	        		while(endStream==0){
	        			osSemaphoreWait(SEM0_ID, osWaitForever);
						if(BuffNum==2){
							totalCount = fread(&Audio_Buffer2[2*i],sizeof(Audio_Buffer2),1,f); // Left channel
							Audio_Buffer2[2*i+1] = Audio_Buffer2[2*i]; // Right channel
							BuffNum=1;
							osMessagePut (mid_buffer_queue, Buffer2,osWaitForever);
							if(totalCount!=1){
								endStream=1;
							}
							}
						else if(BuffNum==1){
							totalCount = fread(&Audio_Buffer[2*i],sizeof(Audio_Buffer),1,f); // Left channel
							Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel
							BuffNum=2;
							osMessagePut (mid_buffer_queue, Buffer1,osWaitForever);
							if(totalCount!=1){
								endStream=1;
							}
						evt = osMessageGet(mid_fs,0);
						if (evt.status == osEventMessage) {
							if(evt.value.v==PauseAction){
								endStream=2;
								BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
							}
							else if(evt.value.v==StopAction){
								endStream=1;
							}

						}
						if(endStream==1){
							fclose(f);
							BSP_AUDIO_OUT_SetMute(AUDIO_MUTE_ON);
						}

	        		}

	        		}



	        	}
	      	  }


  }

  	}

/* User Callbacks: user has to implement these functions if they are needed. */
/* This function is called when the requested data has been completely transferred. */
void    BSP_AUDIO_OUT_TransferComplete_CallBack(void){
	osEvent evt; // Receive message object
	evt = osMessageGet (mid_buffer_queue,0); // receive command
  if (evt.status == osEventMessage) { // check for valid message
	  if(evt.value.v==Buffer1)
	  {
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer, BUF_LEN);
			osSemaphoreRelease(SEM0_ID);
	  }
	  else
	  {
			BSP_AUDIO_OUT_ChangeBuffer((uint16_t*)Audio_Buffer2, BUF_LEN);
			osSemaphoreRelease(SEM0_ID);
	  }
	}
  UART_send(Slide_num,2);

}

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void){
	UART_send(Slide_num,2);

}

/* This function is called when an Interrupt due to transfer error or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void){
		while(1){
		}
}


