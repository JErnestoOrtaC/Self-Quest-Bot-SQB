//GPIOS 
const uint8_t gpsRx = 16;
const uint8_t gpsTx = 17;
const uint8_t rfRx = 2;
const uint8_t rfTx = 4;
const uint8_t pwmPin = 23;
const uint8_t servoPin = 5;
const uint8_t echoPin= 19;
const uint8_t triggerPin= 18;
const uint8_t goPin = 15;

//PMW CHANNEL FOR ledc lib
const uint8_t pwmChannel = 2;

const double homelat = 22.2590318;
const double homelng = -97.8630664;

int pathIndx = 0;
const double pathLat[2] = {22.3248046, 22.3248059};
const double pathLng[2] = {-97.8794966, -97.8798349};