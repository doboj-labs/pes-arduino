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
  //initWifi();
}



void initLcd() {
  lcd.init();
  lcd.backlight();
  line_1 = welcome;
  line_2 = version;
  printLcd();
}

void initWifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    line_1 = status_label;
    line_2 = "init wifi...";
    lcd.clear();
    printLcd();
  }
}

