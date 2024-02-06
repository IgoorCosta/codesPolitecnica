#include "Datagrama.h"

Datagrama::Datagrama(int origem, int destino, int ttl, Segmento* dado) : origem(origem), destino(destino), ttl(ttl), dado(dado) {
}

Datagrama::~Datagrama() {
  delete dado;
}

int Datagrama::getOrigem() {
  return origem;
}

int Datagrama::getDestino() {
  return destino;
}

int Datagrama::getTtl() {
  return ttl;
}

Segmento* Datagrama::getDado() {
  return dado;
}

void Datagrama::processar() {
  ttl--;
}

bool Datagrama::ativo() {
  if (ttl > 0)
    return true;
  return false;
}

void Datagrama::imprimir() {
  dado->imprimir();
}
