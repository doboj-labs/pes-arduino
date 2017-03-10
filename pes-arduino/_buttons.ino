/******************************************************
  Description: Extracted code related to the buttons

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void listenStartStop() {
  if (isButtonPressed(start_stop_btn_pin)) {
    //startStopMatch();
    Serial.println("start/stop");
  }
}

void listenScoreButtons() {
  // Score can be changed only when the match is active
  if (line_2 != status_active) {
   // return;
  }

  for (byte b = 0; b < 4; b++) {
    byte  pin = score_pins[b];
    // Check first if button is pressed
    if (isButtonPressed(pin)) {
      // Check for which team to increase/decrease score
      int request_id;
      switch (pin) {
        case hs_btn_pin:
          home_score++;
          request_id = WS_INCREASE_HOME;
          Serial.print("home score ");
          Serial.println(home_score);
          break;
        case as_btn_pin:
          away_score++;
          request_id = WS_INCREASE_AWAY;
          Serial.print("away score");
          Serial.println(away_score);
          break;
        case hs_btn_cancel_pin:
          home_score--;
          request_id = WS_DECREASE_HOME;
          Serial.print("home cancel");
          Serial.println(home_score);
          break;
        case as_btn_cancel_pin:
          away_score--;
          request_id = WS_DECREASE_AWAY;
          Serial.print("away cancel");
          Serial.println(away_score);
          break;
      }
     // update_web_score(request_id);
    }
  }

}

boolean isButtonPressed(int pin) {
  if (readPressedAsBoolean(pin))       //Detection button interface to low
  {
    delay(10);                        //Delay 10ms for the elimination of key leading-edge jitter
    if (readPressedAsBoolean(pin))   //Confirm button is pressed
    {
      while (readPressedAsBoolean(pin)); //Wait for key interfaces to low
      delay(10);                      //delay 10ms for the elimination of key trailing-edge jitter
      while (readPressedAsBoolean(pin)); //Confirm button press
      return true;
    }
  } else {
    return false;
  }
}

boolean readPressedAsBoolean(int pin){
  if(pin==start_stop_btn_pin){
    return analogRead(pin) > 800;
  }else{
    return digitalRead(pin)==HIGH;
  }
}

