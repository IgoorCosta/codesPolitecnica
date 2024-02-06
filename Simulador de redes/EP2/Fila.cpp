#include "Fila.h"

Fila::Fila(int tamanho) : tamanho (tamanho) {
  fila = new Datagrama*[tamanho];
}

Fila::~Fila() {
  delete []fila;
}

void Fila::enqueue(Datagrama* d) {
  if (cont == tamanho)   /* overflow */
    throw new overflow_error("Fila cheia");
  cont += 1;
  fila[fim] = d;
  if (fim == tamanho)   /* fila circular */
    fim = 0;
  else fim += 1;
}

Datagrama* Fila::dequeue() {
  if (cont == 0)  /*underflow*/
    throw new underflow_error("Fila vazia");

  cont -= 1;
  if (inicio == tamanho) { /* fila circular */
    inicio = 0;
    return fila[tamanho];
  }

  else inicio += 1;
  return fila[inicio-1];
}

bool Fila::isEmpty() {
  if (cont == 0)
    return true;
  return false;
}

void Fila::imprimir() {
}
