// Bibliotecas usadas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> //time()

//bibliotecas do programa
#include "lista.h"
#include "controle.h"
#include "relatorio.h"


int main () {
               //idP = ID dos pousados = pares | idV = ID dos voando = Impares
    int ite = 1, idP = 0, idV = 1, vooGerados = 0, pousadoGerados = 0;  //quantia de pousados = ((id / 2) + 1)      | quantia de voandos = ((id + 1) / 2)
    int decolados = 0, pousados = 0, caidos = 0, pousadosQUASE = 0, alternate = 0; //var alternate é usada para alternar entre voo e pouso, 0 para pouso e 1 para vôo
    float tempMTOTALVOO = 0, tempMTOTALPOUSO = 0;
    char op[2];

    lista fifoVP1, fifoVP2, fifoVP3; //fifos para voos para cada pista
    pointer fifoPP3 = NULL; //ponteiro para pouso de emergencia
    lista fifoPP1, fifoPP2, fifoPP12, fifoPP22; //fifos para pouso para cada pista

    //seedRandom
    srand(time(NULL));

    while (1)
    {
        printf("%d° iteração: \nPRESS ENTER TO START or c to QUIT !\n", ite);
        setbuf(stdin,NULL);
        read(STDIN_FILENO, op, 2);
        ite++;
        if (op[0] == 'c') {
            break;
        }
        centralControle(ite, &idP, &idV, &alternate, &fifoVP1, &fifoVP2, &fifoVP3, &decolados, &fifoPP1, &fifoPP2, &fifoPP12, &fifoPP22, fifoPP3, &pousados, &caidos, &pousadosQUASE, &vooGerados, &pousadoGerados, &tempMTOTALVOO, & tempMTOTALPOUSO);
    }

    relatorioGeral(decolados, pousados, caidos, ite, pousadoGerados, vooGerados, (tempMTOTALVOO / decolados), (tempMTOTALPOUSO / pousados));

    return 0;
}
