#ifndef LPB_H
#define LPB_H

#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>

#include "dadosimagens.h"
#include "descritor.h"

using namespace std;


class LPB : public Descritor
{
public:
    LPB();
    void geraDados(string path, string save);

private:
    string path;
    string save;
    bool pronto;

    void inicializaLBP(string path);
    void executeMatlab();
};

#endif // LPB_H
