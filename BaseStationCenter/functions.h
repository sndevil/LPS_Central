/*
 * functions.h
 *
 * Created: 7/28/2015 9:19:59 AM
 *  Author: CIT_15
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void micro_Initialize();
void ToggleLED(char led_num);
void resetLED(char led_num);
void setLED(char led_num);
void loadBS();
void receivedDataProcess(char* receivedPacket , int dataLength);
void restoreIntrrupt(char intFlag);
char resetAndStoreIntrruptFlag();
void resetGlobalIntrrupt();
void setGlobalIntrrupt();

//void serialDataProcess();
void processSerialReceivedBytes();
void madeGetStatusCommand(char baseStationNumber);
void madeGetStatusCommandBaseOnMACAddress(char macNumber);
void sendAlliveSerialData();
void ResetSearchArray();
void decodedelay(char input);
#endif /* FUNCTIONS_H_ */