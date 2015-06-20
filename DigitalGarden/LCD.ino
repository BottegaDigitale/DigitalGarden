void lcdVisualization() {
  lcd_setContrast(32);
  
  int add = 0;
  if (hourNow > 100) {
    add = 1;
  }
  if ((millis()/1000) - lastLCD >= 2) {
    lcd_setCursor(1, 0);

    lcd_printStr(floatToString(buffer2, hourNow, 0));
    lcd_setCursor(3 + add, 0);
    lcd_printStr(":");
    lcd_setCursor(4 + add, 0);
    lcd_printStr(floatToString(buffer2, minuteNow, 0));
    if (count++ > 7) {
      count = 0;
    }
    lcd_setCursor(0, 0);
    lcd_printStr(" ");
    lcd_setCursor(0, 1);
    lcd_printStr(" ");


    lastLCD = millis()/1000;
    String timeToWater = "";

    lcd_setCursor(0, 1);
    lcd_printStr("        ");

    switch (count) {
      case 0:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("Lt:");
        lcd_setCursor(3, 1);
        lcd_printStr(floatToString(buffer1, litreValue / 1000, 2));
        break;
      case 1:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("T:");
        lcd_setCursor(3, 1);
        lcd_printStr(floatToString(buffer1, tempValue, 1));
        lcd_setCursor(7, 1);
        lcd_printStr("C");
        break;
      case 2:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("H:");
        lcd_setCursor(2, 1);
        lcd_printStr(floatToString(buffer1, humidValue, 1));
        break;
      case 3:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("S:");
        lcd_setCursor(3, 1);
        lcd_printStr(floatToString(buffer1, soilMoistureValue, 0));
        break;
      case 4:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("R:");
        lcd_setCursor(3, 1);
        lcd_printStr(floatToString(buffer1, rainValue, 0));
        break;
      case 5:
        /*
         timeToWater="";
        int timeTo1=secondsAlarm1-secondsNow;
        int timeTo2=secondsAlarm2-secondsNow;

        if(timeTo1<timeTo2){
          timeToWater=String(timeTo1/60);
          timeToWater+=":";
          timeToWater+=String(timeTo1%60);
        }else{
           timeToWater=String(timeTo2/60);
          timeToWater+=":";
          timeToWater+=String(timeTo2%60);
        }


        lcd_setCursor(0, 1);
        // lcd_printStr(timeTo1); float??*/
  if (!water1 && !water2){
    lcd_setCursor(0, 1);
          lcd_printStr("noW1or2");
  }else if (water1 && !water2) {
          lcd_setCursor(0, 1);
          lcd_printStr("W1 done");
        }else if (water1 && water2) {
          lcd_setCursor(0, 1);
          lcd_printStr("W12 done");
        }

        // lcd_setCursor(6, 1);
        // lcd_printStr("F");
        break;
      case 6:
        //
        if (isWatering) {
          lcd_setCursor(0, 1);
          lcd_printStr("WATERING");
        }else{
        lcd_setCursor(0, 1);
          lcd_printStr("Watngoff");
        }
        break;
      case 7:
        //
        lcd_setCursor(0, 1);
        lcd_printStr("E:"),
                     lcd_setCursor(2, 1);
        lcd_printStr(floatToString(buffer1, errors, 0));
        break;
      default:
        break;
    }
  }


}
