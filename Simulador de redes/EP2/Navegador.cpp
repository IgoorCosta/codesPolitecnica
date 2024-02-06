#include "Navegador.h"

Navegador::Navegador(int endereco, int porta, Roteador* gateway) : Processo(endereco, porta, gateway) {
}

Navegador::~Navegador() {
}

void Navegador::abrir(int endereco, int porta) {
  Segmento* seg = new Segmento(getPorta(), porta, "GET");
  Datagrama* dat = new Datagrama(getEndereco(), endereco, getTtlPadrao(), seg);
  gateway->receber(dat);
  esperandoResposta = 1;
}

void Navegador::abrir(int endereco) {
  Segmento* seg = new Segmento(getPorta(), 80, "GET");
  Datagrama* dat = new Datagrama(getEndereco(), endereco, getTtlPadrao(), seg);
  gateway->receber(dat);
  esperandoResposta = 1;
}

string Navegador::getConteudo() {
  if (esperandoResposta == true)
    return ("");
  return ultima;
}

void Navegador::receber(int origem, Segmento* mensagem) {
  cout << "Navegador " << getPorta();

  if (esperandoResposta == true) {
    cout << "\n\tRecebido de " << origem << ":" << mensagem->getPortaDeOrigem()
        << ": " << mensagem->getDado();
    ultima = mensagem->getDado();
    esperandoResposta = 0;
  }
  else {
    cout << "\tMensagem Ignorada: " << origem << ":" << mensagem->getPortaDeOrigem()
    << ": " << mensagem->getDado();
  }
}
