/*
 * GlobalVariables.cpp
 *
 * Created: 3/8/2015 12:47:10 PM
 *  Author: Mobarezi
 */ 

#include "GlobalVariables.h"
#include "Serial.h"
#include "Transceiver.h"
#include "ADF7020.h"

Serial serial;
//Message message;
//Station stations[10];
Packet packetProcessor;

//ADF7020 Board0 ;
Transceiver transceiver;

FIFO serialTxDataBuffer,serialRxDataBuffer;
StationInfo stationInfo;

char intrruptFlag, commandRecievedFlag, automaticSearchModeFlag ,minimumSearchBaseStationIndex =-1, maximumSearchBaseStationIndex=-1;

uint16_t timeoutCounter;
uint16_t searchingarray[32];
uint16_t searchpointer = 0,searchindex=-1;
uint16_t searchdelaycounterH, searchdelaycounterL;
uint16_t searchdelaymaxH, searchdelaymaxL;

//int32_t searchdelayhour=-1,searchdelayhourmax; //each 20counter = 1ms

char commandArray[MAXIMUM_NUMBER_OF_DATA + 5];