#include "Roteador.h"
#include <iostream>

Roteador::Roteador(int endereco) : endereco (endereco) {
  tabela = new TabelaDeRepasse();
  fila = new Fila(TAMANHO_FILA);
}

Roteador::~Roteador() {
  delete tabela;
  delete fila;
}

TabelaDeRepasse* Roteador::getTabela() {
  return tabela;
}

Fila* Roteador::getFila() {
  return fila;
}

int Roteador::getEndereco() {
  return endereco;
}

void Roteador::receber(Datagrama* d) {
  if (!fila->enqueue(d))
    cout << "\tFila em " << endereco << " estourou" << endl << endl;   /* fila cheia */
}

void Roteador::processar() {
  if (fila->isEmpty() == false) {
    cout << "Roteador " << getEndereco() << endl;

    Datagrama *pacote = fila->dequeue();  /* guarda o datagrama que esta sendo processado */
    int destinoP = pacote->getDestino();  /* guarda destino do pacote */
    pacote->processar();

    if (pacote->ativo() == false) {   /* datagrama inativo */
      cout << "\tDestruido por TTL: Origem: " << pacote->getOrigem() << ", Destino: " << destinoP << ", TTL: "
      << pacote->getTtl() << ", " << pacote->getDado() << endl;
      delete pacote;
    }

    else if (destinoP == endereco) {   /* datagrama chegou ao destino */
      ultimoDadoRecebido = pacote->getDado();
      delete pacote;
      cout << "\tRecebido: " << ultimoDadoRecebido << endl;
    }

    else if (tabela->getDestino(destinoP) == NULL)   /* caso não tenha sido definido roteador padrão */
      delete pacote;
    else {
      tabela->getDestino(destinoP)->receber(pacote);   /* envia datagrama a roteador adjacente ou ao padrão */
      cout << "\tEnviado para " << tabela->getDestino(destinoP)->getEndereco() << " Origem: " << pacote->getOrigem()
      << ", Destino: " << destinoP << ", TTL: " << pacote->getTtl() << ", " << pacote->getDado() << endl;
    }
  }
}

string Roteador::getUltimoDadoRecebido() {
  return ultimoDadoRecebido;
}

void Roteador::imprimir() {
}
