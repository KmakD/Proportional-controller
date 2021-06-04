#include <TimerOne.h>

//piny odpowiadajace ze kierunek obrotow oraz obrot silnika
const int dirPin = 2;
const int stepPin = 9;

int settime = 100; //co ile wykonywane sa obliczenia regulatora - 0,1s
float zadana = 100; //zadana wartosc w mm
float odczytana = 0; //odczytana wartosc w mm
float uchyb = 0;
float K = 0.00001;  //wzmocnienie regulatora
float y = 0;        //obliczony sygnal wyjsciowy
float t = 0; //czas podoawany w celu sterowania predkoscia silnika

//obsluga dalmierza
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.print("Zadana wartosc: ");
  Serial.print(zadana);
  Serial.println(" mm");

  Timer1.initialize(500000); //wlaczenie obslugi timera
}

void loop() {
 
    odczytana = 0.1723 * readUltrasonicDistance(7, 7); //odczytana wartosc w mm

    uchyb = zadana - odczytana; //obliczenie uchybu
    Serial.println(uchyb);      //wyswietlenie biezacego uchybu
    
    y = K*uchyb;   //sygnal sterujacy   
  
    t = abs(1/y);  //czas regulujacy predkosc silnika
  
    if(t <= 1000){  //maksymalna predkosc silnika
      t = 1000;
    }
    if(t >= 50000){ //minimalna predkosc silnika
      t = 50000;
    }
  
    //Wyznaczenie kierunku obrotu silnika  
    if(uchyb < 0){
        digitalWrite(dirPin, HIGH);
    }
    if(uchyb > 0){
        digitalWrite(dirPin, LOW);
    }
  
  //Obrót silnika
  if(uchyb != 0){
    Timer1.pwm(stepPin,512,t);
  }else{
    Timer1.pwm(stepPin,0,t);
  }

  delay(settime);
}
