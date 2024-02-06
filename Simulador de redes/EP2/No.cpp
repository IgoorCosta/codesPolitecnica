#include "No.h"

const int No::TAMANHO_FILA;

No::No(int endereco) : endereco(endereco) {
  fila = new Fila(No::TAMANHO_FILA);
}

No::~No() {
  delete fila;
}

Fila* No::getFila() {
  return fila;
}

int No::getEndereco() {
  return endereco;
}

void No::receber(Datagrama* d) {
  fila->enqueue(d);
}

void No::imprimir() {
}
