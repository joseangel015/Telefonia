
#include <SoftwareSerial.h>
//Librerías para el manejo de interrupciones
#include <avr/io.h>
#include <avr/interrupt.h>

//Pines para mostrar las señales generadas (Señales de entrada)
int senialesE[6] = {13,12,11,10,9,8};
//Contadores para la generación de señales
int contadoresE[6] = {0,0,0,0,0,0}; 

//Pines de entrada para el CONTROL a la ENTRADA
int controlEntrada[3] = {7,6,5};
//Banderas para el CONTROL a la ENTRADA
int controlEntradaB[2][3]; //Fila 1 - Selector de OR,  Fila 2 - Selector de AND 
bool etapa1 = true;

//Pines para la ETAPA INTERMEDIA
int salidasIntermedias[5] = {22,23,24,25,26}; 
int salidasIntermediasB[5] = {0,0,0,0,0};

//Pines de entrada para el CONTROL a la SALIDA
int controlSalida[3] = {32,33,34}; //Fila 1 - Selector de Etapa Intermedia - Fila 2 - Control de Salida
int controlSalidaB[2][3]; 
bool etapa2 = true;

//Pines para la Salida 
int salidas[5] = {27,28,29,30,31};
int salidasB[5] = {0,0,0,0,0};

//Botón de confirmación
int btnConfirmacion = 35;

//Botón de reconfiguración
int btnReconfiguracion = 36;
bool reconfigurar = false;


void setup(){
  Serial.begin(9600);

  pinMode(btnConfirmacion,INPUT);
  pinMode(btnReconfiguracion,INPUT);
  
  pinMode(senialesE[0], OUTPUT);
  pinMode(senialesE[1], OUTPUT);
  pinMode(senialesE[2], OUTPUT);
  pinMode(senialesE[3], OUTPUT);
  pinMode(senialesE[4], OUTPUT);
  pinMode(senialesE[5], OUTPUT);

  pinMode(controlEntrada[0], INPUT);
  pinMode(controlEntrada[1], INPUT);
  pinMode(controlEntrada[2], INPUT);

  pinMode(salidasIntermedias[0], OUTPUT);
  pinMode(salidasIntermedias[1], OUTPUT);
  pinMode(salidasIntermedias[2], OUTPUT);
  pinMode(salidasIntermedias[3], OUTPUT);
  pinMode(salidasIntermedias[4], OUTPUT);

  pinMode(salidas[0], OUTPUT);
  pinMode(salidas[1], OUTPUT);
  pinMode(salidas[2], OUTPUT);
  pinMode(salidas[3], OUTPUT);
  pinMode(salidas[4], OUTPUT);

  pinMode(controlSalida[0], INPUT);
  pinMode(controlSalida[1], INPUT);
  pinMode(controlSalida[2], INPUT);


//Configuración del Timer 1
  cli(); //Deshabilita interrupciones globales
  TCCR1A=0; //Controlador Timer/contador 1 Registro A
  TCCR1B=0; //Controlador Timer/contador 1 Registro B
  OCR1A=2603; //Valor de CTC. 15624 para 1s, 2603 para 1/6s 
  TCCR1B |= (1<<WGM12); //Habilita comparaciones con CTC
  TCCR1B |= (1<<CS10); //CS10 = 1, CS11 = 0, CS12 = 1 -> Preescaler = 1024 
  TCCR1B |= (1<<CS12);
  TIMSK1=(1<<OCIE1A); //Habilita comparaciones con CTC
  sei(); //Habilita interrupciones globales
}

