//Ejemplo MQTT-WebSocket & ESP8266 con sensor de temperatura RELE 

//--------------Librerias---------------------------------------- 

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#define D7 13
#define D8 15

SoftwareSerial SerialESP8266(D7, D8); // RX = D7, TX  = D8
 
//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char* ssid = "IoT";
const char* password = "123456789";
const char* mqtt_server = "54.197.41.29";
const int mqttPort = 1883;
 
WiFiClient espClient;
PubSubClient client(espClient);

char RELE[50];

char valueStr[15];
char Datos;
int f_v=0;
int f_a=0;
int f_p=0;
int f_d=0;
int cnt_dato=0;
int cnt_dec=0;
int f_decimal=0;
int VD[5]={0,0,0,0,0};
int VDD[3]={0,0,0};
float var_uart=0;
int var_dec=0;
float VH=0;
int Entero_VH=0;
byte H=0,L=0;
byte Dec_VH=0;
int cnt_envio=0;
float V=0,A=0,P=0;

String strVOLTAJE = "";
String strCORRIENTE = "";
String strPOTENCIA = "";

char VOLTAJE[50];
char CORRIENTE[50];
char POTENCIA[50];

unsigned long previousMillis = 0;

//---------------------------  CALLBACK ------------------------------------------------

void callback(char* topic, byte* payload, unsigned int length) {
 char PAYLOAD[5] = "    ";
  
  Serial.print("Mensaje recibido: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (String(topic) ==  String(RELE)) 
  {
        if (payload[1] == 'F')
        {
         digitalWrite(16, HIGH);
        }
        
        if (payload[1] == 'N')
        {
          digitalWrite(16, LOW);
        }
  }


}

//------------------------------------------------------------------------------

void reconnect() {
  // Loop hasta que nos reconectemos
    while (!client.connected()) 
    {
        Serial.print("Conectando MQTT...");
              
        if (client.connect("IoT-ESP8266")) 
        {
          Serial.println("Conectado");
          client.subscribe(RELE);
          client.subscribe("inTopic");
        } 
        
        else 
        {
          Serial.print("Error de conexion");
          Serial.print(client.state());
          delay(2000);
        }
    }
}

//--------------------------------------------------------------------------------------

void setup() {
 //salida 
  pinMode(16, OUTPUT); // D6 salida digital
  pinMode(D7,INPUT); //d7 is RX, receiver, so define it as input
  pinMode(D8,OUTPUT); //d8 is TX, transmitter, so define it as output

  Serial.begin(115200); //Start Serial
  SerialESP8266.begin(115200); //Start mySerial
  
  // Conexión WIFI
  WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED and contconexion <50) 
  // { //Cuenta hasta 50 si no se puede conectar lo cancela
  //   ++contconexion;
  //   delay(500);
  //   Serial.print(".");
  // }
  //     if (contconexion <50) 
  //   {
  //       //Para conectarnos con una IP estatica
  //       IPAddress ip(192,168,1,29); 
  //       IPAddress gateway(192,168,1,1); 
  //       IPAddress subnet(255,255,255,0); 
  //       WiFi.config(ip, gateway, subnet); 
        
  //       Serial.println("");
  //       Serial.println("WiFi conectado");
  //       Serial.println(WiFi.localIP());
  //   }
  
  //     else 
  //     { 
  //         Serial.println("");
  //         Serial.println("Error de conexion");
  //     }
  
    
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);
  
  String IoTRele = "IoT/Rele"; 
  IoTRele.toCharArray(RELE, 50);

  String voltaje = "IoT/Vol"; 
  voltaje.toCharArray(VOLTAJE, 50);

  String corriente = "IoT/Cor"; 
  corriente.toCharArray(CORRIENTE, 50);

  String potencia = "IoT/Pot"; 
  potencia.toCharArray(POTENCIA, 50);

}
 
