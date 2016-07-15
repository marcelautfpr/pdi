#include "matrizconfusaosvm.h"

MatrizConfusaoSVM::MatrizConfusaoSVM()
{

}

const vector<string> MatrizConfusaoSVM::explode(const string &s, const char &c)
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

double MatrizConfusaoSVM::acuracia(string txt, string predict)
{
    ifstream fileTxt(txt.c_str());
    ifstream filePredict(predict.c_str());
    int x, y;
    if(fileTxt.is_open() && filePredict.is_open()){
        string line1, line2;
        getline(filePredict, line1);
        int clas = explode(line1,' ').size()-1;
        int confusao[clas][clas];
        for(int i=0; i<clas; i++){
            for(int j=0; j<clas; j++){
                confusao[i][j] = 0;
            }
        }
        int cont = 0;
        while (getline(fileTxt,line1) && getline(filePredict,line2)) {
            cont++;
            x = stoi(explode(explode(line1,' ')[0],'+')[0]);
            y = stoi(explode(line2,' ')[0]);
            confusao[x][y]++;
        }
        double soma = 0.0;
        int total = 0;
        for(int i=0; i<clas; i++){
            soma += confusao[i][i];
            for(int j=0; j<clas; j++){
                total += confusao[i][j];
            }
        }
        return soma/(double)total*100;
    }
    return 0.0;
}

