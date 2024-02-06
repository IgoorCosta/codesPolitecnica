#include "Roteador.h"
#include "No.h"
#include "Datagrama.h"
#include "Fila.h"

Roteador::Roteador(int endereco) : No(endereco), endereco(endereco) {
  tabela = new TabelaDeRepasse();
}

Roteador::~Roteador() {
  delete tabela;
}

TabelaDeRepasse* Roteador::getTabela() {
  return tabela;
}

void Roteador::processar() {
  if (getFila()->isEmpty() == 0) {
    cout << "Roteador " << getEndereco() << endl;

    Datagrama* pacote = getFila()->dequeue(); /* guarda o datagrama que esta sendo processado */
    int destinoP = pacote->getDestino();  /* guarda destino do pacote */

    pacote->processar();

    if (pacote->ativo() == 0) {   /* datagrama inativo */
      cout << "\tDestruido por TTL: Origem: " << pacote->getOrigem() << ":" << pacote->getDado()->getPortaDeOrigem()
      << ", Destino: " << destinoP << ":" << pacote->getDado()->getPortaDeDestino() << ", TTL: "
      << pacote->getTtl() << ", " << pacote->getDado()->getDado() << endl;
      delete pacote;
    }

    else if (destinoP == getEndereco()) {   /* datagrama chegou ao destino */
      cout << "\tRecebido: Origem: " << pacote->getOrigem()
      << ":" << pacote->getDado()->getPortaDeOrigem() << ", Destino: " << destinoP << ":" << pacote->getDado()->getPortaDeDestino() <<
      ", TTL: " << pacote->getTtl() << ", " << pacote->getDado()->getDado() << endl;
      delete pacote;
    }

    else if (tabela->getDestino(destinoP) == NULL)   /* caso não tenha sido definido roteador padrão */
      delete pacote;

    else {
      (tabela->getDestino(destinoP))->receber(pacote);

      cout << "\tEnviado para " << tabela->getDestino(destinoP)->getEndereco() << ": Origem: " << pacote->getOrigem()
      << ":" << pacote->getDado()->getPortaDeOrigem() << ", Destino: " << destinoP << ":" << pacote->getDado()->getPortaDeDestino() <<
      ", TTL: " << pacote->getTtl() << ", " << pacote->getDado()->getDado() << endl;
    }
  }
}
