/*
 * To set the RTC for the first time you have to send a string consisting of
 * the letter T followed by ten digit time (as seconds since Jan 1 1970) Also known as EPOCH time.
 *
 * You can send the text "T1357041600" on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 * Or you can use the following command via linux terminal to set the clock to the current time (UTC time zone)
 * date +T%s > /dev/ttyACM0
 * Inside the processSyncMessage() function I'm offsetting the UTC time with Central time.
 * If you want the clock to be accurate for your time zone, you may need to update the value.
 */
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"
#include <Time.h>

RTC_DS1307 RTC;

#define FWDButtonPIN 8
#define REVButtonPIN 9

#define N_LEDS 200 // 115 x 15 grid + four corners
#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define BRIGHTNESSDAY 150 // full on
#define BRIGHTNESSNIGHT 50 // half on

Adafruit_NeoPixel grid = Adafruit_NeoPixel(200 , 4, NEO_GRB + NEO_KHZ800);

// a few vars to keep the peace;
int intBrightness; // the brightness of the clock (0 = off and 255 = 100%)
int intTestMode = 0; // set when both buttons are held down
String strTime = ""; // used to detect if word time has changed
int intTimeUpdated = 0; // used to tell if we need to turn on the brightness again when words change

// a few colors
uint32_t colorWhite = grid.Color(255, 255, 255);
uint32_t colorBlack = grid.Color(0, 0, 0);
uint32_t colorRed = grid.Color(255, 0, 0);
uint32_t colorGreen = grid.Color(0, 255, 0);
uint32_t colorBlue = grid.Color(0, 0, 255);
uint32_t colorJGreen = grid.Color(50, 179, 30);
uint32_t colorLblue = grid.Color(2, 70, 179);
uint32_t colorMagenta = grid.Color(240, 29, 113);
uint32_t colorYellow = grid.Color(246, 252, 45);
uint32_t colorTeal = grid.Color(39, 232, 242);
uint32_t colorOrange = grid.Color(237, 89, 26);
uint32_t colorPeach = grid.Color(255, 188, 130);



// THATS SO METAL
int THATS[5] = {51, 52, 53, 54, 55};
int SO[2] = {38, 37};
int METAL[5] = {9, 10, 11, 12, 13};

// BIRTH DAY
int BIRTH[5] = {15, 16, 17, 18, 19};
int DAY[3] = {2, 1, 0};

// #ONE STUNNA
int NUMONE[4] = {133, 132, 131, 130};
int STUNNA[6] = {56, 57, 58, 59, 60};

// RYANNES AND TOMMYS
int RYANNES[7] = {98, 97, 70, 69, 42, 41, 14}; //RYANNES
int TOMMYS[6] = {34, 33, 32, 31, 30, 29}; //TOMMYS

// RYANNE AND TOMMY
int RYANNE[7] = {98, 97, 70, 69, 42, 41}; //RYANNE
int TOMMY[6] = {934, 33, 32, 31, 30}; //TOMMY

// OTHER RANDOM WORDS
int WEDDING[7] = {96, 72, 66, 46, 36, 20, 6}; //WEDDING
int LOVE[4] = {84, 85, 86, 87};
int HIS[3] = {144, 136, 114};
int OUR[3] = {129, 128, 127};

// PREPOSITIONS
int arrA[1] = {189};
int arrIT[2] = {182, 183};
int arrIS[2] = {185, 186};
int arrIN[2] = {57, 56};
int arrTO[2] = {149, 148};

// INTERVALS
int QUARTER[7] = {181, 180, 179, 178, 177, 176, 175};
int arrHALF[4] = {195, 194, 193, 192};
int arrPAST[4] = {153, 152, 151, 150};
int arrOCLOCK[6] = {22, 23, 24, 25, 26, 27};
int AM[2] = {7, 8};
int PM[2] = {4, 5};

