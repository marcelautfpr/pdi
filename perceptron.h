#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include "dadosimagens.h"

#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <set>

class Perceptron
{
public:
    Perceptron(vector<DadosImagens> *i);
    Perceptron(vector<DadosImagens> *i, string p);
    void treinamento(int era, vector<DadosImagens> &valida);
    void classifica(DadosImagens img);
    void classifica(vector<DadosImagens> img);
    double acuracia();

    map<string, double> getDablios() const;

    double getBias() const;
    void setBias(double value);

    double getEr() const;
    void setEr(double value);

    string getPositiva() const;
    void setPositiva(const string &value);

    int getEra() const;
    void setEra(int value);

private:
    int confusao[2][2];
    double bias;
    double er;
    int era;
    string positiva;
    vector<DadosImagens> *imagens;
    map<string, double> dablios;
    set<string> classes;
    void iniciaPesos(const map<string, double> &img);
    double somatorio(const map<string, double> &img);
    void extraiClasses();
    void classePositiva(string p);
    void classePositiva();
    int esperado(string s);
    int hardLim(double v);
    bool erro(string s, double value);
    void atualiza(map<string, double> img);
    void zeraConfusao();
};

#endif // PERCEPTRON_H
