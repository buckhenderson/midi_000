int totalChannels = 5;
int shifterN = 2;
int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command
int velocity = 100;
int shiftUp = 5;
int shiftDown = 6;

int addressA = 2;
int addressB = 3;
int addressC = 4;
int addressD = 5;
int readMux = 6;

int buttons[7] = {};
int oldState[7] = {};
int newState[7] = {};


int A = 0;      //Address pin A
int B = 0;      //Address pin B
int C = 0;      //Address pin C
int D = 0;      //Address pin D

int note0 = 60;

void setup() {
  Serial.begin(38400);
  //Serial.begin(9600);
  // Prepare address pins for output
  pinMode(addressA, OUTPUT);
  pinMode(addressB, OUTPUT);
  pinMode(addressC, OUTPUT);
  pinMode(addressD, OUTPUT);
  // Prepare read pin
  pinMode(readMux, INPUT);
}

void writeMux(int i){
    A = bitRead(i,0); //Take first bit from binary value of i channel.
    B = bitRead(i,1); //Take second bit from binary value of i channel.
    C = bitRead(i,2); //Take third bit from value of i channel.
    D = bitRead(i,3); //Take third bit from value of i channel.

    //Write address to mux
    digitalWrite(addressA, A);
    digitalWrite(addressB, B);
    digitalWrite(addressC, C);
    digitalWrite(addressD, D);
}

void loop() {
  //Select each pin and read value
  for(int i=0; i<totalChannels; i++){
    writeMux(i);
    newState[i] = digitalRead(readMux);
    if (newState[i] == 1 && oldState[i] == 0){
        MIDImessage(noteON, note0 + i, velocity);
        oldState[i] = newState[i];
    } else if (newState[i] == 0 && oldState[i] == 1){
        MIDImessage(noteOFF, note0 + i, velocity);
        oldState[i] = newState[i];
    }
  }
  for(int i=totalChannels; i<(totalChannels + shifterN); i++){
    writeMux(i);
    newState[i] = digitalRead(readMux);
    if (newState[i] == 1 && oldState[i] == 0){
      if (i == shiftUp){
        note0++;
      }
      if (i == shiftDown){
        note0--;
      }
      oldState[i] = newState[i];
    }
    if (newState[i] == 0 && oldState[i] == 1){
        oldState[i] = newState[i];
    }
  }

}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  //Serial.println(command);
  //Serial.println(MIDInote);
  //Serial.println(MIDIvelocity);
  Serial.write(command);//send note on or note off command
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}