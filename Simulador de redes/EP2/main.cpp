#include "Segmento.h"
#include "Datagrama.h"
#include "Fila.h"
#include "TabelaDeRepasse.h"
#include "No.h"
#include "Roteador.h"
#include "Processo.h"
#include "Rede.h"
#include "PersistenciaDeRede.h"
#include "Hospedeiro.h"
#include "Navegador.h"
#include "ServidorWeb.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

int main() {
  try {
    string arquivo;
    cout << "Digite o nome do arquivo: ";
    cin >> arquivo;

    PersistenciaDeRede* persistencia = new PersistenciaDeRede();
    Rede* rede = new Rede();
    rede = persistencia->carregar(arquivo);

    int opcao;
    do {
      cout << "\nSimulador de Rede" << endl << "---" << endl
           << "1) Usar um navegador" << endl << "2) Passar tempo" << endl
           << "3) Alterar TTL" << endl << "4) Sair" << endl
           << "Escolha uma opcao: ";
      cin >> opcao;
      cout << endl;

      if (opcao == 1) {
        list<Hospedeiro*>::iterator i = (rede->getHospedeiros())->begin();
        for (int x = 0; x < (rede->getHospedeiros())->size(); x++) {
          cout << "Hospedeiro " << (*i)->getEndereco() << endl;
          vector<Processo*>::iterator j = (*i)->getProcessos()->begin(); // ver se o vector está vazio
          while (j != (*i)->getProcessos()->end()) {
            Navegador *v1 = dynamic_cast<Navegador*>(*j);
            if (v1 != NULL) { // Se ele for um Navegador
              cout << "\tNavegador " << v1->getPorta() << endl;
            }
            else { // Se ele for um Servidor Web
              ServidorWeb *v2 = dynamic_cast<ServidorWeb*>(*j);
              cout << "\tServidor Web " << v2->getPorta() << endl;
            }
            j++;
          }
          i++;
        }

        int endereco, porta, pagina, servidor;
        cout << "\nDigite o endereco do hospedeiro: ";
        cin >> endereco;

        Hospedeiro *v1 = dynamic_cast<Hospedeiro*>(rede->getNo(endereco));
        if (v1 != NULL) {
          cout << "Digite a porta do navegador: ";
          cin >> porta;

          Navegador *n = dynamic_cast<Navegador*>(v1->getProcesso(porta));
          if (n != NULL) {
            cout << "Abrir pagina no endereco: ";
            cin >> pagina;
            cout << "Porta do Servidor Web: ";
            cin >> servidor;
            n->abrir(pagina, servidor);
          }
          else  cout << "Porta invalida\n";
        }
        else cout << "Endereco invalido\n";
      }

      else if (opcao == 2) {
        int tempo;

        cout << "\nQuantidade de Tempo: ";
        cin >> tempo;
        for (int t = 1; t <= tempo; t++) {
          cout << endl << "\nInstante " << t << endl << "---" << endl;
          rede->passarTempo();
        }
        cout << endl;
      }

      else if (opcao == 3) {
        int ttl;
        cout << "\nTTL atual: " << Processo::getTtlPadrao();
        cout << "\nNovo TTL: ";
        cin >> ttl;
        Processo::setTtlPadrao(ttl);
      }

      else if (opcao < 1 || opcao > 4) {
        cout << "Opcao invalida" << endl;
      }

    } while (opcao != 4);

    delete rede;
    delete persistencia;

  } catch (overflow_error* e1) {
    cout << "Overflow: " << e1->what() << endl;
    delete e1;
  } catch (underflow_error* e2) {
    cout << "Underflow: " << e2->what() << endl;
    delete e2;
  } catch (invalid_argument* e3) {
    cout << "Invalido: " << e3->what() << endl;
    delete e3;
  }
  return 0;
}
