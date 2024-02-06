#ifndef TABELADEREPASSE_H
#define TABELADEREPASSE_H
#include "No.h"
#include <stdexcept>
#include <iostream>
using namespace std;

class TabelaDeRepasse {
public:
  TabelaDeRepasse();
  virtual ~TabelaDeRepasse();

  virtual void mapear(int endereco, No* adjacente);
  virtual No **getAdjacentes();
  virtual int getQuantidadeDeAdjacentes();

  virtual void setPadrao(No* padrao);

  virtual No* getDestino(int endereco);

  virtual void imprimir();
  static const int MAXIMO_TABELA = 5;

private:
  int* enderecos;
  No** adjacentes;
  No* padrao;
  int quantidadeDeAdjacentes = 0;
};

#endif // TABELADEREPASSE_H
