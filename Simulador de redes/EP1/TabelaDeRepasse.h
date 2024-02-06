#ifndef TABELADEREPASSE_H
#define TABELADEREPASSE_H

#include "Roteador.h"
using namespace std;

#define MAXIMO_TABELA 5

class Roteador;

class TabelaDeRepasse {
public:
  TabelaDeRepasse();
  ~TabelaDeRepasse();

  bool mapear(int endereco, Roteador* adjacente);
  Roteador** getAdjacentes();
  int getQuantidadeDeAdjacentes();

  void setPadrao(Roteador* padrao);

  Roteador* getDestino(int endereco);

  void imprimir();

private:
  int *enderecos;
  Roteador **adjacentes;
  Roteador *padrao;
  int quantidadeDeAdjacentes = 0;
};

#endif // TABELADEREPASSE_H
