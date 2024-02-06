#ifndef HOSPEDEIRO_H
#define HOSPEDEIRO_H

#include "No.h"
#include "Processo.h"
#include "Roteador.h"

#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

class Hospedeiro : public No {
public:
  Hospedeiro(int endereco, Roteador* gateway);
  virtual ~Hospedeiro();

  virtual void adicionarServidorWeb(int porta, string conteudo);
  virtual void adicionarNavegador(int porta);

  virtual Processo* getProcesso(int porta);
  virtual vector<Processo*>* getProcessos();

  void processar();

private:
  Roteador* gateway;
  vector<Processo*>* processos;
};

#endif // HOSPEDEIRO_H