// NUMBERS
int arrONE[3] = {144, 145, 146};
int arrTWO[3] = {140, 141, 142};
int arrTHREE[5] = {127, 128, 129, 130, 131};
int arrFOUR[4] = {133, 134, 135, 136};
int arrFIVE[4] = {125, 124, 123, 122};
int arrMFIVE[4] = {161, 162, 163, 164};
int arrSIX[3] = {120, 119, 118};
int arrSEVEN[5] = {116, 115, 114, 113, 112};
int arrEIGHT[5] = {99, 100, 101, 102, 103};
int arrNINE[4] = {104, 105, 106, 107};
int arrTEN[3] = {109, 110, 111};
int arrMTEN[3] = {173, 172, 171};
int arrELEVEN[6] = {94, 93, 92, 91, 90, 89};
int arrTWELVE[6] = {77, 78, 79, 80, 81, 82};
int arrTWENTY[6] = {154, 155, 156, 157, 158, 159};



void setup() {
  // set up the debuging serial output
  Serial.begin(9600);
    Wire.begin();
    RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    // grid.setPixelColor(0, colorRed);
  }
 

  // setup the LED strip
  grid.begin();
  grid.show();

  // set the bright ness of the strip
  grid.setBrightness(intBrightness);


  // initialize the buttons
  pinMode(FWDButtonPIN, INPUT);
  pinMode(REVButtonPIN, INPUT);

  // lets kick off the clock
  digitalClockDisplay();
}

void loop() {
  DateTime now = RTC.now();

    // time is set lets show the time
    if ((now.hour() < 7) | (now.hour() >= 19)) {
      intBrightness =  BRIGHTNESSNIGHT;
    } else {
      intBrightness =  BRIGHTNESSDAY;
    }
    grid.setBrightness(intBrightness);

    // test to see if a forward button is being held down for time setting
    if (digitalRead(FWDButtonPIN) == HIGH) {
      Serial.println("Forward Button Down");
      //incrementTime(60);
      RTC.adjust(now.unixtime() + 60);
    }

    // test to see if the back button is being held down for time setting
    if (digitalRead(REVButtonPIN) == HIGH) {
      Serial.println("Backwards Button Down");
      //incrementTime(-60);
      RTC.adjust(now.unixtime() -60);
    }

    // and finaly we display the time (provided we are not in self tes mode
    displayTime();
    // displayMonth();
    // numOneStunna();
    //thatsSoMetal();
    //paintWord(RYANNES, sizeof(RYANNES), colorGreen);

    grid.show();
  delay(600);
}

void digitalClockDisplay() {
  DateTime now = RTC.now();
  // digital clock display of the time
  Serial.print(now.hour());
  printDigits(now.minute());
  printDigits(now.second());
  Serial.print(" ");
  Serial.print(now.year());
  Serial.print("-");
  Serial.print(now.month());
  Serial.print("-");
  Serial.print(now.day());
  Serial.println();
}


