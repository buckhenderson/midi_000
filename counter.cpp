
#include "Encoder_Polling.h"

const int encoderPin_A = 20;
const int encoderPin_B = 21;
int counter = 0;
int channelVal = 0;
int channelN = 3;

int multiplexDialIn = A1;
int analReadVal;

int addressA = 27;
int addressB = 29;
int addressC = 31;
int A;
int B;
int C;

int nDials = 2;
int nRows = 2; // this value refers to how many states the dials can exist in


void setup()
{
  Serial.begin(9600);
  encoder_begin(encoderPin_A, encoderPin_B); // Start the decoder
  // Prepare address pins for output
  pinMode(addressA, OUTPUT);
  pinMode(addressB, OUTPUT);
  pinMode(addressC, OUTPUT);
  // Prepare read pins
  pinMode(multiplexDialIn, INPUT);
}

void writeMux(int i){
    A = bitRead(i,0); //Take first bit from binary value of i channel.
    B = bitRead(i,1); //Take second bit from binary value of i channel.
    C = bitRead(i,2); //Take third bit from value of i channel.

    //Write address to mux
    digitalWrite(addressA, A);
    digitalWrite(addressB, B);
    digitalWrite(addressC, C);
}

void loop()
{
  int dir = encoder_data(); // Check for rotation

  if(dir == 1)       // If its forward...
  {
    counter++;       // Increment the counter
    Serial.println(counter);
    counter = counter % (channelN * 2);
    channelVal = counter / 2;
    Serial.print("desired channel = ");
    Serial.println(channelVal);
  }
  else if(dir == -1) // If its backward...
  {
    counter--;       // Decrement the counter
    Serial.println(counter);
    if (counter < 0){
      counter = 2 * channelN - 2;
    }
    channelVal = counter / 2;
    Serial.print("desired channel = ");
    Serial.println(channelVal);
  }
  for (int i = 0; i < nDials; i++){
      writeMux(i);
      analReadVal = analogRead(multiplexDialIn) * channelVal;
      Serial.println(analReadVal);
  }
  delay(200);

}