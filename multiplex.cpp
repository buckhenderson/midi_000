#include "Encoder_Polling.h"

const int encoderPin_A = 20;
const int encoderPin_B = 21;

int processedDial;
int channelVal = 0;
int channelN = 3;
int counter = 0;
int totalDials = 8; //number of analog dials
int totalRows = 2;  //number of theoretical rows of dials (these will be switched by the rotary encoder)
int totalChannels = 5;
int shifterN = 2;
int controlN = 1;
int noteON = 144;        //144 = 10010000 in binary, note on command
int noteOFF = 128;       //128 = 10000000 in binary, note off command
int controlChange = 176; //176 = 10110000 in binary, control change command
int velocity = 100;
int pedalOn = 127;
int pedalOff = 0;
int shiftUp = 5;
int shiftDown = 6;
int pedal = 7;
int pedalMIDI = 64;
int instrumentShift = 8;
int switchMIDI = 1;
int metronomeCC = 127;
int dial00 = A0;
int dial00ReadVal;
float dialScale = 127.0 / 1023.0;
int processedDial00;
int dial00OldState;
int dial00NewState;
//set up multiplexers
//digital
int digitalAddressA = 2;
int digitalAddressB = 3;
int digitalAddressC = 4;
int digitalAddressD = 5;
int readDigitalMux = 6;
//analog
int analogAddressA = 27;
int analogAddressB = 29;
int analogAddressC = 31;
int readAnalogMux = A1;
int readAnalogValue;

int buttons[9] = {};
int digitalOldState[9] = {};
int digitalNewState[9] = {};

int dials[2] = {};
int analogOldState[8];
int analogNewState[8];

int digitalA = 0; //Address pin A
int digitalB = 0; //Address pin B
int digitalC = 0; //Address pin C
int digitalD = 0; //Address pin D
int analogA = 0;
int analogB = 0;
int analogC = 0;

int note0 = 60;

int analogInputs[8] = {A1, A2, A3, A4, A5, A6, A7, A8};

//metronome variables
long startTime;
long currentTime;
int bpm = 120;
int tickRate;
int metronomeOnPin = 31;
int metronomeOnOld = 0;
int metronomeOnNew = 0;
int metronomeDownPin = 33;
int metronomeDownOld = 0;
int metronomeDownNew = 0;
int metronomeUpPin = 35;
int metronomeUpOld = 0;
int metronomeUpNew = 0;
bool metronomeBool = false;

void setup()
{
    Serial.begin(38400);
    encoder_begin(encoderPin_A, encoderPin_B); // Start the decoder
    //Serial.begin(9600);
    // Prepare address pins for output
    pinMode(digitalAddressA, OUTPUT);
    pinMode(digitalAddressB, OUTPUT);
    pinMode(digitalAddressC, OUTPUT);
    pinMode(digitalAddressD, OUTPUT);
    // Prepare read pins
    pinMode(readDigitalMux, INPUT);
    pinMode(analogInputs[0], INPUT);
    pinMode(analogInputs[1], INPUT);
    pinMode(analogInputs[2], INPUT);
    pinMode(analogInputs[3], INPUT);
    pinMode(analogInputs[4], INPUT);
    pinMode(analogInputs[5], INPUT);
    pinMode(analogInputs[6], INPUT);
    pinMode(analogInputs[7], INPUT);
    //metronome read pins
    pinMode(metronomeOnPin, INPUT);
    pinMode(metronomeDownPin, INPUT);
    pinMode(metronomeUpPin, INPUT);
    //main volume dial
    pinMode(dial00, INPUT);
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
    }
    tickRate = 1000.0 * (float)60 / (float)bpm;
    currentTime = millis();
    if ((currentTime - startTime > tickRate) && metronomeBool)
    {
        MIDImessage(controlChange, metronomeCC, 1); // velocity doesn't matter
        startTime = millis();
    }
}


void writeDigitalMux(int i)
{
    digitalA = bitRead(i, 0); //Take first bit from binary value of i channel.
    digitalB = bitRead(i, 1); //Take second bit from binary value of i channel.
    digitalC = bitRead(i, 2); //Take third bit from value of i channel.
    digitalD = bitRead(i, 3); //Take fourth bit from value of i channel.

    //Write address to mux
    digitalWrite(digitalAddressA, digitalA);
    digitalWrite(digitalAddressB, digitalB);
    digitalWrite(digitalAddressC, digitalC);
    digitalWrite(digitalAddressD, digitalD);
}

