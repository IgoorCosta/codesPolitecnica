#include "Rede.h"
#include "Datagrama.h"

Rede::Rede(Roteador** roteadores, int quantidadeDeRoteadores) : roteadores (roteadores), quantidadeDeRoteadores (quantidadeDeRoteadores) {
}

Rede::~Rede() {
}

Roteador* Rede::getRoteador(int endereco) {
  for (int i = 0; i < quantidadeDeRoteadores; i++) {
    if (roteadores[i]->getEndereco() == endereco)
      return roteadores[i];
  }
return NULL;  /* nenhum roteador com o endereço que foi requisitado */
}

void Rede::enviar(string texto, Roteador* origem, int destino, int ttl) {
  Datagrama *novoD = new Datagrama(origem->getEndereco(), destino, ttl, texto);
  origem->receber(novoD);
}

void Rede::passarTempo() {
  for (int i = 0; i < quantidadeDeRoteadores; i++) /* executa método processar para todos os roteadores em ordem */
    roteadores[i]->processar();
}

void Rede::imprimir() {
}
