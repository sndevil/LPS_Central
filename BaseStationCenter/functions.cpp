/*
* functions.cpp
*
* Created: 7/28/2015 9:20:16 AM
*  Author: CIT_15
*/
#include "functions.h"
#include "definition.h"
#include "GlobalVariables.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void micro_Initialize()
{
	PORTA=0x00;
	DDRA=0x0F;

	PORTB=0x10;
	DDRB=0x18;

	PORTC=0x00;
	DDRC=0x1D;

	PORTD=0x00;
	DDRD=0x82;

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: 7.813 kHz
	// Mode: Normal top=0xFF
	// OC0 output: Disconnected
	TCCR0=0x05;
	TCNT0=0x00;
	OCR0=0x00;
	
	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 8000.000 kHz
	// Mode: Normal top=0xFFFF
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: Off
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: On
	// Compare B Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x01;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x03;
	OCR1AL=0x41;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: Timer 2 Stopped
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	ASSR=0x00;
	TCCR2=0x00;
	TCNT2=0x00;
	OCR2=0x00;
	
	//if (transceiver1.isReceiver())
	//{
	//// External Interrupt(s) initialization
	//// INT0: Off
	//// INT1: On
	//// INT1 Mode: Rising Edge
	//// INT2: On
	//// INT2 Mode: Rising Edge
	//GICR|=0xA0;
	//MCUCR=0x0C;
	//MCUCSR=0x40;
	//GIFR=0xA0;
	//
	//}
	//else
	//{
	//// External Interrupt(s) initialization
	//// INT0: Off
	//// INT1: Off
	//// INT2: Off
	//MCUCR=0x00;
	//MCUCSR=0x00;
	//
	//}
	//
	//// Timer(s)/Counter(s) Interrupt(s) initialization
	//if (transceiver1.isReceiver())
	//TIMSK=0x00;
	//else
	//TIMSK=0x10;
	//
	//// Analog Comparator initialization
	//// Analog Comparator: Off
	//// Analog Comparator Input Capture by Timer/Counter 1: Off
	//if (transceiver1.isReceiver())
	//
	//{
	//ACSR=0x00;
	//SFIOR=0x00;
	//}
	//else
	//{
	ACSR=0x80;
	SFIOR=0x00;
	//}
	//
	//// Watchdog Timer initialization
	//// Watchdog Timer Prescaler: OSC/2048k
	//#pragma optsize-
	//WDTCR=0x1F;
	//WDTCR=0x0F;
	//#ifdef _OPTIMIZE_SIZE_
	//#pragma optsize+
	//#endif


}

//////////////////////////////////////////////////////////////////////////
void ToggleLED(char led_num)
{
	switch(led_num)
	{
		case 0:
		Led0_WR(!Led0_RD());
		break;
		case 1:
		Led1_WR(!Led1_RD());
		break;
		case 2:
		Led2_WR(!Led2_RD());
		break;
		case 3:
		Led3_WR(!Led3_RD());
		break;
		case 4:
		Led4_WR(!Led4_RD());
		break;
	}
	
	return ;
}
//////////////////////////////////////////////////////////////////////////
void setLED(char led_num)
{
	switch(led_num)
	{
		case 0:
		Led0_WR(1);
		break;
		case 1:
		Led1_WR(1);
		break;
		case 2:
		Led2_WR(1);
		break;
		case 3:
		Led3_WR(1);
		break;
		case 4:
		Led4_WR(1);
		break;
		default :
		Led0_WR(1);
		break;
	}
	
	return ;
}
//////////////////////////////////////////////////////////////////////////
void resetLED(char led_num)
{
	switch(led_num)
	{
		case 0:
		Led0_WR(0);
		break;
		case 1:
		Led1_WR(0);
		break;
		case 2:
		Led2_WR(0);
		break;
		case 3:
		Led3_WR(0);
		break;
		case 4:
		Led4_WR(0);
		break;
		default :
		Led0_WR(0);
		break;
	}
	
	return ;
}

//////////////////////////////////////////////////////////////////////////

void receivedDataProcess(char* receivedPacket, int dataLength)
{	
	serialTxDataBuffer.writeString(receivedPacket , dataLength );	
}



void setGlobalIntrrupt()
{
	intrruptFlag =1;
	sei();
}

void resetGlobalIntrrupt()
{
	intrruptFlag =0;
	cli();
}

char resetAndStoreIntrruptFlag()
{
	char temp=intrruptFlag;
	intrruptFlag = 0 ;
	cli();
	return temp;
}

