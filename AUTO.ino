//Variables Globales//

String inputString = "";//
bool stringComplete = false;
const char separator = ',';
const unsigned int dataLength = 1;
unsigned int data[dataLength];
double R1, R2, R3, R4, R5, R6, R7;
int error;
int error_2;

//PINES DE MOTORES//
const unsigned int enablePin_1 = 10;
const unsigned int enablePin_2 = 11;
const unsigned int in1A = 6;
const unsigned int in2A = 7;
const unsigned int in1B = 8;
const unsigned int in2B = 9;

//Sensores//
const byte trigPin_1 = A0;
const byte echoPin_1 = A1;
const byte trigPin_2 = A2;
const byte echoPin_2 = A3;


double S1 = 0.0;
double S2 = 0.0;

unsigned int sensor_1 = 0;
unsigned int sensor_2 = 0;

unsigned int estado = 0;
unsigned int velocidad = 0;

double D_Near, D_SP, D_Far;
double E_Neg, E_Pos, E_Cero;
double PWM_Inc,PWM_Equi,PWM_Dec;

//FUNCIONES//
void adelante(){
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, LOW);

  analogWrite(enablePin_1,velocidad);
  analogWrite(enablePin_2,60.0);

  sensor_1 = calcula_dist (trigPin_1,echoPin_1,S1);
  sensor_2 = calcula_dist (trigPin_2,echoPin_2,S2);
  error = 15.0 - sensor_1;
  error_2= 15.0 - sensor_2;

  if (sensor_1 <=30){
    Fuzzy_Frontal (sensor_1, error);
    Desfuzzy_Frontal ();
    delay(1000);
  }
  else if(sensor_1 > 30){
    Fuzzy_Derecha (sensor_2, error_2);
    Desfuzzy_Derecha ();

  }
  
}

void Derecha(){
  digitalWrite(in1A, HIGH);
  digitalWrite(in2A, LOW);
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, HIGH);

  analogWrite(enablePin_1,velocidad);
  analogWrite(enablePin_2,velocidad);

  sensor_1 = calcula_dist (trigPin_1,echoPin_1,S1);
  sensor_2 = calcula_dist (trigPin_2,echoPin_2,S2);
}

void Izquierda(){
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, HIGH);
  digitalWrite(in1B, HIGH);
  digitalWrite(in2B, LOW);

  analogWrite(enablePin_1,velocidad);
  analogWrite(enablePin_2,velocidad);

  sensor_1 = calcula_dist (trigPin_1,echoPin_1,S1);
  sensor_2 = calcula_dist (trigPin_2,echoPin_2,S2);
}

void Atras(){
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, HIGH);
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, HIGH);

  sensor_1 = calcula_dist (trigPin_1,echoPin_1,S1);
  sensor_2 = calcula_dist (trigPin_2,echoPin_2,S2);
}  

void Detencion(){
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, LOW);
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, LOW);

  analogWrite(enablePin_1,LOW);
  analogWrite(enablePin_2,LOW);
}

//FUNCIONES DEL SENSOR//
unsigned int calcula_dist(const byte trigPin_X, const byte echoPin_X, double SX){
long duracion = 0;
const double alpha = 0.7;
double distancia = 0;
digitalWrite(trigPin_X,LOW);
delayMicroseconds(2);
digitalWrite(trigPin_X,HIGH);
delayMicroseconds(10);
digitalWrite(trigPin_X,LOW);
duracion = pulseIn(echoPin_X,HIGH);
distancia = (duracion*0.034)/2;

SX = (alpha*distancia)+((1-alpha)*SX);
if(SX >= 60) SX = 60;
else if (SX <= 0) SX = 0;
return SX;
}

//FUNCIONES FUZZY//

double minimo (double X, double Y){//variable que recibe dos datos 
  if (X < Y) return X;//si x es menor que y retorne x

  else return Y;//de lo contrario retorne y
}
double maximo (double X, double Y){
  if (X > Y) return X;//si y es mayor que x retorne x

  else return Y;//de lo contrario retorne y
}

