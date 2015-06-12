void lcdVisualization(){
  lcd_setContrast(32);
 
  
  char buffer1[8];
char buffer2[2];
int add=0;
if(hourNow>100){
  add=1;
}
 if(millis()-lastLCD>=5000){
   lcd_setCursor(0, 0);

    lcd_printStr(floatToString(buffer2,hourNow,0));
    lcd_setCursor(2+add, 0);
    lcd_printStr(":");
    lcd_setCursor(3+add, 0);
    lcd_printStr(floatToString(buffer2,minuteNow,0));
  if(count++>7){
   count=0;
 }
     lcd_setCursor(0, 0);
    lcd_printStr(" ");
    lcd_setCursor(0, 1);
    lcd_printStr(" ");
   

    lcd_setCursor(0, 0);

    lcd_printStr(floatToString(buffer1,hourNow,0));


    lastLCD=millis();
    String timeToWater="";
    
    lcd_setCursor(0, 1);
    lcd_printStr("        ");
    
    switch(count){
      case 0:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,litreValue,2));
              lcd_setCursor(6, 1);
              lcd_printStr("A");
              break;
      case 1:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,tempValue,1));
              lcd_setCursor(6, 1);
              lcd_printStr("B");
              break;
      case 2:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,humidValue,1));
              lcd_setCursor(6, 1);
              lcd_printStr("C");
              break;
      case 3:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,soilMoistureValue,0));
              lcd_setCursor(6, 1);
              lcd_printStr("D");
              break;
      case 4:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,rainValue,0));
              lcd_setCursor(6, 1);
              lcd_printStr("E");
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
             if(water1){
                lcd_setCursor(0, 1);
             lcd_printStr("W1 done");
             }
             if(water2){
               lcd_setCursor(0, 1);
             lcd_printStr("W2 done");
             }
              
            // lcd_setCursor(6, 1);
             // lcd_printStr("F");
              break;
      case 6:
              //
             if(isWatering){
                lcd_setCursor(0, 1);
             lcd_printStr("WATERING");
             } else if(!isWatering){
                lcd_setCursor(0, 1);
             lcd_printStr("FINISHED");
             }
              break;
      case 7:
              //
              lcd_setCursor(0, 1);
              lcd_printStr(floatToString(buffer1,errors,0));
              lcd_setCursor(6, 1);
              lcd_printStr("H");
              break;
      default:
              break;
    }
 }


}
