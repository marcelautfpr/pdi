#include "perceptron.h"
#define RAND ((aleatorio()% 5000)+3000)/10000.0;

void Perceptron::iniciaPesos(const map<string,double>& img)
{
    minstd_rand0 aleatorio(chrono::system_clock::now().time_since_epoch().count());
    bias = RAND;
    for(auto i:img){
        dablios[i.first] = RAND;
    }
}

double Perceptron::somatorio(const map<string,double>& img)
{
    double soma = 0;
    for(auto i:img){
        soma += i.second*dablios[i.first]+bias;
    }

    return soma;
}

void Perceptron::extraiClasses()
{
    for(auto i:(*imagens)){
        classes.insert(i.getClasse());
    }
}

void Perceptron::classePositiva(string p)
{
    if(!p.empty()){
        positiva = p;
    }else {
        classePositiva();
    }
}

void Perceptron::classePositiva()
{
    if(!imagens->empty()){
        classePositiva((*imagens)[0].getClasse());
    }
}

int Perceptron::esperado(string s)
{
    return (s == positiva)? 1 : 0;
}

int Perceptron::hardLim(double v)
{
    return (v >= 0 )? 1 : 0;
}

bool Perceptron::erro(string s,double value)
{
    er = esperado(s) - hardLim(value);
    return (!er)? false : true;
}

void Perceptron::atualiza(map<string, double> img)
{
    bias += er;
    for(auto w:dablios){
        dablios[w.first] += er * img[w.first];
    }
}

void Perceptron::treinamento(int era, vector<DadosImagens>& valida)
{
    int auxEra;
    int eraInical = era;
    double auxBias;
    double auxEr;
    map<string, double> auxDablios;

    iniciaPesos((*imagens)[0].getAtributos());
    double acc = .0;
    int continua = 5;
    while (era-- && continua){
        zeraConfusao();
        for(auto i:(*imagens)){
            if(erro(i.getClasse(),somatorio(i.getAtributos()))){
                atualiza(i.getAtributos());
            }
        }
        classifica(valida);
        double acc2 = acuracia();
        if(acc2 < acc){
            continua--;
        }else{
            acc = acuracia();
            continua=5;
            auxBias = bias;
            auxEr = er;
            auxDablios = dablios;
            auxEra = eraInical - era;
        }
    }

    bias = auxBias;
    er = auxEr;
    dablios = auxDablios;
    this->era  = auxEra;

    zeraConfusao();
}

void Perceptron::classifica(DadosImagens img)
{
    confusao[esperado(img.getClasse())][hardLim(somatorio(img.getAtributos()))]++;
}

void Perceptron::classifica(vector<DadosImagens> img)
{
    for(auto i:img){
        classifica(i);
    }
}

map<string, double> Perceptron::getDablios() const
{
    return dablios;
}

double Perceptron::getBias() const
{
    return bias;
}

void Perceptron::setBias(double value)
{
    bias = value;
}

double Perceptron::getEr() const
{
    return er;
}

void Perceptron::setEr(double value)
{
    er = value;
}

string Perceptron::getPositiva() const
{
    return positiva;
}

void Perceptron::setPositiva(const string &value)
{
    positiva = value;
}

int Perceptron::getEra() const
{
    return era;
}

void Perceptron::setEra(int value)
{
    era = value;
}

Perceptron::Perceptron(vector<DadosImagens> *i)
{
    if(!i->empty()){
        zeraConfusao();
        imagens = i;
        classePositiva();
    }
}

void Perceptron::zeraConfusao()
{
    for(int i=0; i<2;i++){
        for(int j=0; j<2; j++){
            confusao[i][j] = 0;
        }
    }
}

double Perceptron::acuracia()
{
    double soma = 0;
    double total = 0;
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            total+=confusao[i][j];
        }
        soma += confusao[i][i];
    }

    return (soma/total)*100;
}

Perceptron::Perceptron(vector<DadosImagens> *i, string p)
{
    if(!i->empty()){
        zeraConfusao();
        imagens = i;
        classePositiva(p);
    }
}

