#include "classificadores.h"

Classificadores::Classificadores()
{

}

Classificadores::Classificadores(vector<DadosImagens> images)
{
    setImagens(images);
}

void Classificadores::iniciaDescritor(string s)
{
    ifstream entra(s.c_str());
    if(entra.is_open()){
        string line;
        while (getline(entra, line)) {
            if(!line.empty()){
                DadosImagens img;
                int i = 1;
                vector<string> aux = explode(line,' ');
                img.setClasse(aux.back());
                aux.pop_back();
                for(auto a : aux){
                    img.addAtributo(to_string(i++),stod(a));
                }
                imagens.push_back(img);
            }
        }
    }
}

Classificadores::Classificadores(string s)
{
    iniciaDescritor(s);
}

vector<DadosImagens> Classificadores::getImagens() const
{
    return imagens;
}

void Classificadores::setImagens(const vector<DadosImagens> &value)
{
    imagens = value;
}

set<string> Classificadores::getClasses() const
{
    return classes;
}

void Classificadores::setClasses(const set<string> &value)
{
    classes = value;
}

void Classificadores::addClasse(string s)
{
    classes.insert(s);
}

void Classificadores::addAtributo(string s)
{
    atributos.insert(s);
}

string Classificadores::arff_Titulo(string t)
{
    stringstream title;
    title << "% 1. Title: " << t << endl;
    title << "%" << endl;
    title << "% 2. Sources:" << endl;
    title << "%      (a) Creator: Marcela Marques" << endl;
    title << "%      (b) Donor: Marcela Marques (marcelabarbosa@alunos.utfpr.edu.br)" << endl;
    title << "%      (c) Date: May, 2016" << endl;
    title << "%" << endl;
    title << "@RELATION " << t << endl;

    return title.str();
}

string Classificadores::arffDados()
{
    stringstream dados;
    dados << "@DATA" << endl;
    for(auto imgs:imagens){
        dados << fixed << setprecision(12);
        for(auto atr:imgs.getAtributos()){
            //dados << n << ",";
            dados << atr.second << ", ";
            addAtributo(atr.first);
        }
        dados << imgs.getClasse() << endl;
        addClasse(imgs.getClasse());
    }

    return dados.str();
}

string Classificadores::arff_Atributos()
{
    stringstream atrib;
    for(auto atr:atributos){
        atrib << "@ATTRIBUTE " << atr << "\tNUMERIC" << endl;
    }
    atrib << "@ATTRIBUTE class\t{";
    unsigned i=0;
    for(auto c:classes){
        if(i++ == classes.size()-1){
            atrib << c;
        }else{
            atrib << c << ", ";
        }
    }
    atrib << "}" << endl;

    return atrib.str();
}

void Classificadores::copyMap()
{
//    set<double> mMax;
    map<string,vector<double>> auxAtt;
    for(auto i:imagens){
        for(auto it:i.getAtributos()){
            //normal.insert(make_pair(it.first,it.second));
            auxAtt[it.first].push_back(it.second);
        }
    }

    for(auto a:auxAtt){
        auto result = minmax_element(a.second.begin(),a.second.end());
        minMax[a.first] = make_pair(*result.first,*result.second);
    }
}

double Classificadores::minimoMaximo(double x, double min, double max)
{
    return (max-min)?(double)(x-min)/(double)(max-min):0.0;
}

void Classificadores::normaliza()
{
    vector<DadosImagens> aux;
    //int z = 0;
    for(auto i:imagens){
        DadosImagens img;
        for(auto atr:i.getAtributos()){
            pair<double,double> aux = minMax[atr.first];
            img.addAtributo(atr.first,minimoMaximo(atr.second, aux.first, aux.second));
            img.setClasse(i.getClasse());
        }
        aux.push_back(img);
    }
    imagens = aux;
}

void Classificadores::imprimeArff(string t)
{
    copyMap();
    normaliza();
    string title = arff_Titulo(t);
    string dados = arffDados();
    string atrib = arff_Atributos();

    ofstream saida(t+".arff");
    if(saida.is_open()){
        saida << title << endl;
        saida << atrib << endl;
        saida << dados << endl;
    }
    saida.close();
}

set<string> Classificadores::getAtributos() const
{
    return atributos;
}

