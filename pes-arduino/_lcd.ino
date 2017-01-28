/*******************************************************
  Description: Extracted code related to the LCD display
  
  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
*******************************************************/
void printLcd() {
  if (!(line_1 == welcome || line_1 == status_sync_failed)) {
    lcd.setCursor(0, 0);
    lcd.print(home_score);
    if (away_score > 9) {
      lcd.setCursor(14, 0);
    } else {
      lcd.setCursor(15, 0);
    }
    lcd.print(away_score);
  }

  printCenter(line_1, 0);
  printCenter(line_2, 1);
}

void printCenter(String line, byte index) {
  lcd.setCursor((16 - line.length()) / 2, index);
  lcd.print(line);
}