void displayTime() {
   DateTime now = RTC.now();
   
       Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
   
  String strCurrentTime;
  paintWord(arrIT, sizeof(arrIT),  colorWhite);
  paintWord(arrIS, sizeof(arrIS),  colorWhite);

  // set up the struct
  struct mi {
    int* word;
    int length;
  };

  mi minuteINCMT[6];
  minuteINCMT[0] = mi {arrMFIVE, 4};
  minuteINCMT[1]  = mi {arrMTEN, 3};
  minuteINCMT[2]  = mi {arrA, 1};
  minuteINCMT[3]  = mi {QUARTER, 7};
  minuteINCMT[4]  = mi {arrHALF, 4};
  minuteINCMT[5]  = mi {arrTWENTY, 6};
  int selectedINCMT[] =  {0, 0, 0, 0, 0, 0};

  //hour array
  mi hourINCMT[12];
  hourINCMT[0]  = mi {arrONE, 3};
  hourINCMT[1]  = mi {arrTWO, 3};
  hourINCMT[2]  = mi {arrTHREE, 5};
  hourINCMT[3]  = mi {arrFOUR, 4};
  hourINCMT[4]  = mi {arrFIVE, 4};
  hourINCMT[5]  = mi {arrSIX, 3};
  hourINCMT[6]  = mi {arrSEVEN, 5};
  hourINCMT[7]  = mi {arrEIGHT, 5};
  hourINCMT[8]  = mi {arrNINE, 4};
  hourINCMT[9]  = mi {arrTEN, 3};
  hourINCMT[10]  = mi {arrELEVEN, 6};
  hourINCMT[11]  = mi {arrTWELVE, 6};
  int selectedHourINCMT[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  // now we display the appropriate minute counter
  
  if ((now.minute() > 0) && (now.minute() < 5)) {
    setMinuteInterval(5 - now.minute());
  }
  if ((now.minute() > 4) && (now.minute() < 10)) {
    // FIVE MINUTES
    strCurrentTime = "five ";
    selectedINCMT[0] = 1;
    setMinuteInterval(10 - now.minute());
  }
  if ((now.minute() > 9) && (now.minute() < 15)) {
    //TEN MINUTES;
    strCurrentTime = "ten ";
    selectedINCMT[1] = 1;
    setMinuteInterval(15 - minute());
  }
  if ((now.minute() > 14) && (now.minute() < 20)) {
    // QUARTER
    strCurrentTime = "a quarter ";
    selectedINCMT[2] = 1;
    selectedINCMT[3] = 1;
    setMinuteInterval(20 - now.minute());
  }
  if ((now.minute() > 19) && (now.minute() < 25)) {
    //TWENTY MINUTES
    strCurrentTime = "twenty ";
    selectedINCMT[5] = 1;
    setMinuteInterval(25 - now.minute());
  }
  if ((now.minute() > 24) && (now.minute() < 30)) {
    //TWENTY FIVE
    strCurrentTime = "twenty five ";
    selectedINCMT[0] = 1;
    selectedINCMT[5] = 1;
    setMinuteInterval(30 - now.minute());
  }
  if ((now.minute() > 29) && (now.minute() < 35)) {
    strCurrentTime = "half ";
    selectedINCMT[4] = 1;
    setMinuteInterval(35 - now.minute());
  }
  if ((now.minute() > 34) && (now.minute() < 40)) {
    //TWENTY FIVE
    strCurrentTime = "twenty five ";
    selectedINCMT[0] = 1;
    selectedINCMT[5] = 1;
    setMinuteInterval(40 - now.minute());
  }
  if ((now.minute() > 39) && (now.minute() < 45)) {
    strCurrentTime = "twenty ";
    selectedINCMT[5] = 1;
    setMinuteInterval(45 - minute());
  }
  if ((now.minute() > 44) && (now.minute() < 50)) {
    strCurrentTime = "a quarter ";
    selectedINCMT[2] = 1;
    selectedINCMT[3] = 1;
    setMinuteInterval(50 - now.minute());
  }
  if ((now.minute() > 49) && (now.minute() < 55)) {
    strCurrentTime = "ten ";
    selectedINCMT[1] = 1;
    setMinuteInterval(55 - now.minute());
  }
  if (now.minute() > 54) {
    strCurrentTime = "five ";
    selectedINCMT[0] = 1;
    setMinuteInterval(60 - now.minute());
  }

  //paint the words
  for (int i = 0; i < 6; i++) {
    if (!selectedINCMT[i]) {
      paintTWord(minuteINCMT[i].word, minuteINCMT[i].length,  colorBlack);
    } else {
      paintTWord(minuteINCMT[i].word, minuteINCMT[i].length,  colorWhite);
    }
  }


  // if we are less than 5 minutes past top of the hour
  if (now.minute() < 5) {
    switch (now.hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "one ";
        selectedHourINCMT[0] = 1;
        break;
      case 2:
      case 14:
        strCurrentTime = strCurrentTime + "two ";
        selectedHourINCMT[1] = 1;
        break;
      case 3:
      case 15:
        strCurrentTime = strCurrentTime + "three ";
        selectedHourINCMT[2] = 1;
        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "four ";
        selectedHourINCMT[3] = 1;
        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "five ";
        selectedHourINCMT[4] = 1;
        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "six ";
        selectedHourINCMT[5] = 1;
        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "seven ";
        selectedHourINCMT[6] = 1;
        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "eight ";
        selectedHourINCMT[7] = 1;
        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "nine ";
        selectedHourINCMT[8] = 1;
        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "ten ";
        selectedHourINCMT[9] = 1;
        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "eleven ";
        selectedHourINCMT[10] = 1;
        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "twelve ";
        selectedHourINCMT[11] = 1;
        break;
    }

    //paint the words
    for (int i = 0; i < 12; i++) {
      if (!selectedHourINCMT[i]) {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length, colorBlack);
      } else {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length, colorWhite);
      }
    }

    strCurrentTime = strCurrentTime + "oclock ";
    paintWord(arrPAST, sizeof(arrPAST), colorBlack);
    paintWord(arrOCLOCK, sizeof(arrOCLOCK), colorWhite);
    paintWord(arrTO, sizeof(arrTO), colorBlack);


    // we are more than 4 minutes but less and 35 minutes past top of hour
  } else if ((now.minute() < 35) && (now.minute() > 4)) {
    strCurrentTime = strCurrentTime + "past ";
    paintWord(arrPAST, sizeof(arrPAST),  colorWhite);
    paintWord(arrOCLOCK, sizeof(arrOCLOCK),  colorBlack);
    paintWord(arrTO, sizeof(arrTO), colorBlack);

    switch (now.hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "one ";
        selectedHourINCMT[0] = 1;
        break;
      case 2:
      case 14:
        strCurrentTime = strCurrentTime + "two ";
        selectedHourINCMT[1] = 1;
        break;
      case 3:
      case 15:
        strCurrentTime = strCurrentTime + "three ";
        selectedHourINCMT[2] = 1;
        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "four ";
        selectedHourINCMT[3] = 1;
        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "five ";
        selectedHourINCMT[4] = 1;
        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "six ";
        selectedHourINCMT[5] = 1;
        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "seven ";
        selectedHourINCMT[6] = 1;
        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "eight ";
        selectedHourINCMT[7] = 1;
        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "nine ";
        selectedHourINCMT[8] = 1;
        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "ten ";
        selectedHourINCMT[9] = 1;
        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "eleven ";
        selectedHourINCMT[10] = 1;
        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "twelve ";
        selectedHourINCMT[11] = 1;
        break;
    }
    //paint the words
    for (int i = 0; i < 12; i++) {
      if (!selectedHourINCMT[i]) {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length,  colorBlack);
      } else {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length,  colorWhite);
      }
    }

    // if we are greater than 34 minutes past the hour then display
    // the next hour, as we will be displaying a 'to' sign
  } else {
    strCurrentTime = strCurrentTime + "to ";
    paintWord(arrPAST, sizeof(arrPAST),  colorBlack);
    paintWord(arrOCLOCK, sizeof(arrOCLOCK),  colorBlack);
    paintWord(arrTO, sizeof(arrTO),  colorWhite);

    switch (now.hour()) {
      case 1:
      case 13:
        strCurrentTime = strCurrentTime + "two ";
        selectedHourINCMT[1] = 1;
        break;
      case 14:
      case 2:
        strCurrentTime = strCurrentTime + "three ";
        selectedHourINCMT[2] = 1;
        break;
      case 15:
      case 3:
        strCurrentTime = strCurrentTime + "four ";
        selectedHourINCMT[3] = 1;
        break;
      case 4:
      case 16:
        strCurrentTime = strCurrentTime + "five ";
        selectedHourINCMT[4] = 1;
        break;
      case 5:
      case 17:
        strCurrentTime = strCurrentTime + "six ";
        selectedHourINCMT[5] = 1;
        break;
      case 6:
      case 18:
        strCurrentTime = strCurrentTime + "seven ";
        selectedHourINCMT[6] = 1;
        break;
      case 7:
      case 19:
        strCurrentTime = strCurrentTime + "eight ";
        selectedHourINCMT[7] = 1;
        break;
      case 8:
      case 20:
        strCurrentTime = strCurrentTime + "nine ";
        selectedHourINCMT[8] = 1;
        break;
      case 9:
      case 21:
        strCurrentTime = strCurrentTime + "ten ";
        selectedHourINCMT[9] = 1;
        break;
      case 10:
      case 22:
        strCurrentTime = strCurrentTime + "eleven ";
        selectedHourINCMT[10] = 1;
        break;
      case 11:
      case 23:
        strCurrentTime = strCurrentTime + "twelve ";
        selectedHourINCMT[11] = 1;
        break;
      case 0:
      case 12:
      case 24:
        strCurrentTime = strCurrentTime + "one ";
        selectedHourINCMT[0] = 1;
        break;
    }
    //paint the words
    for (int i = 0; i < 12; i++) {
      if (!selectedHourINCMT[i]) {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length, colorBlack);
      } else {
        paintTWord(hourINCMT[i].word, hourINCMT[i].length, colorWhite);
      }
    }
  }

  // Change AM and PM colors
  if (now.hour() > 0 &&  now.hour() <= 6) {
    paintWord(AM, sizeof(AM), colorMagenta);
    paintWord(PM, sizeof(PM), colorBlack);
  }
  if ( now.hour() > 6 && now.hour() <= 10) {
    paintWord(AM, sizeof(AM), colorYellow);
    paintWord(PM, sizeof(PM), colorBlack);
  }
    if (now.hour() <= 12 && now.hour() > 10) {
    paintWord(AM, sizeof(AM), colorTeal);
    paintWord(PM, sizeof(PM), colorBlack);
  }
  if (now.hour() > 12 && now.hour() < 17) {
    paintWord(PM, sizeof(PM), colorLblue);
    paintWord(AM, sizeof(AM), colorBlack);
  }
  if ( now.hour() >= 17 && now.hour() < 22) {
    paintWord(PM, sizeof(PM), colorOrange);
    paintWord(AM, sizeof(AM), colorBlack);
  }
    if (now.hour() <= 24 && now.hour() > 22) {
    paintWord(PM, sizeof(PM), colorRed);
    paintWord(AM, sizeof(AM), colorBlack);
  }

  if (strCurrentTime != strTime) {
    digitalClockDisplay();
    strTime = strCurrentTime;
    if (strTime == "") {
      fadeIn(20);
    }
  } else {
    //    grid.show();
  }
}

