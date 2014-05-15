#include <LiquidCrystal.h>

/*
 * pin assignment
 */
#define LCD_PIN_RS 13
#define LCD_PIN_EN 12
#define LCD_PIN_D4 11
#define LCD_PIN_D5 10
#define LCD_PIN_D6 9
#define LCD_PIN_D7 8

/*
 * needs interrupt. Uno only has pins 2 and 3 with interrupt support
 * we use pin 2 which has interrupt 0, as pin 3 has PWM which we need
 * see http://arduino.cc/en/Reference/AttachInterrupt
 */
#define IR_PLAY_PAUSE 0
#define BTN_PLAY_PAUSE 2
#define BTN_FUNCTION 3
#define BTN_SET 4
#define BTN_FACTOR 5
#define BTN_PLUS 6
#define BTN_MINUS 7

//init lcd
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

//changes when play/pause button is pushed
volatile boolean running = false;

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  
  //FALLING triggers when releasing the button.
  attachInterrupt(IR_PLAY_PAUSE, onPlayPausePush, FALLING);
  
  /* temp crap */
  lcd.setCursor(0, 0);
  lcd.print("Merge");
  //pinMode(2, INPUT);
  /* end temp crap */
}

void loop()
{
  if(running) {
    generateWaveFunction();
  } else {
    handleButtonInput();
  }
}

void generateWaveFunction()
{
  lcd.setCursor(0, 1);
  lcd.print("wave ");
}

void handleButtonInput()
{
  if(digitalRead(BTN_FUNCTION)) {
    //see which button was pressed
    lcd.setCursor(0, 1);
    lcd.print("function");
  }
}

/**
  * 
  */
void onPlayPausePush()
{
  running = !running;
}
