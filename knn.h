#ifndef KNN_H
#define KNN_H

#include "dadosimagens.h"

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <set>
#include <cmath>
#include <algorithm>

using namespace std;

class Knn
{
public:
    Knn();
    vector<DadosImagens> getImagens() const;
    void setImagens(const vector<DadosImagens> &value);

    map<string, vector<DadosImagens> > getClasses() const;
    void setClasses(const map<string, vector<DadosImagens> > &value);

    double distancia(map<string,double> a, map<string,double> b);
    string classifica(int n, DadosImagens img);
    double classifica(int n, vector<DadosImagens> treino, vector<DadosImagens> teste);

private:
    map<pair<string,string>,int> confusao;
    vector<DadosImagens> imagens;
    map<string,vector<DadosImagens>> classes;
    void teste(string img);

    string desempate(int max, map<string,int> conta, multimap<double,string> dist);
};

#endif // KNN_H
