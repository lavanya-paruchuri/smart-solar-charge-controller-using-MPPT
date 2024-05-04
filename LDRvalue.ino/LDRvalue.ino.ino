#define LDRpin1 A0
#define LDRpin2 A1
#define LDRpin3 A2
#define LDRpin4 A3

int LDRValue1 = 0;
int LDRValue2 = 0;
int LDRValue3 = 0;
int LDRValue4 = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  LDRValue1 = analogRead(LDRpin1);
  LDRValue2 = analogRead(LDRpin2);
  LDRValue3 = analogRead(LDRpin3);
  LDRValue4 = analogRead(LDRpin4);

  Serial.print("LDR1: ");
  Serial.print(LDRValue1);
  Serial.print("\tLDR2: ");
  Serial.print(LDRValue2);
  Serial.print("\tLDR3: ");
  Serial.print(LDRValue3);
  Serial.print("\tLDR4: ");
  Serial.println(LDRValue4);

  delay(2000); // Adjust delay as needed
}
