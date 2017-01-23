/************************************************************
  File name: pes-arduino.ino
  Description:

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-21
************************************************************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Hardware related
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f }; // Representing 0 - 9
LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to "0x3f"(or "0x27") for a 16 chars and 2 line display
const byte stbPin = 5;  //the segment display module STB pin connected to digital pin 5
const byte clkPin = 6;  //the segment display module CLK pin connected to digital pin 6
const byte dioPin = 7;  //the segment display module DIO pin connected to digital pin 7
const byte start_stop_btn_pin = 8; // button for starting/stopping match
const byte hs_btn_pin = 9; // button for home score
const byte as_btn_pin = 10; // button for away score
const byte hc_btn_pin = 11; // button for home cancel
const byte ac_btn_pin = 12; // button for away cancel
const byte score_pins[] = {hs_btn_pin, as_btn_pin, hc_btn_pin, ac_btn_pin};

// LCD lines
String line_1;
String line_2;
String tmp_line;

// Status
const char status_scheduled[] = "SCHEDULED!";
const char status_active[] = "ACTIVE";
const char status_sync[] = "SYNCING...";
const char status_sync_failed[] = "SYNC FAILED";
const char status_restart[] = "PLEASE RESTART";
const char status_revert[] = "REVERTING...";

byte home_score = 0;
byte away_score = 0;
int ms; // variable to keep milliseconds at specific point of time in execution
const int sync_timeout = 60000; // 60 seconds

void setup(void)
{
  // init hardware
  initLcd();
  initButtons();
  initSegmentDisplay();

  // init webservices
  checkWebStatus();
}

void loop(void)
{
  listenStartStop();
  listenScoreButtons();
  printLcd();
}

void initLcd() {
  lcd.init();
  lcd.backlight();
  line_1 = "Welcome!";
  line_2 = "pes-arduino v0.1";
  printLcd();
}

void initButtons() {
  pinMode(start_stop_btn_pin, INPUT);
  pinMode(hs_btn_pin, INPUT);
  pinMode(as_btn_pin, INPUT);
  pinMode(hc_btn_pin, INPUT);
  pinMode(ac_btn_pin, INPUT);
}

void initSegmentDisplay() {
  pinMode(stbPin, OUTPUT); //initialize the stbPin as an output
  pinMode(clkPin, OUTPUT); //initialize the clkPin as an output
  pinMode(dioPin, OUTPUT); //initialize the dioPin as an output
  sendCommand(0x8f);       //activate
  updateSegmentDisplay();
}

void listenStartStop() {
  // dummy implementation, should call web server to check the status
  // currently it's just toggling status active/scheduled and reseting score
  if (isButtonPressed(start_stop_btn_pin)) {
    lcd.clear();
    if (line_2 == status_scheduled)
    { line_2 = status_active;
    } else {
      line_2 = status_scheduled;
      home_score = 0;
      away_score = 0;
      updateSegmentDisplay();
    }
  }
}


void listenScoreButtons() {
  // Score can be changed only when the match is active
  if (line_2 != status_active) {
    return;
  }

  for (byte b = 0; b < 4; b++) {
    byte  pin = score_pins[b];
    if (b == 0 || b == 2) {
      //I'm using only 2 out of 4 buttons, so two pins have to be skipped
      // Otherwise, blank lcd will appear
      // 0 - HS, 1 - AS, 2 - HC, 3 - AC
      continue;
    }

    // Check first if button is pressed
    if (isButtonPressed(pin)) {
      if ((pin == hc_btn_pin && home_score == 0) || (pin == ac_btn_pin && away_score == 0)) {
        // No need for any action if score is 0 and you want to decrease it
        continue;
      }

      // Check for which team to increase/decrease score
      switch (pin) {
        case hs_btn_pin:
          home_score++;
          break;
        case as_btn_pin:
          away_score++;
          break;
        case hc_btn_pin:
          if (home_score > 0)
            home_score--;
          break;
        case ac_btn_pin:
          if (away_score > 0)
            away_score--;
          break;
      }

      updateSegmentDisplay();
      ms = millis();
      tmp_line = line_2;

      // Trying to update web score for specified amount of time, and then suggest
      // device to be restarted (because server is inactive)
      while (!update_web_score(pin)) {
        if (millis() - ms > sync_timeout) {
          line_1 = status_sync_failed;
          line_2 = status_restart;
          break;
        }

      }
      line_2 = tmp_line; // revert line 2 to previous state (before syncing)
      lcd.clear();

    }
  }

}

boolean update_web_score(byte pin) {
  // placeholder method, do it later
  // it should check which pin (home/away or home_cancel/away_cancel) is used in method and update score on the server according to actions that pin represents
  // return true if everything is ok and false if there is some error

  if (pin == hc_btn_pin || pin == ac_btn_pin) {
    line_2 = status_revert;
  } else {
    line_2 = status_sync;

  }

  lcd.clear();
  printLcd();
  delay(500);
  return random(2) > 0; //simulating true or false responsee
}
boolean isButtonPressed(int pin) {
  if (digitalRead(pin) == LOW)       //Detection button interface to low
  {
    delay(10);                        //Delay 10ms for the elimination of key leading-edge jitter
    if (digitalRead(pin) == LOW)   //Confirm button is pressed
    {
      while (digitalRead(pin) == LOW); //Wait for key interfaces to low
      delay(10);                      //delay 10ms for the elimination of key trailing-edge jitter
      while (digitalRead(pin) == LOW); //Confirm button press
      return true;
    }
  } else {
    return false;
  }
}
void printLcd() {
  // This if block will be removed, since home/away score wont be on lcd
  if (!(line_1 == "Welcome!" || line_1 == status_sync_failed)) {
    lcd.setCursor(0, 0);
    lcd.print(home_score);
    if (away_score > 9) {
      lcd.setCursor(14, 0);
    } else {
      lcd.setCursor(15, 0);
    }
    lcd.print(away_score);
  }

  printCenter(line_1, 0);
  printCenter(line_2, 1);
}

void printCenter(String line, byte index) {
  lcd.setCursor((16 - line.length()) / 2, index);
  lcd.print(line);
}

void checkWebStatus() {
  // dummy implementation, should set teams in line 1 and status in line 2 if everything is ok,
  // otherwise set some error msg
  delay(2000);
  lcd.clear();
  line_1 = "BOR vs JUV";
  line_2 = status_scheduled;
}

// SEGMENT DISPLAY
void sendCommand(uint8_t value)
{
  digitalWrite(stbPin, LOW);                   //pin low.  To begin receiving data
  shiftOut(dioPin, clkPin, LSBFIRST, value);   //send data(value) to the segment display module
  digitalWrite(stbPin, HIGH);                  //pin high.  Stop receiving data
}

void segmentNumber(byte number)
{
  sendCommand(0x40);                                       //setting the Write Data Command,using automatic address genuine.
  digitalWrite(stbPin, LOW);                               //pin low.  To begin receiving data
  shiftOut(dioPin, clkPin, LSBFIRST, 0xc0);                //Set the start address 0C0H
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 1000 % 10]); //thousand data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 100 % 10]); //hundred data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 10 % 10]); //ten data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number % 10]);   //bit data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  digitalWrite(stbPin, HIGH);                              //pin high.  Stop receiving data
}

void updateSegmentDisplay() {
  // finish implementation, since we will have two segment displays
  segmentNumber(away_score);
}

