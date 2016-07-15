#include "descritor.h"

Descritor::Descritor()
{

}

vector<DadosImagens> Descritor::getImages() const
{
    return images;
}

void Descritor::setImages(const vector<DadosImagens> &value)
{
    images = value;
}

const vector<string> Descritor::explode(const string &s, const char &c)
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

void Descritor::iniciaDescritor(string s)
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
                    stringstream aux;
                    aux << setfill('0') << setw(3) << i++;
                    img.addAtributo(aux.str(),stod(a));
                }
                images.push_back(img);
            }
        }
    }
}

void Descritor::addClasse(string s)
{
    classes.insert(s);
}

void Descritor::addAtributo(string s)
{
    atributos.insert(s);
}

string Descritor::arff_Titulo(string t)
{
    stringstream title;
    title << "% 1. Title: " << type << "_" << t << endl;
    title << "%" << endl;
    title << "% 2. Sources:" << endl;
    title << "%      (a) Creator: Marcela Marques" << endl;
    title << "%      (b) Donor: Marcela Marques (marcelabarbosa@alunos.utfpr.edu.br)" << endl;
    title << "%      (c) Date: May, 2016" << endl;
    title << "%" << endl;
    title << "@RELATION " << t << endl;

    return title.str();
}

string Descritor::arffDados()
{
    stringstream dados;
    dados << "@DATA" << endl;
    for(auto imgs:images){
        dados << fixed << setprecision(12);
        for(auto atr:imgs.getAtributos()){
            dados << atr.second << ", ";
            addAtributo(atr.first);
        }
        dados << imgs.getClasse() << endl;
        addClasse(imgs.getClasse());
    }

    return dados.str();
}

string Descritor::arff_Atributos()
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

void Descritor::salvaDados(string path)
{
    string arq = path + "/Descritores_" + type + ".dados";
    ofstream fileOut(arq.c_str());
    if(fileOut.is_open()){
        fileOut << fixed << setprecision(12);
        for(auto i:images){
            for(auto a:i.getAtributos()){
                fileOut << a.second << " ";
            }
            fileOut << i.getClasse() << endl;
        }
    }
    fileOut.close();
}

void Descritor::imprimeArff(string t, string save)
{
    string title = arff_Titulo(t);
    string dados = arffDados();
    string atrib = arff_Atributos();
    save.push_back('/');
    string cmd = save + type + "_" + t+".arff";
    ofstream saida(cmd.c_str());
    if(saida.is_open()){
        saida << title << endl;
        saida << atrib << endl;
        saida << dados << endl;
    }
    saida.close();
}

string Descritor::snnTitulo(int a, int c, int b)
{
    stringstream titulo;
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    titulo << "SNNS pattern definition file V3.2" << endl;
    titulo << "generated at " << ctime(&now) << endl << endl;

    titulo << "No. of patterns : " << b << endl;
    titulo << "No. of input units : " << a << endl;
    titulo << "No. of output units : " << c << endl << endl;

    return titulo.str();
}

string Descritor::snnsDados(int classes, map<string, string> mapClass, vector<DadosImagens> *base)
{
    stringstream dados;
    for(auto i:(*base)){
        const map<string,double> &atrib = i.getAtributos();
        dados << fixed << setprecision(12);
        for(auto a:atrib){
            dados << a.second << " ";
        }
//        dados << fixed << setfill('0') << setw(classes);
        dados << mapClass[i.getClasse()] << endl;
    }
    return dados.str();
}

void Descritor::conjuntoTestes(set<string> *classes, vector<DadosImagens> *treino, vector<DadosImagens> *teste, vector<DadosImagens> *valida)
{
    map<string,vector<DadosImagens>> auxMap;
    for(auto c:images){
        classes->insert(c.getClasse());
        auxMap[c.getClasse()].push_back(c);
    }

    int max = min_element(auxMap.begin(),auxMap.end(),[](const pair<string,vector<DadosImagens>>& a, const pair<string,vector<DadosImagens>>& b){
        return a.second.size() < b.second.size();
    })->second.size();

    vector<DadosImagens> particao;

    for(int i=0; i<max; i++){
        for(auto p:auxMap){
            particao.push_back(p.second.front());
            auxMap[p.first].erase(auxMap[p.first].begin());
        }
    }

    int i=0;

    for(auto p:particao){
        if(i<max){
            teste->push_back(p);
        }else if(i>=max && i < (max+max*(3.0/4.0))){
            treino->push_back(p);
        }else{
            valida->push_back(p);
        }
        i++;
    }

//    for(int i=0; i<max/2; i++){
//        for(auto c:(*classes)){
//            if(!auxMap[c].empty()){
//                treino->push_back((*auxMap[c].begin()));
//                auxMap[c].erase(auxMap[c].begin());
//                teste->push_back((*auxMap[c].begin()));
//                auxMap[c].erase(auxMap[c].begin());
//            }
//        }
//    }
}

