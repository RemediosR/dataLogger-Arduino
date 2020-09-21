const int delayTime = 10; // time (in ms) to wait between consecutive loop iterations

const int statePin = 2; // digital input pin used to turn program on and off
const int xPin = 0; // analog input pins
const int yPin = 1;
const int zPin = 2;

boolean pinState = false;  // true iff pulse is being received in statePin
boolean lastState = false; // previous value of pinState to determine if pulse just started
boolean goState = false; // if true, attempt to read accelerometer output in analog pins
boolean firstIteration = true; //if true, then this is the first loop for which goState has been activated

char ch;
//int k = 1;
unsigned long firstTime;
unsigned long nowTime;

void setup()
{
  analogReference(EXTERNAL);
//  pinmode(statePin, INPUT);
  Serial.begin(9600); // note the higher than usual serial speed
}

void loop()
{
  unsigned int xValue; // values from accelerometer stored here
  unsigned int yValue;
  unsigned int zValue;
  byte xOut1 = 0;
  byte xOut2 = 0;
  byte yOut1 = 0;
  byte yOut2 = 0;
  byte zOut1 = 0;
  byte zOut2 = 0;
  byte tOut1 = 0;
  byte tOut2 = 0;
  byte tOut3 = 0;
  byte tOut3 = 0;
  int  i = 0;

//  lastState = pinState;
//  pinState = (digitalRead(statePin)==HIGH);
//  if (pinState && !lastState) // i.e. if pulse just started, toggle goState.
//  {
//    goState=!goState;
//  }
  if (Serial.available())
  {
     ch = Serial.read();
     if (ch == 's') 
     {
      goState=true;
      ch='n';
     }
     if (ch=='e')
     {
       goState=false;
       ch='n';
     }
  }

  if (goState)
  {
    if (firstIteration)
    {
      firstTime=millis();       //Determine baseline time
      firstIteration=false;
    }
    nowTime=millis()-firstTime; //Determine time that has passed since baseline
    xValue = nowTime % 500; //analogRead(xPin);  //Read analog pins
    yValue = nowTime % 200; //analogRead(yPin);
    zValue = nowTime % 150; //analogRead(zPin);

//    xValue = k;
//    yValue = 3*k;
//    zValue = 4;
//    k++;

    for(i=0;i<=7;i++)
    {
      if(bitread(xValue,i))
      {
       xOut1=xOut1+2^i;
      }
    }
    for(i=8;i<=15;i++)
    {
      if(bitread(xValue,i))
      {
       xOut2=xOut2+2^i;
      }
    }
    for(i=0;i<=7;i++)
    {
      if(bitread(yValue,i))
      {
       yOut1=yOut1+2^i;
      }
    }
    for(i=8;i<=15;i++)
    {
      if(bitread(yValue,i))
      {
       yOut2=yOut2+2^i;
      }
    }
    for(i=0;i<=7;i++)
    {
      if(bitread(zValue,i))
      {
       zOut1=zOut1+2^i;
      }
    }
    for(i=8;i<=15;i++)
    {
      if(bitread(zValue,i))
      {
       zOut2=zOut2+2^i;
      }
    }
    for(i=0;i<=7;i++)
    {
      if(bitread(nowTime,i))
      {
       tOut1=tOut1+2^i;
      }
    }
    for(i=8;i<=15;i++)
    {
      if(bitread(nowTime,i))
      {
       tOut2=tOut2+2^i;
      }
    }
    for(i=16;i<=23;i++)
    {
      if(bitread(nowTime,i))
      {
       tOut3=tOut3+2^i;
      }
    }
    for(i=24;i<=31;i++)
    {
      if(bitread(nowTime,i))
      {
       tOut4=zOut4+2^i;
      }
    }
    
    serial.write(tOut
//    Serial.print(nowTime);      //Send results to serial output buffer
//    Serial.print(",");
//    Serial.print(xValue);
//    Serial.print(",");
//    Serial.print(yValue);
//    Serial.print(",");
//    Serial.print(zValue);
//    Serial.print(";");
  }
  delay(delayTime); //Wait before looping again (replace with more precise timer when program is working).
}
