#include "LedControlMS.h"
#include <MaxMatrix.h>
#define NumeroMatrices 4     // Indicamos cuántas matrices vamos a utilizar, 1
#define DEBUG(a) Serial.println(a);
int i=0;
int fps=100;
 String pause;
  
int data = 12;
int load = 10;
int clock = 11;
int maxInUse = 4; //change this variable to set how many MAX7219's you'll use
MaxMatrix m(data, load, clock, maxInUse);
byte buffer[10];
String dato;
LedControl lc=LedControl(12,11,10, NumeroMatrices);



void setup()
{
         //noInterrupts();
         Serial.begin(9600);
         m.init();
         m.setIntensity(8);
       

      for (i=0; i< NumeroMatrices ; i++)
        {
          lc.shutdown(i,false);    // Despertar a la matriz
          lc.setIntensity(i,8);    // Intensidad a 8
          lc.clearDisplay(i);      // Borrar la matriz
        }
}

void loop(){
  String control;
   //DEBUG("Comando Control");
   if (Serial.available()){
      control = Serial.readStringUntil('\n');
      DEBUG("Comando Control");
   }
   if(control == "control!" || control == "CONTROL!"){     
        Serial.flush();
        String command;
        command = Serial.readStringUntil('\n');
        DEBUG(command);
        DEBUG("Ingresa tu comando");
        DEBUG(command);
        
        while(command == ""){
           command = Serial.readStringUntil('\n');
          //DEBUG(command);
          //DEBUG("Entre al while");
        }

        if(command == "write!"){
          DEBUG("Escribir");
          escribirMensaje();
          }
       else if(command == "blank!"){
          DEBUG("Blanquear");
          limpiarPantalla();
          }
       else if(command == "fps!"){
          DEBUG("Frames");
          fps= frames();
          /*DEBUG("Ahora el fps es del main deeee:");
          DEBUG(fps);*/
       }
       else if(command == "pause!"){
          DEBUG("Pausar");
          pausarPantalla();
       }
       else if(command == "logo!"){
          DEBUG("Mostrar logo");
          logo();
       }
       else if(command == "show!"){
          DEBUG("Show");
          escribirMensaje();
          logo();
        }
        else if(command == "data!"){
          DEBUG("Data");
          datasDatos();
        }
   }
}

void escribirMensaje(){

  while(dato==""){
    DEBUG("Debes poner los datos con el comando data");
    datasDatos();
  }
  int stringLength;
  char ch0, ch1, ch2, ch3;
  int nextCharIndex=0;
  delay(500);
  /*DEBUG("Introduce the message");
  //String dato;
  dato = Serial.readStringUntil('\n');
  //while(dato.length()<1){
  while(dato==""){
         dato = Serial.readStringUntil('\n');
  }*/
      
  stringLength=dato.length();
  //Serial.println(dato);
  //Serial.println(stringLength);
  // help=dato.length();
     
 

  for(int i=0;i<stringLength;i++){
    
    pause = Serial.readStringUntil('\n');
    if(pause=="pause!"){
      pausarPantalla();
    }
    ch0= dato[i];
    ch1= dato[i+1];
    ch2= dato[i+2];
    ch3= dato[i+3];
    lc.displayChar(0, lc.getCharArrayPosition(ch0));
    lc.displayChar(1, lc.getCharArrayPosition(ch1));
    lc.displayChar(2, lc.getCharArrayPosition(ch2));
    lc.displayChar(3, lc.getCharArrayPosition(ch3));
  
    delay(fps);
    pause = Serial.readStringUntil('\n');
    if(pause=="pause!"){
      pausarPantalla();
    }
    
  }
 /* DEBUG("Mi fram FINAL:");
  DEBUG(fps);*/
}



void datasDatos(){

  DEBUG("Introduce el  mensanje");
  //String pause;
  dato = Serial.readStringUntil('\n');
  //while(dato.length()<1){
  while(dato==""){
     dato = Serial.readStringUntil('\n');
  }

  DEBUG("El mensaje a escribir es:");
  DEBUG(dato);
}


void limpiarPantalla(){
  //Serial.println("LIMPIAR PANTALLA");
  for(i=0;i<NumeroMatrices;i++)
  lc.clearDisplay(i);
  
  for(i=0;i<NumeroMatrices;i++){
      for(int j=0;j<8;j++){
        lc.setColumn(i,j,0xFF);
       
      }
   }
  
  for(i=0;i<NumeroMatrices;i++)
  lc.clearDisplay(i);
  delay(fps);

  //Serial.println( "ok");
    
}

void pausarPantalla(){
  do{
      pause = Serial.readStringUntil('\n');
      DEBUG("Quita la pausa");
   } 
  while(pause!="pause!");
}


int frames(){
  String stringFPS;
  int fpslocal=0;
  DEBUG("Ingresa un fps nuevo:");
  stringFPS = Serial.readStringUntil('*');
  while(stringFPS==""){
         stringFPS = Serial.readStringUntil('*');
  }
 /* DEBUG("Mi frames es de:");
  DEBUG(stringFPS);
  
  fpslocal=stringFPS.toInt();
  DEBUG("El int es dede:");
  DEBUG(fpslocal);*/

  while(fpslocal<10 || fpslocal >25)
  {
    DEBUG("Ingresa un fps nuevo:");
    stringFPS = Serial.readStringUntil('*');
    while(stringFPS==""){
         stringFPS = Serial.readStringUntil('*');
    }
    fpslocal=stringFPS.toInt();
  }
  
  fpslocal=fpslocal*10;
  return fpslocal;

}

void logo(){
   //Serial.println("LOGO");
   for(int i=0;i<4;i++){
      lc.setColumn(i,3,0xFF);
      lc.setColumn(i,4,0xFF);
      lc.setRow(i,3,0xFF);
      lc.setLed(i,1,2,true);
      lc.setLed(i,2,1,true);
      lc.setLed(i,2,2,true);
      lc.setLed(i,5,2,true);
      lc.setLed(i,5,1,true);
      lc.setLed(i,6,2,true);
      lc.setRow(i,4,0xFF);
      delay(fps);
      lc.setColumn(i,3,0x00);
      lc.setColumn(i,4,0x00);
      lc.setRow(i,3,0x00);
      lc.setLed(i,1,2,false);
      lc.setLed(i,2,1,false);
      lc.setLed(i,2,2,false);
      lc.setLed(i,5,2,false);
      lc.setLed(i,5,1,false);
      lc.setLed(i,6,2,false);
      lc.setRow(i,4,0x00);
      delay(fps);
    }
    //Serial.println( "ok");
}
