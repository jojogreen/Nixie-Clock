int int_Hz1 = 1; //set Hz for interrupt on Timer 1
int OF1_count = 0; //number of counts for overflow for Timer 1
int int_Hz0 = 10000; //set Hz for interrupt on Timer 0
int OF0_count = 0;
int OF2_count = 0;
int int_Hz2 = 1000; //set Hz for interrupt on Timer 0;

int hours1 = 0, hours2 = 0, minutes1 = 0, minutes2 = 0, seconds1 = 0, seconds2 = 0;
const int sec_button = 1; //pin for seconds button
const int min_button = 2; //pin for minutes button
const int hr_button = 3; //pin for hours button

boolean Timer1Run = false;
boolean Timer2Run = false;
boolean A = true;

int clockNum = 5;

void setup()
{
  DDRB = B11110000;//Set ports 8-11 as output
  DDRD = B00111111;
  pinMode(sec_button, INPUT);//Set seconds, minutes, and hour buttons to input
  pinMode(min_button, INPUT);
  pinMode(hr_button, INPUT);
  
  OF1_count = (16*10^6)/(int_Hz1*1024)-1; //calculate timer1 counts before overflow
  OF0_count = (16*10^6)/(int_Hz0*256); //calculate timer0 counts before overflow
  OF2_count = (16*10^6)/(int_Hz2*256); //calculate timer2 counts before overflow
  interrupt_setup(); //run interrupt setup to enable interrupts on timer 1
}
void loop()
{
  //Buttons pressed
  if(sec_button ==  HIGH)
  {
    Sec_Reset();
  }
  else if(min_button == HIGH)
  {
   Min_Increment();
  }
  else if(hr_button == HIGH)
  {
    Hr_Increment();
  }
}

//This function runs the setup for Timer1
void interrupt_setup()
{
  cli(); //stop interrupts

  //Timer1 Setup
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  //set compare for int_Hz1 (in counts)
  OCR1A = OF1_count;
  //turn on ctc mode
  TCCR1B |= (1<<WGM12);
  //  set cs12 and cs10 bits for prescaler
  TCCR1B |= (1<< CS12) | (1<<CS10);
  //enable timer compare interrupt
  TIMSK1 |= (1<<OCIE1A);
  
  //Timer2 Setup
  TCCR2A = 0;
  TCCR0B = 0;
  TCNT2 = 256-OF0_count;
  TCCR2B |=(1<<CS10); //no prescale
  TIMSK2 |= (1<< TOIE2);
  
  sei(); //enable timers
}


ISR(Timer2_OVF_vect)
{
  !A;//switch toggle
  if(A)
  {
    PORTD &= B11000000;//Turns off ports 2-7 and leaves 0 and 1 alone 
    WriteBinary(clockNum);//set the binary function
    clockNum++;
    if(clockNum >7)
    {clockNum =2;}
  }
  else
  {
    TCNT2 = 256-OF2_count; //set overflow to next one
    byte cnum = (byte)clockNum;
    PORTD |= cnum; //turns on clock number and leaves others off
  }
  
}

//This function runs when Timer1 overflows
ISR(TIMER1_COMPA_vect)
{
  seconds1++;
  if(seconds1>=10)
  {
    seconds2++;
    seconds1 = 0;
  }
  if(seconds2 >= 6)
  {
    minutes1++;
  }
  if(minutes1 >= 10)
  {
    minutes2++;
    minutes1 = 0;
  }
  if(minutes2 >=60)
  {
    hours1++;
    minutes2 = 0;
  }
  if(hours1 >=10)
  {
    hours2++;
    hours1 = 0;
  }
  if(hours2 >=24)
  {
    hours2 = 0;
  }
}

void Sec_Reset()
{
  seconds1 = 0;
  seconds2 = 0;
}

void Min_Increment()
{
  minutes1++;
  if(minutes1 >= 10)
  {
    minutes2++;
    minutes1 = 0;
  }
  if(minutes2 >= 60)
  {
    minutes2 = 0;
  }
}

void Hr_Increment()
{
  hours1++;
  if(hours1 >= 10);
  {
    hours2++;
    hours1 = 0;
  }
  if(hours2 >=24)
  {
    hours2 = 0;
  }
}

void WriteBinary(int Cnumb)
{
  byte T = 0;
  if(Cnumb ==2)
  {
    byte T = (byte)hours2;
  }
  else if(Cnumb ==3)
  {
    byte T = (byte)hours1;
  }
  else if(Cnumb ==4)
  {
    byte T = (byte)minutes2;
  }
  else if(Cnumb ==5)
  {
        byte T = (byte)minutes1;
  }
  else if(Cnumb ==6)
  {
        byte T = (byte)seconds2;
  }
  else if(Cnumb ==7)
  {
        byte T = (byte)seconds1;
  }
  PORTB &= B00001111; //reset pins 8-11 to off
  PORTB |= T;         //turn pins 8-11 on if necessary
}

