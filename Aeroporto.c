// Bibliotecas usadas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> //time()

#define maxCombustivel 20
#define maxCreate 3
#define filaTam 20
#define combEmerg 2 //se o avião está na 1° posição da fila com 2 de combústivel e 3° pista está com espaço livre, ele é colocado nela para que na
                    //próxima iteração, ele pouse

typedef struct
{
    float id;
    int combustivel;
} aviao;

typedef struct celula *pointer;

typedef struct celula
{
    aviao item;
    pointer prox;
} celula;

typedef struct
{
    pointer first;
    int tam;
} lista;

int Empt (lista Lista) {
    if (Lista.first == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

void printaPousoORDecolagem(int qual, aviao av) { //qual == 0 = decolagem | qual == 1 = pouso
    if (qual == 0) {
        printf("O avião #%d acaba de decolar !!\n", av.id);
    }
    else {
        printf("O avião #%d pousou com %d combustivel!\n", av.id, av.combustivel);
    }
}

int geraCombustivel() { //gera um número aleatório de combústivel entre min 1 e max 20
    return((rand() % maxCombustivel) + 1);
}

int geraQntAviao() {
    return(rand() % (maxCreate + 1));
}

aviao criaAviao(int id) {
    aviao av;
    
    if (id % 2 == 0) {  //avião pousado, tem ID par
        av.combustivel = maxCombustivel;
        av.id = id;
    }
    else {              //avião voando, tem ID impar
        av.id = id;
        av.combustivel = geraCombustivel();
    }

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

void voar(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, pointer fifoPP3, int *pousados, int *pousadosQUASE) {
    if (pousarEmerg(fifoPP3, pousados, pousadosQUASE) == 0) {  //se não houve pousos de emergência, decole!
        if (fifoVP3->first != NULL) {
            retiraFifo(fifoVP3);
            (*decolados)++;
        }
    }

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

void pousar(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *pousadosQUASE) {
    //verifica se há pousos de emergência
    pousarEmerg(fifoPP3, pousados, pousadosQUASE);

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

void verificaCaidos(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, int *caidos) {
    if (fifoPP1->first != NULL) {
        while (fifoPP1->first->item.combustivel < 1)   //retira da 1° fila da primeira pista os caidos
        {
            retiraFifo(fifoPP1);
            (*caidos)++;
        }
    }

    if (fifoPP12->first != NULL) {
        while (fifoPP12->first->item.combustivel < 1)   //retira da 2° fila da primeira pista os caidos
        {
            retiraFifo(fifoPP12);
            (*caidos)++;
        }
    }
    
    if (fifoPP2->first != NULL) {
        while (fifoPP2->first->item.combustivel < 1)   //retira da 1° fila da segunda pista os caidos
        {
            retiraFifo(fifoPP2);
            (*caidos)++;
        }
    }

    if (fifoPP22->first != NULL) {
        while (fifoPP22->first->item.combustivel < 1)   //retira da 2° fila da segunda pista os caidos
        {
            retiraFifo(fifoPP22);
            (*caidos)++;
        }
    }
}

void avioesPousadosSReserva(int pousadosQUASE) {
    printf("\nAviões pousados sem reserva de combustível: \t%d\n\n", pousadosQUASE);
}

void relatórioFilasVoo(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3) {
    int control = 0;

    printf("\n\t\t!! FILAS DE VOO !!\n");
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

void relatórioFilasAterrissagem(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22) {
    int control = 0;

    printf("\n\t\t!! FILAS DE POUSO !!\n");
    if (fifoPP1->first != NULL) {
        printf("Fila 1 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP1->tam, fifoPP1->first->item.id, fifoPP1->first->item.combustivel);
        control++;
    }
    if (fifoPP12->first != NULL) {
        printf("Fila 2 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP12->tam, fifoPP12->first->item.id, fifoPP12->first->item.combustivel);
    }
    if (fifoPP2->first != NULL) {
        printf("Fila 1 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP2->tam, fifoPP2->first->item.id, fifoPP2->first->item.combustivel);
        control++;
    }
    if (fifoPP22->first != NULL) {
        printf("Fila 2 (Pista 1):     Tempo médio de decolagem: %d iterações \n\tid do avião: %0.0f | combustível: %d \n", fifoPP22->tam, fifoPP22->first->item.id, fifoPP22->first->item.combustivel);
    }
    if (control == 0) {
        printf("\t!! Todas as filas vazias !!\n");
    }
}

void relatorioDinam(int decolados, int pousados, int caidos) {
    printf("\nTotal de aviões pousados: \t%d \nTotal de aviões decolados: \t%d \nTotal quedas de aviões: \t%d \n", pousados, decolados, caidos);
}

void relatorioGeral(int decolados, int pousados, int caidos, int iteracoes) {
    printf("\t\t!! RELATÓRIO FINAL !! \nTotal de aviões pousados: \t%d \nTotal de aviões decolados: \t%d \nTotal quedas de aviões: \t%d \nTotal de iterações: \t%d\n", pousados, decolados, caidos, (iteracoes - 1));
}

void centralControle(int *idP, int *idV, int *alternate, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, 
lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *caidos, int *pousadosQUASE) {
    int qntAV;
    aviao auxAV;
    
    //cria aviões pousados prontos para decolar
    qntAV = geraQntAviao();
    while (qntAV > 0)
    {
        auxAV = criaAviao(*idP);                        //adiciona coisas ao avião, como combustivel e ID
        (*idP) += 2;                                    // incrementa o ID usado para os aviões que estão pousados

        fifoVPista(auxAV, fifoVP1, fifoVP2, fifoVP3);   //adiciona avião em uma das pistas

        qntAV--;
    }

    //cria aviões voando, preparados para pousar
    qntAV = geraQntAviao();
    while (qntAV > 0)
    {
        auxAV = criaAviao(*idV);                        //adiciona coisas ao avião, como combustivel e ID
        (*idV) += 2;                                    // incrementa o ID usado para os aviões que estão pousados

        fifoPPista(auxAV, fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3);   //adiciona avião em uma das pistas

        qntAV--;
    }

    verificaCaidos(fifoPP1, fifoPP2, fifoPP12, fifoPP22, caidos);   //verifica se o primeiro caiu, se sim, retira ele e todos seguintes que cairam

    if (*alternate == 0) {  //momento para pousar os aviões voando
        pousar(fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3, pousados, pousadosQUASE);

        *alternate = 1;
    }
    else {                  //momento de decolar aviões
        voar(fifoVP1, fifoVP2, fifoVP3, decolados, fifoPP3, pousados, pousadosQUASE);

        *alternate = 0;
    }

    printf("\t\t!! RELATÓRIO PARCIAL !!\n");
    relatórioFilasVoo(fifoVP1, fifoVP2, fifoVP3);
    relatórioFilasAterrissagem(fifoPP1, fifoPP2, fifoPP12, fifoPP22);
    relatorioDinam(*decolados, *pousados, *caidos);
    avioesPousadosSReserva(*pousadosQUASE);
}

int main () {
               //idP = ID dos pousados = pares | idV = ID dos voando = Impares
    int ite = 1, idP = 0, idV = 1;  //quantia de pousados = ((id / 2) + 1)      | quantia de voandos = ((id + 1) / 2)
    int decolados = 0, pousados = 0, caidos = 0, pousadosQUASE = 0, alternate = 0; //var alternate é usada para alternar entre voo e pouso, 0 para pouso e 1 para vôo
    char op[2];

    lista fifoVP1, fifoVP2, fifoVP3; //fifos para voos para cada pista
    pointer fifoPP3 = NULL; //ponteiro para pouso de emergencia
    lista fifoPP1, fifoPP2, fifoPP12, fifoPP22; //fifos para pouso para cada pista

    //seedRandom
    srand(time(NULL));

    while (1)
    {
        printf("%d° iteração: \nPRESS ENTER TO START !\n", ite);
        setbuf(stdin,NULL);
        read(STDIN_FILENO, op, 2);
        ite++;
        if (op[0] == 'c') {
            break;
        }
        centralControle(&idP, &idV, &alternate, &fifoVP1, &fifoVP2, &fifoVP3, &decolados, &fifoPP1, &fifoPP2, &fifoPP12, &fifoPP22, fifoPP3, &pousados, &caidos, &pousadosQUASE);
    }

    relatorioGeral(decolados, pousados, caidos, ite);

    return 0;
}