void Fuzzy_Derecha (unsigned int X, int Y)
{
  if (X <= 0){//si x es menor o igual a 0
    D_Near = 1.0;
    D_SP = 0.0;
    D_Far = 0.0;
  }
  if ((X <= 15) && (X > 0))
  {
    D_Near = (15.0 - X)/ (15.0 - 0.0);
    D_SP = (X - 0.0)/(15.0 - 0.0);
    D_Far = 0.0;
  }
  if ((X <= 30) && (X > 15))
  {
    D_Near =  0.0;
    D_SP = (30.0 - X)/(30.0 - 15.0);
    D_Far = (X - 15.0)/(30.0 - 15.0);
  }
   if (X > 30)
   {
    D_Near = 0.0;
    D_SP = 0.0;
    D_Far = 1.0;
  }
  if (Y <= -30)
  {
    E_Neg = 1.0;
    E_Cero = 0.0;
    E_Pos = 0.0;
  }
  if ((Y <= 0) && (Y > -30))
  {
    E_Neg = (0.0 - Y)/(0.0 + 30.0);
    E_Cero = (Y + 30.0)/(0.0 + 30.0);
    E_Pos = 0.0;
  }
  if ((Y <= 30) &&(Y > 0))
  {
    E_Neg = 0.0;
    E_Cero = (30.0 - Y)/(30.0 - 0.0);
    E_Pos = (Y - 0.0)/( 30.0 - 0.0);
  }
  if (Y > 30)
  {
    E_Neg = 0.0;
    E_Cero = 0.0;
    E_Pos = 1.0;
  }
}
  
  void Fuzzy_Frontal (unsigned int X, int Y)
  {
  if (X <= 0){
    D_Near = 1.0;
    D_SP = 0.0;
    D_Far = 0.0;
  }
  if ((X <= 15) && (X > 0)){
    D_Near = (15.0 - X)/ (15.0 - 0.0);
    D_SP = (X - 0.0)/(15.0 - 0.0);
    D_Far = 0.0;
  }
  if ((X <= 30) && (X > 15)){
    D_Near =  0.0;
    D_SP = (30.0 - X)/(30.0 - 15.0);
    D_Far = (X - 15.0)/(30.0 - 15.0);
  }
   if (X > 30){
    D_Near = 0.0;
    D_SP = 0.0;
    D_Far = 1.0;
  }
  if (Y <= -30){
    E_Neg = 1.0;
    E_Cero = 0.0;
    E_Pos = 0.0;
  }
  if ((Y <= 0) && (Y > -30)){
    E_Neg = (0.0 - Y)/(0.0 + 30.0);
    E_Cero = (Y + 30.0)/(0.0 + 30.0);
    E_Pos = 0.0;
  }
  if ((Y <= 30) &&(Y > 0)){
    E_Neg = 0.0;
    E_Cero = (30.0 - Y)/(30.0 - 0.0);
    E_Pos = (Y - 0.0)/( 30.0 - 0.0);
  }
  if (Y > 30){
    E_Neg = 0.0;
    E_Cero = 0.0;
    E_Pos = 1.0;
  }
}


// DESFUZZYFICACIÓN PARED//

void Desfuzzy_Derecha (){
  R1 = minimo(D_Near, E_Cero);
  R2 = minimo(D_Near, E_Pos);
  R3 = minimo(D_SP, E_Neg);
  R4 = minimo(D_SP, E_Cero);
  R5 = minimo(D_SP, E_Pos);
  R6 = minimo(D_Far, E_Neg);
  R7 = minimo(D_Far, E_Cero);

  PWM_Dec = maximo(R3,R6);
  PWM_Equi = maximo(R1,R4);
  PWM_Equi = maximo(PWM_Equi,R7);
  PWM_Inc = maximo(R2,R5);
  velocidad = ((PWM_Dec*0.0)+(PWM_Equi*60.0) + (PWM_Inc*255.0)/(PWM_Dec+PWM_Equi + PWM_Inc));
}
void Desfuzzy_Frontal (){
  R1 = minimo(D_Near, E_Cero);
  R2 = minimo(D_Near, E_Pos);
  R3 = minimo(D_SP, E_Neg);
  R4 = minimo(D_SP, E_Cero);
  R5 = minimo(D_SP, E_Pos);
  R6 = minimo(D_Far, E_Neg);
  R7 = minimo(D_Far, E_Cero);

  PWM_Equi = maximo(R1,R4);
  PWM_Equi = maximo(PWM_Equi,R7);
  PWM_Inc = maximo(R2,R3);
  PWM_Inc = maximo(PWM_Inc,R5);
  PWM_Inc = maximo(PWM_Inc,R6);
  velocidad = ((PWM_Equi*60.0) + (PWM_Inc*255.0)/(PWM_Equi + PWM_Inc));
}

void serialEvent(){
  while(Serial.available()){
    char inChar = Serial.read();
    inputString += inChar;
    if(inChar == '-'){
      stringComplete = true;
    }
    
  }
}

// Configuracion de registros//
void setup(){
  pinMode(enablePin_1, OUTPUT);
  pinMode(enablePin_2, OUTPUT);
  pinMode(in1A,OUTPUT);
  pinMode(in2A,OUTPUT);
  pinMode(in1B,OUTPUT);
  pinMode(in2B,OUTPUT);
  
//confguracion pines sensores//
  pinMode(trigPin_1,OUTPUT);
  pinMode(echoPin_1,INPUT);
  pinMode(trigPin_2,OUTPUT);
  pinMode(echoPin_2,INPUT);
  
  Serial.begin(9600);//indica ak arduino que inicie la comunicacion con el dispositivo a una velocidad de 9600 bits/s
}

//Programa principal
void loop(){
  if(stringComplete){
    for(int i = 0; i < dataLength; i ++){
      int index = inputString.indexOf(separator);
      data[i] = inputString.substring(0,index).toFloat();
      inputString = inputString.substring(index+1);

  }
    estado = data[0];
    stringComplete = false;
    inputString ="";
  }
  if(estado == 0){
    Detencion();
  }
  if(estado == 1){
    adelante();
  }
  if(estado == 2){
    Derecha();
  }
  if(estado == 3){
    Izquierda();
  }
  if(estado == 4){
    Atras();
  }

Serial.println("DISTANCIA FRONTAL: " + String (sensor_1) + "DISTANCIA LATERAL: " + String (sensor_2));   
}
