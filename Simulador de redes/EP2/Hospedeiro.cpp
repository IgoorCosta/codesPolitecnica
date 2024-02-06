#include "Hospedeiro.h"
#include "ServidorWeb.h"
#include "Navegador.h"
#include "Segmento.h"
#include "Datagrama.h"
#include "Fila.h"

Hospedeiro::Hospedeiro(int endereco, Roteador* gateway) : No(endereco), gateway(gateway) {
  processos = new vector<Processo*>();
}

Hospedeiro::~Hospedeiro(){
  delete processos;
}

void Hospedeiro::adicionarServidorWeb(int porta, string conteudo) {
  Processo* E = new ServidorWeb(endereco, porta, gateway, conteudo);
  vector<Processo*>::iterator i = processos->begin();
  while (i != processos->end()) {
    if((*i)->getPorta() == porta) throw new logic_error("Porta ocupada");
    i++;
  }
  processos->push_back(E);
}

void Hospedeiro::adicionarNavegador(int porta) {
  Processo* E = new Navegador(endereco, porta, gateway);
  vector<Processo*>::iterator i = processos->begin();
  while (i != processos->end()) {
    if((*i)->getPorta() == porta) throw new logic_error("Porta ocupada");
    i++;
  }
  processos->push_back(E);
}

Processo* Hospedeiro::getProcesso(int porta) {
  vector<Processo*>::iterator i = processos->begin();
  while (i != processos->end()) {
    if((*i)->getPorta() == porta) return (*i);
    i++;
  }
  return NULL;
}

vector<Processo*>* Hospedeiro::getProcessos() {
  return processos;
}

void Hospedeiro::processar() {
  if (getFila()->isEmpty() == 0) {
    cout << "Hospedeiro " << getEndereco() << endl;

    Datagrama* pacote;
    pacote = getFila()->dequeue();

    pacote->processar();

    if (getProcesso(pacote->getDado()->getPortaDeDestino()) == NULL) {
      cout << "Sem Destino: Origem: " << pacote->getOrigem() << ":" << pacote->getDado()->getPortaDeOrigem()
      << ", Destino: " << pacote->getDestino() << ":" << pacote->getDado()->getPortaDeDestino() <<
      ", TTL: " << pacote->getTtl() << ", " << pacote->getDado()->getDado() << endl;
      delete pacote;
    }

    else {
      Navegador *v1 = dynamic_cast<Navegador*>(getProcesso(pacote->getDado()->getPortaDeDestino()));
      if (v1 != NULL) { // Se ele for um Navegador
        v1->receber(pacote->getOrigem(),pacote->getDado());
      }
      else { // Se ele for um Servidor Web
        ServidorWeb *v2 = dynamic_cast<ServidorWeb*>(getProcesso(pacote->getDado()->getPortaDeDestino()));
        v2->receber(pacote->getOrigem(),pacote->getDado());
      }
      delete pacote;
    }
  }
}
