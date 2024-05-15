#include <QMC5883LCompass.h>
#include <TinyGPS++.h>

QMC5883LCompass compass;
TinyGPSPlus gps;


float Get_heading() {
  int a;
  compass.read();

  a = compass.getAzimuth();

  if( a < 0){
    a += 360;
  }

  return a;
}


// Function to calculate the angle the system faces towards an object
float Aim(double current_lat, double current_lng, double obj_lat, double obj_lng){
  double y = gps.distanceBetween(current_lat, obj_lng, obj_lat, obj_lng);
  double x = gps.distanceBetween(obj_lat, current_lng, obj_lat, obj_lng);
  
  if( current_lat > obj_lat ){
    y *= -1;
  }
  if( current_lng > obj_lng ){
    x *= -1;
  }

  float alpha = atan2(x, y);
  alpha = alpha * 180.0 / PI;
  if (alpha < 0) {
    alpha += 360.0;
  }

  return alpha;
}

bool UltraSonicScan(uint8_t Trigger, uint8_t Echo){
  long t;
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  if(t/59 <= 50){
    return true;
  }             //escalamos el tiempo a una distancia en cm
  else{
    return false;
  }
}