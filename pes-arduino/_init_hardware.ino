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
  initWifi();
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

void initWifi() {
  Serial.print("Connecting to ");
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