void Classificadores::setAtributos(const set<string> &value)
{
    atributos = value;
}

void Classificadores::divideBase(vector<DadosImagens> *treino, vector<DadosImagens> *testes, vector<DadosImagens> *valida)
{
    map<string, vector<DadosImagens>> partes;

    for(auto d:imagens){
        partes[d.getClasse()].push_back(d);
    }

    int max = min_element(partes.begin(),partes.end(),[](const pair<string,vector<DadosImagens>>& a, const pair<string,vector<DadosImagens>>& b){
        return a.second.size() < b.second.size();
    })->second.size();

    vector<DadosImagens> particao;

    for(int i=0; i<max; i++){
        for(auto p:partes){
            particao.push_back(p.second.front());
            partes[p.first].erase(partes[p.first].begin());
        }
    }

    int i=0;

    for(auto p:particao){
        if(i<max){
            testes->push_back(p);
        }else if(i>=max && i < (max+max*(3.0/4.0))){
            treino->push_back(p);
        }else{
            valida->push_back(p);
        }
        i++;
    }
}

void Classificadores::iniciaBase(vector<DadosImagens> *testes, vector<DadosImagens> *treino, vector<DadosImagens> *valida)
{
    copyMap();
    normaliza();

    divideBase(treino, testes, valida);
}

const vector<string> Classificadores::explode(const string &s, const char &c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

string Classificadores::dadosPercetron(Perceptron percp)
{
    stringstream dados;
    dados << "Classe Positiva: " << percp.getPositiva() << endl;
    dados << "Bias: " << percp.getBias() << endl;
    dados << "Eras: " << percp.getEra() << endl;
    dados << "Erro: " << percp.getEr() << endl;
    dados << "Pesos:";
    int i=0;
    for(auto w:percp.getDablios()){
        dados << " W" << i++ << ":" << w.second;
    }

    dados << endl << "Acurácia: " << percp.acuracia() << endl << endl;

    return dados.str();
}

double Classificadores::percetron(int era, string save)
{
    vector<DadosImagens> treino, testes, valida;

    iniciaBase(&testes, &treino, &valida);

    vector<double> acc;
    string texto = "";
    Perceptron percp(&treino);
    for(int i=0; i<10; i++){
        percp.treinamento(era,valida);
        percp.classifica(testes);
        acc.push_back(percp.acuracia());
        texto += dadosPercetron(percp);
    }
    double soma = 0;
    for(auto a:acc){
        soma += a;
    }

    texto += "\nMédia: " + to_string(soma/acc.size()) + "\n";
    save += "/dadosPerceptron.txt";
    ofstream outFile(save.c_str());
    if(outFile.is_open()){
        outFile << texto;
    }
    outFile.close();

    return soma/acc.size();
}

double Classificadores::knn(int v, string save)
{
    vector<DadosImagens> treino, testes;

    map<string,vector<DadosImagens>> auxMap;
    for(auto img:imagens){
        auxMap[img.getClasse()].push_back(img);
    }

    int max = min_element(auxMap.begin(),auxMap.end(),[](const pair<string,vector<DadosImagens>>& a, const pair<string,vector<DadosImagens>>& b){
        return a.second.size() < b.second.size();
    })->second.size();

    for(int i=0; i<max/2; i++){
        for(auto aux:auxMap){
            testes.push_back(aux.second.front());
            auxMap[aux.first].erase(auxMap[aux.first].begin());
            treino.push_back(aux.second.front());
            auxMap[aux.first].erase(auxMap[aux.first].begin());
            i++;
        }
    }

    Knn knn;
    stringstream texto;
    vector<double> acc;
    for(int i=1; i<=v; i++){
        acc.push_back(knn.classifica(i,treino,testes));
        texto << "Vizinhos: " << i << endl << "Acurácia: " << acc[i-1] << "%" << endl << endl;
    }
    double soma = 0;
    for(auto a:acc){
        soma += a;
    }

    save += "/resultadoKnn.txt";
    ofstream outFile(save.c_str());

    if(outFile.is_open()){
        outFile << texto.str() << endl;

        outFile << endl << "Média: " << soma/acc.size() << "%" << endl;
    }

    return soma/acc.size();
}


