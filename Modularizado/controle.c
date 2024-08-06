#include "controle.h"

//retorna 1 caso tenha realizado pouso de emergência e 0 caso não
int pousarEmerg(pointer fifoPP3, int *pousados, int *pousadosQUASE) {
    if (fifoPP3 != NULL) {
        free(fifoPP3);  //libera ponteiro
        fifoPP3 = NULL; //aponta pra nulo
        (*pousados)++;   //incrementa quantia de aviões pousados
        (*pousadosQUASE)++;
        return 1;
    }
    return 0;
}

void voar(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, pointer fifoPP3, int *pousados, int *pousadosQUASE /* lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22 */) {
    if (pousarEmerg(fifoPP3, pousados, pousadosQUASE) == 0) {  //se não houve pousos de emergência, decole!
        if (fifoVP3->first != NULL) {
            retiraFifo(fifoVP3);
            (*decolados)++;
        }
    }

    /*
        //primeira pista
        int auxP1 = 0, auxP2 = 0;
        if (fifoPP1->tam > fifoPP12->tam) {//verifica qual das duas é a maior
            //verifica se é preciso pouso de emergência
            if (pousoPrioritario(fifoPP1, pousadosQUASE) == 0) {
                ;
            }
            else {
                auxP1++;
            }
        }
        else {
            //verifica se é preciso pouso de emergência
            if (pousoPrioritario(fifoPP12, pousadosQUASE) == 0) {
                ;
            }
            else {
                auxP1++;
            }
        }
        if (auxP1 == 0) {   //se não houve pousos de emergencia, decole
            if (fifoVP1->first !=NULL) {
                retiraFifo(fifoVP1);
                (*decolados)++;
            }
        }
    
        //pista 2
        if (fifoPP2->tam > fifoPP22->tam) {//verifica qual das duas é a maior
            //verifica se é preciso pouso de emergência
            if (pousoPrioritario(fifoPP2, pousadosQUASE) == 0) {
                ;
            }
            else {
                auxP2++;
            }
        }
        else {
            //verifica se é preciso pouso de emergência
            if (pousoPrioritario(fifoPP22, pousadosQUASE) == 0) {
                ;
            }
            else {
                auxP2++;
            }
        }
        if (auxP2 == 0) {   //se não houve pousos de emergencia, decole
            if (fifoVP2->first !=NULL) {
                retiraFifo(fifoVP2);
                (*decolados)++;
            }
        }
    */

    //primeira pista
    if (fifoVP1->first !=NULL) {
        retiraFifo(fifoVP1);
        (*decolados)++;
    }
    if (fifoVP2->first !=NULL) {
        retiraFifo(fifoVP2);
        (*decolados)++;
    }
}

void pousar(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *pousadosQUASE, lista *fifoVP3, int *decolados) {
    if (pousarEmerg(fifoPP3, pousados, pousadosQUASE) == 0) {  //se não houve pousos de emergência, decole!
        if (fifoVP3->first != NULL) {
            retiraFifo(fifoVP3);
            (*decolados)++;
        }
    }

    //primeira pista, verifica qual tem maior necessidade de pouso entre as filas da pista, caso nenhum esteja próximo de cair, retira da maior fila
    if ((fifoPP1->first != NULL) && (fifoPP1->first->item.combustivel < combEmerg)) {
        retiraFifo(fifoPP1);
        (*pousados)++;
        (*pousadosQUASE)++;
    }
    else if ((fifoPP12->first != NULL) && (fifoPP12->first->item.combustivel < combEmerg)) {
        retiraFifo(fifoPP12);
        (*pousados)++;
        (*pousadosQUASE)++;
    }
    else if ((fifoPP1->first != NULL) && (fifoPP12->first != NULL) && (fifoPP1->tam >= fifoPP12->tam)) {
        retiraFifo(fifoPP1);
        (*pousados)++;
    }
    else if ((fifoPP12->first != NULL)) {
        retiraFifo(fifoPP12);
        (*pousados)++;
    }

    //segunda pista
    if ((fifoPP2->first != NULL) && (fifoPP2->first->item.combustivel < combEmerg)) {
        retiraFifo(fifoPP2);
        (*pousados)++;
        (*pousadosQUASE)++;
    }
    else if ((fifoPP22->first != NULL) && (fifoPP22->first->item.combustivel < combEmerg)) {
        retiraFifo(fifoPP22);
        (*pousados)++;
        (*pousadosQUASE)++;
    }
    else if ((fifoPP2->first != NULL) && (fifoPP22->first != NULL) && (fifoPP2 >= fifoPP22)) {   //se fila 1 da 2° pista for menor, insere nela
        retiraFifo(fifoPP2);
        (*pousados)++;
    }
    else if ((fifoPP22->first != NULL)) {
        retiraFifo(fifoPP22);
        (*pousados)++;
    }
}

