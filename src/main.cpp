#include <MakerMini.h>
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
 double frames = 70;
double kp = 900;
double ki = 300;
double kd = 10;
 
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double setPoint, output, input;
double cumError, rateError;
double data;

Andromie Mini;

double computePID(double inp,double minLimit, double maxLimit);

 void setup()
 {
   
   Serial.begin(115200);
   Serial.setTimeout(500);
   
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);
   Mini.iniciar();
   
   setPoint = 320; 
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
         else state = 1000;
         break;
         
       case 1: posx = atoi(tok); 
       output = computePID(posx,20,150);
       
       break;
       
       case 2: posy = atoi(tok); 
       //Serial.println(posy);
       break;

       case 3: 
       Mini.motor(1,0,0);
       Mini.motor(1,1,0); 
      Mini.motor(2,0,0);
      Mini.motor(2,1,0); 
      Serial.println("0,0,0");
       break;
       default: break; // Skip any additional tokens
     }
     tok = strtok(0, " \r\n");
   }         
    

}

double computePID(double inp,double minLimit, double maxLimit){     
        currentTime = millis();
        elapsedTime = (double)(currentTime - previousTime); 
        double sampletime = 10;
        double sampletimeInSec = sampletime / 1000;     //compute time elapsed from previous computation
        if (elapsedTime >= sampletime){
        error = setPoint - inp;                                // determine error
        cumError = error*sampletimeInSec +  cumError;                // compute integral
        rateError = (error - lastError)/sampletimeInSec ;   // compute derivative
 
        double out = abs(kp*error + ki*cumError + kd*rateError);                //PID output               
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
        out = map(out,0,262144, minLimit ,maxLimit);
        if (out  > maxLimit) out = maxLimit;
        else if (out < minLimit) out = minLimit;
        else out  = out;
        Serial.print(inp);
        Serial.print(", ");
        Serial.print(error);
        Serial.print(", ");
        Serial.println(out);
        if (error  <  0)  {        
        Mini.motor(1,0,out);
        Mini.motor(2,1,out);
        //Serial.println(out); 
        }else {       
        Mini.motor(1,1,out);
        Mini.motor(2,0,out);
        //Serial.println(out); 
        }
         
        
        return out      ;     //have function return the PID output
        }    
}