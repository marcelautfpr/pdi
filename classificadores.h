#ifndef CLASSIFICADORES_H
#define CLASSIFICADORES_H

#include "dadosimagens.h"
#include "perceptron.h"
#include "knn.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include <map>

using namespace std;


class Classificadores
{
public:
    Classificadores();
    Classificadores(vector<DadosImagens> images);
    Classificadores(string s);

    vector<DadosImagens> getImagens() const;
    void setImagens(const vector<DadosImagens> &value);
    set<string> getClasses() const;
    void setClasses(const set<string> &value);

    void addClasse(string s);
    void addAtributo(string s);
    void imprimeArff(string t);

    set<string> getAtributos() const;
    void setAtributos(const set<string> &value);
    double percetron(int era, string save);
    double knn(int v, string save);

    void iniciaDescritor(string s);

private:
    vector<DadosImagens> imagens;
    set<string> classes;
    set<string> atributos;
    multimap<string,double> normal;
    map<string,pair<double,double>> minMax;
    string arff_Titulo(string t);
    string arffDados();
    string arff_Atributos();
    void copyMap();
    double minimoMaximo(double x, double min, double max);
    void normaliza();
    void divideBase(vector<DadosImagens> *treino, vector<DadosImagens> *testes, vector<DadosImagens> *valida);
    void iniciaBase(vector<DadosImagens> *testes, vector<DadosImagens> *treino, vector<DadosImagens> *valida);
    const vector<string> explode(const string &s, const char &c);
    string dadosPercetron(Perceptron percp);
};

#endif // CLASSIFICADORES_H
