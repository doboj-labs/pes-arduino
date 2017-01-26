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
    Segment display to display home/away score

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
#include <EtherCard.h>

// Hardware related
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 }; // ethernet interface mac address, must be unique on the LAN
const uint8_t digits[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f }; // Representing 0 - 9
LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to "0x3f"(or "0x27") for a 16 chars and 2 line display
const byte stbPin = 5;  //the segment display module STB pin connected to digital pin 5
const byte clkPin = 6;  //the segment display module CLK pin connected to digital pin 6
const byte dioPin = 7;  //the segment display module DIO pin connected to digital pin 7
const byte start_stop_btn_pin = 0; // button for starting/stopping match
const byte hs_btn_pin = 1; // button for home score
const byte as_btn_pin = 2; // button for away score
const byte hc_btn_pin = 3; // button for home cancel
const byte ac_btn_pin = 4; // button for away cancel
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
const char status_error[] = "ERR";

byte home_score = 0;
byte away_score = 0;
int ms; // variable to keep milliseconds at specific point of time in execution
const int sync_timeout = 60000; // 60 seconds

// Ethernet / web
#define BUFFERSIZE 400
byte Ethernet::buffer[BUFFERSIZE];
const char website[] PROGMEM = "doboj-labs-pes-api.herokuapp.com";
int index_begin = -1;
int index_end = -1;
String response = "";
uint32_t nextSeq;

void setup(void)
{
  Serial.begin(57600);
  // init hardware
  initHardware();

  // init webservices
  checkWebStatus();

}

void loop(void)
{
  ether.packetLoop(ether.packetReceive());
  listenStartStop();
  listenScoreButtons();
  printLcd();
}




