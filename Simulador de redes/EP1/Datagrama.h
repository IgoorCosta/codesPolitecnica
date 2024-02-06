#ifndef DATAGRAMA_H
#define DATAGRAMA_H
#include <iostream>
using namespace std;

class Datagrama {
public:
  Datagrama(int origem, int destino, int ttl, string dado);
  ~Datagrama();
  int getOrigem();
  int getDestino();
  int getTtl();
  string getDado();

  void processar();
  bool ativo();

  void imprimir();

private:
  int origem;
  int destino;
  int ttl;
  string dado;
};

#endif // DATAGRAMA_H
