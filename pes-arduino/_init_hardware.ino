/******************************************************
  Description: Extracted code related to the hardware
  initialization.
  
  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void  initHardware() {
  initLcd();
  initButtons();
  initSegmentDisplay();
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
