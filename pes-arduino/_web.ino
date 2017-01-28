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
  ether.browseUrl(PSTR("/get-next-match"), "", website, callback_check_ws);
}

// called when the client request is complete
static void callback_check_ws(byte status, word off, word len) {

  tmp_line = "";


  if (strncmp_P((char*) Ethernet::buffer + off, PSTR("HTTP"), 4) == 0) {
    nextSeq = ether.getSequenceNumber();
  }

  if (nextSeq != ether.getSequenceNumber()) {
    // Serial.print(F("<IGNORE DUPLICATE(?) PACKET>"));
    return;
  }

  uint16_t payloadlength = ether.getTcpPayloadLength();
  int16_t chunk_size   = BUFFERSIZE - off - 1;
  int16_t char_written = 0;
  int16_t char_in_buf  = chunk_size < payloadlength ? chunk_size : payloadlength;


  while (char_written < payloadlength) {
    // Serial.write((char*) Ethernet::buffer + off, char_in_buf);
    tmp_line += (char*) Ethernet::buffer + off;
    index_begin = tmp_line.indexOf("{\"response\":");
    index_end = tmp_line.indexOf("}");
    if (index_begin >= 0 && index_end >= 0) {
      StaticJsonBuffer<200> jsonBuffer;
      response = tmp_line.substring(index_begin + 13, index_end + 1);
      Serial.println(response);
      JsonObject& responseJson = jsonBuffer.parseObject(response);

      responseJson.printTo(Serial);
      lcd.clear();
      line_1 = (const char*)responseJson["match"]; // casting to String
      line_2 = (const char*)responseJson["match_status"];

      home_score = responseJson["home"];
      away_score = responseJson["away"];

   
    }

    char_written += char_in_buf;
    char_in_buf = ether.readPacketSlice((char*) Ethernet::buffer + off, chunk_size, off + char_written);
  }
  nextSeq += ether.getTcpPayloadLength();
}

void startStopMatch() {
  ether.browseUrl(PSTR("/start-stop-match"), "", website, noOp);
}

static void noOp(byte status, word off, word len) {
  // Serial.println("no op called");
}

void changeScoreWS(String changeScore){
 char cx[40];
  changeScore.toCharArray(cx, 40);
   ether.browseUrl(PSTR(("/increment-decrement-score")), cx, website, noOp);
}

