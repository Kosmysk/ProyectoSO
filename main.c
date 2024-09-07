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
/*
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
*/

// Esta funcion, por medio de punteros, modificara el valor de el arreglo de strings execArgs[], y devolverá la cantidad de argumentos
int separarTexto(char input[1000], char *argumentos[]){
    int i = 0;
    char* token = strtok(input, " ");
    while(token != NULL){
      argumentos[i] = (char*)malloc(strlen(token) + 1);
      strcpy(argumentos[i], token);
      i++;
      token = strtok(NULL, " ");
    }

    return i;
    }
    

int main(){
  char input[1000];
  int argCount; 
  system("clear");
  while(1){
    printf("$ ");
    fgets(input, sizeof(input),stdin);//lee toda la linea de entrada
    input[strlen(input)-1]='\0';

    // Separar texto

    // Contar cantidad de espacios
    int space = 0;
    for (int i = 0; i < strlen(input);i++){
      if (input[i] == ' ' && input[i + 1] != ' '){
        space ++;
      }
    }
    // Separar argumentos y guardarlos en execArgs[]
    char** execArgs;
    int CantidadArgs = separarTexto(input, execArgs);

    for (int j = 0; j < CantidadArgs; j++){
      printf("[%s] ", execArgs[j]);
    }
    printf("\nSon %d argumentos", CantidadArgs);
    //TODO: separar execArgs debe tener los valores de input separados por espacio
    // ej: "aaa bbb ccc" -> {"aaa","bbb","ccc"}
    //TODO argCount debe guardar la cantidad de argumentos que tiene el comando ingresado
    
    if(strcmp(execArgs[0],"exit")==0){
      return 0;
    }
    
    
    /*
    if(no hay pipe1){ //TODO:hacer argumento del if
      miExec(execArgs);
    }else{
      //si si hay pipe
    }
    */
  }
  return 0;
}
