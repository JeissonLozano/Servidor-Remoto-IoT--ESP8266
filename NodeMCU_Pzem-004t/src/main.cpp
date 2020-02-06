#include <PZEM004T.h>

HardwareSerial hwserial(UART0);
PZEM004T pzem(&hwserial);  
IPAddress ip(192,168,1,1);

bool pzemrdy = false;

void setup() {
   hwserial.swap();           
   Serial1.begin(115200);     
   while (!pzemrdy) {
      Serial1.println("Connecting to PZEM...");
      pzemrdy = pzem.setAddress(ip);
      delay(1000);
   }
}

void loop() {

float v = pzem.voltage(ip);
 if (v < 0.0){
 v = 0.0;
 }
 Serial1.print("v");Serial1.print(v);Serial1.print(";");
 float i = pzem.current(ip);
 if(i < 0.0){ 
 i=0.0;
 }
Serial1.print("a");Serial1.print(i);Serial1.print(";");

float p = pzem.power(ip);
if(p < 0.0){ 
 p=0.0;
 }
Serial1.print("p");Serial1.print(p);Serial1.print(";");
delay(3000);
}