//Se incluye las librerías necesarias.

#include "LedControlMS.h"
#include <MaxMatrix.h>
// Indicamos cuántas matrices vamos a utilizar
#define NumeroMatrices 4 
//Función para imprimir
#define DEBUG(a) Serial.println(a);

//Variables necesarias para el funcionamiento del código
int i=0;
int fps=100;
String pause;  
int data = 12;
int load = 10;
int clock = 11;
String dato;
MaxMatrix m(data, load, clock, NumeroMatrices);
LedControl lc=LedControl(12,11,10, NumeroMatrices);



void setup()
{
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

/* Es el menú de que hace que reciba los comandos por el serial, cada comando va pidiendo la función necesaria
*/
void loop(){
  String control;
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
        }

        if(command == "write!"|| command == "WRITE!"){
          DEBUG("Escribir");
          escribirMensaje();
          }
       else if(command == "blank!"|| command == "BLANK!"){
          DEBUG("Blanquear");
          limpiarPantalla();
          }
       else if(command == "fps!"|| command == "FPS!"){
          DEBUG("Frames");
          fps= frames();
       }
       else if(command == "pause!"|| command == "PAUSE!"){
          DEBUG("Pausar");
          pausarPantalla();
       }
       else if(command == "logo!"|| command == "LOGO!"){
          DEBUG("Mostrar logo");
          logo();
       }
       else if(command == "show!"|| command == "SHOW!"){
          DEBUG("Show");
          escribirMensaje();
          logo();
        }
        else if(command == "data!"|| command == "DATA!"){
          DEBUG("Data");
          limpiarPantalla();
          datasDatos();
        }
   }
}


/*Va escribir el mensaje previamente guardado en las matrices, las letras
 * se van mostrando con cierta velocidad puesta por los fps; en caso
 * de que no se haya guardado ninguna palabra, se manda a llamar la función 
 * de dataDatos que es donde se guardan los datos y muestra ahora sí el mensaje.
*/
void escribirMensaje(){

  while(dato==""){
    DEBUG("Debes poner los datos, mandamos a llamar el comando data");
    datasDatos();
  }
  int stringLength;
  char ch0, ch1, ch2, ch3;
  int nextCharIndex=0;
  delay(500);
      
  stringLength=dato.length();
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
}

/*Pide los datos a ingresa para que se vean en las matrices de led
*/
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



/*Apaga todos los led de los 4 módulos y 
 * los vuelve a prender para apagarlos nuevamente
*/
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
    
}


/*Va matando tiempo hasta que el usuario meta el comando pausa! y regresará a donde estaba
*/
void pausarPantalla(){
  do{
      pause = Serial.readStringUntil('\n');
      DEBUG("Quita la pausa");
   } 
  while(pause!="pause!");
}




/*Va checando la velocidad en la que se mueve los datos en la matriz de led
Se pide un número de 10 al 25 con una terminación de *, en caso de que no
se cumpla dicha tarea, te sigue pidiendo el número,dicho número va a 
modificar la velocidad con la que pasen los datos en la matrices, para que
la velocidad no fuera muy chica, se multiplica por 10, para tener un rango de
100 a 250 milisegundos.
*/
int frames(){
  String stringFPS;
  int fpslocal=0;
  DEBUG("Ingresa un fps nuevo:");
  stringFPS = Serial.readStringUntil('*');
  while(stringFPS==""){
         stringFPS = Serial.readStringUntil('*');
  }

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


//Imprime el logo, y checa si hay un comando pause! en el serial
void logo(){
   for(int i=0;i<4;i++){
     pause = Serial.readStringUntil('\n');
      if(pause=="pause!"){
       pausarPantalla();
      }
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
      pause = Serial.readStringUntil('\n');
      if(pause=="pause!"){
       pausarPantalla();
      }
    }  
}
