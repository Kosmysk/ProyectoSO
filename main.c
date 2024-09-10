#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>



typedef struct {
    char* comando;
    int id;
} Fav;
Fav favoritos[100];       // Lista de 100 comandos favoritos en memoria
int contadorFavs = 0;     // Cantidad de comandos favoritos actuales
char* archivoFavs = NULL; // Ruta del archivo donde se guardan los favoritos

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

// Comandos Favs

void agregarFav(char *comando) {
    for(int i = 0; i < contadorFavs; i++){
        if(strcmp(favoritos[i].comando, comando) == 0){     // Verifica si el comando ya está en favoritos
            return; // Comando ya en favoritos, no se agrega
        }
    }
    if(contadorFavs < 100){
        favoritos[contadorFavs].comando = strdup(comando);    // Agrega un nuevo favorito si hay espacio
        favoritos[contadorFavs].id = contadorFavs + 1;
        contadorFavs++;
    }
}

// Muestra los comandos favoritos en memoria
void mostrarFav() {
    if(contadorFavs == 0){
        printf("No hay comandos favoritos.\n");
        return;
    }
    for(int i = 0; i < contadorFavs; i++){
        printf("%d: %s\n", favoritos[i].id, favoritos[i].comando);
    }
}

/* Elimina favoritos por número de identificación */
void eliminarFav(char *numeros) {
    char *num = strtok(numeros, ",");
    while(num != NULL){
        int id = atoi(num);
        if(id > 0 && id <= contadorFavs){
            free(favoritos[id - 1].comando); // Libera el comando
            favoritos[id - 1].comando = NULL;
        }
        num = strtok(NULL, ",");
    }
    int aux = 0;
    for(int i = 0, j = 0; i < contadorFavs; i++){   // Reorganiza la lista después de la eliminación
        if(favoritos[i].comando != NULL){
            favoritos[j] = favoritos[i];
            favoritos[j].id = j + 1;
            j++;
        }
        aux = j;
    }
    contadorFavs = aux; // Actualiza el contador
}

// Busca un comando en favoritos que contenga el substring
void buscarcmdFav(char *cmd) {
    for(int i = 0; i < contadorFavs; i++){
        if(strstr(favoritos[i].comando, cmd) != NULL){
            printf("%d: %s\n", favoritos[i].id, favoritos[i].comando);
        }
    }
}

// Borra todos los comandos favoritos
void borrarFav() {
    for(int i = 0; i < contadorFavs; i++){
        free(favoritos[i].comando);
    }
    contadorFavs = 0;
}

// Ejecuta un comando favorito por número
void ejecutarFav(int id) {
    if(id > 0 && id <= contadorFavs){
        char *execArgs[100];
        char *token = strtok(favoritos[id - 1].comando, " ");
        int argCount = 0;
        while(token != NULL && argCount < 99){
            execArgs[argCount++] = token;
            token = strtok(NULL, " ");
        }
        execArgs[argCount] = NULL;
        //miExec(execArgs);
    } else {
        printf("Numero de fav invalido.\n");
    }
}
// Cargar los comandos favoritos desde un archivo
void cargarFav() {
    if(archivoFavs == NULL) {
        printf("No se ha especificado ningún archivo de favoritos.\n");
        return;
    }
    FILE *file = fopen(archivoFavs, "r");
    if(file == NULL) {
        printf("No se pudo abrir el archivo %s.\n", archivoFavs);
        return;
    }
    char buffer[1000];
    while(fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea
        agregarFav(buffer);
    }
    fclose(file);
    printf("Comandos favoritos cargados desde %s.\n", archivoFavs);
}

//Guardar los comandos favoritos en un archivo
void guardarFav() {
    if(archivoFavs == NULL) {                     // Caso en donde no se ingresa ningun parametro
        printf("No se ha especificado ningún archivo de favoritos.\n");
        return;
    }
    FILE *file = fopen(archivoFavs, "w");         // Caso en donde si se ingresa pero falla al abrir el archivo
    if(file == NULL) {
        printf("No se pudo abrir el archivo %s para escritura.\n", archivoFavs);
        return;
    }
    for(int i = 0; i < contadorFavs; i++){       
        fprintf(file, "%s\n", favoritos[i].comando);
    }
    fclose(file);
    printf("Comandos favoritos guardados en %s.\n", archivoFavs);
}
// Manejo del comando favs
void manejarFavs(char *opcion, char *parametro) {
    if(strcmp(opcion, "crear") == 0 && parametro != NULL) {                       // "ejecuta" la opción crear
        archivoFavs = strdup(parametro);
        printf("Archivo de favoritos creado en: %s\n", archivoFavs);
    } else if(strcmp(opcion, "mostrar") == 0) {                                   // "ejecuta" la opción mostrar
        mostrarFav();
    } else if(strcmp(opcion, "eliminar") == 0 && parametro != NULL) {             // "ejecuta" la opción eliminar
        eliminarFav(parametro);
    } else if(strcmp(opcion, "buscar") == 0 && parametro != NULL) {               // "ejecuta" la opción eliminar
        buscarcmdFav(parametro);
    } else if(strcmp(opcion, "borrar") == 0) {                                    // "ejecuta" la opción borrar
        borrarFav();
    } else if(strcmp(opcion, "cargar") == 0) {                                    // "ejecuta" la opción cargar
        cargarFav();
    } else if(strcmp(opcion, "guardar") == 0) {                                    // "ejecuta" la opción guardar
        guardarFav();
    } else if(strcmp(opcion, "ejecutar") == 0 && parametro != NULL) {               // "ejecuta" la opción ejecutar
        ejecutarFav(atoi(parametro));
    } else {
        printf("Comando favs no reconocido o incompleto.\n");
    }
}

//

int main(){
  char input[1000];
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

  }
  return 0;
}
