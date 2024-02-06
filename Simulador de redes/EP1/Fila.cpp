#include "Fila.h"

Fila::Fila(int tamanho) : tamanho (tamanho) {
  fila = new Datagrama*[tamanho];
}

Fila::~Fila() {
  delete []fila;
}

bool Fila::enqueue(Datagrama* d) {
  if (cont == tamanho)  /* overflow */
    return false;
  cont += 1;
  fila[fim] = d;
  if (fim == tamanho)   /* fila circular */
    fim = 0;
  else fim += 1;

return true;
}

Datagrama* Fila::dequeue() {
  if (cont == 0)  /* underflow */
    return NULL;

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
