/******************************************************
  Description: Extracted code related to the web server
  interaction

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
boolean update_web_score(byte pin) {
  // placeholder method, do it later
  // it should check which pin (home/away or home_cancel/away_cancel) is used in method and update score on the server according to actions that pin represents
  // return true if everything is ok and false if there is some error

  line_2 = status_sync;


  lcd.clear();
  printLcd();
  delay(500);
  return random(2) > 0; //simulating true or false responsee
}
void checkWebStatus() {
  StaticJsonBuffer<200> jsonBuffer;
  response_line =0;
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/get-next-match";

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (response_line == 12) {
      Serial.println("Processing response");
      response = line;
      JsonObject& responseJson = jsonBuffer.parseObject(response)["response"];
      line_1 = (const char*)responseJson["match"];
      line_2 = (const char*)responseJson["match_status"];
      home_score = responseJson["home"];
      away_score = responseJson["away"];
    }

    response_line++;
  }
  lcd.clear();

}



void startStopMatch() {
  // implement via WiFI
}



void changeScoreWS(String changeScore){
 // implement via WiFI
}

