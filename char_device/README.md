# Trabajo Practico Inicial de Sistemas Operativos y Redes II
## Device Drivers - Char Device
## Introducción
Para la realización de este TP se creo un modulo de kernel para un char device que haga encriptación cesar


## Pasos

* Ejecutar ```make clean & make``` compilar el modulo
![](imagenes/1makeclean.jpg)

* Insertar el modulo ```sudo insmod michardevice.ko``` 
![](imagenes/2insert.jpg)

* ver el log de kernel con el comando ```dmesg``` para ver que el modulo fue insertado de manera correcta. 
![](imagenes/3logkernel.jpg)

* Creamos el archivo como se explico en el log utilizando ```sudo mknod``` y le habilitamos todos los permisos con ```chmod 666``` para no tener problemas de escritura.
![](imagenes/4mknod.jpg)

* Hacemos un ```echo``` al char device en ```/dev/miCharDevice``` para pasarle la cadena de texto a encriptar. Y luego hacemos un ```cat``` sobre el char device para ver el resultado.
![](imagenes/5catandecho.jpg)

* Eliminamos el modulo con ```sudo rmmod miChardDevice.ko```. Vemos el log con ```dmesg```
![](imagenes/6remove.jpg)