void setMinuteInterval(int interval) {
  switch (interval) {
    case 1:
      grid.setPixelColor(196, colorBlue);
      grid.setPixelColor(199, colorBlue);
      grid.setPixelColor(198, colorBlue);
      grid.setPixelColor(197, colorBlue);
      break;
    case 2:
      grid.setPixelColor(196, colorBlue);
      grid.setPixelColor(199, colorBlue);
      grid.setPixelColor(198, colorBlue);
      grid.setPixelColor(197, colorBlack);
      break;
    case 3:
      grid.setPixelColor(196, colorBlue);
      grid.setPixelColor(199, colorBlue);
      grid.setPixelColor(198, colorBlack);
      grid.setPixelColor(197, colorBlack);
      break;
    case 4:
      grid.setPixelColor(196, colorBlue);
      grid.setPixelColor(199, colorBlack);
      grid.setPixelColor(198, colorBlack);
      grid.setPixelColor(197, colorBlack);
      break;
    case 5:
      grid.setPixelColor(196, colorBlack);
      grid.setPixelColor(199, colorBlack);
      grid.setPixelColor(198, colorBlack);
      grid.setPixelColor(197, colorBlack);
      break;
    case 0:
      grid.setPixelColor(196, colorBlack);
      grid.setPixelColor(199, colorBlack);
      grid.setPixelColor(198, colorBlack);
      grid.setPixelColor(197, colorBlack);
      break;

  }
}



