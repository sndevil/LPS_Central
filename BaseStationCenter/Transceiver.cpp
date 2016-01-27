/*
* Transceiver.cpp
*
* Created: 8/1/2015 5:02:46 PM
* Author: CIT_15
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stddef.h>

#include "Transceiver.h"
#include "definition.h"
#include "GlobalVariables.h"
#include "functions.h"

// default constructor
Transceiver::Transceiver()
{
	preamble[0]=0x12;
	preamble[1]=0x34;
	preamble[2]=0x56;
	
} //Transceiver

// default destructor
Transceiver::~Transceiver()
{
} //~Transceiver

/*
void Transceiver::transmitionProcess()
{
	char dataTemp[DATA_LENGTH],packetTemp[PACKET_LENGTH + PREAMBLE_LENGTH];
	int availableDataLength=0,availableSerialLength=0;
	
	availableDataLength = dataBuffer.getFifoFullLength();
	if ( (availableDataLength > MAX_FIFO_LENGTH* 0.9))
	{
		setLED(2);
	}
	
	if ( availableDataLength > (3 * PACKET_LENGTH ))
	{
		
		;//ToggleLED(2);
	}
	else if (availableDataLength > (2 * PACKET_LENGTH ))
	{
		if ( availableSerialLength >= DATA_LENGTH )
		{
			
			serialTxDataBuffer.readString(dataTemp, DATA_LENGTH );
			transeiverPacket.createPacket(dataTemp , DATA_LENGTH , packetTemp );
			dataBuffer.writeString(packetTemp , PACKET_LENGTH + PREAMBLE_LENGTH );//PACKET_LENGTH +PREAMBLE_LENGTH
		}
	}
	else
	{
		serialTxDataBuffer.readString(dataTemp, availableSerialLength );
		transeiverPacket.createPacket(dataTemp , availableSerialLength , packetTemp );
		dataBuffer.writeString(packetTemp , PACKET_LENGTH + PREAMBLE_LENGTH );
	}
	
	return ;
	
}
*/
//////////////////////////////////////////////////////////////////////////

//void Transceiver::receptionProcess()
//{
	//char dataTemp[DATA_LENGTH],packetTemp[PACKET_LENGTH],i,test[20];
	//int availableDataLength=0,dataLength;
	//availableDataLength = dataBuffer.getFifoFullLength();
	//
	//if ( (availableDataLength > MAX_FIFO_LENGTH* 0.9))
	//{
		//setLED(2);
	//}
	//if ((serialTxDataBuffer.getFifoFullLength() > MAX_FIFO_LENGTH* 0.9))
	//{
		//setLED(1);
	//}
	//
	//if (availableDataLength < (PACKET_LENGTH ))
	//{
		//;//ToggleLED(2);
		//
	//}
	//else
	//{
		//dataBuffer.readString(packetTemp, PACKET_LENGTH );
		//
		//if (transeiverPacket.)
		//{
			//
			//receiverDataBuffer.writeString(dataTemp , dataLength);
			//receiverdataCheck();
			//
		//}
		//
	//}
	//
	//
	//return ;
//}

//////////////////////////////////////////////////////////////////////////

void Transceiver::setMode( TranceiverMode mode )
{
	myadf7020.setMode(mode);
}

void Transceiver::ADF_Program_Check( void )
{
	myadf7020.ADF_Program_Check();
}

uint16_t Transceiver::adfReadback( char readBackMode )
{
	return myadf7020.adfReadback(readBackMode);
}

void Transceiver::receiveInt1ISR()
{
	unsigned char temp;
	myadf7020.receiveInt1ISR();
	if (myadf7020.isNewDataAvailable())
	{
		temp =(myadf7020.receiveByte());
		temp ^= 0xAA ;
		dataBuffer.writeByte(temp);
		receivedByteCounter++;
		
		//serial.putChar(temp);
		
		//receivedByteCounter = PACKET_LENGTH ;
		if (receivedByteCounter > PACKET_LENGTH - 1 )
		{
			myadf7020.clearPreaambleDetect();
			receivedByteCounter = 0;
		}
	}
	

}

