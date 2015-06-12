void RTCSetup() {
  //RTC INIZIO
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    errors *= 10;
    errors += 5;
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2015, 6, 6, 3, 30, 0));
  }
  //RTC FINE
}





void lcdSetup() {

  pinMode(gndPin, OUTPUT);

  digitalWrite(gndPin, LOW);

  pinMode(vddPin, OUTPUT);

  digitalWrite(vddPin, HIGH);

  delay(500);

  Wire.begin();

  lcd_cmd(0b00111000); // function set

  lcd_cmd(0b00111001); // function set

  lcd_cmd(0b00000100); // EntryModeSet

  lcd_cmd(0b00010100); // interval osc

  lcd_cmd(0b01110000 | (contrast & 0xF)); // contrast Low

  lcd_cmd(0b01011100 | ((contrast >> 4) & 0x3)); // contast High/icon/power

  lcd_cmd(0b01101100); // follower control

  delay(200);

  lcd_cmd(0b00111000); // function set

  lcd_cmd(0b00001100); // Display On

  lcd_cmd(0b00000001); // Clear Display

  delay(2);
}


void lcd_cmd(byte x) {

  Wire.beginTransmission(I2Cadr);

  Wire.write(0b00000000); // CO = 0,RS = 0

  Wire.write(x);

  Wire.endTransmission();

}



void lcd_contdata(byte x) {

  Wire.write(0b11000000); // CO = 1, RS = 1

  Wire.write(x);

}



void lcd_lastdata(byte x) {

  Wire.write(0b01000000); // CO = 0, RS = 1

  Wire.write(x);

}



// Show the string.

void lcd_printStr(const char *s) {

  Wire.beginTransmission(I2Cadr);

  while (*s) {

    if (*(s + 1)) {

      lcd_contdata(*s);

    } else {

      lcd_lastdata(*s);

    }

    s++;

  }

  Wire.endTransmission();

}



// Set the character location.

void lcd_setCursor(byte x, byte y) {

  lcd_cmd(0x80 | (y * 0x40 + x));

}

void lcd_setContrast(byte c) {

  lcd_cmd(0x39);

  lcd_cmd(0b01110000 | (c & 0x0f)); // contrast Low

  lcd_cmd(0b01011100 | ((c >> 4) & 0x03)); // contast High/icon/power

  lcd_cmd(0x38);

}
