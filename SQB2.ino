#include "Definitions.h"
#include "Sensors.h"
#include <ESP32Servo.h>

Servo servoDir;
int vel = 0;
int teta = 90;
int prevTeta;
bool go = false;

void setup() {
    // Servo configuration
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    servoDir.setPeriodHertz(50);
    servoDir.attach(servoPin, 1000, 2000);
    servoDir.write(teta);
    delay(100);

    // LED control configuration
    ledcSetup(pwmChannel, 10, 8);
    ledcAttachPin(pwmPin, pwmChannel);
    ledcWrite(pwmChannel, 0);

    // Pinout configuration
    pinMode(goPin, INPUT);            // goPin --> manual traction control.
    pinMode(triggerPin, OUTPUT);      // Ultrasonic sensor trigger.
    pinMode(echoPin, INPUT);          // Ultrasonic sensor echo.
    digitalWrite(triggerPin, LOW);    
    pinMode(LED_BUILTIN, OUTPUT);     // Built-in LED
    digitalWrite(LED_BUILTIN, LOW);

    // Serial communication configuration
    Serial.begin(115200);                           // USB Serial COM
    Serial1.begin(115200, SERIAL_8N1, rfTx, rfRx);  // LoRa Serial COM
    Serial2.begin(9600);                            // GPS Serial COM
    delay(200);

    compass.init();
    compass.setCalibrationOffsets(-114.00, -536.00, -355.00);
    compass.setCalibrationScales(0.79, 0.81, 2.04);

}

void loop() {
    float hdg = Get_heading();

    double objLat = pathLat[pathIndx];
    double objLng = pathLng[pathIndx];

    // Read GPS data
    while (Serial2.available() > 0) {
        if (gps.encode(Serial2.read())) {
            // Cannot encode
        }
    }

    if( gps.location.isValid() ){
      digitalWrite(LED_BUILTIN, LOW);
      float dist2obj = gps.distanceBetween(gps.location.lat(), gps.location.lng(), pathLat[pathIndx], pathLng[pathIndx] );
      float alpha = gps.courseTo( gps.location.lat(), gps.location.lng(), pathLat[pathIndx], pathLng[pathIndx] );
      float gamma = hdg - alpha;
      bool xyz = false;
      if( xyz ){
        if(prevTeta > 90){
          teta = 180;
        }else{
          teta = 0;
        }
      }
      else{
          if (abs(gamma) <= 180) {
          // Object is in front of me
            if (gamma >= 0) {
                // Object is to the left
                teta = map(abs(gamma), 0, 180, 90, 180);
            } else {
                // Object is to the right
                teta = map(abs(gamma), 0, 180, 0, 90);
            }
        } else {
            // Object is behind
            if (gamma >= 0) {
                teta = map((360 - abs(gamma)), 0, 180, 0, 90);
            } else {
                teta = map((360 - abs(gamma)), 0, 180, 90, 180);
            }
        }
        prevTeta = teta;
      }

      if(dist2obj < 10){
        if(pathIndx <= 0){
          pathIndx++;
        }else{
          go = false;
        }
      }
      else{
        go = true;
      }

      // Print values
      Serial.println("-----lat--------------lng----------dist------go-----hdg-----alpha--------gamma---------teta");
      Serial.printf("%10.7f     %10.7f   %9.3f     %5s   %7.2f   %8.2f   %8.2f   %8.2f  \n", gps.location.lat(), gps.location.lng(), dist2obj, go ? "true" : "false", hdg, alpha, gamma, teta);

    }
    else{
      go = false;
      if(UltraSonicScan(triggerPin, echoPin)){
        teta = 180;
      }
      else{
        teta = 90;
      }
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.printf("-->No GPS__hdg: %7.2f \n", hdg);
    }

    if(go){
      vel = 175;
    }
    else{
      vel = 0;
    }
    
    ledcWrite(pwmChannel, vel);
    servoDir.write(teta);
    delay(300);
  }

