
#include <Arduino_HTS221.h>
// read all the sensor values
//float temperature = HTS.readTemperature();
//float humidity    = HTS.readHumidity();

#include <Arduino_LPS22HB.h>
// read the sensor value
// float pressure = BARO.readPressure();



#define FWver 220713
//************************************ Timer config************************************
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
bool timeForData = 0;
bool debug = 0;

//************************************ Serial parser config************************************

char serialString[64] = {0};         // a String to hold incoming data
char tempChars[64] = {0};        // temporary array for use when parsing
bool stringComplete = false;  // whether the string is complete
bool Receiving = false;       // 1 while receiving data
char stringidx = 0;

//************************************ Output config************************************
char V00, V01, V02, V03, V04, V05, V06, V07, V08 = 0; // Biary valve set
unsigned int  T00, T01, T02, T03, T04, T05, T06, T07, T08, T09, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21; //Temperature setpoints
unsigned int S00, S01 = 0;
bool Pump_cmd = false;
bool Data_cmd = false;

//************************************ Temporary stuff************************************
int HTStemp = 0;

//----------------------------------Void  setup --------------------------------------------

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.print("Automixer, FW version ");
  Serial.print(FWver, DEC);
  Serial.println(" ;");

  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  randomSeed(analogRead(0));
}

void loop() {

  currentMillis = millis();

  if ((currentMillis - previousMillis) > 1000)    // One second stuff
  {
    HTStemp = HTS.readTemperature();
    //Serial.println(HTStemp);
    previousMillis = currentMillis;
  }





  if (Serial.available())
  {
    // get the new byte:
    char inChar = (char)Serial.read();

    if ((inChar == '>') && (Receiving == true))
    {
      Receiving = false;
      stringComplete = true;
    }
    else if ((Receiving == true) && (inChar != '<'))
    {
      serialString[stringidx] = inChar;
      stringidx++;
    }
    else if (inChar == '<')
    {
      for (byte i = 0; i < 64; i++)   // Clear input buffer
      {
        serialString[i] = 0;
      }
      Receiving = true;
      stringidx = 0;
    };

  }



  //************************************ Decode Serial Command ************************************
  if (stringComplete) {
    strcpy(tempChars, serialString);  // Copy peceived string to temp buffer
    if (debug) {
      Serial.print("OK: ");
    }
    if (debug) {
      Serial.println(tempChars);
    }
    parseData();

    // clear the string:
    for (byte i = 0; i < 64; i++)
    {
      serialString[i] = 0;
      tempChars[i] = 0;
    }
    stringComplete = false;
  }

  if (timeForData)
  {
    // Here will be 1s update of actual parameters
    timeForData = false;
  }

  // wait 1 second to print again
  //delay(1000);

} // End of Void Loop


void SYSinit (void)
{

}

//---------------------------------- Parse data function --------------------------------------------
// Commands be like <GET_T01; SET_V30=11; SET_P=1; DATA_ON; PUMP_OFF>
// V for valve (2 valves on channel)
// T for temperature
// PUMP for pump
// DATA for auto update of data