void diminuiCombustivel(lista *fifo, int iteraAtual, int *caidos) {
    if (fifo->first != NULL) {
        if (iteraAtual > fifo->first->item.iteracaoCriacao) {   //se não tiver sido criado na mesma iteração que a atual
            if ((fifo->first->item.combustivel - (iteraAtual - fifo->first->item.iteracaoCriacao)) >= 1) {
                fifo->first->item.combustivel = fifo->first->item.combustivel - (iteraAtual - fifo->first->item.iteracaoCriacao);
            }
            else {  //se o primeiro item da fifo não ter combustivel maior ou igual a 1, ele é retirado, pois o avião caiu
                while ((fifo->first->item.combustivel - (iteraAtual - fifo->first->item.iteracaoCriacao)) < 1)
                {
                    retiraFifo(fifo);
                    (*caidos)++;
                    diminuiCombustivel(fifo, iteraAtual, caidos);
                    return;
                }
                
            }
        }
    }
}

void verificaCaidos(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, int iteraAtual, int *caidos) {
    //fila 1 | 1° pista
    diminuiCombustivel(fifoPP1, iteraAtual, caidos);

    //fila 2 | 1° pista
    diminuiCombustivel(fifoPP12, iteraAtual, caidos);

    //fila 1 | 2° pista
    diminuiCombustivel(fifoPP2, iteraAtual, caidos);

    //fila 2 | 2° pista
    diminuiCombustivel(fifoPP22, iteraAtual, caidos);
}

int retornaTempoFila(lista FIFO, int iteracaoAtual) {
    if (FIFO.first != NULL) {
        return((iteracaoAtual - FIFO.first->item.iteracaoCriacao));
    }
    return 0;
}

void centralControle(int iteracao, int *idP, int *idV, int *alternate, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, 
lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *caidos, int *pousadosQUASE, 
int *vooGerados, int *pousadoGerados, float *tempMTOTALVOO, float *tempMTOTALPOUSO) {
    int qntAV, pousadosIteracao, voandoIteracao;
    float tempMedioVOO = 0, tempMedioPOUSO = 0;
    aviao auxAV;
    
    //cria aviões pousados prontos para decolar
    qntAV = geraQntAviao();
    pousadosIteracao = qntAV;
    (*pousadoGerados) += qntAV;
    while (qntAV > 0)
    {
        auxAV = criaAviao(*idP, iteracao);                        //adiciona coisas ao avião, como combustivel e ID
        (*idP) += 2;                                    // incrementa o ID usado para os aviões que estão pousados

        fifoVPista(auxAV, fifoVP1, fifoVP2, fifoVP3);   //adiciona avião em uma das pistas

        qntAV--;
    }

    //cria aviões voando, preparados para pousar
    qntAV = geraQntAviao();
    voandoIteracao = qntAV;
    (*vooGerados) += qntAV;
    while (qntAV > 0)
    {
        auxAV = criaAviao(*idV, iteracao);                        //adiciona coisas ao avião, como combustivel e ID
        (*idV) += 2;                                    // incrementa o ID usado para os aviões que estão pousados

        fifoPPista(auxAV, fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3);   //adiciona avião em uma das pistas

        qntAV--;
    }

    verificaCaidos(fifoPP1, fifoPP2, fifoPP12, fifoPP22, iteracao, caidos);   //verifica se o primeiro caiu, se sim, retira ele e todos seguintes que cairam

    //tempo medio
    //filas decolagem
    tempMedioVOO += retornaTempoFila(*fifoVP1, iteracao);
    tempMedioVOO += retornaTempoFila(*fifoVP2, iteracao);
    tempMedioVOO += retornaTempoFila(*fifoVP3, iteracao);
    (*tempMTOTALVOO) += tempMedioVOO;

    //filas pouso
    tempMedioPOUSO += retornaTempoFila(*fifoPP1, iteracao);
    tempMedioPOUSO += retornaTempoFila(*fifoPP2, iteracao);
    tempMedioPOUSO += retornaTempoFila(*fifoPP12, iteracao);
    tempMedioPOUSO += retornaTempoFila(*fifoPP22, iteracao);
    (*tempMTOTALPOUSO) += tempMedioPOUSO;
    //fim tempo médio

    if (*alternate == 0) {  //momento para pousar os aviões voando
        pousar(fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3, pousados, pousadosQUASE, fifoVP3, pousados);

        *alternate = 1;
    }
    else {                  //momento de decolar aviões
        voar(fifoVP1, fifoVP2, fifoVP3, decolados, fifoPP3, pousados, pousadosQUASE);

        *alternate = 0;
    }

    printf("\t\t!! RELATÓRIO PARCIAL !!\n");
    relatórioFilasVoo(fifoVP1, fifoVP2, fifoVP3, (tempMedioVOO / 3));
    relatórioFilasAterrissagem(fifoPP1, fifoPP2, fifoPP12, fifoPP22, (tempMedioPOUSO / 4));
    relatorioDinam(*decolados, *pousados, *caidos);
    relatorioAvioes(*pousadosQUASE, voandoIteracao, pousadosIteracao);
}


