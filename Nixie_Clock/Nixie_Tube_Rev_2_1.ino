#define Hr1 6
#define Hr2 7
#define Min1 8
#define Min2 9
#define Sec1 10
#define Sec2 11

#define Decoder_A 3
#define Decoder_B 1
#define Decoder_C 0
#define Decoder_D 16

#define Display_MS 2
#define Blanking_US 200

unsigned short Hr = 1;
unsigned short Min = 0;
unsigned short Sec = 0;


void setup()
{
  pinMode(Decoder_A, OUTPUT);
  pinMode(Decoder_B, OUTPUT);
  pinMode(Decoder_C, OUTPUT);
  pinMode(Decoder_D, OUTPUT);
  pinMode(Hr1, OUTPUT);
  pinMode(Hr2, OUTPUT);
  pinMode(Min1, OUTPUT);
  pinMode(Min2, OUTPUT);
  pinMode(Sec1, OUTPUT);
  pinMode(Sec2, OUTPUT);
}
void displayDigit(unsigned short digit, unsigned short pin)
{
  /*
  digitalWrite(Decoder_A,LOW);
  digitalWrite(Decoder_B,HIGH);
  digitalWrite(Decoder_C,LOW);
  digitalWrite(Decoder_D,LOW);
*/
  digitalWrite(Decoder_A, (1<<0) & digit);
  digitalWrite(Decoder_B, (1<<1) & digit);
  digitalWrite(Decoder_C, (1<<2) & digit);
  digitalWrite(Decoder_D, (1<<3) & digit);
  
  digitalWrite(pin, HIGH);
  delay(Display_MS);
  digitalWrite(pin, LOW);
  delayMicroseconds(Blanking_US);
}
void loop()
{
  displayDigit(Hr,Hr1);
  displayDigit(Hr,Hr2);
  displayDigit(Hr,Min1);
  displayDigit(Hr,Min2);
  displayDigit(Hr,Sec1);
  displayDigit(Hr,Sec2);
}
