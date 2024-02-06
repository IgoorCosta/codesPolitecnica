#ifndef REDE_H
#define REDE_H
#include "No.h"
#include "Hospedeiro.h"
#include "Fila.h"

#include <list>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;


class Rede {
public:
  Rede();
  virtual ~Rede();

  virtual void adicionar(No* no);
  virtual No* getNo(int endereco);
  virtual list<Hospedeiro*>* getHospedeiros();

  virtual void passarTempo();
  virtual void imprimir();

private:
  list<No*>* nos;
};

#endif // REDE_H
