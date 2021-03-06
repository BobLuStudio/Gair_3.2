
#include "COM.h"

#ifndef _COM_H
#define _COM_H

#define Xaxis 0
#define Yaxis 1
#define Zaxis 2

#define landMode 3
#define takeOffMode 2
#define keepMode 1
#define stopMode 0

struct Datas
{
 float rstate[3], ustate[3];
 unsigned _int8 tpower=0, mode=0;
 bool modes[4];
 bool remoteSignal,UAVSignal,commandCenterSignal;
};

comPort Serial = comPort(4);


unsigned _int8 receiveCommandFromCenter(char* command)
{
	command[0] =Serial.read();
	for (unsigned _int8 i = 0; command[0] != '$'&&i <= 20; i++)
		{
			command[0] = Serial.read();
		}
		if (command[0] == '$')
		{
			unsigned _int8 checkByte = 0;
			for (unsigned _int8 i = 1; i <= 10;i++)
			{
				command[i] = Serial.read();
				checkByte ^= command[i];
			}
			if (command[10] == '&'&&checkByte == command[10])return 11;
		}
		return 0;
}


float floatStructor(int beginNumber, char* command) //recive the data begin space
{
	_int16 temp = 0;
	temp = (unsigned _int8)command[beginNumber];
	*((char*)(&temp) + 1) = (char)command[beginNumber + 1];
	return (float)temp / 100;
}

void decodeMode(unsigned _int8 code, bool* modeSwitch)
{
	for (unsigned _int8 i = 0; i <= 3; i++)
	{
		modeSwitch[i] = (bool)((code & (1<<i))>>i);
	}
}
bool getData(Datas &data)
{
	char commandFromRemote[11]; 
	unsigned _int8 commandlength = 0;
	commandlength = receiveCommandFromCenter(commandFromRemote);

	if (commandlength == 11)
	{
		unsigned _int8 modeTest = (unsigned _int8)commandFromRemote[1];
		float rstateXTest = floatStructor(2, commandFromRemote);
		float rstateYTest = floatStructor(4, commandFromRemote);
		float rstateZTest = floatStructor(6, commandFromRemote);
		unsigned _int8 rtpowerTest = (unsigned _int8)commandFromRemote[8];
			if (rtpowerTest != 101)
			{
				data.rstate[Xaxis] = rstateXTest;
				data.rstate[Yaxis] = rstateYTest;
				data.rstate[Zaxis] = rstateZTest;
				data.tpower = rtpowerTest;
				data.mode = modeTest;
				decodeMode(data.mode, data.modes);
				if (data.rstate[Xaxis]  || data.rstate[Yaxis]  || data.rstate[Zaxis] )
					data.remoteSignal = 1;
				else
					data.remoteSignal = 0;
			}
			else
			{
				data.ustate[Xaxis] = rstateXTest;
				data.ustate[Yaxis] = rstateYTest;
				data.ustate[Zaxis] = rstateZTest;
				data.tpower = rtpowerTest;
				data.mode = modeTest;
				decodeMode(data.mode, data.modes);
				data.UAVSignal = 0;
				/*
				if (data.ustate[Xaxis] || data.ustate[Yaxis] || data.ustate[Zaxis])
					data.UAVSignal = 1;
				else
					data.UAVSignal = 0;
					*/
			}
		
		return 1;
	}
	else
	return 0;
	
}

bool fromUAV()
{
	return 0;
}

#endif