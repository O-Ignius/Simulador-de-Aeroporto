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
    int iteracaoCriacao;
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

//retorna 1 caso seja necessário efetuar o pouso, caso não, retorna 1
int pousoPrioritario(lista *fifoPouso, int *pousadosQUASE) {
    if (fifoPouso->first->item.combustivel == 1) {  //pouso é prioridade para avião não cair
        retiraFifo(fifoPouso);
        (*pousadosQUASE)++;
        return 1;
    }

    return 0;
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

void relatorioAvioes(int pousadosQUASE, int vooGerados, int pousadoGerados) {
    printf("\nAviões pousados sem reserva de combustível: \t\t%d\n", pousadosQUASE);
    printf("Aviões voando gerados nesta iteração: \t\t\t%d \nAviões prontos para decolar gerados nesta iteração: \t%d \n\n", vooGerados, pousadoGerados);
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

void relatorioGeral(int decolados, int pousados, int caidos, int iteracoes, int pousadoGerados, int vooGerados) {
    printf("\t\t!! RELATÓRIO FINAL !! \nTotal de aviões prontos para decolagem criados: \t%d\nTotal de aviões voando criados: \t\t\t%d \nTotal de aviões pousados: \t\t\t\t%d \nTotal de aviões decolados: \t\t\t\t%d \nTotal quedas de aviões: \t\t\t\t%d \nTotal de iterações: \t\t\t\t\t%d\n", pousadoGerados, vooGerados, pousados, decolados, caidos, (iteracoes - 2));
}

void centralControle(int iteracao, int *idP, int *idV, int *alternate, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, 
lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *caidos, int *pousadosQUASE, int *vooGerados, int *pousadoGerados) {
    int qntAV, pousadosIteracao, voandoIteracao;
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

    if (*alternate == 0) {  //momento para pousar os aviões voando
        pousar(fifoPP1, fifoPP2, fifoPP12, fifoPP22, fifoPP3, pousados, pousadosQUASE, fifoVP3, pousados);

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
    relatorioAvioes(*pousadosQUASE, voandoIteracao, pousadosIteracao);
}

int main () {
               //idP = ID dos pousados = pares | idV = ID dos voando = Impares
    int ite = 1, idP = 0, idV = 1, vooGerados = 0, pousadoGerados = 0;  //quantia de pousados = ((id / 2) + 1)      | quantia de voandos = ((id + 1) / 2)
    int decolados = 0, pousados = 0, caidos = 0, pousadosQUASE = 0, alternate = 0; //var alternate é usada para alternar entre voo e pouso, 0 para pouso e 1 para vôo
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
        centralControle(ite, &idP, &idV, &alternate, &fifoVP1, &fifoVP2, &fifoVP3, &decolados, &fifoPP1, &fifoPP2, &fifoPP12, &fifoPP22, fifoPP3, &pousados, &caidos, &pousadosQUASE, &vooGerados, &pousadoGerados);
    }

    relatorioGeral(decolados, pousados, caidos, ite, pousadoGerados, vooGerados);

    return 0;
}