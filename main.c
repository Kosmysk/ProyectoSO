#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
/*esta funcion va a tomar dos arreglos de argumentos, y usara la funcion pipe
para hacer la salida de uno la entrada del otro. Si args2 tambien contiene pipe,
se va a llamar a si misma, recursivamente
retorna 0 si se ejecuta correctamente y -1 si hay un error*/
int recursivePipe(char* args1[], char* args2[]){
  //TODO
  return 0;
}
int miExec(char* args[]){
  int pid = fork();
      if(pid==0){  
        int e = execvp(args[0],args);
        printf("hubo un error\n");
      }else{
       wait(NULL);
      }
  return 0;
}

  // Nueva función para manejar el recordatorio
  void setRecordatorio(int tiempo, char* mensaje) {
        int pid = fork();
        if(pid == 0) { // Proceso hijo
            sleep(tiempo);
            printf("\nRecordatorio: %s\n", mensaje);
            exit(0);
        }
}

int main(){
  char input[1000];
  int argCount; 
  system("clear");
  while(1){
    printf("$ ");
    fgets(input, sizeof(input),stdin);//lee toda la linea de entrada
    input[strlen(input)-1]='\0';


    //TODO: separar execArgs debe tener los valores de input separados por espacio
    // ej: "aaa bbb ccc" -> {"aaa","bbb","ccc"}
    //TODO argCount debe guardar la cantidad de argumentos que tiene el comando ingresado

      // TODO: Código temporal para separar argumentos, será reemplazado por la implementación final.
          char* execArgs[100];
          char* token = strtok(input, " ");
          argCount = 0;
          while(token != NULL) {
           execArgs[argCount++] = token;
           token = strtok(NULL, " ");
         }
          execArgs[argCount] = NULL;

      if(strcmp(execArgs[0], "exit") == 0) {
        return 0;
      }
      // Comando personalizado "set recordatorio"     
      if(strcmp(execArgs[0], "set") == 0 && strcmp(execArgs[1], "recordatorio") == 0) {
        int tiempo = atoi(execArgs[2]);
        char mensaje[1000] = "";
        for(int i = 3; i < argCount; i++) {
          strcat(mensaje, execArgs[i]);
          if (i < argCount - 1) strcat(mensaje, " ");
        }
      setRecordatorio(tiempo, mensaje);
        } else {
          // Ejecución normal del comando
          miExec(execArgs);
        }
  }
  return 0;
}