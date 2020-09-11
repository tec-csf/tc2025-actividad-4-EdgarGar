/*/**
 Recibe como argumento en la línea de comandos el número de procesos hijos a crear (opción -n). 
 El programa debe validar que el valor especificado sea un número entero. 
Sugerencia: revise la función atoi (Enlaces a un sitio externo.).
Crea los n procesos hijos siempre y cuando no existan errores. Si al crear un proceso hijo se genera un error, 
entonces debe mostrar un mensaje indicando el número de procesos hijos creados hasta ese momento y no debe intentar 
crear más procesos hijos.
Espera a que terminen todos los procesos hijos en el mismo orden en que se crearon.
A medida que los procesos hijos van terminando, debe ir guardando el valor de retorno de cada proceso hijo.
Una vez que terminen todos los procesos hijos, el proceso padre debe mostrar una tabla en el siguiente formato:
PID Hijo	Promedio	Histograma
20	20	********************
10	15	**************
50	35	*****************************
60	40	**************************************
Sugerencia: Para determinar la cantidad de * a imprimir, defina una relación respecto al promedio máximo obtenido. 
Por ejemplo, si el promedio máximo obtenido es 1000, entonces divida los promedios entre 100. De esta manera, 
para el promedio máximo imprimirá 10 * y todos los demás estarán acotados por debajo de este máximo.
Por su parte, cada proceso hijo debe:
Calcular el promedio entre su PID y el PID de su padre.
Imprimir un mensaje que diga: "Soy el proceso hijo con PID = $pid y mi promedio es = $promedio".
$pid debe sustituirse por el valor del PID del proceso hijo.
$promedio debe sustituirse por el resultado de calcular el promedio entre el PID del proceso hijo y el PID 
del proceso padre.
Al terminar, debe regresar el promedio calculado como valor de retorno  al proceso padre.

Edgar Garcia
Trabajo junto a Rafael Miranda

 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

typedef struct {
    int id;
    int average;
} Child;

void histograma(Child * childs, Child * fin){
    int maximo = 0;
    for (Child * h = childs; h < fin; ++h){
        if (h->average > maximo){
            maximo = h->average;
        }
    }
    printf("PID Hijo \tPromedio \tHistograma\n\n");

    for (Child * h = childs; h < fin; ++h){
        printf("%d \t\t%d \t\t", h->id, h->average);
        for (int i = 0; i < ((h->average * 5) / maximo); ++i){
            printf("*");
        }
        printf("\n\n");
    }
}

int main(int argc, char * const * argv){
    int date;
    int numberChild;
    char * cvalue = NULL;

    opterr = 0;

    while ((date = getopt(argc, argv, "n:")) != -1){
        switch (date){
            case 'n':
                cvalue = optarg;

                if (isdigit(*cvalue) > 0){
                    numberChild = atoi(cvalue);
                    printf("Cantidad de hijos: %d\n\n", numberChild);
                }

                else {
                    printf("No proporcionó un valor numérico\n");
                }
            break;

            case '?':
                if (optopt == 'n'){
                    fprintf(stderr, "Opción -%c requiere un argumento.\n", optopt);
                }
                else if (isprint (optopt)){
                    fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
                }   
                else {
                    fprintf (stderr, "Opción desconocida '\\x%x'.\n", optopt);
                }
                return 1;
            default:
                abort();
            break;
        }
    }

    pid_t * ids = malloc(sizeof(pid_t) * numberChild);
    pid_t * total = ids + numberChild;
    pid_t * pos = ids;
    int i = 0;
    int newChild = 0;
    int averageVal;
    pid_t pid;

    while ((pos < total) && (i < numberChild)){
        pid = fork();
        *pos = pid;

        if (pid == -1){
            printf("Hubo un error al crear el proceso hijo %d\n", i);
            printf("Procesos hijos creados hasta el momento: %d\n\n", newChild);

            break;
        }

        else if (pid == 0){
            int average = (getppid() + getpid()) / 2;
            printf("Estamos en el proceso hijo con PID = %d, su padre es PPID = %d y su promedio es %d\n", getpid(), getppid(), average);
            
            sleep(1);

            exit(average);
        }

        else {
            ++newChild;
            printf("Hijos creados: %d\n\n", newChild);
        }

        ++pos;
        ++i;
    }

    if (newChild == 0){
        printf("No se pudo crear más de un hijo.");
        free(ids);
        return 0;
    }

    else if (newChild > 0){
        Child * childs = (Child *) malloc(sizeof(Child) * newChild);
        Child * fin = childs + newChild;
        Child * h = childs;
        pos = ids;

        while ((h < fin) && (pos < total)){
            if (waitpid(*pos, &averageVal, 0) != -1){
                if (WIFEXITED(averageVal)){
                    h->id = *pos;
                    h->average = WEXITSTATUS(averageVal);
                }
            }

            ++pos;
            ++h;
        }
        printf("\n");
        histograma(childs, fin);
        free(ids);
        free(childs);
        return 0;
    }
}