void Descritor::snns(string save)
{
    vector<DadosImagens> treino, teste, valida;
    set<string> classes;
    DadosImagens &dados = images[0];
    const map<string,double> &atr = dados.getAtributos();
    conjuntoTestes(&classes, &treino, &teste, &valida);

    map<string,string> mapClass;
//    int x = 0;
    int w = 0;
    for(auto c:classes){
        string aux = "";
        for(int i=0; i<classes.size(); i++){
            if(i==0){
                aux += "0";
            }else{
                aux+=" 0";
            }
        }
        aux[w++*2] = '1';
        mapClass[c] = aux;
//        x *= 10;
    }

    save.push_back('/');
    string train = save + "Descritores_" + type + "_train.pat";
    string test = save + "Descritores_" + type + "_test.pat";
    string val = save + "Descritores_" + type + "_valida.pat";

    ofstream out(train.c_str());
    if(out.is_open()){
        out << snnTitulo(atr.size(), classes.size(), treino.size());
        out << snnsDados(classes.size(), mapClass, &treino);
    }

    ofstream out2(test.c_str());
    if(out2.is_open()){
        out2 << snnTitulo(atr.size(), classes.size(), teste.size());
        out2 << snnsDados(classes.size(), mapClass, &teste);
    }

    ofstream out3(val.c_str());
    if(out3.is_open()){
        out3 << snnTitulo(atr.size(), classes.size(), valida.size());
        out3 << snnsDados(classes.size(), mapClass, &valida);
    }
}

void Descritor::arqSvm(map<string, int> mapClass, vector<DadosImagens> base, string save)
{
    int i;
    ofstream outFile(save.c_str());
    if(outFile.is_open()){
        for(auto t:base){
            outFile << "+" << mapClass[t.getClasse()];
            i=0;
            outFile << fixed << setprecision(12);
            for(auto atr:t.getAtributos()){
                outFile << " " << i++ << ":" << atr.second;
            }
            outFile << endl;
        }
    }
    outFile.close();
}

void Descritor::svm(string save)
{
    vector<DadosImagens> treino, teste, valida;
    set<string> classes;
    conjuntoTestes(&classes,&treino, &teste, &valida);

    map<string, int> mapClass;
    int i = 0;
    for(auto c:classes){
        mapClass[c] = i++;
    }
    save.push_back('/');
    string train = save + "Descritores_" + type + "Svm_train.txt";
    string test = save + "Descritores_" + type + "Svm_test.txt";
    string val = save + "Descritores_" + type + "Svm_valida.txt";

    arqSvm(mapClass, treino, train);
    arqSvm(mapClass, teste, test);
    arqSvm(mapClass, valida, val);
}

void Descritor::normaliza()
{
    copyMap();
    vector<DadosImagens> aux;
    for(auto i:images){
        DadosImagens img;
        for(auto atr:i.getAtributos()){
            pair<double,double> aux = minMax[atr.first];
            img.addAtributo(atr.first,minimoMaximo(atr.second, aux.first, aux.second));
            img.setClasse(i.getClasse());
        }
        aux.push_back(img);
    }
    images = aux;
}

void Descritor::copyMap()
{
    map<string,vector<double>> auxAtt;
    for(auto i:images){
        for(auto it:i.getAtributos()){
            auxAtt[it.first].push_back(it.second);
        }
    }

    for(auto a:auxAtt){
        auto result = minmax_element(a.second.begin(),a.second.end());
        minMax[a.first] = make_pair(*result.first,*result.second);
    }
}

double Descritor::minimoMaximo(double x, double min, double max)
{
    return (max-min)?(double)(x-min)/(double)(max-min):0.0;
}

