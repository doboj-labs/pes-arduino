/******************************************************
  Description: Extracted code related to the
  7-segment display
  
  Website: www.codecentric.ba
  E-mail: bruno.raljic@codecentric.de
  Author: Bruno Raljic
  Date: 2017-01-23
******************************************************/
void sendCommand(uint8_t value)
{
  digitalWrite(stbPin, LOW);                   //pin low.  To begin receiving data
  shiftOut(dioPin, clkPin, LSBFIRST, value);   //send data(value) to the segment display module
  digitalWrite(stbPin, HIGH);                  //pin high.  Stop receiving data
}

void segmentNumber(byte number)
{
  sendCommand(0x40);                                       //setting the Write Data Command,using automatic address genuine.
  digitalWrite(stbPin, LOW);                               //pin low.  To begin receiving data
  shiftOut(dioPin, clkPin, LSBFIRST, 0xc0);                //Set the start address 0C0H
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 1000 % 10]); //thousand data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 100 % 10]); //hundred data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number / 10 % 10]); //ten data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  shiftOut(dioPin, clkPin, LSBFIRST, digits[number % 10]);   //bit data
  shiftOut(dioPin, clkPin, LSBFIRST, 0x00);                //filling high 8-bit data
  digitalWrite(stbPin, HIGH);                              //pin high.  Stop receiving data
}

void updateSegmentDisplay() {
  // finish implementation, since we will have two segment displays
  segmentNumber(away_score);
}
