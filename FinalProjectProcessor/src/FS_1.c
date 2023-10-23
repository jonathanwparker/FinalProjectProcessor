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
  Idle,
  List,
};

enum Triggers{
  Show_Files,
  End_Stream

};

enum Buffers{
  Buffer1,
  Buffer2

};



// Receive characters from the VB GUI 
#define Show_Files_char "1"
#define Send_File_char "4"
char *StartFileList_msg = "2\n";
char *EndFileList_msg = "3\n";

osSemaphoreDef (SEM0);
osSemaphoreId  (SEM0_ID);

//////////////////////////////////////////////////////////
void Control (void const *argument); // thread function
osThreadId tid_Control; // thread id
osThreadDef (Control, osPriorityNormal, 1, 0); // thread object

void FS (void const *argument);
osThreadId tid_FS;
osThreadDef (FS,osPriorityAboveNormal,1,0);

// Command queue from Rx_Command to Controller
osMessageQId mid_CMDQueue; // message queue for commands to Thread
osMessageQDef (CMDQueue, 1, uint32_t); // message queue object

// Command queue from Controller Blue_Blink_Thread
osMessageQId mid_list_files; // message queue for commands to Thread
osMessageQDef (list_files_queue, 1, uint32_t); // message queue object

osMessageQId mid_buffer_queue; // message queue for commands to Thread
osMessageQDef (buffer_queue, 1, uint32_t); // message queue object



// UART receive thread
void Rx_Command (void const *argument);  // thread function
osThreadId tid_RX_Command;  // thread id
osThreadDef (Rx_Command, osPriorityNormal, 1, 0); // thread object

// Blue Blink thread
void listFiles (void const *argument);                             // thread function
osThreadId tid_listFiles;                                          // thread id
osThreadDef (listFiles, osPriorityNormal, 1, 0);                   // thread object

void Process_Event(uint16_t event){
  osEvent evt;
  static uint16_t   Current_State = NoState; // Current state of the SM
  switch(Current_State){
    case NoState:
      // Next State
      Current_State = Idle;
      // Exit actions
      // Transition actions
      // State1 entry actions
      LED_On(LED_Red);
    
      break;
    case Idle:
      if(event == Show_Files){
        Current_State = List;
        // Exit actions
        LED_Off(LED_Red);
        // Transition actions
        // State2 entry actions
        LED_On(LED_Green);
        osMessagePut (mid_list_files, 1, osWaitForever);
      }
      break;
    case List:
      if(event == End_Stream){
        Current_State = Idle;
        // Exit actions
        LED_Off(LED_Green);
        LED_On(LED_Red);
      }
      break;
    default:
      break;
  } // end case(Current_State)
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

  SEM0_ID = osSemaphoreCreate(osSemaphore(SEM0), 0);

  // Create threads
   tid_RX_Command = osThreadCreate (osThread(Rx_Command), NULL);
  if (!tid_RX_Command) return;
   tid_Control = osThreadCreate (osThread(Control), NULL);
  if (!tid_Control) return;
  tid_listFiles = osThreadCreate (osThread(listFiles), NULL);
  if (!tid_listFiles) return;
  tid_FS = osThreadCreate (osThread(FS), NULL);
  if (!tid_FS) return;
}

// Thread function
void Control(void const *arg){
  osEvent evt; // Receive message object
  Process_Event(0); // Initialize the State Machine
   while(1){
    evt = osMessageGet (mid_CMDQueue, osWaitForever); // receive command
      if (evt.status == osEventMessage) { // check for valid message
      Process_Event(evt.value.v); // Process event
    }
   }
}

void Rx_Command (void const *argument){
   char rx_char[2]={0,0};
   char name[256];
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
    	  i=1;
      }
   }
} // end Rx_Command

void listFiles(void const *arg){
  osEvent evt; // Receive message object
	fsFileInfo info;
	usbStatus ustatus; // USB driver status variable
	uint8_t drivenum = 0; // Using U0: drive number
	char *drive_name = "U0:"; // USB drive name
	fsStatus fstatus; // file system status
	static FILE *f;
	char count[400];
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
		info.fileID = 0;
		if (fstatus != fsOK){
			// handle the error, fmount didn't work
		}
	}
	while(1){
	evt = osMessageGet (mid_list_files, osWaitForever);
	info.fileID = 0;
	UART_send(StartFileList_msg,2); // Send start string
	while(ffind("*.txt",&info)==fsOK){
		UART_send(info.name, strlen(info.name));
		UART_send("\n",1); // this is the VB string terminator "\n"
	}
	UART_send(EndFileList_msg,2); // Send start string
	osMessagePut (mid_CMDQueue, End_Stream, osWaitForever);
	}
	} // while(1)

#define NUM_CHAN	2 // number of audio channels
#define NUM_POINTS 1024 // number of points per channel
#define BUF_LEN NUM_CHAN*NUM_POINTS // length of the audio buffer
/* buffer used for audio play */
int16_t Audio_Buffer[BUF_LEN];
int16_t Audio_Buffer2[BUF_LEN];
float32_t cnt = 0.0f; // time index
float32_t Fs = 8000.0; // sample frequency
float32_t freq = 200.0; // signal frequency in Hz
float32_t tmp; // factor

void FS (void const *argument) {
	static uint8_t rtrn = 0;  // return variable

	int16_t i; // loop counter

	tmp = 6.28f*freq/Fs; // only calc this factor once

	// initialize the audio output
	rtrn = BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, 0x46, 8000);
	if (rtrn != AUDIO_OK)return;

	// generate data for the audio buffer
 	for(i=0;i<NUM_POINTS;i++){
		Audio_Buffer[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
		Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel
		cnt = cnt + 1.0f;
	}
	// Start the audio player, size is number of bytes so mult. by 2
  BSP_AUDIO_OUT_Play((uint16_t *)Audio_Buffer, BUF_LEN*2);
  for(i=0;i<NUM_POINTS;i++){
    					Audio_Buffer2[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
    					Audio_Buffer2[2*i+1] = Audio_Buffer2[2*i]; // Right channel
    					cnt = cnt + 1.0f;
    				}
	osMessagePut (mid_buffer_queue, Buffer2,osWaitForever);
  int BuffNum=1;
  	while(1){
  		osSemaphoreWait(SEM0_ID, osWaitForever);
  		if(BuffNum==2){
  			for(i=0;i<NUM_POINTS;i++){
  					Audio_Buffer2[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
  					Audio_Buffer2[2*i+1] = Audio_Buffer2[2*i]; // Right channel
  					cnt = cnt + 1.0f;
  				}
  			BuffNum=1;
  			osMessagePut (mid_buffer_queue, Buffer2,osWaitForever);
  		}
  		else if(BuffNum==1){
  			for(i=0;i<NUM_POINTS;i++){
  					Audio_Buffer[2*i] = arm_cos_f32(tmp*cnt)*32000.0f; // Left channel
  					Audio_Buffer[2*i+1] = Audio_Buffer[2*i]; // Right channel
  					cnt = cnt + 1.0f;
  				}
  			BuffNum=2;
  			osMessagePut (mid_buffer_queue, Buffer1,osWaitForever);
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
}

/* This function is called when half of the requested buffer has been transferred. */
void    BSP_AUDIO_OUT_HalfTransfer_CallBack(void){

}

/* This function is called when an Interrupt due to transfer error or peripheral
   error occurs. */
void    BSP_AUDIO_OUT_Error_CallBack(void){
		while(1){
		}
}


