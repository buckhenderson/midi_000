
int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder

int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command
int oldButtonState = 0;
int newButtonState = 0;
int buttonPin = 2;
int ledPin = 13;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(38400);
pinMode(ledPin, OUTPUT);
pinMode(buttonPin, INPUT);
oldButtonState = digitalRead(buttonPin);
}

void loop() {
  newButtonState = digitalRead(buttonPin);
  if (newButtonState == HIGH && oldButtonState == LOW){
    digitalWrite(ledPin, HIGH);
    MIDImessage(noteON, 60, velocity);
    oldButtonState = newButtonState;
  } else if (newButtonState == LOW && oldButtonState == HIGH){
    digitalWrite(ledPin, LOW);
    MIDImessage(noteOFF, 60, velocity);
    oldButtonState = newButtonState;
  }
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}

