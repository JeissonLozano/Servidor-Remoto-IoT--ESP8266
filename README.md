# Servidor remoto AWS ESP8266

Este código pertenece al microcontrolador esp8266 de una serie de IoT, en esta primera etapa se hizo un ejemplo donde se evidencia el funcionamiento de un servidor de forma local.

# A continuación el esquema de conexión: 
![ESQUEMA DE CONEXIÓN ESP8266](https://github.com/JeissonLozano/Servidor-Remoto-IoT--ESP8266/blob/master/Conexi%C3%B3n%20AWS.png)
 
# A continuación video paso a paso de instalación y creación de un servidor de forma local IoT:
[![](http://img.youtube.com/vi/tKb1fhK6npc/0.jpg)](https://youtu.be/tKb1fhK6npc "Servidor remoto AWS IoT: MQTT-WebSocket")

# Listado de comandos usados:

# Para instalar MQTT:

Comandos:
```
$ apt-get update
$ apt-get install mosquitto
$ apt-get install mosquitto-clients
```

-Para la prueba Comandos:
```
$ mosquitto_sub -h localhost -t "topic/IoT"
$ mosquitto_pub -h localhost -t "topic/IoT" -m "Hola mundo!"
```
# Para WebSocket:
```
-En esta direeción /etc/mosquitto/conf.d creamos un archivo websockets.conf 
-El comando "touch" sirve para crear y "nano" para modificar
```
Comandos:
```
$ cd /etc/mosquitto/conf.d
$ touch websockets.conf
$ nano websockets.conf  
```

-Agregamos lo siguiente dentro de websockets.conf 
```
listener 1883
protocol mqtt

listener 9001
protocol websockets
```
# Luego:
-Reiniciamos mosquitto con el siguiente comandos:
```
$ systemctl restart mosquitto
```
*Reiniciamos la máquina virtual (IMPORTANTE).

-Instalamos Apache y git Comandos:
```
$ apt-get install apache2
$ apt-get install git
```
-Luego clonamos el siguiente repositorio en la siguiente dirección:
```
$ cd /var/www/html/
$ git clone https://github.com/hivemq/hivemq-mqtt-web-client.git mqtt-web
$ chown www-data:www-data -R mqtt-web
```
-Listo accedemos desde nuestro navegador ej:
```
http://"localhost"/mqtt-web/
```
-En este caso "localhost" es la ip que te brinda el servidor

## Construido con 🛠️
* [VsCode](https://code.visualstudio.com/) - Editor de texto Visual Studio Code
* [PlatformIO](https://platformio.org/) - PlatformIO IDE

## Autor ✒️
* **Jeisson Lozano** - *Desarrollador* - [Jeisson Lozano](https://github.com/JeissonLozano)

