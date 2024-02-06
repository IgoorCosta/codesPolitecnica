#ifndef NAVEGADOR_H
#define NAVEGADOR_H
#include "Processo.h"
#include "Roteador.h"
#include "Segmento.h"
#include <iostream>
#include <string>
using namespace std;

class Navegador : public Processo {
public:
  Navegador(int endereco, int porta, Roteador* gateway);
  virtual ~Navegador();

  virtual void abrir(int endereco, int porta);
  virtual void abrir(int endereco);

  virtual string getConteudo();

  void receber(int origem, Segmento* mensagem);

private:
  string ultima = "";
  int esperandoResposta = 0;
};

#endif // NAVEGADOR_H