void parseData() {      // split the data into its parts
  //Serial.println("Parsing...");

  char * strtokIndx; // this is used by strtok() as an index
  char parsedString[64] = {0};
  char DataString[10] = {0};
  bool completed = 0;
  unsigned int tmpval = 0;
  unsigned int selector = 0;

  strtokIndx = strtok(tempChars, "_");      // get the command word from string

  while (strtokIndx != NULL)
  {
    strcpy(parsedString, strtokIndx);         // Copy the command word from string for later comparison


    //---------------------------------------------  SET --------------------------------------------

    if (strcmp(parsedString, "SET") == 0)   // == 0 Because someone thought reverse logic will be fine :D
    {
      strtokIndx = strtok(NULL, "="); // this continues where the previous call left off, to find data between _ and =
      strcpy(DataString, strtokIndx);

      if (DataString[0] == 'T')   // Search for commmand in 1st data symbol
      {
        selector = (DataString[1] - 48) * 10 + (DataString[2] - 48);    // Easy conversion to int
        strtokIndx = strtok(NULL, ";");
        tmpval = atoi(strtokIndx);      // Convert it to integer
        if ((tmpval < 0) || (tmpval > 999))
        {
          tmpval = 0;
          if (debug) {
            Serial.println("Range Error");
          }
        }
        switch (selector)
        {
          case 0:
            {
              T00 = tmpval;
              break;
            }
          case 1:
            {
              T01 = tmpval;
              break;
            }
          case 2:
            {
              T02 = tmpval;
              break;
            }
          case 3:
            {
              T03 = tmpval;
              break;
            }
          case 4:
            {
              T04 = tmpval;
              break;
            }
          case 5:
            {
              T05 = tmpval;
              break;
            }
          case 6:
            {
              T06 = tmpval;
              break;
            }
          case 7:
            {
              T07 = tmpval;
              break;
            }
          case 8:
            {
              T08 = tmpval;
              break;
            }
          case 9:
            {
              T09 = tmpval;
              break;
            }
          case 10:
            {
              T10 = tmpval;
              break;
            }
          case 11:
            {
              T11 = tmpval;
              break;
            }
          case 12:
            {
              T12 = tmpval;
              break;
            }
          case 13:
            {
              T13 = tmpval;
              break;
            }
          case 14:
            {
              T14 = tmpval;
              break;
            }
          case 15:
            {
              T15 = tmpval;
              break;
            }
          case 16:
            {
              T16 = tmpval;
              break;
            }
          case 17:
            {
              T17 = tmpval;
              break;
            }
          case 18:
            {
              T18 = tmpval;
              break;
            }
          case 19:
            {
              T19 = tmpval;
              break;
            }
          case 20:
            {
              T20 = tmpval;
              break;
            }
          case 21:
            {
              T21 = tmpval;
              break;
            }
          default:
            {
              if (debug) {
                Serial.println("Selecor Error");
              }
              break;
            }
        }   // End of Switch Temp Selector

      } // End of If et Temperature

      else if (DataString[0] == 'V')
      {
        selector = (DataString[1] - 48) * 10 + (DataString[2] - 48);    // Easy conversion to int
        strtokIndx = strtok(NULL, ";");
        tmpval = atoi(strtokIndx);      // Convert it to integer
        if ((tmpval < 0) || (tmpval > 255))
        {
          tmpval = 0;
          if (debug) {
            Serial.println("Range Error");
          }
        }
        switch (selector)
        {
          case 0:
            {
              V00 = tmpval;
              break;
            }
          case 1:
            {
              V01 = tmpval;
              break;
            }
          case 2:
            {
              V02 = tmpval;
              break;
            }
          case 3:
            {
              V03 = tmpval;
              break;
            }
          case 4:
            {
              V04 = tmpval;
              break;
            }
          case 5:
            {
              V05 = tmpval;
              break;
            }
          case 6:
            {
              V06 = tmpval;
              break;
            }
          case 7:
            {
              V07 = tmpval;
              break;
            }
          case 8:
            {
              V08 = tmpval;
              break;
            }
          default:
            {
              if (debug) {
                Serial.println("Selecor Error");
              }
              break;
            }
        }   // End of Switch valve Selector

      }
      else
      {
        if (debug) {
          Serial.println("Wrong Command!");
        }
      }
      parsedString[0] = '\0';
      DataString[0] = '\0';

    } // End of "SET"

    //---------------------------------------------  GET --------------------------------------------
    else if (strcmp(parsedString, "GET") == 0)
    {
      strtokIndx = strtok(NULL, ";"); // this continues where the previous call left off, get data from input between _ and ;
      strcpy(DataString, strtokIndx);

      if (strcmp(DataString, "ALL") == 0)   // Print All
      {
        PrintAll();
      }
      else if (strcmp(DataString, "TEMP") == 0)   // Print All
      {
        PrintTemp();
      }
      else if (strcmp(DataString, "PRES") == 0)   // Print All
      {
        PrintPres();
      }
      else if (strcmp(DataString, "DATA") == 0)   // Print All
      {
        PrintData();
      }
      else
      {
        if (debug) {
          Serial.println("Wrong Command!");
        };
      }
      DataString[0] = '\0';
      parsedString[0] = '\0';

    }   // End of "GET"



    else if (strcmp(parsedString, "PUMP") == 0)    // Sample time external setting
    {
      strtokIndx = strtok(NULL, ";"); // this continues where the previous call left off, get data from input between _ and ;
      strcpy(DataString, strtokIndx);
      if (strcmp(DataString, "ON") == 0)
      {
        Pump_cmd = true;
        //Serial.println("Pump ON");
      }
      else if (strcmp(DataString, "OFF") == 0)
      {
        Pump_cmd = false;
        //Serial.println("Pump OFF");
      }
      else
      {
        //Serial.println("BAD PUMP");
      }

      DataString[0] = '\0';
      parsedString[0] = '\0';

    }   // End of "PUMP"
    else
    {
      if (debug) {
        Serial.println("Unknown command!");
      };
      parsedString[0] = '\0';
    };

    strtokIndx = strtok(NULL, "_;");      // get the command word from string

  } // End of while completed

}