// void writeAnalogMux(int i)
// {
//     analogA = bitRead(i, 0); //Take first bit from binary value of i channel.
//     analogB = bitRead(i, 1); //Take second bit from binary value of i channel.
//     analogC = bitRead(i, 2); //Take third bit from value of i channel.

//     //Write address to mux
//     digitalWrite(analogAddressA, analogA);
//     digitalWrite(analogAddressB, analogB);
//     digitalWrite(analogAddressC, analogC);
// }

void loop()
{
    metronome();
    //read keys
    for (int i = 0; i < totalChannels; i++)
    {
        writeDigitalMux(i);
        digitalNewState[i] = digitalRead(readDigitalMux);
        if (digitalNewState[i] == 1 && digitalOldState[i] == 0)
        {
            MIDImessage(noteON, note0 + i, velocity);
            digitalOldState[i] = digitalNewState[i];
        }
        else if (digitalNewState[i] == 0 && digitalOldState[i] == 1)
        {
            MIDImessage(noteOFF, note0 + i, velocity);
            digitalOldState[i] = digitalNewState[i];
        }
    }
    //read shifter
    for (int i = totalChannels; i < (totalChannels + shifterN); i++)
    {
        writeDigitalMux(i);
        digitalNewState[i] = digitalRead(readDigitalMux);
        if (digitalNewState[i] == 1 && digitalOldState[i] == 0)
        {
            if (i == shiftUp)
            {
                note0++;
            }
            if (i == shiftDown)
            {
                note0--;
            }
            digitalOldState[i] = digitalNewState[i];
        }
        if (digitalNewState[i] == 0 && digitalOldState[i] == 1)
        {
            digitalOldState[i] = digitalNewState[i];
        }
    }
    //read pedal
    for (int i = (totalChannels + shifterN); i < (totalChannels + shifterN + controlN); i++)
    {
        writeDigitalMux(i);
        digitalNewState[i] = digitalRead(readDigitalMux);
        if (digitalNewState[i] == 1 && digitalOldState[i] == 0)
        {
            MIDImessage(controlChange, pedalMIDI, pedalOn);
            digitalOldState[i] = digitalNewState[i];
        }
        else if (digitalNewState[i] == 0 && digitalOldState[i] == 1)
        {
            MIDImessage(controlChange, pedalMIDI, pedalOff);
            digitalOldState[i] = digitalNewState[i];
        }
    }
    //read instrument (channel) switch
    for (int i = (totalChannels + shifterN + controlN); i < (totalChannels + shifterN + controlN + 1); i++)
    {
        writeDigitalMux(i);
        digitalNewState[i] = digitalRead(readDigitalMux);
        if (digitalNewState[i] == 1 && digitalOldState[i] == 0)
        {
            MIDImessage(controlChange, switchMIDI, pedalOn);
            digitalOldState[i] = digitalNewState[i];
        }
        else if (digitalNewState[i] == 0 && digitalOldState[i] == 1)
        {
            digitalOldState[i] = digitalNewState[i];
        }
    }
    //read single dial  - probably hook this up to volume, if there can be a general volume
    dial00NewState = analogRead(dial00);
    if (abs(dial00OldState - dial00NewState) > 5)
    {
        processedDial00 = dialScale * dial00NewState;
        MIDImessage(controlChange, 2, processedDial00);
        dial00OldState = dial00NewState;
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
    //read dial multiplexer
    //this set of dials will do different things depending on the encoder
    for (int i = 0; i < totalDials; i++)
    {
        //writeAnalogMux(i);
        analogNewState[i] = analogRead(analogInputs[i]);
        if (abs(analogNewState[i] - analogOldState[i]) > 5)
        {
            processedDial = dialScale * analogNewState[i];
            MIDImessage(controlChange, (3 + i + channelVal * totalDials), 127 - processedDial);
            analogOldState[i] = analogNewState[i];
        }
    }
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity)
{
    //Serial.println(command);
    //Serial.println(MIDInote);
    //Serial.println(MIDIvelocity);
    Serial.write(command);      //send note on or note off command
    Serial.write(MIDInote);     //send pitch data
    Serial.write(MIDIvelocity); //send velocity data
}