/* 
* Transceiver.h
*
* Created: 8/1/2015 5:02:47 PM
* Author: CIT_15
*/


#ifndef __TRANSCEIVER_H__
#define __TRANSCEIVER_H__

enum AD7020_READBACK_MODE
{
	AFC_READBACK,
	RSSI_READBACK,
	BATTERY_VLOTAGE_READBACK,
	TEMPERATURE_SENSOR_READBACK,
	ADCIN_VOLTAGE_READBACK,
	SILICON_REVISION_READBACK,
	FILTER_CALIBRETION_READBACK
};


#include <stdlib.h>

#include "Packet.h"
#include "ADF7020.h"
#include "FIFO.h"
#include "definition.h"
#include "functions.h"

class Transceiver
{
//variables
public:
protected:
 int  transmittedByteCounter;
 char preamble[PREAMBLE_LENGTH];
private:
	ADF7020 myadf7020;
	char receivedByteCounter;
	FIFO dataBuffer;
	void sendByte(char data);
	char receiveByte();
	void ADF_Program_Check(void);
	bool isNewDataAvailable();
	bool isTransmitterReady();
//functions
public:
	Transceiver();
	~Transceiver();
	void transmitionProcess();
	
	void setMode(TranceiverMode mode);
	void changeMode(void);
	
	uint16_t adfReadback(char readBackMode );
	void receiveInt1ISR();
	void receiveInt2ISR();
	void transmitTimerISR();
	
	bool writePacket(char *str);
	bool readPacket(char *str,int strSize);
	
	int getFifoFullLength();
	void clearFifo();
	
	bool readReceivedPacket(char *str);
	bool isPreambleDetected();
	bool isReceiver();
	
protected:
private:
	Transceiver( const Transceiver &c );
	Transceiver& operator=( const Transceiver &c );
	
	//void receptionProcess();
}; //Transceiver

#endif //__TRANSCEIVER_H__
