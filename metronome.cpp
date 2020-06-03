long startTime;
long currentTime;
int bpm = 120;
int tickRate;
int metronomeOnPin = 7;
int metronomeOnOld = 0;
int metronomeOnNew = 0;
int metronomeDownPin = 8;
int metronomeDownOld = 0;
int metronomeDownNew = 0;
int metronomeUpPin = 9;
int metronomeUpOld = 0;
int metronomeUpNew = 0;
bool metronomeBool = false;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    startTime = millis();
    pinMode(metronomeOnPin, INPUT);
    pinMode(metronomeDownPin, INPUT);
    pinMode(metronomeUpPin, INPUT);
}

void loop()
{
    // put your main code here, to run repeatedly:
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
        Serial.print("bpm = ");
        Serial.println(bpm);
        Serial.print("tickRate = ");
        Serial.println(tickRate);
        Serial.println("tick");
        startTime = millis();
    }
}