void restoreIntrrupt(char intFlag)
{
	if (intFlag)
	{
		sei();
		intrruptFlag = 1 ;
	}
	else
	{
		cli();
		intrruptFlag = 0 ;
	}
}


/////////////////////////////////////////
void processSerialReceivedBytes()
{
	static unsigned char receivedData[MAXIMUM_NUMBER_OF_DATA+4]; //commandArray[MAXIMUM_NUMBER_OF_DATA+4],
	static int counter=0;
	char temp;
	
	if (serialRxDataBuffer.getFifoFullLength() == 0 || commandRecievedFlag == 1)
	return;
	temp = serialRxDataBuffer.readByte();
	receivedData[counter] = temp;
	

	if (counter == 0 && receivedData[0] != '$')
		counter = -1;
	if (counter == 1 && receivedData[1] != 'C')
		counter = -1;
	if (counter == 2 && receivedData[2] != 'M')
		counter = -1;
	if (counter == 3 && receivedData[3] != 'D')
		counter = -1;

	if(counter>3)
	{
		switch(receivedData[4])
		{
			/*			case START_CMD:
			if (counter > startCommandCount + 4)
			{
			DEBUGMESSAGE("Start command received\r\n");
			for (int i=0; i<nBASESTATIONS; ++i)
			baseStations[i].deassignFlag = true;


			manageTrackingModules();
			if (receivedData[0] == 'H')
			{
			}
			if (receivedData[0] == 'W')
			{
			}
			if (receivedData[0] == 'C')
			{
			CartesianCoordinate position;
			position.set(0, 0, 0);
			for (int i=0; i<nBASESTATIONS; ++i)
			baseStations[i].initialize(i, position, 0);


			}
			counter = -1;
			}
			break;

			case SERIAL_SETTING_CMD:
			if (counter > serialSettingsCommandCount + 4)
			{
			int serialPort = receivedData[0] - '1';
			int baudRate = receivedData[1];
			baudRate = (baudRate<<8) + receivedData[2];
			baudRate = (baudRate<<8) + receivedData[3];
			serial[serialPort]->setBaudRate(baudRate);
			settings.messageType[serialPort] = receivedData[4];
			settings.messageType[serialPort] <<= 8;
			settings.messageType[serialPort] += receivedData[5];
			DEBUGMESSAGE("Serial settings command received, baudRate: %d, Port: %d, messageType: %d\r\n", baudRate, serialPort, settings.messageType[serialPort]);
			counter = -1;
			}
			break;

			case decimationRateCommand:
			if (counter > decimationRateCommandCount + 4)
			{
			DEBUGMESSAGE("Decimation rate command received\r\n");
			settings.decimationRate = 50 / receivedData[0];
			
			counter = -1;
			}
			break;

			case DEASSIGN_SATELLITES_CMD:
			if (counter > deassignCommandCount + 4)
			{
			DEBUGMESSAGE("Deassign command received\r\n");
			long long int satelliteList;
			satelliteList = receivedData[0];
			satelliteList <<= 8;
			satelliteList |= receivedData[1];
			satelliteList <<= 8;
			satelliteList |= receivedData[2];
			satelliteList <<= 8;
			satelliteList |= receivedData[3];
			DEBUGMESSAGE("GPS satellite to be deassigned: %#lld\r\n", satelliteList);
			for (int i=0; i<nBASESTATIONS; ++i)
			if ((satelliteList & (1<<i)) == (1<<i))
			baseStations[i].deassignFlag = true;

			satelliteList = receivedData[4];
			satelliteList <<= 8;
			satelliteList |= receivedData[5];
			satelliteList <<= 8;
			satelliteList |= receivedData[6];
			satelliteList <<= 8;
			satelliteList |= receivedData[7];
			DEBUGMESSAGE("GLONASS satellite to be deassigned: %lld\r\n", satelliteList);


			manageTrackingModules();
			
			counter = -1;
			}
			break;

			case SEARCH_TYPE_CMD:
			if (counter > searchTypeCommandCount + 4)
			{
			DEBUGMESSAGE("Search type command received: No action\r\n");
			counter = -1;
			}
			break;

			case POSITIONING_TYPE_CMD:
			if (counter > positioningTypeCommandCount + 4)
			{
			DEBUGMESSAGE("Position type command received: No action\r\n");
			counter = -1;
			}
			break;

			case baseStationNumberCommand:
			if (counter > baseStationNumberCommandCount + 4)
			{
			DEBUGMESSAGE("BaseStation number command received\r\n");
			settings.numberOfBaseStationToSearch = receivedData[0];
			int nAll = 0;
			for (int i=0; i<nBASESTATIONS; ++i)
			{
			if (baseStations[i].isAssigned())
			++nAll;
			if (nAll > settings.numberOfBaseStationToSearch)
			baseStations[i].deassignFlag = true;
			}



			manageTrackingModules();
			counter = -1;
			}
			break;

			case PDOP_THRESHOLD_CMD:
			if (counter > gdopThresholdCommandCount + 4)
			{
			settings.pdopThreshold = receivedData[0];
			settings.pdopThreshold = settings.pdopThreshold*256 + receivedData[1];
			DEBUGMESSAGE("PDOP threshold command received: %f\r\n", settings.pdopThreshold);
			counter = -1;
			}
			break;

			case distanceErrorThresholdCommand:
			if (counter > distanceErrorThresholdCommandCount + 4)
			{
			settings.distanceErrorThreshold = receivedData[0];
			settings.distanceErrorThreshold = settings.distanceErrorThreshold*256 + receivedData[1];
			settings.distanceErrorThreshold /= 10.0;
			DEBUGMESSAGE("BaseStation distance error threshold command received: %f\r\n", settings.distanceErrorThreshold);
			counter = -1;
			}
			break;

			case deassignThresholdCommand:
			if (counter > deassignThresholdCommandCount + 4)
			{
			settings.trackingDeassignThreshold = receivedData[0]/4.0;
			DEBUGMESSAGE("GPS deassign threshold command received: %f\r\n", settings.trackingDeassignThreshold);
			counter = -1;
			}
			break;

			case GLONASS_DEASSIGN_THRESHOLD_CMD:
			if (counter > glonassDeassignThresholdCommandCount + 4)
			{
			DEBUGMESSAGE("GLONASS deassign threshold command received: No action\r\n");
			counter = -1;
			}
			break;

			case useThresholdCommand:
			if (counter > useThresholdCommandCount + 4)
			{
			settings.trackingUseThreshold = receivedData[0]/4.0;
			DEBUGMESSAGE("GPS used threshold command received: %f\r\n", settings.trackingUseThreshold);
			counter = -1;
			}
			break;

			case GPS_USE_THRESHOLD_CMD:
			if (counter > glonassUseThresholdCommandCount + 4)
			{
			DEBUGMESSAGE("GLONASS used threshold command received: No action\r\n");
			counter = -1;
			}
			break;

			case RELIABILITY_DEASSIGN_THRESHOLD_CMD:
			if (counter > reliabilityDeassignThresholdCommandCount + 4)
			{
			settings.trackingReliabilityDeassignThreshold = receivedData[0]/100.0;
			DEBUGMESSAGE("Reliability deassign threshold command received: %f\r\n", settings.trackingReliabilityDeassignThreshold);
			counter = -1;
			}
			break;

			case TROPOSPHORIC_CORRECTION_CMD:
			if (counter > useTroposphericCorrectionCommandCount + 4)
			{
			settings.useTroposphericCorrection = receivedData[0];
			DEBUGMESSAGE("Tropospheric correction command received: %d\r\n", settings.useTroposphericCorrection);
			counter = -1;
			}
			break;

			case IONOSPHORIC_CORRECTION_CMD:
			if (counter > useIonosphericCorrectionCommandCount + 4)
			{
			settings.useIonoposphericCorrection = receivedData[0];
			DEBUGMESSAGE("Ionospheric correction command received: %d\r\n", settings.useIonoposphericCorrection);
			counter = -1;
			}
			break;

			case MAX_SPEED_CMD:
			if (counter > maxSpeedCommandCount + 4)
			{
			int speed=0;
			speed = receivedData[0];
			speed <<= 8;
			speed |= receivedData[1];
			settings.maxSpeed = speed;
			DEBUGMESSAGE("maximum speed command received: %f\r\n", settings.maxSpeed);
			counter = -1;
			}
			break;

			case MAX_ACCELERATION_CMD:
			if (counter > maxAccelerationCommandCount + 4)
			{
			int acceleration=0;
			acceleration = receivedData[0];
			acceleration <<= 8;
			acceleration |= receivedData[1];
			settings.maxAcceleration = acceleration/10.0;
			DEBUGMESSAGE("maximum acceleration command received: %f\r\n", settings.maxAcceleration);
			counter = -1;
			}
			break;

			case GREEN_SAT_TYP_CMD:
			if (counter > greenTypeCommandCount + 4)
			{
			settings.greenBaseStationType = receivedData[0] == 0 ? UsedBaseStation : ReadyBaseStation;
			DEBUGMESSAGE("Green satellite type command received: %d\r\n", settings.greenBaseStationType);
			counter = -1;
			}
			break;

			case MASK_ANGLE_CMD:
			if (counter > maskAngleCommandCount + 4)
			{
			settings.elevationMask = (char)receivedData[0];
			DEBUGMESSAGE("Mask angle command received: %f\r\n", settings.elevationMask);
			counter = -1;
			}
			break;

			case AUTO_MAX_ANGLE_ATTITUDE_CMD:
			if (counter > automaticmaskAngleAtHighAttitudeCommandCount + 4)
			{
			int automaticMaskAngle = receivedData[0];
			DEBUGMESSAGE("Automatic mask angle command received: %d\r\n", automaticMaskAngle);
			counter = -1;
			}
			break;/**/

			case SAVE_SETTING_CMD:
			if (counter > saveSettingsCommandCount + 4)
			{
				commandArray[0]=START_PACKET_BYTE ;
				commandArray[1]=SAVE_SETTING_CMD;
				commandArray[2] = receivedData[5];
				for (char i =0; i <saveSettingsCommandCount  ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = saveSettingsCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;

			/*case READ_SETTING_CMD:
			if (counter > readSettingsCommandCount + 4)
			{
			//TODO: implement this function
			int readSettingFlag = 1;
			DEBUGMESSAGE("Read settings command received: %d\r\n", readSettingFlag);
			counter = -1;
			}
			break;/**/
			case BASE_STATION_SET_TIME_CMD:
			if (counter > baseStationSetTimeCommandCount + 4)
			{
				commandArray[0]=START_PACKET_BYTE ;
				commandArray[1]=BASE_STATION_SET_TIME_CMD;
				commandArray[2] = receivedData[5];
				for (char i =0; i <baseStationSetTimeCommandCount  ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = baseStationSetTimeCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			case BASE_STATION_SET_POS_CMD:
			if (counter > baseStationSetPositionCommandCount + 4)
			{
				commandArray[0]=START_PACKET_BYTE ;
				commandArray[1]=BASE_STATION_SET_POS_CMD;
				commandArray[2] = receivedData[5];
				for (char i =0; i < baseStationSetPositionCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = baseStationSetPositionCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			case BASE_STATION_GET_STATUS_CMD:
				if (counter > baseStationGetStatusCommandCount + 4)
				{
					//madeGetStatusCommand(receivedData[5]);
					commandArray[0]=START_PACKET_BYTE ;
					commandArray[1]=BASE_STATION_GET_STATUS_CMD;
					commandArray[2] = receivedData[5];
					for (char i =0; i < baseStationGetStatusCommandCount ; i++)
					{
						commandArray[3+i] = receivedData[6+i];
					}
					commandArray[MAXIMUM_NUMBER_OF_DATA] = baseStationGetStatusCommandCount + 3;
					commandRecievedFlag =1;
					counter = -1 ;
				}
			
			break;
			
			case BASE_STATION_RESET_LTR_CMD:
			if (counter > baseStationResetLTRCommandCount + 4)
			{
				commandArray[0]=START_PACKET_BYTE ;
				commandArray[1]= BASE_STATION_RESET_LTR_CMD;
				commandArray[2] = receivedData[5];
				for (char i =0; i < baseStationResetLTRCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = baseStationResetLTRCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			
			break;
			case AOUTOMATIC_SEARCH_ENABLE_CMD:
			if (counter > aoutomaticSearchEnableCommandCount + 4)
			{
				minimumSearchBaseStationIndex = receivedData[5];
				maximumSearchBaseStationIndex = receivedData[6];
				char mode = receivedData[7];
				char delay = receivedData[8];
				decodedelay(delay);
				searchindex=-1;
				
				if (mode == 'R')
				{
					ResetSearchArray();
					searchpointer = 0;
				}
				for (int i = minimumSearchBaseStationIndex; i <= maximumSearchBaseStationIndex; i++)
				{
					searchingarray[searchpointer] = i;
					//serial.putChar('X');
					//serial.putChar(searchpointer);
					//serial.putChar(searchingarray[searchpointer]);
					searchpointer++;
				}
				
				commandRecievedFlag =1;
				automaticSearchModeFlag = 1;
				counter = -1 ;
			}
			
			break;
			case AOUTOMATIC_SEARCH_DISABLE_CMD:
			if (counter > aoutomaticSearchDisableCommandCount + 4)
			{
				serial.putChar('R');
				minimumSearchBaseStationIndex = -1;
				maximumSearchBaseStationIndex = -1;
				searchindex = -1;
				automaticSearchModeFlag = 0;
				commandRecievedFlag =0;
				counter = -1 ;
			}
			break;
			case CHANGE_BASE_STATION_NUMBER_CMD:
			if (counter > changeBaseStationNumberCommandCount + 4)
			{
				commandArray[0] = START_PACKET_BYTE ;
				commandArray[1] = CHANGE_BASE_STATION_NUMBER_CMD;
				commandArray[2] =  receivedData[5];
				for (char i =0; i < changeBaseStationNumberCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = changeBaseStationNumberCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			case CHANGE_BASE_STATION_POSITIONING_MODE_CMD:
			if (counter > changeBseStationPositioningModeCommandCount + 4)
			{
				commandArray[0] = START_PACKET_BYTE ;
				commandArray[1] = CHANGE_BASE_STATION_POSITIONING_MODE_CMD;
				commandArray[2] =  receivedData[5];
				for (char i =0; i < changeBseStationPositioningModeCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = changeBseStationPositioningModeCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			case SET_RANGE_OFFSET_CMD:
			if (counter > setRangeOffsetCommandCount + 4)
			{
				commandArray[0] = START_PACKET_BYTE ;
				commandArray[1] = SET_RANGE_OFFSET_CMD;
				commandArray[2] =  receivedData[5];
				for (char i =0; i < setRangeOffsetCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = setRangeOffsetCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			case SET_LTR_TX_POWER_LEVEL_CMD:
			if (counter > setLTRTxPowerLevelCommandCount + 4)
			{
				commandArray[0] = START_PACKET_BYTE ;
				commandArray[1] = SET_LTR_TX_POWER_LEVEL_CMD;
				commandArray[2] =  receivedData[5];
				for (char i =0; i < setLTRTxPowerLevelCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = setLTRTxPowerLevelCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			case TURN_ON_OFF_LTR_CMD:
			if (counter > turnOnOffLTRCommandCount + 4)
			{
				commandArray[0] = START_PACKET_BYTE;
				commandArray[1] = TURN_ON_OFF_LTR_CMD;
				commandArray[2] =  receivedData[5];
				for (char i =0; i < turnOnOffLTRCommandCount ; i++)
				{
					commandArray[3+i] = receivedData[6+i];
				}
				commandArray[MAXIMUM_NUMBER_OF_DATA] = turnOnOffLTRCommandCount + 3;
				commandRecievedFlag =1;
				counter = -1 ;
			}
			break;
			

		}

	}

		if (counter>=MAXIMUM_NUMBER_OF_DATA+4)
			counter = -1;
		counter++;
}



void madeGetStatusCommand(char baseStationNumber)
{
	commandArray[0]=START_PACKET_BYTE ;
	commandArray[1]=SEND_STATUS;
	commandArray[2]= baseStationNumber;
	commandArray[MAXIMUM_NUMBER_OF_DATA] = 3;
			
}

void madeGetStatusCommandBaseOnMACAddress(char macNumber)
{
	commandArray[0]=START_PACKET_BYTE ;
	commandArray[1]=GET_STATUS_BASED_ON_MAC_CMD;
	commandArray[2]= macNumber;
	commandArray[MAXIMUM_NUMBER_OF_DATA] = 3;	
}


void sendAlliveSerialData()
{
	if (serialTxDataBuffer.getFifoFullLength() == 0 )
	{
		serialTxDataBuffer.writeByte(START_PACKET_BYTE);
		serialTxDataBuffer.writeByte(BIN_KEEP_ALIVE);
		serialTxDataBuffer.writeString("I AM ALIVE" , sizeof("I AM ALIVE"));	
	}
}

void ResetSearchArray()
{
	for (int i = 0; i < 32; i++)
		searchingarray[i] = -1;
}

void decodedelay(char input)
{
	searchdelaycounterH = 0;
	searchdelaycounterL = 0;
	if (input < 19) // ms
	{
		searchdelaymaxH = ((uint16_t)(input+1)*5000) / 65536;
		searchdelaymaxL = ((uint16_t)(input+1)*5000) % 65536;
	}
	else if (input < 138) // sec
	{
		uint32_t temp = ((uint32_t)(input-18)) * 20000;
		searchdelaymaxH = temp / 65536;
		searchdelaymaxL = temp % 65536;			
	}
	else if (input < 227) // minute
	{
			searchdelaymaxH = ((uint32_t)(input - 137) * 1200000) /65536;
			searchdelaymaxL = ((uint16_t)(input - 137) * 1200000);		
	}
	else // hour
	{
		if (input > 255)
			input = 255;
			
		uint16_t hour = (input - 227);
		
		searchdelaymaxH = ((uint32_t)(72000000 * hour)) / 65536;
		searchdelaymaxL = ((uint32_t)(72000000 * hour)) % 65536;
		
	}
}
