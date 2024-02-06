#include "ServidorWeb.h"

ServidorWeb::ServidorWeb(int endereco, int porta, Roteador* gateway, string conteudo) : Processo(endereco, porta, gateway), conteudo(conteudo) {
}

ServidorWeb::~ServidorWeb() {
}

void ServidorWeb::receber(int origem, Segmento* mensagem) {
  Segmento * seg = new Segmento(porta, mensagem->getPortaDeOrigem(), conteudo);
  Datagrama* dat = new Datagrama(endereco, origem, ttlPadrao, seg);
  cout << "Servidor Web " << getPorta() << "\n\tDevolvendo mensagem para: "
       << origem << ":" << mensagem->getPortaDeOrigem();
  gateway->receber(dat);
}
