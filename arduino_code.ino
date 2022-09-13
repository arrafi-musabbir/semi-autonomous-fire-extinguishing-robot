#include <Servo.h>
#include <NewPing.h> 
Servo myservo1;  //for pump
Servo myservo2; //for ultrasonic

boolean fire = false;
boolean goesForward = false;
int distance = 100;
int pos = 0; 
char val;
char con = 'M';
int i=0;

/*-------defining Inputs------*/
#define maximum_distance 200
#define Forward_S 2 //forward sensor
#define Forward_LS 4 //forward sensor
#define Forward_RS 5 //forward sensor

/*-------defining Outputs------*/
#define trig A3
#define echo A4
#define ENA 5
#define LM1 6       // left motor                                                                             
#define LM2 7       // left motor
#define RM1 8       // right motor
#define RM2 9       // right motor
#define ENB 4                    
#define pump 12

NewPing sonar(trig, echo, maximum_distance); //sensor function
void setup() {
    Serial.begin(9600);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(Forward_S, INPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(LM1, OUTPUT);
    pinMode(LM2, OUTPUT);
    pinMode(RM1, OUTPUT);
    pinMode(RM2, OUTPUT);
    pinMode(pump, OUTPUT);
    digitalWrite(pump, LOW);
    
    myservo1.attach(13);
    myservo1.write(90); 
    myservo2.attach(3);
    myservo2.write(115);
    delay(2000); 
    
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
    distance = readPing();
    delay(100);
  
}

// the loop function runs over and over again forever
void loop() {
while (Serial.available()>0){
  val = Serial.read();
  Serial.println(val);
//  i+=1;
//  Serial.println(i);
  }
  if( val == 'M'){
    con = 'M' ;
    val = 'F';
  }
  else if( val == 'I'){
    con = 'I' ;
    val = 'K';
  }
  else if( val == 'W') // Forward
    { 
      analogWrite(ENA, 100);
      analogWrite(ENB, 100);
      digitalWrite(LM1, HIGH);
      digitalWrite(LM2, LOW);
      digitalWrite(RM1, HIGH);
      digitalWrite(RM2, LOW);  
    }
  else if(val == 'S') // Backward
    {
      analogWrite(ENA, 100);
      analogWrite(ENB, 100);
      digitalWrite(LM1, LOW);
      digitalWrite(LM2, HIGH);
      digitalWrite(RM1, LOW);
      digitalWrite(RM2, HIGH); 
    }
  
  else if(val == 'A') //Left
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    }
    
  else if(val == 'D') //Right
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW); 
    }
    
  else if(val == 'F') //Stop
    {
    digitalWrite(pump, LOW);
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW); 
    }
  else if(val == 'E') //Forward Right
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
    }
  else if(val == 'X') //Backward Right
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, LOW);
    }
   else if(val == 'Q') //Forward Left
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);     
    digitalWrite(RM2, LOW);
    }
  else if(val == 'Z') //Backward Left
    {
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(LM1, LOW);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, LOW);
    digitalWrite(RM2, HIGH); 
    }
  else if(val == 'P') //PUMP
    {
    digitalWrite(pump, HIGH);
    delay(2000);
    digitalWrite(pump, LOW);
    }
   else if (con == 'I'){
        digitalWrite(pump, LOW);
        analogWrite(ENA, 100);
        analogWrite(ENB, 100);
        digitalWrite(LM1, HIGH);
        digitalWrite(RM1, HIGH);
        digitalWrite(LM2, LOW);
        digitalWrite(RM2, LOW); 
      
        myservo1.write(90); //Sweep_Servo();    
        int distanceRight = 0;
        int distanceLeft = 0;
        delay(50);
        if (digitalRead(Forward_S) ==1){ //If Fire not detected all sensors are zero
      
          if (distance <= 20){
      
            moveStop();
            delay(300);
            moveBackward();
            delay(100);
            moveStop();
            delay(300);
            distanceRight = lookRight();
            delay(300);
            distanceLeft = lookLeft();
            delay(300);
      
              if (distance >= distanceLeft){
                turnRight();
                }
      
              else{
                turnLeft();
                }
            }
      
          else{
            moveForward(); 
            }
      
          distance = readPing();
          }
      
        else if (digitalRead(Forward_S) ==0){ //If Fire is straight ahead
      
          digitalWrite(LM1, LOW);
          digitalWrite(LM2, LOW);
          digitalWrite(RM1, LOW);
          digitalWrite(RM2, LOW);
          put_off_fire();
          }

          
          
          }
         
}

void put_off_fire(){

  delay(500);
  digitalWrite(LM1, LOW);
  digitalWrite(RM1, LOW);
  digitalWrite(LM2, LOW);
  digitalWrite(RM2, LOW); 
  digitalWrite(pump, HIGH); 
  delay(500);
  
  for (pos = 50; pos <= 130; pos += 1){ 
    myservo1.write(pos); 
    delay(50);  
    }

  for (pos = 130; pos >= 50; pos -= 1){ 
    myservo1.write(pos); 
    delay(50);
    }
  
  myservo1.write(90);
  fire=false;
  digitalWrite(pump, LOW); 
  }
  
int lookRight(){  
  myservo2.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo2.write(115);
  return distance;
  }

int lookLeft(){
  myservo2.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  myservo2.write(115);
  return distance;
  delay(100);
  }

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
    }
  return cm;
}

void moveStop(){
  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(RM1, LOW);
  digitalWrite(LM1, LOW);
  digitalWrite(RM2, LOW);
  digitalWrite(LM2, LOW);
  }

void moveForward(){

  if(!goesForward){

    goesForward=true;
    analogWrite(ENA, 100);    
    analogWrite(ENB, 100);
    digitalWrite(LM1, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM2, LOW); 
    }
  }

void moveBackward(){

  goesForward=false;
  analogWrite(ENA, 100);  
  analogWrite(ENB, 100);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM2, HIGH);
  digitalWrite(LM1, LOW);
  digitalWrite(RM1, LOW);
  }

void turnRight(){

  analogWrite(ENA, 100);
  analogWrite(ENB, 100);
  digitalWrite(LM1, HIGH);
  digitalWrite(RM2, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM1, LOW);
  delay(1500);
  analogWrite(ENA, 100);    
  analogWrite(ENB, 100);
  digitalWrite(LM1, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM2, LOW);
  }

void turnLeft(){

  analogWrite(ENA, 100);  
  analogWrite(ENB, 100);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(LM1, LOW);
  digitalWrite(RM2, LOW);
  delay(1500);
  analogWrite(ENA, 100);    
  analogWrite(ENB, 100);
  digitalWrite(LM1, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(LM2, LOW);
  digitalWrite(RM2, LOW);
  }
