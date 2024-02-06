#include "Rede.h"
#include "Processo.h"
#include "Roteador.h"
#include "Fila.h"

Rede::Rede() {
  nos = new list<No*>();
}

Rede::~Rede() {
  delete nos;
}

void Rede::adicionar(No* no) {
  list<No*>::iterator i = nos->begin();
  while (i != nos->end()) {
    if((*i)->getEndereco() == no->getEndereco()) throw new logic_error("No ja existe");
    i++;
  }
  nos->push_back(no);
}

No* Rede::getNo(int endereco) {
  list<No*>::iterator i = nos->begin();
  while (i != nos->end()) {
    if((*i)->getEndereco() == endereco) return (*i);
    i++;
  }
  return NULL;
}

list<Hospedeiro*>* Rede::getHospedeiros() {
  list<Hospedeiro*>* hosp = new list<Hospedeiro*>();

  list<No*>::iterator i = nos->begin();
  while (i != nos->end()) {
    Hospedeiro* v = dynamic_cast<Hospedeiro*>(*i);
    if (v != NULL) {
      hosp->push_back(v);
    }
    i++;
  }
  return hosp;
}

void Rede::passarTempo() {
  list<No*>::iterator i = nos->begin();
  while (i != nos->end()) {
    (*i)->processar();
     i++;
  }
}

void Rede::imprimir() {
}
