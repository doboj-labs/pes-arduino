/*******************************************************************************
  File name: pes-arduino.ino
  Description: This code runs a device which is used to
  automate the process of information transfer in our
  PES tournaments.

  HARDWARE:
    5 buttons
    - start/stop
    - home score
    - away score
    - home cancel score
    - away cancel score
    LCD display to show additional information (current match, status msgs etc.)
    - Arduino UNO D1 (wifi integrated)

  WORKFLOW:
    After starting the device, webservice is called to check if connection
           is secured and to retrieve current match,
    Match can be on status SCHEDULED/ACTIVE,
    If status is active, users can press the score buttons
           (to increase or decrease buttons),
    On every score button press, web service is called again
           to trigger further actions,
    If score is 0, user can't decrease score,
    There is a timeout for updating score, if that timeout is reached,
           web server is to be checked and device should be restarted
    Switching from status active to status scheduled will reset score to 0:0

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-21
********************************************************************************/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// Hardware related
LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to "0x3f"(or "0x27") for a 16 chars and 2 line display
const byte hs_btn_pin = 13; // button for home score
const byte as_btn_pin = 14; // button for away score
const byte hs_btn_cancel_pin=12; // button for home score cancel
const byte as_btn_cancel_pin=15; // button for home score cancel
const byte start_stop_btn_pin = A0; // button for starting/stopping match
const byte score_pins[] = {hs_btn_pin, as_btn_pin, hs_btn_cancel_pin, as_btn_cancel_pin};

// WiFi/Web
WiFiClient client;
const int httpPort = 80;
const char* ssid     = "*";
const char* password = "*";
const char* host = "doboj-labs-pes-api.herokuapp.com";
int response_line = 0;
const int WS_GET_CURRENT_MATCH = 0;
const int WS_START_STOP_MATCH = 1;
const int WS_INCREASE_HOME = 2;
const int WS_INCREASE_AWAY = 3;
const int WS_DECREASE_HOME = 4;
const int WS_DECREASE_AWAY = 5;

// LCD lines
String line_1;
String line_2;

// Status
const char status_active[] = "active";
const char status_label[] = "Status";
const char welcome[] = "Welcome!";
const char version[] = "Jim Beglin v2000";

byte home_score = 0;
byte away_score = 0;
int ms; // variable to keep milliseconds at specific point of time in execution


void setup(void)
{
  Serial.begin(250000);

  // init hardware
  initHardware();

  // init webservices
  getCurrentMatch();

  // ms = millis(); // for butonlessTest
}

void loop(void)
{
  printLcd();
  listenStartStop();
  listenScoreButtons();
}
