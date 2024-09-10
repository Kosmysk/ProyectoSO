# Proyecto Sistemas Operativos - Shell

Este proyecto consiste en el desarrollo de una shell usando C, diseñada para ejecutar comandos de Linux con capacidades de manejo de procesos y pipes, así como para manejar recordatorios personalizados.

## Características
- Ejecución de comandos Linux.
- Soporte para pipes (**|**) para encadenar comandos.
- Comando para crear recordatorios.
- Sistemas de favoritos para comandos frecuentes.

## Compilación
1) Clona el repositorio en tu máquina local.
```
git clone https://github.com/Kosmysk/ProyectoSO.git
```
2) Navega al directorio del proyecto.
```
cd ProyectoSO
```
3) Compila el código utilizando gcc.
```
gcc -o mishell main.c

```
Esto generará un ejecutable llamado mishell.exe 

Para ejecutar la shell, simplemente ejecuta el archivo compilado:
```
./mishell
```
Una vez ejecutado, verás un prompt ($:) donde podrás ingresar comandos.

## Uso 
### Ejecución de comandos
Puedes ingresar cualquier comando válido de Linux, por ejemplo:
```
$: ls -l

```

### Uso de pipes
Puedes encadenar comandos usando pipes (|), por ejemplo:
```
$: ps aux | grep systemd | wc -l

```
### Comando Personalizado: Recordatorio
Puedes crear un recordatorio utilizando el comando **set recordatorio**, por ejemplo:
```
$: set recordatorio 10 "hacer pausa activa"

```
Esto mostrará el mensaje "hacer pausa activa" después de 10 segundos.
