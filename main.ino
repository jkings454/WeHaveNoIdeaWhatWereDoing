int val = 0;
int pin = A0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  // reply only when you receive data:
  // read the incoming byte:
  val = analogRead(pin);

  // say what you got:
  Serial.print("I received: ");
  Serial.println(val, DEC);
  delay(600);
}
