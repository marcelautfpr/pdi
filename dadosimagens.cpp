#include "dadosimagens.h"

DadosImagens::DadosImagens()
{

}

DadosImagens::DadosImagens(string s, string c)
{
    setImagem(s);
    setClasse(c);
}

string DadosImagens::getImagem() const
{
    return imagem;
}

void DadosImagens::setImagem(const string &value)
{
    imagem = value;
}

string DadosImagens::getClasse() const
{
    return classe;
}

void DadosImagens::setClasse(const string &value)
{
    classe = value;
}

void DadosImagens::addAtributo(string s, double v)
{
    atributos[s] = v;
}

void DadosImagens::addAtributo(map<string, double> dados)
{
    for(auto d:dados){
        atributos.insert(d);
    }
}

map<string, double> DadosImagens::getAtributos() const
{
    return atributos;
}


