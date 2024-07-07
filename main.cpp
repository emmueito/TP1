#include "mbed.h"

// main() runs in its own thread in the OS
int main()
{

   #include <PID.h>

const int sensorPin = A0; // LM35 sensor pin
const int heaterPin = PB9; // Heating element pin (PWM)
const int potPin = A1;    // Potentiometer pin

float setPoint = 50;    // Desired temperature (initial 50°C)
float Kp = 1.0;         // Proportional constant of PID
float Ki = 0.1;         // Integral constant of PID
float Kd = 0.0;        // Derivative constant of PID

PID<float> myPID(&setPoint, 0, 0, Kp, Ki, Kd); // PID object

void setup() {
  Serial.begin(9600);   // Initialize serial communication

  // Configure heating element pin as PWM output
  pinMode(heaterPin, OUTPUT);
  tim2_pwm_config(heaterPin);

  // Configure potentiometer pin as analog input
  pinMode(potPin, INPUT);

  myPID.setSampleTime(1000); // Set PID sample time to 1 second

  myPID.autoTune(); // Perform PID auto-tuning
}
}

void loop() {
  // Read LM35 sensor
  int sensorValue = analogRead(sensorPin);
  float celsius = sensorValue * 5.0 / 1024.0; // Convert analog value to temperature in °C

  // Read potentiometer
  int potValue = analogRead(potPin);
  setPoint = map(potValue, 0, 1023, 30, 70); // Update desired temperature based on potentiometer

  // Calculate temperature error
  float error = setPoint - celsius;

  // Update PID controller
  myPID.update(error);

  // Get heater output power (between 0 and 1)
  float heaterOutput = myPID.getOutput() / 100.0;

  // Control heating element (PWM)
  tim2_pwm_set_duty(heaterPin, heaterOutput * 255.0);

  // Print information to serial monitor
  Serial.print("Current temperature: ");
  Serial.print(celsius);
  Serial.print("°C\tDesired temperature: ");
  Serial.print(setPoint);
  Serial.print("°C\tError: ");
  Serial.print(error);
  Serial.print("°C\tHeater output: ");
  Serial.println(heaterOutput * 100.0);

  delay(1000); // 1-second delay
}




//Codigo que genere con IA. estoy tratando de entender este y entender lo del cuso. al no tener la placa no puedo jugar.Pero lo estoy intetando hacer con arduino

