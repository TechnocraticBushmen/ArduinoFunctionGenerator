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
#define BUTTON_WAVEFORM 3
#define BUTTON_SETTING 4
#define BUTTON_MULTIPLIER 5
#define BUTTON_PLUS 6
#define BUTTON_MINUS 7

#define REPETITIVE_DELAY_1 500;
#define REPETITIVE_DELAY_2 50;

//init lcd
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

//changes when play/pause button is pushed
volatile boolean running = false;

//button states
boolean buttonWaveformPressed = false;
boolean buttonSettingPressed = false;
boolean buttonMultiplierPressed = false;

const char *waveforms[] = {"Sine", "Square", "Triangle", "Ramp (sawtooth)", "Pulse"};
int currentWaveform = 0;

const char *settings[] = {"Frequency", "Amplitude", "Bias", "DC Offset", "Custom"};
long values[] = {1, 1, 50, 0, 0};
int currentSetting = 0;

const int multipliers[] = {1, 10, 100, 1000};
int currentMultiplier = 0;

void generateWaveWaveform()
{
  lcd.setCursor(0, 1);
  lcd.print("wave ");
}

void onButtonWaveformPressed(boolean firstTime)
{
  if(!firstTime) {
    currentWaveform = (currentWaveform + 1) % (sizeof(waveforms) / sizeof(waveforms[0]));
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waveform:");
  lcd.setCursor(0, 1);
  lcd.print(waveforms[currentWaveform]);
}

void onButtonSettingPressed(boolean firstTime)
{
  if(!firstTime) {
    currentSetting = (currentSetting + 1) % (sizeof(settings) / sizeof(settings[0]));
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setting:");
  lcd.setCursor(0, 1);
  lcd.print(settings[currentSetting]);
}

void onButtonMultiplierPressed(boolean firstTime)
{
  if(!firstTime) {
    currentMultiplier = (currentMultiplier + 1) % (sizeof(multipliers) / sizeof(multipliers[0]));
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Multiplier:");
  lcd.setCursor(0, 1);
  lcd.print("X");
  lcd.print(multipliers[currentMultiplier]);
}

void onButtonPlusPressed(boolean firstTime)
{
  /*
  if(!firstTime) {
    currentMultiplier = (currentMultiplier + 1) % (sizeof(multipliers) / sizeof(multipliers[0]));
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Multiplier:");
  lcd.setCursor(0, 1);
  lcd.print("X");
  lcd.print(multipliers[currentMultiplier]);
  */
}

void handleButtonInput(int button, boolean *buttonPressed, void (*callback)(boolean), boolean repetitive = false)
{
  static int lastButtonPressed = 0;
  if(digitalRead(button)) {
    if(!*buttonPressed) {
      *buttonPressed = true;
      callback(lastButtonPressed != button);
      lastButtonPressed = button;
      if(repetitive) {
        delay(REPETITIVE_DELAY_1);
      }
    } else{
      if(repetitive) {
        callback(lastButtonPressed != button);
        delay(REPETITIVE_DELAY_2);
      }
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
    generateWaveWaveform();
  } else {
//    handleButtonInput(BUTTON_WAVEFORM, &buttonWaveformPressed, &onButtonWaveformPressed);
//    handleButtonInput(BUTTON_SETTING, &buttonSettingPressed, &onButtonSettingPressed);
    handleButtonInput(BUTTON_MULTIPLIER, &buttonMultiplierPressed, &onButtonMultiplierPressed);
    handleButtonInput(BUTTON_PLUS, &buttonPlusPressed, &onButtonPlusPressed, true);
//    handleButtonInput(BUTTON_MINUS, &buttonMinusPressed, &onButtonMinusPressed, true);
  }
}