void PrintAll(void)
{

  // Temporary debug
  HTStemp = 13; //HTS.readTemperature() + 0.5;
  int HTStempi = HTStemp;
  int Pressi = 990; //(BARO.readPressure() + 0.5)*10;
  Serial.print("V00=");
  Serial.print(V00, HEX);
  Serial.print(";V01=");
  Serial.print(V01, HEX);
  Serial.print(";V02=");
  Serial.print(V02, HEX);
  Serial.print(";V03=");
  Serial.print(V03, HEX);
  Serial.print(";V04=");
  Serial.print(V04, HEX);
  Serial.print(";V05=");
  Serial.print(V05, HEX);
  Serial.print(";V06=");
  Serial.print(V06, HEX);
  Serial.print(";V07=");
  Serial.print(V07, HEX);
  Serial.print(";V08=");
  Serial.print(V08, HEX);


  Serial.print(";T00=");
  Serial.print(T00 + random(0, 3), DEC);
  Serial.print(";T01=");
  Serial.print(T01 + random(0, 3), DEC);
  Serial.print(";T02=");
  Serial.print(T02 + random(0, 3), DEC);
  Serial.print(";T03=");
  Serial.print(T03 + random(0, 3), DEC);
  Serial.print(";T04=");
  Serial.print(T04 + random(0, 3), DEC);
  Serial.print(";T05=");
  Serial.print(T05 + random(0, 3), DEC);
  Serial.print(";T06=");
  Serial.print(T06 + random(0, 3), DEC);
  Serial.print(";T07=");
  Serial.print(T07 + random(0, 3), DEC);
  Serial.print(";T08=");
  Serial.print(T08 + random(0, 3), DEC);
  Serial.print(";T09=");
  Serial.print(T09 + random(0, 3), DEC);
  Serial.print(";T10=");
  Serial.print(T10 + random(0, 3), DEC);
  Serial.print(";T11=");
  Serial.print(T11 + random(0, 3), DEC);
  Serial.print(";T12=");
  Serial.print(T12 + random(0, 3), DEC);
  Serial.print(";T13=");
  Serial.print(T13 + random(0, 3), DEC);
  Serial.print(";T14=");
  Serial.print(T14 + random(0, 3), DEC);
  Serial.print(";T15=");
  Serial.print(T15 + random(0, 3), DEC);
  Serial.print(";T16=");
  Serial.print(T16 + random(0, 3), DEC);
  Serial.print(";T17=");
  Serial.print(T17 + random(0, 3), DEC);
  Serial.print(";T18=");
  Serial.print(T18 + random(0, 3), DEC);
  Serial.print(";T19=");
  Serial.print(T19 + random(0, 3), DEC);
  Serial.print(";T20=");
  Serial.print(T20 + random(0, 3), DEC);
  Serial.print(";T21=");
  Serial.print(T21 + random(0, 3), DEC);

  Serial.print(";P00=");
  Serial.print(Pressi);
  Serial.print(";P01=");
  Serial.print(Pressi);
  Serial.print(";P02=");
  Serial.print(Pressi);
  Serial.print(";P03=");
  Serial.print(Pressi);
  Serial.print(";P04=");
  Serial.print(Pressi);
  Serial.print(";P05=");
  Serial.print(Pressi);
  Serial.print(";P06=");
  Serial.print(Pressi);
  Serial.print(";P07=");
  Serial.print(Pressi);
  Serial.print(";P08=");
  Serial.print(Pressi);
  Serial.print(";P09=");
  Serial.print(Pressi);

  Serial.print(";S00=");
  Serial.print("00");
  Serial.print(";S01=");
  Serial.print("00");
  Serial.print(";PUMP=");
  Serial.print(Pump_cmd, BIN);
  Serial.println(";");
}


