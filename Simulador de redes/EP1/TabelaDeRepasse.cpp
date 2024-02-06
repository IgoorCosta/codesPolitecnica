#include "TabelaDeRepasse.h"

TabelaDeRepasse::TabelaDeRepasse() {
  adjacentes = new Roteador*[MAXIMO_TABELA];  /* cria��o da tabela */
  enderecos = new int[MAXIMO_TABELA];

  padrao = NULL;
}

TabelaDeRepasse::~TabelaDeRepasse() {
  delete [] adjacentes;
  delete [] enderecos;
}

bool TabelaDeRepasse::mapear(int endereco, Roteador* adjacente) {
  for (int i = 0; i < quantidadeDeAdjacentes; i++) /* verifica se endere�o ja est� na tabela */
    if (enderecos[i] == endereco) {
      adjacentes[i] = adjacente;
      return true;
    }
  if (quantidadeDeAdjacentes < MAXIMO_TABELA) {   /* mapeia novo endere�o */
    enderecos[quantidadeDeAdjacentes] = endereco;
    adjacentes[quantidadeDeAdjacentes] = adjacente;
    quantidadeDeAdjacentes += 1;
    return true;
  }
  return false;  /* tabela cheia */
}

Roteador** TabelaDeRepasse::getAdjacentes() {
  return adjacentes;
}

int TabelaDeRepasse::getQuantidadeDeAdjacentes() {
  return quantidadeDeAdjacentes;
}

void TabelaDeRepasse::setPadrao(Roteador* padrao) {
  this->padrao = padrao;
}

Roteador* TabelaDeRepasse::getDestino(int endereco) {
  for (int i = 0; i < quantidadeDeAdjacentes; i++) {
    if (endereco == enderecos[i])
      return adjacentes[i];
  }
  return padrao;
}

void TabelaDeRepasse::imprimir() {
}


