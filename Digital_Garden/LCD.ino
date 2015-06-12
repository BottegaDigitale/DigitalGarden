void lcdVisualization(){
 if(count++>7){
   count=0;
 }
 if(millis()-lastLCD>=5000){
     lcd_setCursor(0, 0);
    lcd_printStr(" ");
    lcd_setCursor(0, 1);
    lcd_printStr(" ");

    lcd_setCursor(0, 0);

    lcd_printStr("Bottega");


    lastLCD=millis();
    String abbato="";
    switch(count){
      case 0:
              //
              lcd_setCursor(0, 1);
              abbato=floatToString(litreValue);
              break;
      case 1:
              //
              break;
      case 2:
              //
              break;
      case 3:
              //
              break;
      case 4:
              //
              break;
      case 5:
              //
              break;
      case 6:
              //
              break;
      case 7:
              //
              break;
      default:
              break;
    }
 }


}
