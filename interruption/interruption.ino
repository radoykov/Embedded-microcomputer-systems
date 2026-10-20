#define INTERR_BTN 2

void btn_press(void);
uint16_t readPot(void);

volatile bool pressed = false;

void setup()
{
  Serial.begin(9600);
  pinMode(INTERR_BTN, INPUT_PULLUP);

  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  attachInterrupt(digitalPinToInterrupt(INTERR_BTN), btn_press, FALLING);
}

void loop()
{
  if (pressed)
  {
    pressed = false;
    Serial.println(readPot());
  }
}

void btn_press(void)
{
  pressed = true;
}

uint16_t readPot(void)
{
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}