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
void separarComandos(int cantidadArgumentos, char* argumentos[], int cantidadComandos, char** comandos[]){
    int index=0;
    printf("in sep\n");//debug
    int cLen = 0; //cLen es la cantidad de argumentos de cada comando individual
    for(int i=0;i<cantidadComandos-1; i++){ //repetir por la cantidad de comandos (menos uno porque el ultimo se hace fuera del loop)
      printf("splt cmd %d\n",i);//debug
      cLen=0;
      for(int j=index; strcmp(argumentos[j],"|")!=0;j++){//contar la cantidad de argumentos que hay hasta el siguiente |
        printf("  %s is not |\n",argumentos[j]);//debug
        cLen++;
      }
      printf("  finna malloc\n");//debug
      comandos[i] = (char**)malloc(sizeof(char*)*(cLen+1));
      printf("  malloced\n");//debug
      for(int j=0; j<cLen; j++){
        printf("    finna copy %s into space number %d\n",argumentos[index+j],j);//debug
        comandos[i][j] = malloc(sizeof(char)*strlen(argumentos[index+j]));
        strcpy(comandos[i][j],argumentos[index+j]);
      }
      comandos[i][cLen] = NULL;
      index += cLen+1;
    }
    //terminar de guardar el ultimo comando
    cLen =  cantidadArgumentos - index;
    printf("  finna malloc\n");//debug
    comandos[cantidadComandos-1] = (char**)malloc(sizeof(char*)*(cLen+1));
    printf("  malloced\n");//debug
    for(int j=0; j<cLen; j++){
      printf("    finna copy %s into space number %d\n",argumentos[index+j],j);//debug
      comandos[cantidadComandos-1][j] = malloc(sizeof(char)*strlen(argumentos[index+j]));
      strcpy(comandos[cantidadComandos-1][j],argumentos[index+j]);
    }
    printf("finished spliting commands\n");//debug
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
    char** execArgs = malloc(sizeof(char**)*space);
    //TODO: free execArgs
    int CantidadArgs = separarTexto(input, execArgs);

    for (int j = 0; j < CantidadArgs; j++){
      printf("[%s] ", execArgs[j]);
    }
    printf("\nSon %d argumentos\n", CantidadArgs);

    if(strcmp(execArgs[0],"exit")==0){
      return 0;
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
    printf("separar comandos\n");
    separarComandos(CantidadArgs, execArgs,cantidadComandos, comandos);
    for(int i=0;i<cantidadComandos;i++){//debug
      printf("cmd %d: ",i+1);
      for(int j=0; comandos[i][j]!=NULL; j++){
        printf("%s ",comandos[i][j]);
      }
      printf("\n");
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
