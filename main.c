#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
/*esta funcion va a tomar dos arreglos de argumentos, y usara la funcion pipe
para hacer la salida de uno la entrada del otro. Si args2 tambien contiene pipe,
se va a llamar a si misma, recursivamente
retorna 0 si se ejecuta correctamente y -1 si hay un error*/
int recursivePipe(char* args1[], char* args2){
  //TODO
  return 0;
}

int main(){
  char input[1000];
  int argCount; 

  while(1){
    fgets(input, sizeof(input),stdin);//lee toda la linea de entrada
    char* execArgs[] = {input};
    //TODO: separar execArgs debe tener los valores de input separados por espacio
    // ej: "aaa bbb ccc" -> {"aaa","bbb","ccc"}
    //TODO argCount debe guardar la cantidad de argumentos que tiene el comando ingresado

    if(strcmp(execArgs[0],"exit\n")==0){
      break;
    }
  }
  return 0;
}
