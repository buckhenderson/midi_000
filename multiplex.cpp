int totalChannels = 5;
int shifterN = 2;
int controlN = 1;
int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command
int controlChange = 176;//176 = 10110000 in binary, control change command
int velocity = 100;
int pedalOn = 127;
int pedalOff = 0;
int shiftUp = 5;
int shiftDown = 6;
int pedal = 7;
int pedalMIDI = 64;
int instrumentShift = 8;
int switchMIDI = 1;
int dial00 = A0;
int dial00ReadVal;
float dialScale = 127.0 / 1023.0;
int processedDial00;
int dial00OldState;
int dial00NewState;

int addressA = 2;
int addressB = 3;
int addressC = 4;
int addressD = 5;
int readMux = 6;

int buttons[9] = {};
int oldState[9] = {};
int newState[9] = {};


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
  // Prepare read pins
  pinMode(readMux, INPUT);
  pinMode(dial00, INPUT);
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
  for(int i=(totalChannels + shifterN); i<(totalChannels + shifterN + controlN); i++){
    writeMux(i);
    newState[i] = digitalRead(readMux);
    if (newState[i] == 1 && oldState[i] == 0){
        MIDImessage(controlChange, pedalMIDI, pedalOn);
        oldState[i] = newState[i];
    } else if (newState[i] == 0 && oldState[i] == 1){
        MIDImessage(controlChange, pedalMIDI, pedalOff);
        oldState[i] = newState[i];
    }
  }

  for(int i=(totalChannels + shifterN + controlN); i<(totalChannels + shifterN + controlN + 1); i++){
    writeMux(i);
    newState[i] = digitalRead(readMux);
    if (newState[i] == 1 && oldState[i] == 0){
      MIDImessage(controlChange, switchMIDI, pedalOn);
      oldState[i] = newState[i];
    } else if (newState[i] == 0 && oldState[i] == 1){
      oldState[i] = newState[i];
    }
    }
    dial00NewState = analogRead(dial00);
    if (abs(dial00OldState - dial00NewState) > 5){
      processedDial00 = dialScale * dial00NewState;
      MIDImessage(controlChange, 2, processedDial00);
      dial00OldState = dial00NewState;
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