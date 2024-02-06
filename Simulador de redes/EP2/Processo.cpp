#include "Processo.h"
#include "No.h"

int Processo::ttlPadrao = 5;

Processo::Processo(int endereco, int porta, Roteador* gateway) : endereco(endereco), porta(porta), gateway(gateway) {
}

Processo::~Processo() {
}

int Processo::getEndereco() {
  return endereco;
}

int Processo::getPorta() {
  return porta;
}

void Processo::imprimir() {
}

void Processo::setTtlPadrao(int padrao) {
  Processo::ttlPadrao = padrao;
}

int Processo::getTtlPadrao() {
  return Processo::ttlPadrao;
}
