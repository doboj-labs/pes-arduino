/******************************************************
  Description: Extracted code related to the buttons

  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void listenStartStop() {
  // dummy implementation, should call web server to check the status
  // currently it's just toggling status active/scheduled and reseting score
  if (isButtonPressed(start_stop_btn_pin)) {
    lcd.clear();
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
      String changeScore;
      switch (pin) {
        case hs_btn_pin:
          home_score++;
          changeScore = "?command=inc&team=home";
          break;
        case as_btn_pin:
          away_score++;
          changeScore = "?command=inc&team=away";
          break;
      }
      changeScoreWS(changeScore);
      ms = millis();
      tmp_line = line_2;

      // Trying to update web score for specified amount of time, and then suggest
      // device to be restarted (because server is inactive)
      while (!update_web_score(pin)) {
        if (millis() - ms > sync_timeout) {
          line_1 = status_sync_failed;
          line_2 = status_restart;
          break;
        }

      }
      line_2 = tmp_line; // revert line 2 to previous state (before syncing)
      lcd.clear();

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
