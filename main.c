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

Edgar Garcia Con colaboración junto a Rafael Miranda para ver estrcuctura del programa*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	pid_t value;
    pid_t average;
} child;

void showStarts(int, int);
//Funcion Main
int main(int argc, char const *argv[])
{
    int children = 0;
    if(argc > 1){
        children = atoi(argv[1]);
    }else{
        printf("Por favor introduce el numero de procesos hijos a crear al momento de correr el programa.\n");
        return -1;
    }
    pid_t pid;
    child *pidAverages = (child *)malloc(children * sizeof(child));
    pid_t parentPID = getpid();
    int i = 0;
    child *flag = pidAverages;

    while (i < children){
        pid = fork();
        if (pid == 0){
            sleep(1);
            printf("Proceso hijo con PID = %d  Promedio es = %d\n", getpid(), (getpid() + parentPID) / 2);
            exit(0);
        }else if (pid == -1){
            flag->value = 0;
            printf("Hubo un error creando proces hijo. Numero de procesos hijos creados hasta ahora: %d\n", i + 1);
            break;
        }else{
            flag->value = pid;
            flag->average = rand() % 10;
        }        
        i++;
        flag++;
    }

    child *final = pidAverages + children;
    pid_t biggest = 0;

    for (flag = pidAverages; flag < final; ++flag){
        waitpid(flag->value, NULL, 0);
        if (flag->average > biggest){
            biggest = flag->average;
        }
    }
	/*
	PID Hijo	Promedio	Histograma
	20	20	********************
	10	15	**************
	50	35	*****************************
	60	40	***************************************/
    printf("PID Hijo\tPromedio\tHistograma\n");
    for (flag = pidAverages; flag < final; ++flag){
        printf("%d\t\t\t%d\t\t\t", flag->value, flag->average);
        showStarts(flag->average, biggest);
    }
    //Memoria Liberada
    free(pidAverages);
    return 0;
}

/* 
	10	15	***************/
void showStarts(int average, int biggestAverage){
    float size = (average * 10) /biggestAverage;
    for (int i = 0; i < size; ++i){
        printf("*");
    }
    printf("\n");
}