void loop(){
  
  //-------------------------------------ENTRADAS------------------------------------
  while(etapa1 == true){
    
      //Leyendo banderas para seleccionar la compuerta OR
      Serial.println("Controles de Compuertas ORs");
      while(digitalRead(btnConfirmacion)!=1){
        if(digitalRead(btnReconfiguracion) == 1){
           reconfigurar = true;
           Serial.println("Modo de reconfiguración activado");
           Serial.println(reconfigurar);
           delay(500);
          }
        controlEntradaB[0][0] = digitalRead(controlEntrada[0]); 
        controlEntradaB[0][1] = digitalRead(controlEntrada[1]);
        controlEntradaB[0][2] = digitalRead(controlEntrada[2]); 
        }
       Serial.println("controlEntradaB[0][0]:");
       Serial.println(controlEntradaB[0][0]);
       Serial.println("controlEntradaB[0][1]:");
       Serial.println(controlEntradaB[0][1]);
       Serial.println("controlEntradaB[0][2]:");
       Serial.println(controlEntradaB[0][2]);
       delay(1000);
    
    
       //Leyendo banderas para seleccionar la compuerta AND
       Serial.println("Controles de ANDs");
       while(digitalRead(btnConfirmacion)!=1){
        controlEntradaB[1][0] = digitalRead(controlEntrada[0]); 
        controlEntradaB[1][1] = digitalRead(controlEntrada[1]);
        controlEntradaB[1][2] = digitalRead(controlEntrada[2]);
        }
       Serial.println("controlEntradaB[1][0]:");
       Serial.println(controlEntradaB[1][0]);
       Serial.println("controlEntradaB[1][1]:");
       Serial.println(controlEntradaB[1][1]);
       Serial.println("controlEntradaB[1][2]:");
       Serial.println(controlEntradaB[1][2]);
        delay(1000);
        
        
        //ETAPA 1 - CONTROL A LA ENTRADA
        //ETAPA 1.1 - SELECCIÓN DE LA COMPUERTA OR (SALIDA A ETAPA INTERMEDIA)
      
        if(controlEntradaB[0][0] == 0 && controlEntradaB[0][1] == 0 && controlEntradaB[0][2] == 1){ // 001 - E1 - salidasIntermediasB[0]      
    
          //ETAPA 1.2 - SELECCION DE LA COMPUERTA AND (SEÑAL SELECCIONADA)
    
            if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //001 - s1
              salidasIntermediasB[0] = 1;
              Serial.println("E1-S1");
            }else if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //011 - s2
              salidasIntermediasB[0] = 2;
              Serial.println("E1-S2");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 0){ //100 - s3
              salidasIntermediasB[0] = 3;
              Serial.println("E1-S3");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //101 - s4
              salidasIntermediasB[0] = 4;
              Serial.println("E1-S4");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 0){ //110 - s5
              salidasIntermediasB[0] = 5;
              Serial.println("E1-S5");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //111 - s6
              salidasIntermediasB[0] = 6;
              Serial.println("E1-S6");
            }else{
              salidasIntermediasB[0] = 0;
            }
        }else if(controlEntradaB[0][0] == 0 && controlEntradaB[0][1] == 1 && controlEntradaB[0][2] == 1){ // 011 - E2 - salidasIntermediasB[1]      
    
          //ETAPA 1.2 - SELECCION DE LA COMPUERTA AND (SEÑAL SELECCIONADA)
    
            if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //001 - s1
              salidasIntermediasB[1] = 1;
              Serial.println("E2-S1");
            }else if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //011 - s2
              salidasIntermediasB[1] = 2;
              Serial.println("E2-S2");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 0){ //100 - s3
              salidasIntermediasB[1] = 3;
              Serial.println("E2-S3");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //101 - s4
              salidasIntermediasB[1] = 4;
              Serial.println("E2-S4");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 0){ //110 - s5
              salidasIntermediasB[1] = 5;
              Serial.println("E2-S5");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //111 - s6
              salidasIntermediasB[1] = 6;
              Serial.println("E2-S6");
            }else{
              salidasIntermediasB[1] = 0;
            } 
        }else if(controlEntradaB[0][0] == 1 && controlEntradaB[0][1] == 0 && controlEntradaB[0][2] == 0){ // 100 - E3 - salidasIntermediasB[2]      
    
            //ETAPA 1.2 - SELECCION DE LA COMPUERTA AND (SEÑAL SELECCIONADA)
    
            if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //001 - s1
              salidasIntermediasB[2] = 1;
              Serial.println("E3-S1");
            }else if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //011 - s2
              salidasIntermediasB[2] = 2;
              Serial.println("E3-S2");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 0){ //100 - s3
              salidasIntermediasB[2] = 3;
              Serial.println("E3-S3");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //101 - s4
              salidasIntermediasB[2] = 4;
              Serial.println("E3-S4");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 0){ //110 - s5
              salidasIntermediasB[2] = 5;
              Serial.println("E3-S5");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //111 - s6
              salidasIntermediasB[2] = 6;
              Serial.println("E3-S6");
            }else{
              salidasIntermediasB[2] = 0;
            } 
        }else if(controlEntradaB[0][0] == 1 && controlEntradaB[0][1] == 0 && controlEntradaB[0][2] == 1){ // 101 - E4 - salidasIntermediasB[3]      
    
            //ETAPA 1.2 - SELECCION DE LA COMPUERTA AND (SEÑAL SELECCIONADA)
    
            if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //001 - s1
              salidasIntermediasB[3] = 1;
              Serial.println("E4-S1");
            }else if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //011 - s2
              salidasIntermediasB[3] = 2;
              Serial.println("E4-S2");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 0){ //100 - s3
              salidasIntermediasB[3] = 3;
              Serial.println("E4-S3");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //101 - s4
              salidasIntermediasB[3] = 4;
              Serial.println("E4-S4");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 0){ //110 - s5
              salidasIntermediasB[3] = 5;
              Serial.println("E4-S5");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //111 - s6
              salidasIntermediasB[3] = 6;
              Serial.println("E4-S6");
            }else{
              salidasIntermediasB[3] = 0;
            } 
        }else if(controlEntradaB[0][0] == 1 && controlEntradaB[0][1] == 1 && controlEntradaB[0][2] == 0){ // 110 - E5 - salidasIntermediasB[4]      
    
          //ETAPA 1.2 - SELECCION DE LA COMPUERTA AND (SEÑAL SELECCIONADA)
    
            if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //001 - s1
              salidasIntermediasB[4] = 1;
              Serial.println("E5-S1");
            }else if(controlEntradaB[1][0] == 0 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //011 - s2
              salidasIntermediasB[4] = 2;
              Serial.println("E5-S2");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 0){ //100 - s3
              salidasIntermediasB[4] = 3;
              Serial.println("E5-S3");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 0 && controlEntradaB[1][2] == 1){ //101 - s4
              salidasIntermediasB[4] = 4;
              Serial.println("E5-S4");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 0){ //110 - s5
              salidasIntermediasB[4] = 5;
              Serial.println("E5-S5");
            }else if(controlEntradaB[1][0] == 1 && controlEntradaB[1][1] == 1 && controlEntradaB[1][2] == 1){ //111 - s6
              salidasIntermediasB[4] = 6;
              Serial.println("E5-S6");
            }else{
              salidasIntermediasB[4] = 0;
            } 
        }   
    etapa1 = false;
    etapa2 = true;
    }
   Serial.print("Reconfiguar: ");
  Serial.println(reconfigurar);
    if(reconfigurar == false){
      //--------------------------------------------------------SALIDA-----------------------------------------
        while(etapa2 == true){
      
      //Leyendo banderas para seleccionar la Etapa intermedia a leer (E1, E2, E3, E4 y E5)
      Serial.println("Selector de Etapa Intermedia");
      while(digitalRead(btnConfirmacion)!=1){
        controlSalidaB[0][0] = digitalRead(controlSalida[0]); 
        controlSalidaB[0][1] = digitalRead(controlSalida[1]);
        controlSalidaB[0][2] = digitalRead(controlSalida[2]); 
        }
       Serial.println("controlSalidaB[0][0]:");
       Serial.println(controlSalidaB[0][0]);
       Serial.println("controlSalidaB[0][1]:");
       Serial.println(controlSalidaB[0][1]);
       Serial.println("controlSalidaB[0][2]:");
       Serial.println(controlSalidaB[0][2]);
       delay(1000);
    
    
       //Leyendo banderas para seleccionar la SALIDA en la que se mostrará el contenido de la EX seleccionada
       Serial.println("Selector de Salida");
       while(digitalRead(btnConfirmacion)!=1){
        controlSalidaB[1][0] = digitalRead(controlSalida[0]); 
        controlSalidaB[1][1] = digitalRead(controlSalida[1]);
        controlSalidaB[1][2] = digitalRead(controlSalida[2]); 
        }
       Serial.println("controlSalidaB[1][0]:");
       Serial.println(controlSalidaB[1][0]);
       Serial.println("controlSalidaB[1][1]:");
       Serial.println(controlSalidaB[1][1]);
       Serial.println("controlSalidaB[1][2]:");
       Serial.println(controlSalidaB[1][2]);
        delay(1000);
        
        
        
        //ETAPA 2 - CONTROL A LA SALIDA
        //ETAPA 2.1 - SELECCIÓN DE LA SALIDA INTERMEDIA A MOSTRAR (EX)
      
        if(controlSalidaB[0][0] == 0 && controlSalidaB[0][1] == 1 && controlSalidaB[0][2] == 0){ // 010 - E1 - controlSalidaB[0][x]      
    
            //ETAPA 2.2 - SELECCION DE LA SALIDA (OUTPUTX)
    
            if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //010 - O1 - controlSalidaB[1][x]
              salidasB[0] = 1;
              Serial.println("O1-E1");
            }else if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 1){ //011 - O2 - controlSalidaB[1][x]
              salidasB[0] = 2;
              Serial.println("O2-E1");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 0){ //100 - O3 - controlSalidaB[1][x]
              salidasB[0] = 3;
              Serial.println("O3-E1");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 1){ //101 - O4 - controlSalidaB[1][x]
              salidasB[0] = 4;
              Serial.println("O4-E1");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //110 - O5 - controlSalidaB[1][x]
              salidasB[0] = 5;
              Serial.println("O5-E1");
            }else{
              salidasB[0] = 0;
            }
            
        }else if(controlSalidaB[0][0] == 0 && controlSalidaB[0][1] == 1 && controlSalidaB[0][2] == 1){ // 011 - E2 - controlSalidaB[0][x]     
    
          //ETAPA 2.2 - SELECCION DE LA SALIDA (OUTPUTX)
    
            if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //010 - O1 - controlSalidaB[1][x]
              salidasB[1] = 1;
              Serial.println("O1-E2");
            }else if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 1){ //011 - O2 - controlSalidaB[1][x]
              salidasB[1] = 2;
              Serial.println("O2-E2");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 0){ //100 - O3 - controlSalidaB[1][x]
              salidasB[1] = 3;
              Serial.println("O3-E2");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 1){ //101 - O4 - controlSalidaB[1][x]
              salidasB[1] = 4;
              Serial.println("O4-E2");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //110 - O5 - controlSalidaB[1][x]
              salidasB[1] = 5;
              Serial.println("O5-E2");
            }else{
              salidasB[1] = 0;
            }
            
        }else if(controlSalidaB[0][0] == 1 && controlSalidaB[0][1] == 0 && controlSalidaB[0][2] == 0){ // 100 - E3 - controlSalidaB[0][x]        
    
          //ETAPA 2.2 - SELECCION DE LA SALIDA (OUTPUTX)
    
            if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //010 - O1 - controlSalidaB[1][x]
              salidasB[2] = 1;
              Serial.println("O1-E3");
            }else if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 1){ //011 - O2 - controlSalidaB[1][x]
              salidasB[2] = 2;
              Serial.println("O2-E3");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 0){ //100 - O3 - controlSalidaB[1][x]
              salidasB[2] = 3;
              Serial.println("O3-E3");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 1){ //101 - O4 - controlSalidaB[1][x]
              salidasB[2] = 4;
              Serial.println("O4-E3");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //110 - O5 - controlSalidaB[1][x]
              salidasB[2] = 5;
              Serial.println("O5-E3");
            }else{
              salidasB[2] = 0;
            }
            
        }else if(controlSalidaB[0][0] == 1 && controlSalidaB[0][1] == 0 && controlSalidaB[0][2] == 1){ // 101 - E4 - controlSalidaB[0][x]            
    
          //ETAPA 2.2 - SELECCION DE LA SALIDA (OUTPUTX)
    
            if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //010 - O1 - controlSalidaB[1][x]
              salidasB[3] = 1;
              Serial.println("O1-E4");
            }else if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 1){ //011 - O2 - controlSalidaB[1][x]
              salidasB[3] = 2;
              Serial.println("O2-E4");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 0){ //100 - O3 - controlSalidaB[1][x]
              salidasB[3] = 3;
              Serial.println("O3-E4");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 1){ //101 - O4 - controlSalidaB[1][x]
              salidasB[3] = 4;
              Serial.println("O4-E4");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //110 - O5 - controlSalidaB[1][x]
              salidasB[3] = 5;
              Serial.println("O5-E4");
            }else{
              salidasB[3] = 0;
            }
            
        }else if(controlSalidaB[0][0] == 1 && controlSalidaB[0][1] == 1 && controlSalidaB[0][2] == 0){ // 110 - E5 - controlSalidaB[0][x]             
    
          //ETAPA 2.2 - SELECCION DE LA SALIDA (OUTPUTX)
    
            if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //010 - O1 - controlSalidaB[1][x]
              salidasB[4] = 1;
              Serial.println("O1-E5");
            }else if(controlSalidaB[1][0] == 0 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 1){ //011 - O2 - controlSalidaB[1][x]
              salidasB[4] = 2;
              Serial.println("O2-E5");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 0){ //100 - O3 - controlSalidaB[1][x]
              salidasB[4] = 3;
              Serial.println("O3-E5");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 0 && controlSalidaB[1][2] == 1){ //101 - O4 - controlSalidaB[1][x]
              salidasB[4] = 4;
              Serial.println("O4-E5");
            }else if(controlSalidaB[1][0] == 1 && controlSalidaB[1][1] == 1 && controlSalidaB[1][2] == 0){ //110 - O5 - controlSalidaB[1][x]
              salidasB[4] = 5;
              Serial.println("O5-E5");
            }else{
              salidasB[3] = 0;
            }
        }    
    etapa2 = false;
    etapa1 = true;
    }
   }
    
    reconfigurar = false;
    etapa1 = true;
    etapa2 = false;
}