void PrintTemp(void)
{

  // Temporary debug
  HTStemp = 13; //HTS.readTemperature() + 0.5;
  int HTStempi = HTStemp;
  int Pressi = 990; //(BARO.readPressure() + 0.5)*10;

  Serial.print("T00=");
  Serial.print(T00 + random(0, 3), DEC);
  Serial.print(";T01=");
  Serial.print(T01 + random(0, 3), DEC);
  Serial.print(";T02=");
  Serial.print(T02 + random(0, 3), DEC);
  Serial.print(";T03=");
  Serial.print(T03 + random(0, 3), DEC);
  Serial.print(";T04=");
  Serial.print(T04 + random(0, 3), DEC);
  Serial.print(";T05=");
  Serial.print(T05 + random(0, 3), DEC);
  Serial.print(";T06=");
  Serial.print(T06 + random(0, 3), DEC);
  Serial.print(";T07=");
  Serial.print(T07 + random(0, 3), DEC);
  Serial.print(";T08=");
  Serial.print(T08 + random(0, 3), DEC);
  Serial.print(";T09=");
  Serial.print(T09 + random(0, 3), DEC);
  Serial.print(";T10=");
  Serial.print(T10 + random(0, 3), DEC);
  Serial.print(";T11=");
  Serial.print(T11 + random(0, 3), DEC);
  Serial.print(";T12=");
  Serial.print(T12 + random(0, 3), DEC);
  Serial.print(";T13=");
  Serial.print(T13 + random(0, 3), DEC);
  Serial.print(";T14=");
  Serial.print(T14 + random(0, 3), DEC);
  Serial.print(";T15=");
  Serial.print(T15 + random(0, 3), DEC);
  Serial.print(";T16=");
  Serial.print(T16 + random(0, 3), DEC);
  Serial.print(";T17=");
  Serial.print(T17 + random(0, 3), DEC);
  Serial.print(";T18=");
  Serial.print(T18 + random(0, 3), DEC);
  Serial.print(";T19=");
  Serial.print(T19 + random(0, 3), DEC);
  Serial.print(";T20=");
  Serial.print(T20 + random(0, 3), DEC);
  Serial.print(";T21=");
  Serial.print(T21 + random(0, 3), DEC);
  Serial.println(";");
}
void PrintPres(void)
{

  // Temporary debug
  HTStemp = 13; //HTS.readTemperature() + 0.5;
  int HTStempi = HTStemp;
  int Pressi = 990; //(BARO.readPressure() + 0.5)*10;

  Serial.print("P00=");
  Serial.print(Pressi);
  Serial.print(";P01=");
  Serial.print(Pressi);
  Serial.print(";P02=");
  Serial.print(Pressi);
  Serial.print(";P03=");
  Serial.print(Pressi);
  Serial.print(";P04=");
  Serial.print(Pressi);
  Serial.print(";P05=");
  Serial.print(Pressi);
  Serial.print(";P06=");
  Serial.print(Pressi);
  Serial.print(";P07=");
  Serial.print(Pressi);
  Serial.print(";P08=");
  Serial.print(Pressi);
  Serial.print(";P09=");
  Serial.print(Pressi);
  Serial.println(";");
}

void PrintData(void)
{

  // Temporary debug
  HTStemp = 13; //HTS.readTemperature() + 0.5;
  int HTStempi = HTStemp;
  int Pressi = 990; //(BARO.readPressure() + 0.5)*10;

  Serial.print("T00=");
  Serial.print(T00 + random(0, 3), DEC);
  Serial.print(";T01=");
  Serial.print(T01 + random(0, 3), DEC);
  Serial.print(";T02=");
  Serial.print(T02 + random(0, 3), DEC);
  Serial.print(";T03=");
  Serial.print(T03 + random(0, 3), DEC);
  Serial.print(";T04=");
  Serial.print(T04 + random(0, 3), DEC);
  Serial.print(";T05=");
  Serial.print(T05 + random(0, 3), DEC);
  Serial.print(";T06=");
  Serial.print(T06 + random(0, 3), DEC);
  Serial.print(";T07=");
  Serial.print(T07 + random(0, 3), DEC);
  Serial.print(";T08=");
  Serial.print(T08 + random(0, 3), DEC);
  Serial.print(";T09=");
  Serial.print(T09 + random(0, 3), DEC);
  Serial.print(";T10=");
  Serial.print(T10 + random(0, 3), DEC);
  Serial.print(";T11=");
  Serial.print(T11 + random(0, 3), DEC);
  Serial.print(";T12=");
  Serial.print(T12 + random(0, 3), DEC);
  Serial.print(";T13=");
  Serial.print(T13 + random(0, 3), DEC);
  Serial.print(";T14=");
  Serial.print(T14 + random(0, 3), DEC);
  Serial.print(";T15=");
  Serial.print(T15 + random(0, 3), DEC);
  Serial.print(";T16=");
  Serial.print(T16 + random(0, 3), DEC);
  Serial.print(";T17=");
  Serial.print(T17 + random(0, 3), DEC);
  Serial.print(";T18=");
  Serial.print(T18 + random(0, 3), DEC);
  Serial.print(";T19=");
  Serial.print(T19 + random(0, 3), DEC);
  Serial.print(";T20=");
  Serial.print(T20 + random(0, 3), DEC);
  Serial.print(";T21=");
  Serial.print(T21 + random(0, 3), DEC);

  Serial.print(";P00=");
  Serial.print(Pressi);
  Serial.print(";P01=");
  Serial.print(Pressi);
  Serial.print(";P02=");
  Serial.print(Pressi);
  Serial.print(";P03=");
  Serial.print(Pressi);
  Serial.print(";P04=");
  Serial.print(Pressi);
  Serial.print(";P05=");
  Serial.print(Pressi);
  Serial.print(";P06=");
  Serial.print(Pressi);
  Serial.print(";P07=");
  Serial.print(Pressi);
  Serial.print(";P08=");
  Serial.print(Pressi);
  Serial.print(";P09=");
  Serial.print(Pressi);
  Serial.println(";");
}