void Transceiver::receiveInt2ISR()
{
	myadf7020.receiveInt2ISR();
}

void Transceiver::transmitTimerISR()
{
	unsigned char temp;
	timeoutCounter ++;
	if (searchdelaycounterL > 0)
		searchdelaycounterL--;
	else if (searchdelaycounterH > 0)
	{
		searchdelaycounterL = searchdelaymaxL;
		searchdelaycounterH--;
	}
	//else
	//	serial.putChar('F');
		
	if(isReceiver())
		return;
	else
	{
		myadf7020.transmitTimerISR();

		if (isTransmitterReady())
		{
			
			if (transmittedByteCounter > 39 )
			{
				temp = dataBuffer.readByte();
				//serial.putChar(temp);
				temp ^= 0xAA;
			}
			else if (transmittedByteCounter == 37)
				temp = preamble[0];
			else if (transmittedByteCounter == 38)
				temp = preamble[1];
			else if (transmittedByteCounter == 39)
				temp = preamble[2];
			else
				temp = 0xAA;
			
			
			sendByte(temp);
			//serial.putChar(temp);
			transmittedByteCounter ++ ;
			//serial.putChar(temp);
		}
	}
	
	
}

void Transceiver::sendByte( char data )
{
	myadf7020.sendByte(data);
}

bool Transceiver::isReceiver()
{
	return myadf7020.isReceiver();
}

bool Transceiver::isTransmitterReady()
{
	return myadf7020.isTransmitterReady();
}

void Transceiver::changeMode( void )
{
	char intTemp;
	intTemp = resetAndStoreIntrruptFlag();
	
	if (isReceiver())
	{
		myadf7020.adf7020_ChangeMode(TRANSCEIVER_MODE_TRANSMITTER);
		dataBuffer.clearFifo();
		transmittedByteCounter = 0;
	}
	else
	{
		myadf7020.adf7020_ChangeMode(TRANSCEIVER_MODE_RECEIVER);
		serialTxDataBuffer.clearFifo();
		dataBuffer.clearFifo();
	}
	
	GIFR &=0x1F;
	
	restoreIntrrupt(intTemp);
}

bool Transceiver::writePacket( char *str )
{
	dataBuffer.writeString(str , PACKET_LENGTH );
	
	//add three byte for correct send data length
	dataBuffer.writeByte(0x00);
	dataBuffer.writeByte(0x00);
	dataBuffer.writeByte(0x00);
	
	return true;
}

//bool Transceiver::readPacket( char *str,int strSize )
//{
	//return true;
//}

int Transceiver::getFifoFullLength()
{
	return dataBuffer.getFifoFullLength();
}

bool Transceiver::readReceivedPacket( char *str )
{
	//cli();
	//for (int i=0 ; i<PACKET_LENGTH  ;i++ )
	//{
		//serial.putChar(dataBuffer.readByte());
	//}
	//sei();
	dataBuffer.readString(str , PACKET_LENGTH );
	return true;
}

void Transceiver::clearFifo()
{
	dataBuffer.clearFifo();
	return ;
}

//////////////////////////////////////////////////////////////////////////
bool Transceiver::isPreambleDetected()
{
	return myadf7020.isPreambleDetected();
}




///////////////////////////////////////////////////
ISR(INT1_vect)
{
	transceiver.receiveInt1ISR();
}

ISR(INT2_vect)
{
	transceiver.receiveInt2ISR();
}

ISR(TIMER1_COMPA_vect)
// Timer 1 output compare A interrupt service routine
//interrupt [TIM1_COMPA] void timer1_compa_isr(void)
{
	TCNT1H=0x00;
	TCNT1L=44;
	TCNT0 = 0;
	transceiver.transmitTimerISR();

	if (serial.isSerialRxFull())
	{
		serialRxDataBuffer.writeByte(UDR);//TODO: read serial
	}
}
//////////////////////////////////////////////////////////////////////////