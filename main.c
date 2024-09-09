#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>




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

void separarComandos(int cantidadArgumentos, char* argumentos[], int cantidadComandos, char** comandos[]){
    int index=0;
    int cLen = 0; //cLen es la cantidad de argumentos de cada comando individual
    for(int i=0;i<cantidadComandos-1; i++){ //repetir por la cantidad de comandos (menos uno porque el ultimo se hace fuera del loop)
      cLen=0;
      for(int j=index; strcmp(argumentos[j],"|")!=0;j++){//contar la cantidad de argumentos que hay hasta el siguiente |
        cLen++;

      }
      comandos[i] = (char**)malloc(sizeof(char*)*(cLen+1));
      for(int j=0; j<cLen; j++){
        comandos[i][j] = malloc(sizeof(char)*strlen(argumentos[index+j]));
        strcpy(comandos[i][j],argumentos[index+j]);
      }
      comandos[i][cLen] = NULL;
      index += cLen+1;
    }
    //terminar de guardar el ultimo comando
    cLen =  cantidadArgumentos - index;
    comandos[cantidadComandos-1] = (char**)malloc(sizeof(char*)*(cLen+1));
    for(int j=0; j<cLen; j++){
      comandos[cantidadComandos-1][j] = malloc(sizeof(char)*strlen(argumentos[index+j]));
      strcpy(comandos[cantidadComandos-1][j],argumentos[index+j]);
    }
}
void swapPipes(int p1[], int p2[]){
    int tmp = p1[0];
    p1[0]=p2[0];
    p2[0]=tmp;

    tmp = p1[1];
    p1[1]=p2[1];
    p2[1]=tmp;
    return;
}
  // Nueva función para manejar el recordatorio
  void setRecordatorio(int tiempo, char* mensaje) {
        int pid = fork();
        if(pid == 0) { // Proceso hijo
            sleep(tiempo);
            printf("\nRecordatorio: %s\n$: ", mensaje);
            exit(0);
        }
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
    printf("$: ");
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
    char** execArgs = malloc(sizeof(char**)*space);
    //TODO: free execArgs
    int CantidadArgs = separarTexto(input, execArgs);

    for (int j = 0; j < CantidadArgs; j++){
      //printf("[%s] ", execArgs[j]);
    }
    //printf("\nSon %d argumentos\n", CantidadArgs);

    if(strcmp(execArgs[0], "exit") == 0) {
      for(int i = 0; i < contadorHistorial; i++) {
        free(historial[i]);
      }
        free(historial);
        return 0;
    }
    // Comando personalizado "set recordatorio"     
    if(strcmp(execArgs[0], "set") == 0 && strcmp(execArgs[1], "recordatorio") == 0) {
      int tiempo = atoi(execArgs[2]);
      char mensaje[1000] = "";
      for(int i = 3; i < CantidadArgs; i++) {
        strcat(mensaje, execArgs[i]);
        if (i < CantidadArgs - 1) strcat(mensaje, " ");
        }
        setRecordatorio(tiempo, mensaje);
        continue;
      }
      // Comando personalizado "history"
      almacenarComando(&historial, &contadorHistorial, input);
      
      if(strcmp(execArgs[0], "history") == 0){
      // Imprime los comandos almacenados
      for(int i = 0; i < contadorHistorial; i++){
        printf("%d: %s\n", i+1, historial[i]);
      }
      }


    //contar cantidad de comandos que se estan haciendo pipe
    int cantidadComandos = 0;
    for(int i=0;i<CantidadArgs;i++){
      if(strcmp(execArgs[i],"|")==0){
        cantidadComandos++;
      }
    }
    cantidadComandos += 1;
    char*** comandos = (char***)malloc(sizeof(char**)*cantidadComandos);
    //TODO: free comandos
    separarComandos(CantidadArgs, execArgs,cantidadComandos, comandos);
    

    int p1[2];
    int p2[2];
    if(pipe(p1) == -1 || pipe(p2) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid;
    for(int i=0;i<cantidadComandos;i++){
        pipe(p2); //make a new pipe in p2, to write to
        pid = fork();
        if(pid==0){
            //read from p1, write to p2
            close(p1[1]);
            if(i!=0)dup2(p1[0],STDIN_FILENO);
            close(p1[0]);

            close(p2[0]);
            if(i!=cantidadComandos-1)dup2(p2[1],STDOUT_FILENO);
            close(p2[1]);
            //execute the command
            execvp(comandos[i][0],comandos[i]);
            printf("error\n");
            exit(1);
          }
        }
        close(p1[1]);
        //wait for previous process to finish
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            // Process exited normally
            int exit_status = WEXITSTATUS(status);
        }
        //swap the pipes so that next process read from the pipe that was written to
        swapPipes(p1,p2);
    }
    return 0;
  }