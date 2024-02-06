#include <fstream>
#include "PersistenciaDeRede.h"
#include "Roteador.h"
#include "Hospedeiro.h"
#include "ServidorWeb.h"
#include "Navegador.h"
#include "TabelaDeRepasse.h"
#include "Rede.h"

PersistenciaDeRede::PersistenciaDeRede() {
}

PersistenciaDeRede::~PersistenciaDeRede() {
}

Rede* PersistenciaDeRede::carregar(string arquivo) {
  ifstream entrada;
  entrada.open (arquivo.c_str());

  if (entrada.fail()) {
    entrada.close();
    throw new invalid_argument ("Arquivo nao encontrado");
  }

  int quantidadeRoteadores, endereco1;
  entrada >> quantidadeRoteadores;

  if (quantidadeRoteadores < 0) {
    throw new invalid_argument("Arquivo invalido");
    entrada.close();
  }

  Roteador** roteadores = new Roteador*[quantidadeRoteadores];

  for (int i = 0; i < quantidadeRoteadores; i++) {
    entrada >> endereco1;
    roteadores[i] = new Roteador(endereco1);
  }

  int quantidadeHospedeiros, endereco2, gateway, processos, porta;

  entrada >> quantidadeHospedeiros;
  Hospedeiro** hospedeiros = new Hospedeiro*[quantidadeHospedeiros];

  for (int i = 0; i < quantidadeHospedeiros; i++) {
    entrada >> endereco2;
    entrada >> gateway;
    entrada >> processos;

    Roteador* gatewayHosp = NULL;
    for (int j = 0; j < quantidadeRoteadores; j++) {
      if (roteadores[j]->getEndereco() == gateway) {
        gatewayHosp = roteadores[j];
      }
    }

    hospedeiros[i] = new Hospedeiro(endereco2, gatewayHosp);
    for (int k = 0; k < processos; k++) {
      char c;
      entrada >> c;
      entrada >> porta;
      if (c == 'n') {
        hospedeiros[i]->adicionarNavegador(porta);
      }
      else if (c == 'w') {
        string s1;
        entrada >> s1;
        hospedeiros[i]->adicionarServidorWeb(porta, s1);
      }
      else {
        entrada.close();
        throw new invalid_argument ("Arquivo invalido");
      }
    }
  }

  int quantidadeMapeamentos, rot, rotPadrao, endereco3, dest, cont;

  for (int i = 0 ; i < quantidadeRoteadores ; i++){
    entrada >> rot;
    entrada >> rotPadrao;
    entrada >> quantidadeMapeamentos;
    Roteador* rotMapeamento;
    // Analisar se existem os roteadores:

    for (int j = 0; j < quantidadeRoteadores; j++) {
      if (roteadores[j]->getEndereco() == rotPadrao) {
        rotMapeamento = roteadores[j];
      }
    }

    for (int k = 0; k < quantidadeRoteadores; k++) {
      if (roteadores[k]->getEndereco() == rot) {
        cont = k;
        roteadores[k]->getTabela()->setPadrao(rotMapeamento);
      }
    }

    for (int l = 0; l < quantidadeMapeamentos; l++) {
      entrada >> endereco3;
      entrada >> dest;

      for (int m = 0; m < quantidadeRoteadores; m++) {
        if (roteadores[m]->getEndereco() == dest) {
           roteadores[cont]->getTabela()->mapear(endereco3, roteadores[m]);
        }
      }

      for (int n = 0; n < quantidadeHospedeiros; n++) {
        if (hospedeiros[n]->getEndereco() == dest) {
          roteadores[cont]->getTabela()->mapear(endereco3, hospedeiros[n]);
        }
      }
    }
  }
  Rede* rede = new Rede();
  for (int x = 0; x < quantidadeRoteadores; x++) {
    rede->adicionar(roteadores[x]);
  }
  for (int y = 0; y < quantidadeHospedeiros; y++) {
    rede->adicionar(hospedeiros[y]);
  }
  entrada.close();
  return rede;
}
