
#define reportPosition 10000

void printOut(char rsc)
{
  Serial3.print(rsc);//U can add some encryption algorithm here
}

void floatSplitter(char elements[2], float rsc)
{
	int16_t rscInt = rsc * 100;
	elements[0] = (char)rscInt;
	elements[1] = (char)*((uint8_t*)(&rscInt) + 1);
}


void ReportState()
{
      char commandString[6];
      floatSplitter(&commandString[0], rstate[Xaxis]);
      floatSplitter(&commandString[2], rstate[Yaxis]);
      floatSplitter(&commandString[4], rstate[Zaxis]);


      printOut('$');//0
      uint8_t checkByte = 0;
      for (uint8_t i = 0; i < 6; i++) //6 bytes
      {
        checkByte ^= commandString[i];
        printOut(commandString[i]);
      }
      printOut((char)checkByte);//7
      printOut('&');//8

}
void reportGPS()
{
	GPS.GetInform();

  
      char commandString[10];
	  commandString[0] = (char)GPS.speed_Knots; 
	  commandString[1] = (char)GPS.direction_North;
      commandString[2] = (char)GPS.longitude;
      commandString[3] = (char)*((uint8_t*)(&(GPS.longitude)) + 1);
      commandString[4] = (char)*((uint8_t*)(&(GPS.longitude)) + 2);
      commandString[5] = (char)*((uint8_t*)(&(GPS.longitude)) + 3);
      commandString[6] = (char)GPS.attitude;
      commandString[7] = (char)*((uint8_t*)(&(GPS.attitude)) + 1);
      commandString[8] = (char)*((uint8_t*)(&(GPS.attitude)) + 2);
      commandString[9] = (char)*((uint8_t*)(&(GPS.attitude)) + 3);


      printOut('$');//0
      uint8_t checkByte = 0;
      for (uint8_t i = 0; i < 9; i++) //9 bytes
      {
        checkByte ^= commandString[i];
        printOut(commandString[i]);
      }
      printOut((char)checkByte);//11
      printOut('&');//12
      
}
void Report(uint16_t index)
{
  switch (index)
  {
    case reportPosition: reportGPS();break;
    default: ReportState(); break;
  }
}

