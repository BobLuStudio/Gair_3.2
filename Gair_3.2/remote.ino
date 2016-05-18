
bool fromremote()
{
  char commandFromRemote[11];
  if (receiveCommandFromRemote(commandFromRemote))
  {
      mode = (uint8_t)commandFromRemote[1];
      rstate[Xaxis] = floatStructor(2, commandFromRemote);
      rstate[Yaxis] = floatStructor(4, commandFromRemote);
      rstate[Zaxis] = floatStructor(6, commandFromRemote);
      tpower = (uint8_t)commandFromRemote[8];
      decodeMode(mode, modes);
    
    return 1;
  }
  return 0;
}


bool receiveCommandFromRemote(char* command)
{
  if (Serial3.available() > 11)
  {
    command[0] = Serial3.read();
    uint8_t serialCatch = Serial3.available();
    while (serialCatch > 10 || command[0] != '$')
    {
      command[0] = Serial3.read();
      serialCatch = Serial3.available();
      if (serialCatch == 0)
      {
        if (command[0] != '$')
          return 0;
      }
    }

    for (uint16_t c = 0;  serialCatch < 10; c++) //waiting for data
    {
      serialCatch = Serial3.available();
      if (c > 20000)
      {
        return 0;
      }
    }

    uint8_t checkByte = 0;
    for ( uint8_t i = 1; i <= 10; i++)
    {
      command[i] = Serial3.read();
      checkByte ^= command[i];
    }
    if (command[10] == '&' && checkByte == command[10])
      return 1;
  }
  return 0;
}


float floatStructor(int beginNumber, char command[12]) //recive the data begin space
{
  int16_t temp = 0;
  temp = (uint8_t)command[beginNumber];
  *((uint8_t*)(&temp) + 1) = (uint8_t)command[beginNumber + 1];
  return (float)temp / 100;
}

void decodeMode(uint8_t code, bool modeSwitch[4])
{
  for (uint8_t i = 0; i <= 3; i++)
 {
    modeSwitch[i] = (bool)((code & (1<<i))>>i);
  }
}

