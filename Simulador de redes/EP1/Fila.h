#ifndef FILA_H
#define FILA_H

#include "Datagrama.h"
using namespace std;

class Fila {
public:
  Fila(int tamanho);
  ~Fila();

  bool enqueue(Datagrama* d);
  Datagrama* dequeue();
  bool isEmpty();

  void imprimir();

private:
  int tamanho;  /* tamanho do vetor fila */
  Datagrama **fila;
  int inicio = 0;  /* posição do proximo elemento a ser retirado */
  int fim = 0;    /* posição de inserção do proximo elemento da fila */
  int cont = 0;  /* guarda o numero de espaços do vetor que foram efetivamente utilizados */
};

#endif // FILA_H
