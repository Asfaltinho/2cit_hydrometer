#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PIN_MISURATORE 2
#define RESET_TEMPO_US 30000000

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pulse per L = 450

const float LITRI_X_RISING = 1.0/450.0;

volatile float litri = 0;

void setup() {
  //pinMode(PIN_LCD_V, OUTPUT);
  //analogWrite(PIN_LCD_V, 254);
  
  pinMode(PIN_MISURATORE, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_MISURATORE), on_pulse, RISING);
  
  lcd.begin(16, 2);
  lcd.setBacklight((uint8_t)1);
  lcd.print("   HYDROMETER");

  Serial.begin(9600);
}

void on_pulse() {
  litri += LITRI_X_RISING;
}

char str[16];

void loop() {
  unsigned long pulse = pulseIn(PIN_MISURATORE, HIGH, RESET_TEMPO_US);
  if(pulse == 0){
    litri = 0;
  }

  lcd.home();
  lcd.clear();
  if(litri != 0){
    lcd.print("ml utilizzati:");
    lcd.setCursor(0, 1);
    dtostrf(litri * 1000.0, 13, 1, str);
    lcd.print(str);
    lcd.print(" ml");
  } else {
    lcd.print("Non passa acqua");
  }
  
  Serial.println(litri);
}