//Interrupt Service Rutine
ISR(TIMER1_COMPA_vect){

  //Señales de entrada
  if(contadoresE[0]==1){
    digitalWrite(senialesE[0], !digitalRead(senialesE[0]));
    contadoresE[0]=0;
  }
  if(contadoresE[1]==2){
    digitalWrite(senialesE[1], !digitalRead(senialesE[1]));
    contadoresE[1]=0;
  }
  if(contadoresE[2]==3){
    digitalWrite(senialesE[2], !digitalRead(senialesE[2]));
    contadoresE[2]=0;
  }
  if(contadoresE[3]==4){
    digitalWrite(senialesE[3], !digitalRead(senialesE[3]));
    contadoresE[3]=0;
  }
  if(contadoresE[4]==5){
    digitalWrite(senialesE[4], !digitalRead(senialesE[4]));
    contadoresE[4]=0;
  }
  if(contadoresE[5]==6){
    digitalWrite(senialesE[5], !digitalRead(senialesE[5]));
    contadoresE[5]=0;
  }

  //Salidas intermedias 
  //E1
  if(salidasIntermediasB[0] == 1){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[0]));
  }else if(salidasIntermediasB[0] == 2){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[1]));
  }else if(salidasIntermediasB[0] == 3){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[2]));
  }else if(salidasIntermediasB[0] == 4){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[3]));
  }else if(salidasIntermediasB[0] == 5){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[4]));
  }else if(salidasIntermediasB[0] == 6){
    digitalWrite(salidasIntermedias[0],digitalRead(senialesE[5]));
  }

  //E2
  if(salidasIntermediasB[1] == 1){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[0]));
  }else if(salidasIntermediasB[1] == 2){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[1]));
  }else if(salidasIntermediasB[1] == 3){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[2]));
  }else if(salidasIntermediasB[1] == 4){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[3]));
  }else if(salidasIntermediasB[1] == 5){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[4]));
  }else if(salidasIntermediasB[1] == 6){
    digitalWrite(salidasIntermedias[1],digitalRead(senialesE[5]));
  }

  //E3
  if(salidasIntermediasB[2] == 1){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[0]));
  }else if(salidasIntermediasB[2] == 2){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[1]));
  }else if(salidasIntermediasB[2] == 3){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[2]));
  }else if(salidasIntermediasB[2] == 4){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[3]));
  }else if(salidasIntermediasB[2] == 5){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[4]));
  }else if(salidasIntermediasB[2] == 6){
    digitalWrite(salidasIntermedias[2],digitalRead(senialesE[5]));
  }

  //E4
  if(salidasIntermediasB[3] == 1){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[0]));
  }else if(salidasIntermediasB[3] == 2){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[1]));
  }else if(salidasIntermediasB[3] == 3){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[2]));
  }else if(salidasIntermediasB[3] == 4){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[3]));
  }else if(salidasIntermediasB[3] == 5){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[4]));
  }else if(salidasIntermediasB[3] == 6){
    digitalWrite(salidasIntermedias[3],digitalRead(senialesE[5]));
  }

  //E5
  if(salidasIntermediasB[4] == 1){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[0]));
  }else if(salidasIntermediasB[4] == 2){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[1]));
  }else if(salidasIntermediasB[4] == 3){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[2]));
  }else if(salidasIntermediasB[4] == 4){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[3]));
  }else if(salidasIntermediasB[4] == 5){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[4]));
  }else if(salidasIntermediasB[4] == 6){
    digitalWrite(salidasIntermedias[4],digitalRead(senialesE[5]));
  }

