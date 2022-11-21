#include <math.h>
#include <Arduino.h>
 // Pin for LED, blinks as we receive serial commands:
#define LED_BUILTIN 13
 // Serial port to use: on chips with USB (e.g., 32u4), that usually is Serial1.
 // On chips without USB, use Serial:

 // Buffer for received serial port bytes:
#define INLEN 128
char instr[INLEN + 1];
double posx = 0;
double posy = 0;
double posz = 0;

void setup()
 {
   
   Serial.begin(115200);
   Serial.setTimeout(500);
   
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);
   
   Serial.println("Conexion");
}
  
 void loop()
 {
  // Read a line of data from JeVois:
   byte len = Serial.readBytesUntil('\n', instr, INLEN);
   instr[len] = 0;
   char * tok = strtok(instr, " \r\n");
   int state = 0;
   while (tok)
   {
     switch (state)
     {
       case 0:
         if (strcmp(tok, "T1") == 0) state = 1;
         else if (strcmp(tok, "T2") == 0) state = 2;
         else if (strcmp(tok, "T3") == 0) state = 3;
         else if (strcmp(tok, "T4") == 0) state = 4;
         else state = 1000;
         break;
         
       case 1: posx = atoi(tok); 
       Serial.print(posx);
       Serial.print(", ");
       break;
       
       case 2: posy = atoi(tok); 
       Serial.print(posy);
       Serial.println(", ");
       break;

       case 3: 
       Serial.println(" ");
       break;

       case 4: 
       Serial.println("0,0,0,0");
       break;
       default: break; // Skip any additional tokens
     }
     tok = strtok(0, " \r\n");
   }         
    

}
