#ifndef RELATORIO_H
#define RELATORIO_H

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void printaPousoORDecolagem(int qual, aviao av);

//printa quantia de aviões gerados para pouso ou decolagem em cada iteração
void relatorioAvioes(int pousadosQUASE, int vooGerados, int pousadoGerados);

//printa o primeiro elemento de cada fila de voo
void relatórioFilasVoo(lista *fifoVP1, lista *fifoVP2, lista *fifoVP3, float tempM);

//printa o primeiro elemento de cada fila de aterrissagem
void relatórioFilasAterrissagem(lista *fifoPP1, lista *fifoPP2, lista *fifoPP12, lista *fifoPP22, float tempM);

//printa o total de aviões decolados, pousados e caidos
void relatorioDinam(int decolados, int pousados, int caidos);

//printa a quantia total de aviões pousados, caidos e decolados, a quantia total de iterações, pousos gerados e aviões gerados
void relatorioGeral(int decolados, int pousados, int caidos, int iteracoes, int pousadoGerados, int vooGerados, float tempMTOTALVOO, float tempMTOTALPOUSO);

#endif
