#include "knn.h"

Knn::Knn()
{

}

vector<DadosImagens> Knn::getImagens() const
{
    return imagens;
}

void Knn::setImagens(const vector<DadosImagens> &value)
{
    imagens = value;
}

map<string, vector<DadosImagens> > Knn::getClasses() const
{
    return classes;
}

void Knn::setClasses(const map<string, vector<DadosImagens> > &value)
{
    classes = value;
}

double Knn::distancia(map<string, double> a, map<string, double> b)
{
    double soma = 0;
    for(auto i:a){
        soma += pow(b[i.first]-i.second,2);
    }
    return pow(soma,(double)1/2);
}

string Knn::desempate(int max, map<string,int> conta, multimap<double,string> dist)
{
    map<string,int> auxConta;
    for(auto c:conta){
        if(c.second == max){
            auxConta.insert(make_pair(c.first,c.second));
        }
    }
    map<double,string> aux;
    for(auto c:auxConta){
        string s = c.first;
        map<double,string>::iterator it = find_if(dist.begin(),dist.end(),[s](const pair<double,string>& a){
            return a.second == s;
        });
        aux[it->first] = it->second;
    }

    return aux.begin()->second;
}

string Knn::classifica(int n, DadosImagens img)
{
    set<string> cls;
    multimap<double,string> dist;
    map<string,int> conta;
    for(auto i:imagens){
        dist.insert(make_pair(distancia(img.getAtributos(),i.getAtributos()),i.getClasse()));
        cls.insert(i.getClasse());
        if(dist.size() > n){
            dist.erase(dist.end().operator --());
        }
    }

    for(auto c:cls){
        conta[c] = count_if(dist.begin(),dist.end(), [c](const pair<double,string>& a){
                                return a.second == c;
                            });
    }

    int max = max_element(conta.begin(),conta.end(),[](const pair<string,int>& a,const pair<string,int>& b){
                                                        return a.second < b.second;
                                                    })->second;

    if( count_if(conta.begin(),conta.end(),[max](const pair<string,int>& a){
                 return a.second == max;}) == 1 ){
        return find_if(conta.begin(),conta.end(),[max](const pair<string,int>& a){
            return a.second == max;
        })->first;
    }else{
        return desempate(max, conta, dist);
    }
}

double Knn::classifica(int n, vector<DadosImagens> treino, vector<DadosImagens> teste)
{
    confusao.clear();
    imagens = treino;
    for(auto i:teste){
        confusao[make_pair(i.getClasse(),classifica(n,i))]++;
    }

    int qnt = 0;

    for_each(confusao.begin(),confusao.end(),[&qnt](const pair<pair<string,string>,int>& a){
       if(a.first.first == a.first.second){
           qnt += a.second;
       }
    });

    return (double)(qnt/(double)teste.size())*100.0;
}

