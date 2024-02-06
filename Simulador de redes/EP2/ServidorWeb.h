#ifndef SERVIDORWEB_H
#define SERVIDORWEB_H
#include "Processo.h"
#include "Roteador.h"
#include <string>
#include <iostream>
using namespace std;


class ServidorWeb : public Processo {
public:
  ServidorWeb(int endereco, int porta, Roteador* gateway, string conteudo);
  virtual ~ServidorWeb();

  void receber(int origem, Segmento* mensagem);

private:
  string conteudo;
};

#endif // SERVIDORWEB_H
