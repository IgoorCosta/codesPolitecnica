#ifndef ROTEADOR_H
#define ROTEADOR_H
#include "TabelaDeRepasse.h"

#include <iostream>
using namespace std;

class Roteador : public No {
public:
  Roteador(int endereco);
  virtual ~Roteador();

  virtual TabelaDeRepasse* getTabela();

  void processar();

private:
  int endereco;
  TabelaDeRepasse* tabela;
};

#endif // ROTEADOR_H
