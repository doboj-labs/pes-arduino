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
   initWifi();
  // initButtons();
}



void initLcd() {
  lcd.init();
  lcd.backlight();
  line_1 = welcome;
  line_2 = version;
  printLcd();
}

void initButtons() {
  pinMode(start_stop_btn_pin, INPUT_PULLUP);
  pinMode(hs_btn_pin, INPUT_PULLUP);
  pinMode(as_btn_pin, INPUT_PULLUP);
}

void initWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

