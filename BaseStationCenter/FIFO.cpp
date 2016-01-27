/* 
* FIFO.cpp
*
* Created: 8/1/2015 7:38:17 PM
* Author: CIT_15
*/


#include "FIFO.h"
#include "definition.h"
#include "GlobalVariables.h"
#include <avr/interrupt.h>
// default constructor
FIFO::FIFO()
{
	readIndex = 0;
	writeIndex = 0;
} //FIFO

// default destructor
FIFO::~FIFO()
{
} //~FIFO

char FIFO::readByte()
{
	char temp = 0;
	if(getFifoFullLength() > 0 )
	{
		temp = fifoBuffer[readIndex];
		readIndex ++ ;
		if ( readIndex > MAX_FIFO_LENGTH )
			readIndex =0;
	}
	return temp;
}

void FIFO::writeByte( char data )
{
	if (getFifoFullLength() < MAX_FIFO_LENGTH )
	{
		fifoBuffer [writeIndex] = data;
		writeIndex ++ ;
		if ( writeIndex > MAX_FIFO_LENGTH )
			writeIndex =0;
	}
	
}

void FIFO::readString( char *str,int strSize )
{
	char temp;
	if ( strSize > MAX_FIFO_LENGTH )
	{
		strSize = MAX_FIFO_LENGTH ;
	}
	
	while (strSize)
	{
		temp = readByte();
		*str = temp;
		str++;
		strSize --;
	};
}

void FIFO::writeString( char *str,int strSize )
{
	if ( strSize > MAX_FIFO_LENGTH )
	{
		strSize = MAX_FIFO_LENGTH ;
	}
	
	while (strSize)
	{
		writeByte(*str);
		str++;
		strSize --;
	};
}



int FIFO::getFifoFullLength()
{
	
	int temp=0;
	char intTemp= 0;
	
	intTemp = resetAndStoreIntrruptFlag();
	
	if (writeIndex >= readIndex)
		temp =	writeIndex - readIndex;
	else
		temp = MAX_FIFO_LENGTH + 1 - (readIndex - writeIndex);
		
	restoreIntrrupt( intTemp );
		return temp;
}

int FIFO::getFifoFreeLength()
{
	int temp;
	//cli();
	temp = (MAX_FIFO_LENGTH + 1 - getFifoFullLength());
	//sei();
	return temp;
}

void FIFO::clearFifo()
{
	writeIndex=readIndex=0;
}
