/*
* BaseStationCenter.cpp
*
* Created: 3/8/2015 9:35:52 AM
*  Author: Zamani
*/

#define F_CPU 8000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "GlobalVariables.h"
#include "definition.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "FIFO.h"
#include "Packet.h"
#include "StationInfo.h"
#include <avr/wdt.h>
#include <avr/eeprom.h>

char receivedPacket[PACKET_LENGTH +1] , commandPacket[PACKET_LENGTH + 1];



int main(void)
{
	char interruptFlag=0 , answerReceivedFlag = 0 ;
	char i ;
	uint16_t ret , rssivalue;
	
	int dataLength ;
	unsigned char baseStationNumber;
	uint8_t temp =0;
	//_delay_ms(5000);
	micro_Initialize();
	resetGlobalIntrrupt();
	ResetSearchArray();
	//while(1)
	//{
		//DDRB = 0xFF;
		//PORTB =0xFF;
	//}
	
	transceiver.setMode(TRANSCEIVER_MODE_RECEIVER);
	for (i=0;i<3 ;i++)
	{
		setLED(i);
		_delay_ms(50);
		resetLED(i);
		_delay_ms(50);
	}
	
	_delay_ms(100);
	if( (PIND & 0x04) >> 2)
	setLED(0);

	_delay_ms(100);
	setLED(0);
	setGlobalIntrrupt();
	
	baseStationNumber =0;
	//eeprom_write_byte(&test_index2 , test_index2);
	//
	//temp = eeprom_is_ready();
	////eeprom_write_byte(0x5,(uint8_t)0xA4);
	////eeprom_write_byte(&test_index2 , 0x9E);
	////test_index2	= 0x4D;
	//temp = eeprom_read_byte(&test_index2);
	////temp = eeprom_read_byte(&test_index2);
	//while(1)
	//{
	////serial.putChar(0x5A);
	//if (serialRxDataBuffer.getFifoFullLength() > 0)
	//{
	//ret = serialRxDataBuffer.readByte() ;
	//if (temp == ret)
	//{
	//;
	//}
	//else
	//eeprom_update_byte(&test_index2 , ret);
	//}
	//
	////temp = 0;
	////if ( temp == test_index2)
	////;
	////else
	//////test_index2 = temp;
	//serial.putChar(temp);
	////serial.putChar(test_index2);
	//}
	 minimumSearchBaseStationIndex =-1; 
	 maximumSearchBaseStationIndex =-1;
	 automaticSearchModeFlag = 0;	 
	while (1)
	{

		processSerialReceivedBytes();
		
		if ((serialTxDataBuffer.getFifoFullLength() > 0 ) )
			if (serial.isSerialTxEmpty())
				serial.putChar(serialTxDataBuffer.readByte());
				
		if(commandRecievedFlag)
			ToggleLED(0);
		if (transceiver.isReceiver())
		{
			
			if (transceiver.isPreambleDetected())
			{
				rssivalue = transceiver.adfReadback(RSSI_READBACK);
				//rssivalue = 0x34E5;
			}
			
			if (transceiver.getFifoFullLength() > PACKET_LENGTH - 1  )
			{
				
				transceiver.readReceivedPacket(receivedPacket);
				resetLED(2);
				
				if (packetProcessor.extractData(receivedPacket , dataLength))
				{			
					ToggleLED(1);
					StationInfoStructure* structPtr = (StationInfoStructure*) receivedPacket;
					structPtr->rssiCenterStation = rssivalue ;
					receivedDataProcess(receivedPacket , dataLength);
					answerReceivedFlag = 1;
				}
				
			}
			else if ((timeoutCounter > 2500) || answerReceivedFlag )   //timeout counter must be bigger than 1889 for baudrate 9600;
			{
				if(!commandRecievedFlag)
				{
					if (automaticSearchModeFlag)
					{
						if ( !answerReceivedFlag)
						{
							;//serialTxDataBuffer.writeString("$CMD" , sizeof("$CMD"));
							//serialTxDataBuffer.writeByte(0);
						}
						
						sendAlliveSerialData();

						if (searchdelaycounterL <= 0 && searchdelaycounterH <= 0)
						{
							ToggleLED(1);
							int nextindex = -1;
							for (int i = searchindex+1; i <= searchpointer; i++)
							{
								//serial.getChar(searchingarray[i]);
								if (searchingarray[i] != -1)
								{
									nextindex = i;
									break;
								}
							}
							if (nextindex != -1)
							{
								searchindex = nextindex;
								//serial.putChar('X');
								//serial.putChar(searchindex);
								//serial.putChar(searchingarray[searchindex]);
								madeGetStatusCommandBaseOnMACAddress(searchingarray[searchindex]);
							}
							else
							{
								searchindex = -1;
								searchdelaycounterH = searchdelaymaxH;
								searchdelaycounterL = searchdelaymaxL;
							}
														
							commandRecievedFlag = 1;
						}
						else
						{
							//if (searchdelaycounter > -1)
								//searchdelaycounter--;
							//else
								//searchdelaycounter = searchdelaymax;
							//if (searchdelaycounterH == 0)
							ToggleLED(2);
						}
						
					}
					 
				}
				else
				{
					if ( !answerReceivedFlag)
					{
						;//serialTxDataBuffer.writeString("$CMD" , sizeof("$CMD"));
						//serialTxDataBuffer.writeByte(0);
					}
					
				}

				answerReceivedFlag = 0;
				timeoutCounter =0;
				
				if (commandRecievedFlag)
				{
					interruptFlag = resetAndStoreIntrruptFlag();
					transceiver.changeMode();
					
					packetProcessor.createPacket(commandArray , commandArray[MAXIMUM_NUMBER_OF_DATA] , commandPacket);
					transceiver.writePacket(commandPacket);
					commandRecievedFlag	= 0;
					restoreIntrrupt(interruptFlag);
				}
				else
					sendAlliveSerialData();
			}
			else
			{
				;//_delay_us(10);
				
				
			}
		}
		else
		{
			setLED(2);
			if (transceiver.getFifoFullLength() == 0 )
				transceiver.changeMode();	
				//{
					//packetProcessor.createPacket((char *)commandRecievedFlag , 1 , commandPacket);
					//transceiver.writePacket(commandPacket);	
				//}//

		}
		
	}
}

	//***********************************************************************


