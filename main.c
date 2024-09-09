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
      wait(pid);
    }
  return 0;
}

void almacenarComando(char ***almacenamiento, int *contador, char *input) {
  *almacenamiento = realloc(*almacenamiento, (*contador + 1) * sizeof(char*));
  (*almacenamiento)[*contador] = strdup(input); // Almacena una copia del input
  (*contador)++;
}

int main(){
  char input[1000];
  char **historial = NULL; // Arreglo dinamico para almacenar los comandos ingresados
  int contadorHistorial = 0;
  int argCount; 
  system("clear");
  while(1){
    printf("$ ");
    fgets(input, sizeof(input),stdin);//lee toda la linea de entrada
    input[strlen(input)-1]='\0';

    almacenarComando(&historial, &contadorHistorial, input);

    //asumiendo input es "ls -l"
    char* execArgs[] = {input,NULL};
    char *token = strtok(input, " ");
    argCount = 0;
    //TODO: separar execArgs debe tener los valores de input separados por espacio
    // ej: "aaa bbb ccc" -> {"aaa","bbb","ccc"}
    //TODO argCount debe guardar la cantidad de argumentos que tiene el comando ingresado
    
    if(strcmp(execArgs[0],"exit")==0){
      for(int i = 0; i < contadorHistorial; i++) {
        free(historial[i]);
      }
      free(historial);
      return 0;
    }
    if(strcmp(execArgs[0], "history") == 0){
      // Imprime los comandos almacenados
      for(int i = 0; i < contadorHistorial; i++){
        printf("%d: %s\n", i+1, historial[i]);
      }
    }else if(/*no hay pipe*/1){ //TODO:hacer argumento del if
      miExec(execArgs);
    }else{
      //si si hay pipe
    }
  }
  return 0;
}
