/*
 * Bennett Schoonerman
 * IGME 470
 * 2/9/2019
 * Project 1 LED
 */
//Globals
int trigPin = 13;
int echoPin = 12;
int startGamePin = 8;
int testPin = 9;
long duration, cm, inches;
//Game vars
int lives = 5;
long targetDistance;
long tollerance = 5;
long redLimit = 20;
bool playingGame = false;


void setup() {
  
  //Distance Sensor
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  //Buttons
  pinMode(startGamePin,INPUT);
  pinMode(testPin,INPUT);
    
  //Game Signals
  pinMode(7, OUTPUT);//Way Lower
  pinMode(6, OUTPUT);//Lower
  pinMode(5, OUTPUT);//Found it
  pinMode(4, OUTPUT);//Higher
  pinMode(3, OUTPUT);//Way Higher

   
  Serial.begin(9600);
  Serial.print("Setup Done");
}

void loop() {
  lightsOff();
  //getDistance();
  if(digitalRead(testPin)==HIGH){
    if(!playingGame){
      //calibrate and set maxDistance and a random target value
      targetDistance = random(10,120);
      Serial.println(targetDistance);
      playingGame = true;
      showLives();
    }
    else{
      Serial.println("Game in progress");
      showLives();
    }
  }
  if(digitalRead(startGamePin)==HIGH){
    if(playingGame){
      long userDistance = getDistance();
      Serial.print("SAW USER AT:");
      Serial.println(userDistance);
      if(userDistance > targetDistance + redLimit){
        digitalWrite(3, HIGH);//Way Higher
        lives--;
      }
      else if(userDistance > targetDistance + tollerance){
        digitalWrite(4, HIGH);//Higher
        lives--;
      }
      else if(userDistance < targetDistance - redLimit){
        digitalWrite(7, HIGH);//Way Lower
        lives--;
      }
      else if(userDistance < targetDistance - tollerance){
        digitalWrite(6, HIGH);//Lower
        lives--;
      }
      else if(userDistance > targetDistance - tollerance && userDistance < targetDistance + tollerance){
        //You Won!
        Serial.println("YOU WIN!");
        for(int i = 0; i < 5; i++){
          digitalWrite(5,HIGH);
          delay(200);
          digitalWrite(5,LOW);
          delay(200);
        }
        //reset game globals
        playingGame = false;
        lives = 5; 
      }
      //Check if they lost!
      if(lives == 0){
        Serial.println("GAME OVER!");
        //Blink RED lights 5 times
        for(int i = 0; i < 5; i++){
          digitalWrite(7,HIGH);
          digitalWrite(3,HIGH);
          delay(200);
          digitalWrite(7,LOW);
          digitalWrite(3,LOW);
          delay(200);
        }
        //reset global game vars
        playingGame = false;
        lives = 5;
      }
      delay(2000);//show the hint light for 2 seconds
      lightsOff();
    }
    else{
        Serial.println("Start a game first");
        return;//if they are not playing the game just return
    }
  }

  //Play waiting blink sequence
  if(!playingGame){
    WaitingMode();
  }

}//end loop
//shows the number of lives left to the user
void showLives(){
  Serial.println(lives);
  Serial.println(" lives left");
  int topPin = 7; 
  for(int i = 0; i < lives; i++){
    digitalWrite(topPin - i,HIGH);
  }
  delay(1000);
  lightsOff();
}

//wait mode blink sequence
void WaitingMode(){
  int waitTime = 500;
  digitalWrite(7, HIGH);
  delay(waitTime);
  digitalWrite(6, HIGH);
  delay(waitTime);
  digitalWrite(5, HIGH);
  delay(waitTime);
  digitalWrite(4,HIGH);
  delay(waitTime);
  digitalWrite(3,HIGH);
  delay(waitTime);
  lightsOff();
  delay(waitTime);
  digitalWrite(5, HIGH);
  delay(waitTime);
  digitalWrite(7, HIGH);
  digitalWrite(3,HIGH);\
  delay(waitTime);
  digitalWrite(6, HIGH);
  digitalWrite(4,HIGH);
  delay(waitTime);
  lightsOff();
  delay(waitTime);
  //blink
  lightsOn();
  delay(waitTime/2);
  lightsOff();
  delay(waitTime/2);
  lightsOn();
  delay(waitTime/2);
  lightsOff();
  delay(waitTime/2);
  lightsOn();
  delay(waitTime/2);
  lightsOff();
  delay(waitTime/2);
}

//helper function to shut off all lights EXCEPT BLUE SENESOR LIGHT
void lightsOff(){
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
}

//  turn all lights on
void lightsOn(){
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
}


long getDistance(){
// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  /*
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  */
  return cm;//using cm 
}
