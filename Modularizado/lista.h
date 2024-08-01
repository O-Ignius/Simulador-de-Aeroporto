#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>

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

#define maxCombustivel 20
#define maxCreate 3
#define filaTam 20
#define combEmerg 2 //se o avião está na 1° posição da fila com 2 de combústivel e 3° pista está com espaço livre, ele é colocado nela para que na
                    //próxima iteração, ele pouse

//gera um número aleatório de combústivel entre min 1 e max 20
int geraCombustivel();

//gera um número aleatório de aviões entre min 0 e max 3
int geraQntAviao();

//cria o avião recebendo a iteração atual e o id e retorna-o
aviao criaAviao(int id, int iteracao);

//Operação de inserção em uma fila, insere no fim
void insertFifo(aviao av, lista *fifo);

//Operação de retirada em uma fila, retira do inicio e retorna o avião retirado
aviao retiraFifo(lista *fifo);

//Caso algum dos aviões na 1° posição de uma das filas de pouso estejam com 1 combustivel, coloca ele na pista 3 para um pouso de emergência
void pousoEmergencia(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3);

//Decide em qual das filas os aviões gerados para voar irão ser inseridos, a inserção ocorre de forma a tentar manter o tamanho das filas semelhante
void fifoVPista(aviao decolar, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3);

//Decide em qual das filas os aviões gerados para pousar irão ser inseridos, a inserção ocorre de forma a tentar manter o tamanho das filas semelhante
void fifoPPista(aviao pousar, lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3);

//usado somente caso queira deixar habilitado o pouso prioritário
int pousoPrioritario(lista *fifoPouso, int *pousadosQUASE);


#endif
