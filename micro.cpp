#include "Encoder_Polling.h"

const int encoderPin_A = 23;
const int encoderPin_B = 25;

//metronome variables
long startTime;
long currentTime;
int bpm = 120;
int tickRate;
int metronomeOnPin = 2;
int metronomeOnOld = 0;
int metronomeOnNew = 0;
int metronomeDownPin = 4;
int metronomeDownOld = 0;
int metronomeDownNew = 0;
int metronomeUpPin = 5;
int metronomeUpOld = 0;
int metronomeUpNew = 0;
bool metronomeBool = false;
int metronomeResetPin = 7;
int metronomeDown5Pin = 3;
int metronomeUp5Pin = 6;
int metronomeResetOldState = 0;
int metronomeResetNewState = 0;
int metronomeDown5OldState = 0;
int metronomeDown5NewState = 0;
int metronomeUp5OldState = 0;
int metronomeUp5NewState = 0;

int instrumentSwitchPin = 12;
int instrumentSwitchOldState = 0;
int instrumentSwitchNewState = 0;

int processedDial;
int channelVal = 0;
int channelN = 3;
int counter = 0;

int noteON = 144;        //144 = 10010000 in binary, note on command
int noteOFF = 128;       //128 = 10000000 in binary, note off command
int controlChange = 176; //176 = 10110000 in binary, control change command
int totalDials = 10;

int dials[10] = {A13, A12, A11, A10, A9, A8, A7, A6, A5, A4};
int dialsOldState[10];
int dialsNewState[10];

int volumeDialPin = A0;
int volumeDialReadVal;
float dialScale = 127.0 / 1023.0;
int processedVolumeDial;
int volumeDialOldState;
int volumeDialNewState;

int switchMIDI = 1;
int metronomeCC = 127;

int velocity = 100;
int pedalOn = 127;
int pedalOff = 0;

void setup()
{
    Serial.begin(38400);
    encoder_begin(encoderPin_A, encoderPin_B); // Start the decoder
//    Serial.begin(9600);
    // Prepare read pins
    // set up analog array
    for (int i = 1; i < totalDials; i++)
    {
        pinMode(dials[i], INPUT);
    }
    // set up master
    pinMode(volumeDialPin, INPUT);
    // set up metronome buttons
    pinMode(metronomeOnPin, INPUT);
    pinMode(metronomeDown5Pin, INPUT);
    pinMode(metronomeDownPin, INPUT);
    pinMode(metronomeUpPin, INPUT);
    pinMode(metronomeUp5Pin, INPUT);
    pinMode(metronomeResetPin, INPUT);
    // set up switch
    pinMode(instrumentSwitchPin, INPUT);
}

void metronome()
{
    metronomeOnNew = digitalRead(metronomeOnPin);
    if (metronomeOnNew != metronomeOnOld && metronomeOnOld == 0)
    {
        metronomeBool = !metronomeBool;
        metronomeOnOld = metronomeOnNew;
    }
    if (metronomeOnNew != metronomeOnOld && metronomeOnOld == 1)
    {
        metronomeOnOld = metronomeOnNew;
    }
    if (metronomeBool)
    {
        metronomeUpNew = digitalRead(metronomeUpPin);
        if (metronomeUpNew != metronomeUpOld && metronomeUpOld == 0)
        {
            bpm++;
            metronomeUpOld = metronomeUpNew;
        }
        if (metronomeUpNew != metronomeUpOld && metronomeUpOld == 1)
        {
            metronomeUpOld = metronomeUpNew;
        }
        metronomeDownNew = digitalRead(metronomeDownPin);
        if (metronomeDownNew != metronomeDownOld && metronomeDownOld == 0)
        {
            bpm--;
            metronomeDownOld = metronomeDownNew;
        }
        if (metronomeDownNew != metronomeDownOld && metronomeDownOld == 1)
        {
            metronomeDownOld = metronomeDownNew;
        }
        metronomeResetNewState = digitalRead(metronomeResetPin);
        if (metronomeResetNewState != metronomeResetOldState && metronomeResetOldState == 0)
        {
            bpm = 120;
            metronomeResetOldState = metronomeResetNewState;
        }
        if (metronomeResetNewState != metronomeResetOldState && metronomeResetOldState == 1)
        {
            metronomeResetOldState = metronomeResetNewState;
        }
        metronomeDown5NewState = digitalRead(metronomeDown5Pin);
        if (metronomeDown5NewState != metronomeDown5OldState && metronomeDown5OldState == 0)
        {
            bpm = bpm - 5;
            metronomeDown5OldState = metronomeDown5NewState;
        }
        if (metronomeDown5NewState != metronomeDown5OldState && metronomeDown5OldState == 1)
        {
            metronomeDown5OldState = metronomeDown5NewState;
        }
        metronomeUp5NewState = digitalRead(metronomeUp5Pin);
        if (metronomeUp5NewState != metronomeUp5OldState && metronomeUp5OldState == 0)
        {
            bpm = bpm + 5;
            metronomeUp5OldState = metronomeUp5NewState;
        }
        if (metronomeUp5NewState != metronomeUp5OldState && metronomeUp5OldState == 1)
        {
            metronomeUp5OldState = metronomeUp5NewState;
        }
    }
    tickRate = 1000.0 * (float)60 / (float)bpm;
    currentTime = millis();
    if ((currentTime - startTime > tickRate) && metronomeBool)
    {
        MIDImessage(controlChange, metronomeCC, 1); // velocity doesn't matter
        startTime = millis();
    }
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity)
{
//    Serial.println(command);
//    Serial.println(MIDInote);
//    Serial.println(MIDIvelocity);
    Serial.write(command);      //send note on or note off command
    Serial.write(MIDInote);     //send pitch data
    Serial.write(MIDIvelocity); //send velocity data
}

void loop()
{
    metronome();

    // read instrument (channel) switch
    instrumentSwitchNewState = digitalRead(instrumentSwitchPin);
    if (instrumentSwitchNewState == 1 && instrumentSwitchOldState == 0)
    {
        MIDImessage(controlChange, switchMIDI, pedalOn);
        instrumentSwitchOldState = instrumentSwitchNewState;
        counter = 0;
    }
    else if (instrumentSwitchNewState == 0 && instrumentSwitchOldState == 1)
    {
        instrumentSwitchOldState = instrumentSwitchNewState;
    }
    //read single dial  - probably hook this up to volume, if there can be a general volume
    volumeDialNewState = analogRead(volumeDialPin);
    if (abs(volumeDialOldState - volumeDialNewState) > 5)
    {
        processedVolumeDial = dialScale * volumeDialNewState;
        MIDImessage(controlChange, 2, processedVolumeDial);
        volumeDialOldState = volumeDialNewState;
    }
    //read rotary switch
    int dir = encoder_data(); // Check for rotation

    if (dir == 1) // If its forward...
    {
        counter++; // Increment the counter
        counter = counter % (channelN * 2);
        channelVal = counter / 2;
    }
    else if (dir == -1) // If its backward...
    {
        counter--; // Decrement the counter
        if (counter < 0)
        {
            counter = 2 * channelN - 2;
        }
        channelVal = counter / 2;
    }
    //read dials
    for (int i = 0; i < totalDials; i++)
    {
        dialsNewState[i] = analogRead(dials[i]);
        if (abs(dialsNewState[i] - dialsOldState[i]) > 5)
        {
            processedDial = dialScale * dialsNewState[i];
            MIDImessage(controlChange, (3 + i + channelVal * totalDials), 127 - processedDial);
            dialsOldState[i] = dialsNewState[i];
        }
    }
}