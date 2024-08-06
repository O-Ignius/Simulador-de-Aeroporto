#ifndef CONTROLE_H
#define CONTROLE_H

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "relatorio.h"

//realiza pouso de emergencia caso necessário, retorna 1 caso pousou e 0 caso não fosse necessário
int pousarEmerg(pointer fifoPP3, int *pousados, int *pousadosQUASE);

//faz com que os primeiros aviões de cada pista voem, exceto caso a pista 3 tenha um pouso de emergência
//a parte comentada serve para caso tenha a necessidade de se priorizar pousos sobre os voos
void voar(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, pointer fifoPP3, int *pousados, int *pousadosQUASE /* lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22 */);

//pousa os aviões variante a necessidade, ou seja, caso um esteja quase caindo, caso não, pousa de acordo com a maior fila
//pousa aviões em pouso de emergência na pista 3, caso não haja, faz com que aviões possam voar na pista 3
void pousar(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *pousadosQUASE, lista *fifoVP3, int *decolados);

//recebe a fila e, com relação a iteração de criação do avião, remove a quantia de combustivel dele, caso a mesma chegue a zero, ele é retirado da fila
//aumenta-se o número de caidos e verifica se o próximo caiu ou não, caso sim, continua o ciclo, caso não, diminui seu combustivel
void diminuiCombustivel(lista *fifo, int iteraAtual, int *caidos);

//função para controle, onde basicamente separa os paramêtros de cada fila a serem passados para a função diminuiCombustivel
void verificaCaidos(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, int iteraAtual, int *caidos);

//função central do programa
void centralControle(int iteracao, int *idP, int *idV, int *alternate, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, int *decolados, 
lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, pointer fifoPP3, int *pousados, int *caidos, int *pousadosQUASE,
int *vooGerados, int *pousadoGerados, float *tempMTOTALVOO, float *tempMTOTALPOUSO);

#endif
