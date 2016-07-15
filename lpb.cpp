#include "lpb.h"

LPB::LPB()
{
    type = "LBP";
}

void LPB::inicializaLBP(string path)
{
    ifstream inFile(save);
    if(inFile.is_open()){
        string line;
        vector<string> dados;
        while (getline(inFile,line)) {
            if(!line.empty()){
                DadosImagens img;
                dados = explode(line,' ');
                string classe = dados.back();
                dados.pop_back();
                string aux(path + classe);
                img.setImagem(aux);
                img.setClasse(explode(classe,'_')[0]);
                int i = 1;
                for(auto s:dados){
                    stringstream auxString;
                    auxString << setfill('0') << setw(3) << i++;
                    img.addAtributo(auxString.str(), stod(s));
                }
                images.push_back(img);
            }
        }
    }
}

void LPB::executeMatlab()
{
    save += "/lbp.mat";
    path.push_back('/');
    string cmd = "matlab -nodisplay -nojvm -r GeraDescritoresLBP('" + path + "','" + save + "');exit";
    system(cmd.c_str());
}

void LPB::geraDados(string path, string save)
{
    this->path = path;
    this->save = save;
    executeMatlab();
    ifstream ok;
    do{
        ok.open("C:/temp/ok.dados");
    }while(!ok.is_open());
    inicializaLBP(path);
}
