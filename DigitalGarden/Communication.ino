boolean communication() {
  digitalWrite(13, HIGH);
  Serial.println("I am communicating!!!");
  boolean done = false;
  lastCommunication = millis() / 1000;
  //All the communication needed


  if (done) {
    litreValue = 0;
  }
  digitalWrite(13, LOW);
  return done;
  
}
