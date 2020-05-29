
int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder

int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command
int oldButton0State = 0;
int newButton0State = 0;
int button0Pin = 2;
int oldButton1State = 0;
int newButton1State = 0;
int button1Pin = 3;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(38400);
pinMode(button0Pin, INPUT);
oldButton0State = digitalRead(button0Pin);
pinMode(button1Pin, INPUT);
oldButton1State = digitalRead(button0Pin);
}

void loop() {
  newButton0State = digitalRead(button0Pin);
  if (newButton0State == HIGH && oldButton0State == LOW){
    digitalWrite(led0Pin, HIGH);
    MIDImessage(noteON, 60, velocity);
    oldButton0State = newButton0State;
  } else if (newButton0State == LOW && oldButton0State == HIGH){
    digitalWrite(led0Pin, LOW);
    MIDImessage(noteOFF, 60, velocity);
    oldButton0State = newButton0State;
  }
  newButton1State = digitalRead(button1Pin);
  if (newButton1State == HIGH && oldButton1State == LOW){
    digitalWrite(led1Pin, HIGH);
    MIDImessage(noteON, 62, velocity);
    oldButton1State = newButton1State;
  } else if (newButton1State == LOW el&& oldButton1State == HIGH){
    digitalWrite(led1Pin, LOW);
    MIDImessage(noteOFF, 62, velocity);
    oldButton1State = newButton1State;
  }
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

