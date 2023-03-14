// Clasificador de flores iris mediante Perceptron Multicapa MLP
#include <ArduinoBLE.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <math.h>
#include <Arduino_LSM9DS1.h>

#define OLED_RESET  -1 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Variables Globales
float aX, aY, aZ;
float x1,x2,x1F,x2F;
float X1a1,X2a1,X1a2,X2a2,X1a3,X2a3=0;
float X1,X2;

//Medias y Varianzas
float x_y[10][2]={{0.7154910714285714 , 0.2718917850495367 },
                  {0.1000357142857143 , 0.15414133869899524 },
                  {-0.6939017857142856 , 0.27292512578090994 },
                  {0.188375 , 0.26424285789104796 } ,
                  {0.07196428571428572 , 0.12621798947593382 } ,
                  {0.7499642857142857 , 0.24574532870765348 } ,
                  {0.02628571428571429 , 0.22388917972050004 } ,
                  {-0.8044017857142859 , 0.1902860375020716 } ,
                  {-0.08961607142857143 , 0.1723669386147572 } ,
                  {-0.06876785714285713 , 0.25454472600756295 }};
                  
//Probabilidades de las caracteristicas dadas las clases
float Px1_y1,Px2_y1,Px1_y2,Px2_y2,Px1_y3,Px2_y3,Px1_y4,Px2_y4,Px1_y5,Px2_y5;
float Py1,Py2,Py3,Py4,Py5;

void setup() {
  // Configuración del Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  display.setRotation(0);
  display.invertDisplay(false);
  display.setTextSize(1);
  display.setTextColor(WHITE,BLACK);
  display.display();
  delay(10);
  //Configuración del puerto
  Serial.begin(115200);
  //while (!Serial);
  delay(200);
  //Serial.println("Started");
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
    }
    
}
void loop() {
  if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(aX, aY, aZ);
      Serial.print(aX, 3);Serial.print(',');
      Serial.print(aY, 3);Serial.print(',');
      Serial.print(aZ, 3);Serial.print(',');
      Serial.println();
      delay(20);
    }

  digitalWrite(LEDR,HIGH);
  digitalWrite(LEDG,HIGH);
  digitalWrite(LEDB,HIGH);
  delay(100);
  
  display.setCursor(1,1);
  display.print("Accel. en X = ");
  display.println(aX);
  display.setCursor(1,11);
  display.print("Accel. en Y = ");
  display.println(aY);
  display.display();
  X1=aX;
  X2=aY;

  //Clasificador C++, Naives-Gaussiano
  
  Px1_y1 = ((exp(-((pow((X1-x_y[0][0]),2))/(2*pow(x_y[0][1],2)))))/((x_y[0][1])*sqrt(2*PI)));
  Px2_y1 = ((exp(-((pow((X2-x_y[1][0]),2))/(2*pow(x_y[1][1],2)))))/((x_y[1][1])*sqrt(2*PI)));
  Px1_y2 = ((exp(-((pow((X1-x_y[2][0]),2))/(2*pow(x_y[2][1],2)))))/((x_y[2][1])*sqrt(2*PI)));
  Px2_y2 = ((exp(-((pow((X2-x_y[3][0]),2))/(2*pow(x_y[3][1],2)))))/((x_y[3][1])*sqrt(2*PI)));
  Px1_y3 = ((exp(-((pow((X1-x_y[4][0]),2))/(2*pow(x_y[4][1],2)))))/((x_y[4][1])*sqrt(2*PI)));
  Px2_y3 = ((exp(-((pow((X2-x_y[5][0]),2))/(2*pow(x_y[5][1],2)))))/((x_y[5][1])*sqrt(2*PI)));
  Px1_y4 = ((exp(-((pow((X1-x_y[6][0]),2))/(2*pow(x_y[6][1],2)))))/((x_y[6][1])*sqrt(2*PI)));
  Px2_y4 = ((exp(-((pow((X2-x_y[7][0]),2))/(2*pow(x_y[7][1],2)))))/((x_y[7][1])*sqrt(2*PI)));
  Px1_y5 = ((exp(-((pow((X1-x_y[8][0]),2))/(2*pow(x_y[8][1],2)))))/((x_y[8][1])*sqrt(2*PI)));
  Px2_y5 = ((exp(-((pow((X2-x_y[9][0]),2))/(2*pow(x_y[9][1],2)))))/((x_y[9][1])*sqrt(2*PI)));

  Py1 = ((Px1_y1)*(Px2_y1))/5;
  Py2 = ((Px1_y2)*(Px2_y2))/5;
  Py3 = ((Px1_y3)*(Px2_y3))/5;
  Py4 = ((Px1_y4)*(Px2_y4))/5;
  Py5 = ((Px1_y4)*(Px2_y5))/5;

  if(Py1>Py2 && Py1>Py3 && Py1>Py4 && Py1>Py5)
    {
      digitalWrite(LEDB,LOW);delay(20);
      display.setCursor(10,24);display.println("Clase: Arriba    ");display.display();
    }
    if(Py2>Py1 && Py2>Py3 && Py2>Py4 && Py2>Py5)
    {
      digitalWrite(LEDB,LOW);delay(20);
      display.setCursor(10,24);display.println("Clase: Abajo    ");display.display();
    }
    if(Py3>Py2 && Py3>Py1 && Py3>Py4 && Py3>Py5)
    {
      digitalWrite(LEDG,LOW);delay(20);
      display.setCursor(10,24);display.println("Clase: Izquierda    ");display.display();
    }
    if(Py4>Py2 && Py4>Py3 && Py4>Py1 && Py4>Py5)
    {
      digitalWrite(LEDG,LOW);delay(20);
      display.setCursor(10,24);display.println("Clase: Derecha    ");display.display();
    }
    if(Py5>Py2 && Py5>Py3 && Py5>Py4 && Py5>Py1)
    {
      digitalWrite(LEDR,LOW);delay(20);
      display.setCursor(10,24);display.println("Clase: Estable    ");display.display();
    }
  
  if(!IMU.accelerationAvailable()){
      digitalWrite(LEDG,LOW);
      display.setCursor(10,24);display.println("IMU Desconectada");display.display();
      delay(15);
    }
  
}
//Funciones
float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
