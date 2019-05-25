const int speakerPin = 8;
const byte interruptPin = 2;
volatile unsigned long count = 0;
volatile unsigned long freq = 0;
bool speakerState = LOW;


volatile unsigned timer1_counter;
void setup()
{
  // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
//  timer1_counter = 65441;   // preload timer 65536-16MHz/256/500Hz

  //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
  //timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts
  setup2();
}

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;   // preload timer

  freq = count;
  count = 0;
}

void setup2() {
  pinMode(speakerPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr_c, RISING);

  Serial.begin(9600);
}


void loop() {
  //delay(500);
  float myFreq = 1.0e6/(float)freq;
  myFreq = 4.0*(myFreq-750.0)+200;
//  float myFreq = (float)freq;
//  myFreq = 5.0*(myFreq-1100.0)-100;

  if (myFreq<50) myFreq = 50;
    Serial.println(myFreq);
    tone(speakerPin, myFreq);

#if (0)

  float myDelay;
  float myFreq;
  myFreq = freq-500;
//  myFreq *= myFreq;
  myFreq *= myFreq;
  //myDelay = 7*(float)1e7/(float)freq+20;
  myDelay = 1e6/myFreq;

  toSpeaker(myDelay);
{
  Serial.print(freq);
  Serial.print(",");
  Serial.println(myDelay);
  delay(5);
}
#endif
}

void isr_c() {
  ++count;
}
