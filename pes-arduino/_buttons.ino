/******************************************************
  Description: Extracted code related to the buttons

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void listenStartStop() {
  if (isButtonPressed(start_stop_btn_pin)) {
    startStopMatch();
  }
}

void listenScoreButtons() {
  // Score can be changed only when the match is active
  if (line_2 != status_active) {
    return;
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
          break;
        case as_btn_pin:
          away_score++;
          request_id = WS_INCREASE_AWAY;
          break;
          //Continue for other pins
      }
      update_web_score(request_id);
    }
  }

}

boolean isButtonPressed(int pin) {
  if (digitalRead(pin) == LOW)       //Detection button interface to low
  {
    delay(10);                        //Delay 10ms for the elimination of key leading-edge jitter
    if (digitalRead(pin) == LOW)   //Confirm button is pressed
    {
      while (digitalRead(pin) == LOW); //Wait for key interfaces to low
      delay(10);                      //delay 10ms for the elimination of key trailing-edge jitter
      while (digitalRead(pin) == LOW); //Confirm button press
      return true;
    }
  } else {
    return false;
  }
}
