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
#define BUTTON_PLAY_PAUSE 2
#define BUTTON_FUNCTION 3
#define BUTTON_SET 4
#define BUTTON_FACTOR 5
#define BUTTON_PLUS 6
#define BUTTON_MINUS 7

//init lcd
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

//changes when play/pause button is pushed
volatile boolean running = false;

//button states
boolean buttonFunctionPressed = false;

const char *functions[] = {"Sine", "Square", "Triangle"};
int currentFunction = 0;

void generateWaveFunction()
{
  lcd.setCursor(0, 1);
  lcd.print("wave ");
}

void onButtonFunctionPressed()
{
  currentFunction = (currentFunction + 1) % (sizeof(functions) / sizeof(functions[0]));
  lcd.setCursor(0, 1);
  lcd.print(functions[currentFunction]);
  lcd.print(" func");
}

void handleButtonInput(int button, boolean *buttonPressed, void (*callback)())
{
  if(digitalRead(button)) {
    if(!*buttonPressed) {
      *buttonPressed = true;
      callback();
    }
  } else {
    if(*buttonPressed) {
      *buttonPressed = false;
    }
  }
}

/**
  * this is actually an interrupt callback
  */
void onPlayPausePush()
{
  running = !running;
}

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  
//FALLING triggers when releasing the button.
//  attachInterrupt(IR_PLAY_PAUSE, onPlayPausePush, FALLING);
}

void loop()
{
  if(running) {
    generateWaveFunction();
  } else {
    handleButtonInput(BUTTON_FUNCTION, &buttonFunctionPressed, &onButtonFunctionPressed);
  }
}