void loop() {

  if (!client.connected()) 
    {
      reconnect();
    }
  
  client.loop();

//------------------------------
 if (SerialESP8266.available() > 0) {
   Datos=SerialESP8266.read();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(f_d==0){
    //banderas de seleccion de dato
      if(Datos=='v'){
      f_v=1;  
      f_d=1;
      }
      if(Datos=='a'){
      f_a=1;  
      f_d=1;
      }
      if(Datos=='p'){
      f_p=1;  
      f_d=1;
      }
      
      if(f_d==1)
      {
        //limpiar todo
        cnt_dato=0;
        cnt_dec=0;
        f_decimal=0;
        VD[0]=0;VD[1]=0;VD[2]=0;VD[3]=0;VD[4]=0;
        VDD[0]=0;VDD[1]=0;VDD[2]=0;VDD[3]=0;VDD[4]=0;
        var_uart=0;
        var_dec=0;
      
        }
 }    
else{ 
      if(Datos==';'){
           if(cnt_dato>0 && cnt_dato<5 && cnt_dec<3){
          
            if(cnt_dato==1){
             var_uart=VD[1]; 
            }
            if(cnt_dato==2){
            var_uart=VD[1]*10+VD[2]; 
            }
            if(cnt_dato==3){
            var_uart=VD[1]*100+VD[2]*10+VD[3]; 
            }
            if(cnt_dato==4){
            var_uart=VD[1]*1000+VD[2]*100+VD[3]*10+VD[4]; 
            }

            if(f_decimal==1){
            if(cnt_dec==1){
            var_dec=VDD[1];
            var_uart=var_uart+var_dec/10.0;
           }
            if(cnt_dec==2){
            var_dec=VDD[1]*10+VDD[2];
            var_uart=var_uart+var_dec/100.0;
            }
           }
        if(f_v==1){
        V=var_uart;
        f_v=0;
        f_a=0;
        f_p=0;
        f_d=0;
        cnt_dato=0;
        cnt_dec=0;
        f_decimal=0;
        VD[0]=0;VD[1]=0;VD[2]=0;VD[3]=0;VD[4]=0;
        VDD[0]=0;VDD[1]=0;VDD[2]=0;VDD[3]=0;VDD[4]=0;
        var_uart=0;
        var_dec=0;
          }
         else if(f_a==1){
        A=var_uart;
        f_v=0;
        f_a=0;
        f_p=0;
        f_d=0;
        cnt_dato=0;
        cnt_dec=0;
        f_decimal=0;
        VD[0]=0;VD[1]=0;VD[2]=0;VD[3]=0;VD[4]=0;
        VDD[0]=0;VDD[1]=0;VDD[2]=0;VDD[3]=0;VDD[4]=0;
        var_uart=0;
        var_dec=0;
          }
         else if(f_p==1){
          P=var_uart;
          f_v=0;
        f_a=0;
        f_p=0;
        f_d=0;
        cnt_dato=0;
        cnt_dec=0;
        f_decimal=0;
        VD[0]=0;VD[1]=0;VD[2]=0;VD[3]=0;VD[4]=0;
        VDD[0]=0;VDD[1]=0;VDD[2]=0;VDD[3]=0;VDD[4]=0;
        var_uart=0;
        var_dec=0;
        cnt_envio++;
          } 
        }
      else{
        f_v=0;
        f_a=0;
        f_p=0;
        f_d=0;
        cnt_dato=0;
        cnt_dec=0;
        f_decimal=0;
        VD[0]=0;VD[1]=0;VD[2]=0;VD[3]=0;VD[4]=0;
        VDD[0]=0;VDD[1]=0;VDD[2]=0;VDD[3]=0;VDD[4]=0;
        var_uart=0;
        var_dec=0;
        
        }
        
      }
  else if(Datos=='.'){
      //si es un punto
      f_decimal=1;
      }
   else{
        
        //dato valido
          if(f_decimal==0){
          cnt_dato++;
          VD[cnt_dato]=Datos-48;
          }
          else{
          cnt_dec++;
          VDD[cnt_dec]=Datos-48;
          }
       }
///
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
}

  
    VH=VH+(0.00555556*P);
    Entero_VH=VH;
    Dec_VH=(VH-Entero_VH)*100;


  unsigned long currentMillis = millis();
    
  if (currentMillis - previousMillis >= 5000) 
  { //enviar datos cada 5 segundos
    previousMillis = currentMillis;


    strVOLTAJE = String((float)V);
    strVOLTAJE.toCharArray(valueStr, 15);
    Serial.print("[" +  String(VOLTAJE) + "] " + strVOLTAJE + "V; ");
    client.publish(VOLTAJE, valueStr);
    
    strCORRIENTE = String((float)A);
    strCORRIENTE.toCharArray(valueStr, 15);
    Serial.print("[" +  String(CORRIENTE) + "] " + strCORRIENTE + "A; ");
    client.publish(CORRIENTE, valueStr);
    
    strPOTENCIA = String((float)P);
    strPOTENCIA.toCharArray(valueStr, 15);
    Serial.print("[" +  String(POTENCIA) + "] " + strPOTENCIA + "W; ");
    client.publish(POTENCIA, valueStr);
    
    Serial.println();
   
  }

}