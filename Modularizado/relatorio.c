#include "relatorio.h"

void printaPousoORDecolagem(int qual, aviao av) { //qual == 0 = decolagem | qual == 1 = pouso
    if (qual == 0) {
        printf("O avião #%0.0f acaba de decolar !!\n", av.id);
    }
    else {
        printf("O avião #%0.0f pousou com %d combustivel!\n", av.id, av.combustivel);
    }
}


void relatorioAvioes(int pousadosQUASE, int vooGerados, int pousadoGerados) {
    printf("\nAviões pousados sem reserva de combustível: \t\t%d\n", pousadosQUASE);
    printf("Aviões voando gerados nesta iteração: \t\t\t%d \nAviões prontos para decolar gerados nesta iteração: \t%d \n\n", vooGerados, pousadoGerados);
}

void relatórioFilasVoo(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, float tempM) {
    int control = 0;

    printf("\n\t\t!! FILAS DE VOO !!    Tempo médio de fila: %0.2f\n", tempM);
    if (fifoVP1->first != NULL) {
        printf("Fila 1 (Pista 1)     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoVP1->tam, fifoVP1->first->item.id, fifoVP1->first->item.combustivel);
        control++;
    }
    if (fifoVP2->first != NULL) {
        printf("Fila 1 (Pista 2)     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoVP2->tam, fifoVP2->first->item.id, fifoVP2->first->item.combustivel);
        control++;
    }
    if (fifoVP3->first != NULL) {
        printf("Fila 1 (Pista 3)     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoVP3->tam, fifoVP3->first->item.id, fifoVP3->first->item.combustivel);
        control++;
    }
    if (control <= 0) {
        printf("\t!! Todas as filas vazias !!\n");
    }
}

void relatórioFilasAterrissagem(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, float tempM) {
    int control = 0;

    printf("\n\t\t!! FILAS DE POUSO !!    Tempo médio de fila: %0.2f\n", tempM);
    if (fifoPP1->first != NULL) {
        printf("Fila 1 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP1->tam, fifoPP1->first->item.id, fifoPP1->first->item.combustivel);
        control++;
    }
    if (fifoPP12->first != NULL) {
        printf("Fila 2 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP12->tam, fifoPP12->first->item.id, fifoPP12->first->item.combustivel);
    }
    if (fifoPP2->first != NULL) {
        printf("Fila 1 (Pista 2):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP2->tam, fifoPP2->first->item.id, fifoPP2->first->item.combustivel);
        control++;
    }
    if (fifoPP22->first != NULL) {
        printf("Fila 2 (Pista 2):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP22->tam, fifoPP22->first->item.id, fifoPP22->first->item.combustivel);
    }
    if (control == 0) {
        printf("\t!! Todas as filas vazias !!\n");
    }
}

void relatorioDinam(int decolados, int pousados, int caidos) {
    printf("\nTotal de aviões pousados: \t%d \nTotal de aviões decolados: \t%d \nTotal quedas de aviões: \t%d \n", pousados, decolados, caidos);
}

void relatorioGeral(int decolados, int pousados, int caidos, int iteracoes, int pousadoGerados, int vooGerados, float tempMTOTALVOO, float tempMTOTALPOUSO) {
    printf("\t\t!! RELATÓRIO FINAL !! \nTotal de aviões prontos para decolagem criados: \t%d\nTotal de aviões voando criados: \t\t\t%d \nTotal de aviões pousados: \t\t\t\t%d \nTotal de aviões decolados: \t\t\t\t%d \nTotal quedas de aviões: \t\t\t\t%d \nTotal de iterações: \t\t\t\t\t%d\nTempo médio total das pistas de VOO: \t\t\t%0.2f\nTempo médio total das pistas de POUSO: \t\t\t%0.2f\n", pousadoGerados, vooGerados, pousados, decolados, caidos, (iteracoes - 2), tempMTOTALVOO, tempMTOTALPOUSO);
}

