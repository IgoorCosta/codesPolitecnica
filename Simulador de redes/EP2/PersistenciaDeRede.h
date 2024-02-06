#ifndef PERSISTENCIADEREDE_H
#define PERSISTENCIADEREDE_H

#include <string>
#include "Rede.h"

using namespace std;

class PersistenciaDeRede {
protected:

public:
    PersistenciaDeRede();
    virtual ~PersistenciaDeRede();

    virtual Rede* carregar(string arquivo);
};
#endif //PERSISTENCIADEREDE_H
