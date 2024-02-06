#include "Segmento.h"

Segmento::Segmento(int portaDeOrigem, int portaDeDestino, string dado) : portaOrigem(portaDeOrigem), portaDestino(portaDeDestino), dado(dado) {
}

Segmento::~Segmento() {
}

int Segmento::getPortaDeOrigem() {
  return portaOrigem;
}

int Segmento::getPortaDeDestino() {
  return portaDestino;
}

string Segmento::getDado() {
  return dado;
}

void Segmento::imprimir() {
  cout << "Dado: " << dado << endl;
}
