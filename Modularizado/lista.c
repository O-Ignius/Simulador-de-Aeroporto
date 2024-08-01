#include "lista.h"

//retorna 1 se lista vazia
int Empt (lista Lista) {
    if (Lista.first == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

int geraCombustivel() { //gera um número aleatório de combústivel entre min 1 e max 20
    return((rand() % maxCombustivel) + 1);
}

int geraQntAviao() {
    return(rand() % (maxCreate + 1));
}

aviao criaAviao(int id, int iteracao) {
    aviao av;
    
    if (id % 2 == 0) {  //avião pousado, tem ID par
        av.combustivel = maxCombustivel;
        av.id = id;
    }
    else {              //avião voando, tem ID impar
        av.id = id;
        av.combustivel = geraCombustivel();
    }
    av.iteracaoCriacao = iteracao;

    return (av);
}

//insere ao final
void insertFifo(aviao av, lista *fifo) {
    pointer aux = fifo->first;
    
    //caso a fila esteja vazia aloca e insere na 1° posição
    if (Empt(*fifo) == 1) {
        fifo->first = malloc(sizeof(fifo->first));
        fifo->first->item = av;
        fifo->first->prox = NULL;
    }
    else {
        //vai pra próxima célula enquanto não achar a ultima
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = malloc(sizeof(fifo->first));    // aloca a ultima posição
        aux->prox->item = av;                       // guarda valores da struct
        aux->prox->prox = NULL;                     // faz a proxima posição apontar pra nulo, uma vez que ela será a ultima
    }

    (fifo->tam)++;    
}

//retira da 1° posição
aviao retiraFifo(lista *fifo) {  
    pointer aux = fifo->first;
    aviao av = fifo->first->item; //se for preciso retornar o avião
    fifo->first = fifo->first->prox; //avança ponteiro para o próximo da fila;
    (fifo->tam)--;
    free(aux);
}

void fifoVPista(aviao decolar, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3) {    // adiciona as fifo's de voo
    //verifica qual fila é menor e coloca o avião nela:
    if (fifoVP3->tam < fifoVP2->tam && fifoVP3->tam < fifoVP1->tam) {       //fila 3 é a menor
        insertFifo(decolar, fifoVP3);
    }
    else if (fifoVP1->tam < fifoVP2->tam && fifoVP1->tam < fifoVP3->tam) {   //fila 1 é a menor
        insertFifo(decolar, fifoVP1);
    }
    else if (fifoVP2->tam < fifoVP1->tam && fifoVP2->tam < fifoVP3->tam) {   //fila 2 é a menor
        insertFifo(decolar, fifoVP2);
    }
    else if (fifoVP1->tam < fifoVP3->tam) {     //caso nenhuma delas seja a menor, ve se fila 1 é menor que 3
        insertFifo(decolar, fifoVP1);
    }
    else if (fifoVP2->tam < fifoVP3->tam) {     //caso nenhuma delas seja a menor, ve se fila 2 é menor que 3
        insertFifo(decolar, fifoVP2);
    }
    else {      //caso nenhuma seja a menor, adiciona a fila 3 pois é a com maior chance de ser rapidamente diminuida
        insertFifo(decolar, fifoVP3);
    }
}

void pousoEmergencia(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3) {
    //isso só ocorre caso fifoPP3 seja nulo, ou seja, já não tenha um avião
    //pista 1
    if ((fifoPP1->first != NULL) && (fifoPP1->first->item.combustivel == combEmerg)) {    //se primeira posição da 1° fila da 1° pista tem avião com 1 de combustível, avança fila e coloca apontador fifoPP3 para apontar pra esse avião
        if (fifoPP3 != NULL) {
            fifoPP3 = fifoPP1->first;
            fifoPP1->first = fifoPP1->first->prox;
        }
    }
    //pista 2
    else if ((fifoPP2->first != NULL) && (fifoPP2->first->item.combustivel == combEmerg)) {  //se primeira posição da 1° fila da 2° pista tem avião com 1 de combustível, avança fila e coloca apontador fifoPP3 para apontar pra esse avião
        if (fifoPP3 != NULL) {
            fifoPP3 = fifoPP2->first;
            fifoPP2->first = fifoPP2->first->prox;
        }
    }
    //pista 1
    else if ((fifoPP12->first != NULL) && (fifoPP12->first->item.combustivel == combEmerg)) {  //se primeira posição da 2° fila da 1° pista tem avião com 1 de combustível, avança fila e coloca apontador fifoPP3 para apontar pra esse avião
        if (fifoPP3 != NULL) {
            fifoPP3 = fifoPP12->first;
            fifoPP12->first = fifoPP12->first->prox;
        }
    }
    //pista 2
    else if ((fifoPP22->first != NULL) && (fifoPP22->first->item.combustivel == combEmerg)) {  //se primeira posição da 2° fila da 2° pista tem avião com 1 de combustível, avança fila e coloca apontador fifoPP3 para apontar pra esse avião
        if (fifoPP3 != NULL) {
            fifoPP3 = fifoPP22->first;
            fifoPP22->first = fifoPP22->first->prox;
        }
    }
}

void fifoPPista(aviao pousar, lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3) { //adiciona as fifo's de pouso
    //verifica qual das pistas possui menor fila
    if ((fifoPP1->tam + fifoPP12->tam) < (fifoPP2->tam + fifoPP22->tam)) {  //primeira pista tem a maior velocidade de pouso
        if (fifoPP1->tam < fifoPP12->tam) {   //se fila 1 da 1° pista for menor, insere nela
            insertFifo(pousar, fifoPP1);
        }
        else {
            insertFifo(pousar, fifoPP12);
        }
    }
    else {                                                                  //segunda pista é mais rápida
        if (fifoPP2->tam < fifoPP22->tam) {   //se fila 1 da 2° pista for menor, insere nela
            insertFifo(pousar, fifoPP2);
        }
        else {
            insertFifo(pousar, fifoPP22);
        }
    }
    
    //verifica se é necessário um pouso de emergência na próxima iteração
    pousoEmergencia(fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3);
}

//retorna 1 caso seja necessário efetuar o pouso, caso não, retorna 1
int pousoPrioritario(lista *fifoPouso, int *pousadosQUASE) {
    if (fifoPouso->first->item.combustivel == 1) {  //pouso é prioridade para avião não cair
        retiraFifo(fifoPouso);
        (*pousadosQUASE)++;
        return 1;
    }

    return 0;
}
