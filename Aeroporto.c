// Bibliotecas usadas
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> //time()

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
    pointer first, now;
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
        printf("O avião #%d acaba de decolar !!", av.id);
    }
    else {
        printf("O avião #%d pousou com %d combustivel!", av.id, av.combustivel);
    }
}

int geraCombustivel() { //gera um número aleatório de combústivel entre min 1 e max 20
    return((rand() % 20) + 1);
}

int geraQntAviao() {
    return(rand() % 3);
}

void insertEndLista(aviao av, lista *Lista) {
    pointer aux = Lista->first;

    //caso a lista esteja vazia aloca e insere na 1° posição
    if (Empt(*Lista) == 1) {
        Lista->first = malloc(sizeof(*Lista->first));
        Lista->first->item = av;
        Lista->first->prox = NULL;
        Lista->now = Lista->first;
    }
    else {
        //vai pra próxima célula enquanto não achar a ultima
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = malloc(sizeof(*Lista->first));  // aloca a ultima posição
        aux->prox->item = av;                       // guarda valores da struct
        aux->prox->prox = NULL;                     // faz a proxima posição apontar pra nulo, uma vez que ela será a ultima
    }
}

void insertFifo(lista *Lista, lista *fifo) {
    pointer aux = Lista->first;
    
    (fifo->tam)++;
    fifo->first = Lista->now;
    Lista->now = Lista->now->prox;
}

aviao retiraFifo(lista *fifo) {  
    pointer aux = fifo->first;
    aviao av = fifo->first->item; //se for preciso retornar o avião
    fifo->first = fifo->first->prox; //avança ponteiro para o próximo da fila;
    (fifo->tam)--;
    free(aux);
}

void listaPousados(lista *pousado, int *idP) {  //adiciona aviões a lista de pousados
    int qntGerada;
    aviao av;

    qntGerada = geraQntAviao();

    while (qntGerada > 0)
    {
        av.id = (*idP);
        (*idP)+= 2;
        av.combustivel = 20;

        insertEndLista(av, pousado);

        qntGerada--;
    }
}

void listaVoando(lista *voando, int *idV) { //adiciona aviões a lista de voandos
    int qntGerada;
    aviao av;

    qntGerada = geraQntAviao();

    while (qntGerada > 0)
    {
        av.id = (*idV);
        (*idV)+= 2;
        av.combustivel = geraCombustivel();

        insertEndLista(av, voando);

        qntGerada--;
    }
}

void fifoVPista(lista *decolar, lista *fifoVP1, lista *fifoVP2, lista *fifoVP3) {
    if (fifoVP3->tam < 20) {
        insertFifo(decolar, fifoVP3);
    }
    else if (fifoVP2->tam < 20) {
        insertFifo(decolar, fifoVP2);
    }
    else if (fifoVP1->tam < 20) {
        insertFifo(decolar, fifoVP1);
    }
    else {
        insertFifo(decolar, fifoVP3);   //possivel sobrecarga de posto na pista 3, mas ok pois ela dificilmente será usada para pouso!
    }
}

void fifoPPista(lista *pousar, lista *fifoPP1, lista *fifoPP2, lista *fifoPP3, lista *fifoPP12, lista *fifoPP22) {
    if (pousar->now->item.combustivel == 1) {
        if (fifoPP3 == NULL) {
            insertFifo(pousar, fifoPP3);
        }
    }
    else if (fifoPP1->tam < 20) {
        insertFifo(pousar, fifoPP1);
    }
    else if (fifoPP2->tam < 20) {
        insertFifo(pousar, fifoPP2);
    }
    else if (fifoPP22->tam < 20) {
        insertFifo(pousar, fifoPP22);
    }
    else {
        insertFifo(pousar, fifoPP12);      //possível sobrecarga de pouso na pista 1
    }
}

//conversar com denise sobre tal, pode implementar pesquisa em fifo?? provavelmente não!
void decidePousoDecolagem(lista *fifoPouso, lista *fifoDecol) {
    int pousoM = 0, i = 0;
    pointer aux = fifoPouso;

    while (i < 5) {

        i++;
    }    
}

void pista1() {
    
}

void pista2() {
    
}

void pista3(lista *fifoPP3, lista *fifoVP3, int *pousados, int *decolados) {
    aviao temp;

    if (fifoPP3 != NULL) {  //pouse imediatamente!
        temp = retiraFifo(fifoPP3); //retira o avião da fila única de pouso, ou seja, o pousa
        fifoPP3 = NULL;             //aponta fila pra nulo após retirada
        printaPousoORDecolagem(1, temp);
        (*pousados)++;              //aumenta contador de quantos aviões foram pousados
    }
    else {
        temp = retiraFifo(fifoVP3); //retira o 1° avião da fila de decolagem ou seja, decóla-o
        printaPousoORDecolagem(0, temp);
        (*decolados)++;             //aumenta contador de quantos aviões foram decolados
    }
}

int main () {
               //idP = ID dos pousados = pares | idV = ID dos voando = Impares
    int ite = 1, idP = 0, idV = 1;  //quantia de pousados = ((id / 2) + 1)      | quantia de voandos = ((id + 1) / 2)
    char op[2];

    lista pousado, voando; //lista base para aviões voando/pousados
    lista fifoVP1, fifoVP2, fifoVP3; //fifos para voos para cada pista
    lista fifoPP1, fifoPP2, fifoPP3, fifoPP12, fifoPP22; //fifos para pouso para cada pista

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
    }

    return 0;
}