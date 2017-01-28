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
  // implement via WiFI
}



void startStopMatch() {
  // implement via WiFI
}



void changeScoreWS(String changeScore){
 // implement via WiFI
}

