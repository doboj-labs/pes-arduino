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
void checkWebStatus() {
  // dummy implementation, should set teams in line 1 and status in line 2 if everything is ok,
  // otherwise set some error msg

  ms = millis();

  // Trying to initialize for specified amount of time, and then suggest
  // device to be restarted (because server is inactive)
  JsonObject& response = getCurrentMatch();
  String resp_status = (const char*)response["status"];
  while (resp_status == status_error) {
    resp_status = (const char*)getCurrentMatch()["status"];
    if (millis() - ms > sync_timeout) {
      line_1 = status_sync_failed;
      line_2 = status_restart;
      lcd.clear();
      return;
    }
  }


  line_1 = (const char*)response["match"]; // casting to String
  line_2 = (const char*)response["match_status"];



  if (line_2 == status_active) {
    home_score = response["home"];
    away_score = response["away"];
  } else if (line_2 == status_scheduled) {
    home_score = 0;
    away_score = 0;
  }
}

JsonObject& getCurrentMatch() {
  delay(random(2000));
  // Agree on returning status
  char response_1[] = "{\"status\":\"OK\",\"match\":\"MCU vs LIV\",\"match_status\":\"ACTIVE\",\"home\":3, \"away\":1}";
  char response_2[] = "{\"status\":\"OK\",\"match\":\"BOR vs JUV\",\"match_status\":\"SCHEDULED!\",\"home\":0, \"away\":0}";
  char response_3[] = "{\"status\":\"ERR\",\"err_msg\":\"something wrong\"}";
  String responses[] = {response_1, response_2, response_3};


  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(responses[random(3)]);
  return root;
}

