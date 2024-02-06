#ifndef FILA_H
#define FILA_H
#include "Datagrama.h"
#include <stdexcept>
#include <iostream>
using namespace std;

class Fila {
public:
  Fila(int tamanho);
  virtual ~Fila();

  virtual void enqueue(Datagrama* d);
  virtual Datagrama* dequeue();
  virtual bool isEmpty();

  virtual void imprimir();

private:
  int tamanho;  /* tamanho do vetor fila */
  Datagrama **fila;
  int inicio = 0;  /* posi��o do proximo elemento a ser retirado */
  int fim = 0;    /* posi��o de inser��o do proximo elemento da fila */
  int cont = 0;  /* guarda o numero de espa�os do vetor que foram efetivamente utilizados */
};

#endif // FILA_H
