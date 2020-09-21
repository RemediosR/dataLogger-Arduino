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
  unsigned long xValue; // values from accelerometer stored here
  unsigned long yValue;
  unsigned long zValue;
  
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
    Serial.print(nowTime);      //Send results to serial output buffer
    Serial.print(",");
    Serial.print(xValue); 
    Serial.print(",");
    Serial.print(yValue);
    Serial.print(",");
    Serial.print(zValue);
    Serial.print(";");
  }
  delay(delayTime); //Wait before looping again (replace with more precise timer when program is working).
}
