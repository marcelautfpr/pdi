#ifndef DESCRITOR_H
#define DESCRITOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <chrono>
#include <set>

#include "dadosimagens.h"

using namespace std;
using namespace cv;

class Descritor
{
public:
    Descritor();
    vector<DadosImagens> getImages() const;
    void setImages(const vector<DadosImagens> &value);

    void iniciaDescritor(string s);
    void salvaDados(string path);
    void imprimeArff(string t, string save);
    void snns(string save);
    void svm(string save);
    void normaliza();

protected:
    vector<DadosImagens> images;
    string type;

    const vector<string> explode(const string &s, const char &c);

private:
    map<string,pair<double,double>> minMax;
    set<string> classes;
    set<string> atributos;

    void addClasse(string s);
    void addAtributo(string s);
    string arff_Titulo(string t);
    string arffDados();
    string arff_Atributos();
    void copyMap();
    double minimoMaximo(double x, double min, double max);
    string snnTitulo(int a, int c, int b);
    string snnsDados(int classes, map<string, string> mapClass, vector<DadosImagens> *base);
    void conjuntoTestes(set<string> *classes, vector<DadosImagens> *treino, vector<DadosImagens> *teste, vector<DadosImagens> *valida);
    void arqSvm(map<string, int> mapClass, vector<DadosImagens> treino, string save);
    void arqSnns(string train, const map<string,double> &atr, set<string> classes, map<string,int> mapClass, vector<DadosImagens> treino);
};

#endif // DESCRITOR_H
