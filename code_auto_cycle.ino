// Example sketch to control a stepper motor with TB6600 stepper motor driver and Arduino without a library: continuous rotation. More info: https://www.makerguides.com */

// Define stepper motor connections:
#define dir_pin 8
#define step_pin 7
#define LS_start 4

const int enPin   = 6;
int pos;

//int direction;    // Variable to determine the sense of the motor
int stepsDelay = 300;
int stepDelayMax = 230;   
int siklus;
int siklusSave;
int hold;
int holdSave;
int first;
int firstSave;
int calibFirst;
int edge;
int edgeSave;
int calibEdge;
int steps;
int lastDistance;
int Distance;
int x;

String input;

boolean stringComplete = true; 
boolean siklusReverse = false;
boolean siklusForward = false;



void setup() {
  Serial.begin(9600);
  
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(enPin,OUTPUT);
  
  pinMode(LS_start, INPUT_PULLUP);
  
  digitalWrite(enPin,LOW);
  digitalWrite(dir_pin, HIGH);

  helpvoid();
  while (!Serial){}
}

void loop() {
  //Serial.println(digitalRead (LS_start)); 
  //Serial.print("||");Serial.println(digitalRead (siklusForward));
  SerialRead(); 
  if (stringComplete){
   if (input=="move\n") {
    Serial.println("move");
   Move();
     }
    if (input=="help\n"){
      helpvoid();
    }
    if (input.charAt(0)=='c')   {
      input.setCharAt(0,' ');
      siklus=input.toInt();
      siklusvoid();
      }
     if (input.charAt(0)=='d') {
          input.setCharAt(0,' ');
          hold=input.toInt();
          holdvoid();
         }
    if (input.charAt(0)=='a') {
          input.setCharAt(0,' ');
          first=input.toInt();
          firstvoid();
     }
     if (input.charAt(0)=='b') {
      input.setCharAt(0,' ');
      edge=input.toInt();
      edgevoid();
     }
    /*if (input !="") {
      Serial.println("wrong command");     
        }*/
  input = ""; 
  stringComplete = false; 
 } 
     
  }


void Move(){
  logicMove();
 if (siklusForward==true && siklusReverse==false){
 for (int x=0; x<siklusSave; x++){
  for ( pos = firstSave; pos<=edgeSave; pos++){
   digitalWrite(dir_pin, HIGH); // move in the LOW direction
   digitalWrite(step_pin, LOW);
   delayMicroseconds(stepsDelay);
   digitalWrite(step_pin, HIGH);
   delayMicroseconds(stepsDelay);
  }
  delay(holdSave);
  Serial.println(x+1);
  for (pos =edgeSave; pos>=firstSave; pos--){
    digitalWrite(dir_pin, LOW); // move in the LOW direction
    digitalWrite(step_pin, LOW);
    delayMicroseconds(stepsDelay);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepsDelay);
  }
  delay(holdSave);
  
 }  
}
  else if(siklusForward==false && siklusReverse==true){
  for (int x=0; x<siklusSave; x++){
  for ( pos = firstSave; pos>=edgeSave; pos--){
   digitalWrite(dir_pin, LOW); // move in the LOW direction
   digitalWrite(step_pin, LOW);
   delayMicroseconds(stepsDelay);
   digitalWrite(step_pin, HIGH);
   delayMicroseconds(stepsDelay);
  }
  delay(holdSave);
  for (pos =edgeSave; pos<=firstSave; pos++){
    digitalWrite(dir_pin, HIGH); // move in the LOW direction
    digitalWrite(step_pin, LOW);
    delayMicroseconds(stepsDelay);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepsDelay);
  }
  delay(holdSave);
  Serial.println(x);
   }  
 }
}
  
void helpvoid(){
  Serial.println("------COMMAND LINE-----------");
  Serial.println("help command show : help                //ex:help");
  Serial.println("point A       : a+value(range 1-100mm)  //ex:a12");
  Serial.println("point B       : b+value(range 1-100mm)  //ex:b12");
  Serial.println("delay holding : d+value(in milisecond)  //ex:d1000");
  Serial.println("cycle         : c+value                 //ex:c3");
  Serial.println("execute/move  : move                    //ex:move");
  Serial.println("presshold 5s reset button to change mode)");
}
void SerialRead(){
  while (Serial.available()>0){
      char in = (char )Serial.read();
      if (in>0) { input+= in;}
      if (in == '\n') { stringComplete = true;}
      //Serial.print(input); Serial.print("---"); 
  }
}

void siklusvoid(){
  if (siklus>0){ siklusSave=siklus;}
 Serial.print ("cycle :"); Serial.println(siklusSave);
}

void holdvoid(){
  if (hold>0){ holdSave=hold;}
  Serial.print ("delay hold(milisecond) :");Serial.println(holdSave);
}

void firstvoid(){
  if (first>0 && first<=100){ calibFirst=first;}
  else if (first>=101){Serial.print("exceeds the max/min, "); }
  firstSave=calibFirst*320;//calib milimeter to steps
  Serial.print ("point A(mm) :");Serial.println(calibFirst);
  //logicDistance();
  repotition();
}

void edgevoid(){
  if (edge>0 && edge<=100){ calibEdge=edge;}
  else if(edge>=101){Serial.print("exceeds the max/min, ");}
  edgeSave=calibEdge*320;//calib milimeter to steps
  Serial.print ("point B(mm) :");Serial.println(calibEdge);
  }


void logicDistance(){//move to firstSave without repotition before
 
  Distance = firstSave;
  if (Distance<lastDistance){
    steps=lastDistance-Distance;
    lastDistance=Distance;
    for (int i = 0; i< steps; i++) {
    digitalWrite(dir_pin, LOW);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepsDelay);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(stepsDelay);
   }
  }
  else if(Distance>lastDistance){
    steps=Distance-lastDistance;
    lastDistance=Distance;
    for (int i = 0; i< steps; i++) {
    digitalWrite(dir_pin, HIGH);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepsDelay);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(stepsDelay);
    }
   } 
  
}
void repotition(){ //move to repotition than to firstSave
   //logic repotition
   Distance = firstSave;
   
   while (digitalRead (LS_start) == 1){
    digitalWrite(dir_pin, LOW);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepDelayMax);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(stepDelayMax);
   }
   digitalWrite(step_pin, HIGH);
   delay (500);
   firstPotition();
}

void firstPotition(){//move to firstSave
   //move to first potition
   for (int i = 0; i< firstSave; i++) {
    digitalWrite(dir_pin, HIGH);
    digitalWrite(step_pin, HIGH);
    delayMicroseconds(stepsDelay);
    digitalWrite(step_pin,LOW);
    delayMicroseconds(stepsDelay);
    }
}

void logicMove(){//defining direction motor when siklus runing 
  if (edgeSave<firstSave){
    siklusForward=false;
    siklusReverse=true;
  }
  else if (edgeSave>firstSave){
    siklusForward=true;
    siklusReverse=false;
  }
}
