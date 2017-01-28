/******************************************************
  Description: Extracted code related to the hardware
  initialization.

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void  initHardware() {
   initWifi();
  initLcd();
  // initButtons();
  Serial.println("init hw finished");
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
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

