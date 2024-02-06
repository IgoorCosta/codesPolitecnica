#include "TabelaDeRepasse.h"

const int TabelaDeRepasse::MAXIMO_TABELA;

TabelaDeRepasse::TabelaDeRepasse() {
  adjacentes = new No*[TabelaDeRepasse::MAXIMO_TABELA];
  enderecos = new int[TabelaDeRepasse::MAXIMO_TABELA];
  padrao = NULL;
}

TabelaDeRepasse::~TabelaDeRepasse() {
  delete adjacentes;
  delete enderecos;
}

void TabelaDeRepasse::mapear(int endereco, No* adjacente) {
  for (int i = 0; i < quantidadeDeAdjacentes; i++) /* verifica se endereço ja está na tabela */
    if (enderecos[i] == endereco) {
      adjacentes[i] = adjacente;
      return;
    }
  if (quantidadeDeAdjacentes < MAXIMO_TABELA) {   /* mapeia novo endereço */
    enderecos[quantidadeDeAdjacentes] = endereco;
    adjacentes[quantidadeDeAdjacentes] = adjacente;
    quantidadeDeAdjacentes++;
  }
  else throw new overflow_error("Tabela cheia");
}

No** TabelaDeRepasse::getAdjacentes() {
  return adjacentes;
}

int TabelaDeRepasse::getQuantidadeDeAdjacentes() {
  return quantidadeDeAdjacentes;
}

void TabelaDeRepasse::setPadrao(No* padrao) {
  this->padrao = padrao;
}

No* TabelaDeRepasse::getDestino(int endereco) {
  for (int i = 0; i < quantidadeDeAdjacentes; i++) {
    if (enderecos[i] == endereco)
      return adjacentes[i];
  }
  return padrao;
}

void TabelaDeRepasse::imprimir() {
}
