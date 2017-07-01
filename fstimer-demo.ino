/***************************************************
  FS-TIMER DEMO

  Written by Jiri Blazek
  License GPLv3
 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS   31
#define TFT_RST  30
#define TFT_DC   29
#define TFT_SCLK 27
#define TFT_MOSI 28
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// buttons
#define BTN_START 5
#define BTN_STOP  6

int isTime = 0;
String m = "0";
String ss = "00";
String xx = "00";

int setin = 0;
int setinClear = 0;
int second = 0;
int secondClear = 0;
int minute = 0;
int minuteClear = 0;

uint16_t time;

int startTime = 0;
int buttonStartState = 0;
int buttonStopState = 0;

int topTimer = 60;

int memMinute;
int memSecond;
int memHundredthOfSecond;

void setup(void) {
  Serial.begin(9600);
  
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  Serial.println("Initialized");

  time = millis();
  tft.fillScreen(ST7735_BLACK);
  
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(2, 2);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.println("FS-TIMER");
  tft.setCursor(2, 20);
  tft.println("DEMO");
  
  tft.setCursor(2, topTimer);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(3);
  tft.println(m + ":" + ss + ":" + xx);  
}

void loop() {
  if (buttonStartState == 0 & buttonStopState == 0) {
    if (digitalRead(BTN_START) == 0) {
      startTime = millis();
      buttonStartState = 1;
    }
  }
  
  if (buttonStopState == 0 & buttonStartState == 1) {
    if (digitalRead(BTN_STOP) == 0) {
      isTime = millis() - startTime;     
      memMinute = getMinute(isTime);
      memSecond = getSecond(isTime);
      memHundredthOfSecond = getHundredthOfSecond(isTime);
      
      buttonStopState = 1;
      buttonStartState = 0;
    }
  }
  
  if (buttonStartState == 1) {   
    isTime = millis() - startTime;
    setin = getHundredthOfSecond(isTime);
    if (setin != setinClear) {
      // clear previous time value
      tft.setCursor(92, topTimer);
      tft.setTextColor(ST7735_BLACK);
      tft.println(fillZero(setinClear));
       
      // draw current time value    
      tft.setCursor(92, topTimer);
      tft.setTextColor(ST7735_GREEN);
      tft.println(fillZero(setin));    
      
      setinClear = setin;  
    }
    
    second = getSecond(isTime);
    if (second != secondClear) {
      // clear previous time value
      tft.setCursor(38, topTimer);
      tft.setTextColor(ST7735_BLACK);
      tft.println(fillZero(secondClear));
       
      // draw current time value    
      tft.setCursor(38, topTimer);
      tft.setTextColor(ST7735_GREEN);
      tft.println(fillZero(second));    
      
      secondClear = second;
    }
    
    minute = getMinute(isTime);
    if (minute != minuteClear) {
      // clear previous time value
      tft.setCursor(2, topTimer);
      tft.setTextColor(ST7735_BLACK);
      tft.println(minuteClear);
       
      // draw current time value    
      tft.setCursor(2, topTimer);
      tft.setTextColor(ST7735_GREEN);
      tft.println(minute);    
      
      minuteClear = minute;
    }
  }
}

int getMinute(int millisTime) {
  return millisTime / (60 * 1000) % 60;
}

int getSecond(int millisTime) {
  return millisTime / 1000 % 60;
}

int getHundredthOfSecond(int millisTime) {
  return millisTime / 10 % 100;
}

String fillZero(int t) {
  String x = String(t);
  if (t < 10) {
    x = "0" + String(t);
  }
  return String(x);
}
