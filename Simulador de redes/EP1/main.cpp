#include <iostream>
#include "Rede.h"

using namespace std;

int main() {
  /* Inicialização de roteadores e de vetor de roteadores: */

  Roteador* r1 = new Roteador(1);
  Roteador* r2 = new Roteador(2);
  Roteador* r3 = new Roteador(3);
  Roteador* r4 = new Roteador(4);
  Roteador* r5 = new Roteador(5);
  Roteador* r6 = new Roteador(6);
  Roteador *roteadores[6] {r1, r2, r3, r4, r5, r6};

  /* Criação da rede e mapeamento de roteadores: */

  Rede *rede = new Rede(roteadores, 6);
  r1->getTabela()->setPadrao(r2);
  r2->getTabela()->setPadrao(r5);
  r3->getTabela()->setPadrao(r2);
  r4->getTabela()->setPadrao(r5);
  r5->getTabela()->setPadrao(r2);
  r6->getTabela()->setPadrao(r5);
  r1->getTabela()->mapear(4, r4);
  r2->getTabela()->mapear(1, r1);
  r2->getTabela()->mapear(3, r3);
  r3->getTabela()->mapear(6, r6);
  r4->getTabela()->mapear(1, r1);
  r5->getTabela()->mapear(4, r4);
  r5->getTabela()->mapear(6, r6);
  r6->getTabela()->mapear(3, r3);

  /* Interface: */

  int op; /* opção escolhida pelo usuário */
  do {

    cout << "Simulador de Rede" << endl << "---" << endl << "1) Enviar um datagrama" << endl
    << "2) Passar tempo" << endl << "3) sair" << endl << "Escolha uma opcao: ";
    cin >> op;
    cout << endl;

    if (op == 1) {
      int origem, destino, ttl;
      string mensagem;
      cout << "Endereco do roteador de origem: ";
      cin >> origem;
      cout << "Endereco de destino: ";
      cin >> destino;
      cout << "TTL: ";
      cin >> ttl;
      cout << "Mensagem: ";
      cin >> mensagem;
      cout << endl;
      if (rede->getRoteador(origem) == NULL) /* retorna erro caso nenhum roteador da rede tenha sido mapeado para o endereço "origem" */
        cout << "Erro: origem desconhecida" << endl << endl;
      else {
        rede->enviar(mensagem, rede->getRoteador(origem), destino, ttl);
      }
    }

    if (op == 2) {
      int quantidade;
      cout << "Quantidade de tempo: ";
      cin >> quantidade;
      for (int tempo = 1; tempo <= quantidade; tempo++) {
        cout << endl << "Instante " << tempo << endl << "---" << endl;
        rede->passarTempo();
      }
      cout << endl;
    }

  } while (op != 3);


delete rede;
delete r1;
delete r2;
delete r3;
delete r4;
delete r5;
delete r6;
return 0;
}