//Control de las salidas  

  //E1
  if(salidasB[0] == 1){
    digitalWrite(salidas[0],digitalRead(salidasIntermedias[0])); //O1
  }else if(salidasB[0] == 2){
    digitalWrite(salidas[1],digitalRead(salidasIntermedias[0])); //O2
  }else if(salidasB[0] == 3){
    digitalWrite(salidas[2],digitalRead(salidasIntermedias[0])); //O3
  }else if(salidasB[0] == 4){
    digitalWrite(salidas[3],digitalRead(salidasIntermedias[0])); //O4
  }else if(salidasB[0] == 5){
    digitalWrite(salidas[4],digitalRead(salidasIntermedias[0])); //O5
  }

  //E2
  if(salidasB[1] == 1){
    digitalWrite(salidas[0],digitalRead(salidasIntermedias[1])); //O1
  }else if(salidasB[1] == 2){
    digitalWrite(salidas[1],digitalRead(salidasIntermedias[1])); //O2
  }else if(salidasB[1] == 3){
    digitalWrite(salidas[2],digitalRead(salidasIntermedias[1])); //O3
  }else if(salidasB[1] == 4){
    digitalWrite(salidas[3],digitalRead(salidasIntermedias[1])); //O4
  }else if(salidasB[1] == 5){
    digitalWrite(salidas[4],digitalRead(salidasIntermedias[1])); //O5
  }

  //E3
  if(salidasB[2] == 1){
    digitalWrite(salidas[0],digitalRead(salidasIntermedias[2])); //O1
  }else if(salidasB[2] == 2){
    digitalWrite(salidas[1],digitalRead(salidasIntermedias[2])); //O2
  }else if(salidasB[2] == 3){
    digitalWrite(salidas[2],digitalRead(salidasIntermedias[2])); //O3
  }else if(salidasB[2] == 4){
    digitalWrite(salidas[3],digitalRead(salidasIntermedias[2])); //O4
  }else if(salidasB[2] == 5){
    digitalWrite(salidas[4],digitalRead(salidasIntermedias[2])); //O5
  }

  //E4
  if(salidasB[3] == 1){
    digitalWrite(salidas[0],digitalRead(salidasIntermedias[3])); //O1
  }else if(salidasB[3] == 2){
    digitalWrite(salidas[1],digitalRead(salidasIntermedias[3])); //O2
  }else if(salidasB[3] == 3){
    digitalWrite(salidas[2],digitalRead(salidasIntermedias[3])); //O3
  }else if(salidasB[3] == 4){
    digitalWrite(salidas[3],digitalRead(salidasIntermedias[3])); //O4
  }else if(salidasB[3] == 5){
    digitalWrite(salidas[4],digitalRead(salidasIntermedias[3])); //O5
  }

  //E5
  if(salidasB[4] == 1){
    digitalWrite(salidas[0],digitalRead(salidasIntermedias[4]));
  }else if(salidasB[4] == 2){
    digitalWrite(salidas[1],digitalRead(salidasIntermedias[4])); //O2
  }else if(salidasB[4] == 3){
    digitalWrite(salidas[2],digitalRead(salidasIntermedias[4])); //O3
  }else if(salidasB[4] == 4){
    digitalWrite(salidas[3],digitalRead(salidasIntermedias[4])); //O4
  }else if(salidasB[4] == 5){
    digitalWrite(salidas[4],digitalRead(salidasIntermedias[4])); //O5
  }
  

  //Contadores para el generador de señales
   contadoresE[0]++;contadoresE[1]++;contadoresE[2]++;contadoresE[3]++;contadoresE[4]++;contadoresE[5]++;
  
  
}
