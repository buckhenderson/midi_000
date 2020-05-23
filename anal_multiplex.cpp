// step 1: have two dials through multiplexer
// step 2: have dials switch through rotary switch

int multiplexDialIn = A1;
int analReadVal;

int addressA = 27;
int addressB = 29;
int addressC = 31;

int nDials = 2;

void setup() {
  //Serial.begin(38400);
  Serial.begin(9600);
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

void loop() {
  for (int i = 0; i < nDials; i++){
  writeMux(i);
  analReadVal = analogRead(multiplexDialIn);
  Serial.println(analReadVal);
  }
  delay(200);
}