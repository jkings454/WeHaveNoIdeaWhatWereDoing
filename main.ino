int val = 0;
int prevVal = 0;

int pin = A0;
int outPin = 2;
void setup() {
  pinMode(outPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // reply only when you receive data:
  // read the incoming byte:
  val = analogRead(pin);
  if (val - prevVal > 0){
    digitalWrite(outPin, HIGH);
  }
  else {
    digitalWrite(outPin, LOW);
  }

  prevVal = val;
  //  // say what you got:
  // Serial.print("I received: ");
  // Serial.println(val, DEC);
  // delay(600);
}
