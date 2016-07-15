#ifndef DADOSIMAGENS_H
#define DADOSIMAGENS_H

#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class DadosImagens
{
public:
    DadosImagens();
    DadosImagens(string s, string c);
    string getImagem() const;
    void setImagem(const string &value);

    string getClasse() const;
    void setClasse(const string &value);
    void addAtributo(string s, double v);
    void addAtributo(map<string, double> dados);
    map<string, double> getAtributos() const;

private:
    string imagem;
    string classe;
    map<string,double> atributos;
};

#endif // DADOSIMAGENS_H
