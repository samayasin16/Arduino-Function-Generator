#include <Arduino.h>
#include <LiquidCrystal.h>

/* ===== Buttons ===== */
const int BTN_FREQ = A2;   // Frequency step button
const int BTN_DUTY = A3;   // Duty cycle button (Pulse only)
const int BTN_WAVE = A1;   // Waveform select button

/* ===== Constants ===== */
const int DAC_PINS = 8;    // 8-bit DAC
const int SAMPLES  = 64;   // Samples per cycle

/* ===== Global Variables ===== */
volatile int index = 0;
volatile uint16_t freq = 50;

int freqSteps[] = {50, 100, 150, 200, 250, 300};
int stepIndex = 0;

int dutySteps[] = {10,20,30,40,50,60,70,80,90};
int dutyIndex = 4;
volatile int dutyCycle = 50;

/* Wave types: 0=Sine, 1=Triangle, 2=Square, 3=Sawtooth, 4=Pulse */
volatile int waveType = 0;

/* ===== LCD ===== */
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

/* ===== Waveform Tables ===== */
uint8_t sineTable[SAMPLES] = {
  128,140,152,165,176,188,198,208,218,226,234,240,245,250,253,254,
  255,254,253,250,245,240,234,226,218,208,198,188,176,165,152,140,
  128,115,103,90,79,67,57,47,37,29,21,15,10,5,2,1,
  0,1,2,5,10,15,21,29,37,47,57,67,79,90,103,115
};

uint8_t triangleTable[SAMPLES] = {
  0,8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,
  128,136,144,152,160,168,176,184,192,200,208,216,224,232,240,248,
  255,247,239,231,223,215,207,199,191,183,175,167,159,151,143,135,
  127,119,111,103,95,87,79,71,63,55,47,39,31,23,15,7
};

uint8_t squareTable[SAMPLES] = {
  255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,
  255,255,255,255,255,255,255,255,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};

uint8_t sawtoothTable[SAMPLES] = {
  0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,
  64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,
  128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,
  192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,255
};

/* ===== Timer1 Setup ===== */
void setupTimer(uint16_t f) {
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // Prescaler = 8
  uint32_t ocr = (16000000UL / (8UL * f * SAMPLES)) - 1;
  if (ocr > 65535) ocr = 65535;
  OCR1A = ocr;
  TIMSK1 |= (1 << OCIE1A);  // Enable interrupt
  interrupts();
}

/* ===== Timer ISR ===== */
ISR(TIMER1_COMPA_vect) {
  uint8_t value;

  switch(waveType){
    case 0: value = sineTable[index]; break;
    case 1: value = triangleTable[index]; break;
    case 2: value = squareTable[index]; break;
    case 3: value = sawtoothTable[index]; break;
    case 4: {
      int highSamples = (SAMPLES * dutyCycle) / 100;
      value = (index < highSamples) ? 0 : 255; // Inverted pulse
      break;
    }
    default: value = 0;
  }

  PORTD = value;                 // Output to DAC
  index = (index + 1) % SAMPLES; // Next sample
}

/* ===== LCD Update ===== */
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0,0);

  const char* names[] = {
    "SINE", "TRIANGLE", "SQUARE", "SAWTOOTH", "PULSE"
  };
  lcd.print("Wave: ");
  lcd.print(names[waveType]);

  lcd.setCursor(0,1);
  if(waveType == 4){
    lcd.print("Duty:");
    lcd.print(dutyCycle);
    lcd.print("% f:");
  } else {
    lcd.print("f: ");
  }
  lcd.print(freq);
  lcd.print("Hz");
}

/* ===== Setup ===== */
void setup() {
  for(int i=0; i<8; i++) pinMode(i, OUTPUT);

  pinMode(BTN_FREQ, INPUT_PULLUP);
  pinMode(BTN_DUTY, INPUT_PULLUP);
  pinMode(BTN_WAVE, INPUT_PULLUP);

  lcd.begin(16,2);
  updateLCD();
  setupTimer(freq);
}

/* ===== Main Loop ===== */
void loop() {
  static bool lastFreq=HIGH, lastDuty=HIGH, lastWave=HIGH;

  bool btnFreq = digitalRead(BTN_FREQ);
  bool btnDuty = digitalRead(BTN_DUTY);
  bool btnWave = digitalRead(BTN_WAVE);

  if(btnWave == LOW && lastWave == HIGH){
    waveType = (waveType + 1) % 5;
    if(waveType == 4){ dutyIndex = 4; dutyCycle = 50; }
    updateLCD();
    delay(200);
  }

  if(btnFreq == LOW && lastFreq == HIGH){
    stepIndex = (stepIndex + 1) % 6;
    freq = freqSteps[stepIndex];
    setupTimer(freq);
    updateLCD();
    delay(200);
  }

  if(waveType == 4 && btnDuty == LOW && lastDuty == HIGH){
    dutyIndex = (dutyIndex + 1) % 9;
    dutyCycle = dutySteps[dutyIndex];
    updateLCD();
    delay(200);
  }

  lastFreq = btnFreq;
  lastDuty = btnDuty;
  lastWave = btnWave;
}
