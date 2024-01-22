
void Measure_Breath_Heart_rate() {
  radar.Breath_Heart();  //Breath and heartbeat information output
  if (radar.sensor_report != 0x00) {
    switch (radar.sensor_report) {
      case HEARTRATEVAL:
        Serial.print("Sensor monitored the current heart rate value is: ");
        Serial.print(radar.heart_rate, DEC);
        myData.heartbeatRate = int(radar.heart_rate);
        Serial.println();
        break;
      case BREATHVAL:
        Serial.print("Sensor monitored the current breath rate value is: ");
        Serial.print(radar.breath_rate, DEC);
        myData.breathingsPerMinute = int(radar.breath_rate);
        Serial.println();
        break;
    }
    
    //always send a new message if any data changed
    sendESPNowMessage();
  }
}

void human_presence()
{
  radar.HumanExis_Func();           //Human existence information output
  if(radar.sensor_report != 0x00){
    switch(radar.sensor_report){
      case BODYVAL:
        Serial.print("The parameters of human body signs are: ");
        Serial.println(radar.bodysign_val, DEC);
        Serial.println("----------------------------");
        break;
    }
  }
}

void HR_BR() {
  measure_Intial = millis();
  while ((millis() - measure_Intial) <= measure_interval) {
    Measure_Breath_Heart_rate();
  }
}