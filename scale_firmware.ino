

// Time - Version: Latest 
#include <Time.h>
#include <TimeLib.h>

// HX711 - Version: Latest 
#include <HX711.h>

HX711 scale;
#define DOUT A0
#define CLK A1
#define redLED 12
#define redLedOn digitalWrite(redLED, HIGH)
#define redLedOff digitalWrite(redLED, LOW)

const long maxCapacity = 1000; //Weight that can be removed
long currentCapacity;
long lastReading;
int lastMinute;
long reading;
time_t t = now();

void setup() {
  Serial.begin(9600);
  Serial.println("Calibration of scale");
  pinMode(redLED, OUTPUT);
  scale.begin(DOUT, CLK);
  scale.set_scale(-26.48); //Calibration factor -25442 / 961
  scale.tare();
  ledBlink(1, 300);
  currentCapacity = maxCapacity;
  lastReading = getReading();
  lastMinute = 0;
  Serial.println("Done");
}

void loop() {
  
  reading = getReading();
  
  if(reading < lastReading){
    long diff = lastReading-reading;
    currentCapacity -= diff;
    lastReading = reading;
  }
  if(reading > lastReading){
    Serial.println("--Weight added--");
    scale.tare();
    lastReading = 0;
  }

  
  if(hasOneMinutePassed()) {
    Serial.println("--New minute--");
    if(currentCapacity < maxCapacity){
      currentCapacity += 50;
    }
    if(currentCapacity > maxCapacity){
      currentCapacity = maxCapacity;
    }
  }
  
  if(currentCapacity < 300){
    ledBlink(1, 200);
    Serial.println("--Warning, Capacity--");
  }
  
  Serial.print("Reading: ");
  Serial.println(reading);
  Serial.print("current capacity: ");
  Serial.println(currentCapacity);
  
}

boolean hasOneMinutePassed(){
  t = now();
  Serial.print("Time: ");
  Serial.print(minute(t));
  Serial.print(":");
  Serial.println(second(t));
  
  if(lastMinute != minute(t)){
    lastMinute = minute(t);
    return true;
  }
  return false;
}

void ledBlink(int blinkNr, int delaytime){
  
  for (int i = 0; i < blinkNr; i++){
    redLedOn;
    delay(delaytime);
    redLedOff;
    delay(delaytime);
  }
}

long getReading(){
  return 10*((long)scale.get_units(10)/10);
}
