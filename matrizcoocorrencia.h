#ifndef MATRIZCOOCORRENCIA_H
#define MATRIZCOOCORRENCIA_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

#include "dadosimagens.h"
#include "descritor.h"

using namespace cv;
using namespace std;

class MatrizCooCorrencia : public Descritor
{
    std::string imgPath;
    double matriz[256][256];
    void zeraMatriz(double [256][256]);
    void geraMatriz(string image, int ang, int dist);

public:
    string str;
    MatrizCooCorrencia(int dist);

    double energia();
    double entropia();
    double contraste();
    double correlacao();
    double homogeneidade();
    double variancia_x();
    double variancia_y();
    void geraDados(vector<string> img, bool ener, bool entro, bool cont, bool corre, bool homo, bool vari);

private:
    string path;
    int distancia;
    vector<double> auxValores;
    vector<string> atributos;
    void norma_liza(int dist, Mat img);
    void geraDados(bool vari, string img, bool homo, bool entro, bool ener, bool cont, bool corre);
    string buscaClasse(string s);
    string retornaAtributo(string s, int i);
};

#endif // MATRIZCOOCORRENCIA_H
