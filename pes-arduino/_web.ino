/******************************************************
  Description: Extracted code related to the web server
  interaction

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void update_web_score(int request_id) {
  makeRequest(request_id);
  getCurrentMatch();
}
void getCurrentMatch() {
  makeRequest(WS_GET_CURRENT_MATCH);
}

void startStopMatch() {
  makeRequest(WS_START_STOP_MATCH);
  getCurrentMatch();
}

void makeRequest(int request_id) {
  response_line = 0;

  if (!client.connect(host, httpPort)) {
    line_1 = status_label;
    line_2 = "srv unavailable";
    lcd.clear();
    return;
  }

  String url;
  switch (request_id) {
    case WS_GET_CURRENT_MATCH:
      url = "/get-next-match";
      break;
    case WS_START_STOP_MATCH:
      url = "/start-stop-match";
      break;
    case WS_INCREASE_HOME:
      url = "/increment-decrement-score?command=inc&team=home";
      break;
    case WS_INCREASE_AWAY:
      url = "/increment-decrement-score?command=inc&team=away";
      break;
    case WS_DECRESASE_HOME:
      url = "/increment-decrement-score?command=dec&team=home";
      break;
    case WS_DECRESASE_AWAY:
      url = "/increment-decrement-score?command=dec&team=away";
      break;

  }

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      line_1 = status_label;
      line_2 = "Client Timeout !";
      client.stop();
      return;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (request_id == WS_GET_CURRENT_MATCH) {
      if (response_line == 0 && line.substring(9, 12) == "404") {
        // no matches found, tournament is over
        line_1 = status_label;
        line_2 = "No more matches!";
        lcd.clear();
        printLcd();
        return;
      }

      if (response_line == 12) {
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& responseJson = jsonBuffer.parseObject(line)["response"];
        line_1 = (const char*)responseJson["match"];
        line_2 = (const char*)responseJson["match_status"];
        home_score = responseJson["home"];
        away_score = responseJson["away"];
        lcd.clear();
      }
    }

    response_line++;
  }

}