//@todo
void RYANNESBIRTHDAY() {
  paintWord(DAY, sizeof(DAY), colorJGreen);
  delay(1000);
  paintWord(BIRTH, sizeof(BIRTH), colorJGreen);
  delay(1000);
  paintWord(RYANNE, sizeof(RYANNE), colorJGreen);
  delay(1000);
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void rainbow(uint8_t wait) {
  //secret rainbow mode
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < grid.numPixels(); i++) {
      grid.setPixelColor(i, Wheel((i + j) & 255));
    }
    grid.show();
    delay(wait);
  }
}

static void chase(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < grid.numPixels() + 4; i++) {
    grid.setPixelColor(i  , color); // Draw new pixel
    grid.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    grid.show();
    delay(wait);
  }
}

void fadeOut(int time) {
  for (int i = intBrightness; i > 0; --i) {
    grid.setBrightness(i);
    grid.show();
    delay(time);
  }
}

void fadeIn(int time) {
  for (int i = 1; i < intBrightness; ++i) {
    grid.setBrightness(i);
    grid.show();
    delay(time);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t color, uint8_t wait) {
  for (uint16_t i = 0; i < grid.numPixels(); i++) {
    grid.setPixelColor(i, color);
  }
  grid.show();
  delay(wait);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return grid.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return grid.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return grid.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



// Paint Time Keeping Word
void paintTWord(int* arrWord, int len, uint32_t intColor) {
  uint32_t pixelColor;
  for (int i = 0; i < len; i++) {
    grid.setPixelColor(arrWord[i], intColor);
  }
}

void paintWord(int* arrWord, size_t len, uint32_t intColor) {
  len = (len  / sizeof( int ));
  for (int i = 0; i < len; i++) {
    grid.setPixelColor(arrWord[i], intColor);
  }
}

void spellWord(int arrWord[], uint32_t intColor) {
  for (int i = 0; i < grid.numPixels() + 1; i++) {
    if (arrWord[i] == -1) {
      break;
    } else {
      grid.setPixelColor(arrWord[i], intColor);
      grid.show();
      delay(500);
    }
  }
}

// print out the software version number
void printVersion(void) {
  delay(2000);
  Serial.println();
  Serial.println("This word clock was specially built to celebrate the Marriage of Tommy and Ryanne");
  Serial.println("June 20th 2015");
  Serial.println();
}

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    pctime = pctime - 18000;
    return pctime;
    if ( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
    Serial.println();
    Serial.println("Time Set via Serial");
    Serial.println();
  }
  return pctime;
}

void thatsSoMetal() {
  paintWord(THATS, sizeof(THATS), colorRed);
  paintWord(SO, sizeof(SO), colorRed);
  paintWord(METAL, sizeof(METAL), colorRed);
}

void numOneStunna() {
  paintWord(NUMONE, sizeof(NUMONE), colorBlue);
  paintWord(STUNNA, sizeof(STUNNA), colorBlue);
}


//Birthdays
void isBirthday() {
   DateTime now = RTC.now();
  // Ryannes Birthday
  if ((now.month() == 3) && (now.day() == 29)) {
    paintWord(RYANNES, sizeof(RYANNES), colorBlue);
    paintWord(BIRTH, sizeof(BIRTH), colorBlue);
    paintWord(DAY, sizeof(DAY), colorBlue);
  }

  // Toms Birthday
  if ((now.month() == 7) && (now.day() == 10)) {
    paintWord(TOMMYS, sizeof(TOMMYS), colorRed);
    paintWord(BIRTH, sizeof(BIRTH), colorRed);
    paintWord(DAY, sizeof(DAY), colorRed);
  }

  // Jack's Birthday
  if ((now.month() == 7) && (now.day() == 11)) {
    paintWord(BIRTH, sizeof(BIRTH), colorRed);
    paintWord(DAY, sizeof(DAY), colorRed);
  }
}

