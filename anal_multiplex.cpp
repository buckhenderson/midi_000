// step 1: have two dials through multiplexer
// step 2: have dials switch through rotary switch

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

int encoderPin1 = 20;
int encoderPin2 = 21;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  //Serial.begin(38400);
  Serial.begin(9600);
  // Prepare address pins for output
  pinMode(addressA, OUTPUT);
  pinMode(addressB, OUTPUT);
  pinMode(addressC, OUTPUT);
  // Prepare read pins
  pinMode(multiplexDialIn, INPUT);

  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoderPin1), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder, CHANGE);
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

void loop() {
  for (int i = 0; i < nDials; i++){
  writeMux(i);
  analReadVal = analogRead(multiplexDialIn) * encoderValue;
  Serial.println(analReadVal);
  Serial.print("Encoder value: ");
  Serial.println(encoderValue);
  }
  delay(200);
}

void updateEncoder(){
  Serial.println("running updateEncoder()");
  bool bounce = false;
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  while (!bounce){
      int MSBn = digitalRead(encoderPin1); //MSB = most significant bit
      int LSBn = digitalRead(encoderPin2); //LSB = least significant bit
      bounce = MSB == MSBn && LSB == LSBn;
      delay(5);
  }
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    Serial.print("encoderValue in loop 0");
    Serial.println(encoderValue);
    encoderValue = encoderValue % nRows;
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
     encoderValue --;
     Serial.print("encoderValue in loop 1");
     Serial.println(encoderValue);
     encoderValue = max(0, encoderValue);
  }
  lastEncoded = encoded; //store this value for next time
  Serial.print('lastEncoded: ');
  Serial.println(lastEncoded);
  delay(1000);
}