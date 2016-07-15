#include "matrizcoocorrencia.h"

MatrizCooCorrencia::MatrizCooCorrencia(int dist)
{
    distancia = dist;
    type = "GLCM";
}

string MatrizCooCorrencia::buscaClasse(string s)
{
    string aux = (*explode(s,'/').rbegin());
    return explode(aux,'_')[0];
}

string MatrizCooCorrencia::retornaAtributo(string s, int i)
{
    stringstream auxString;
    auxString << s << "_" << setfill('0') << setw(3) << i;
    return auxString.str();
}

void MatrizCooCorrencia::zeraMatriz(double m[256][256])
{
    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            m[i][j] = 0;
        }
    }
}

void MatrizCooCorrencia::norma_liza(int dist, Mat img)
{
    for(int i=0; i<256; i++){
        for (int j = 0; j < 256; j++) {
            matriz[i][j] = matriz[i][j]/(2*(img.rows-(dist + 1))*((img.cols-(dist + 1))));
        }
    }
}

double MatrizCooCorrencia::energia()
{
    double aux = 0.0;
    for(int i=0; i<256; i++){
        for (int j = 0; j < 256; j++) {
            if(matriz[i][j] > 0.0){
                aux += matriz[i][j]*matriz[i][j];
            }
        }
    }
    auxValores.push_back(aux);

    return aux;
}

double MatrizCooCorrencia::entropia()
{
    double aux = 0.0;
    for(int i=0; i<256; i++){
        for (int j = 0; j < 256; j++) {
            if(matriz[i][j] > 0.0){
                aux += matriz[i][j]*log(matriz[i][j]);
            }
        }
    }
    auxValores.push_back(-aux);
    return -aux;
}

double MatrizCooCorrencia::contraste()
{
    double aux = 0.0;
    for(int i=0; i<256; i++){
        for (int j = 0; j < 256; j++) {
            if(matriz[i][j] > 0.0){
                aux += pow((i-j),2)*matriz[i][j];
            }
        }
    }
    auxValores.push_back(aux);
    return aux;
}

double MatrizCooCorrencia::correlacao()
{
    double aux = 0.0;
    double px, py, desv_i, desv_j;
    px = py = desv_i = desv_j = 0;

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            px+=j*matriz[i][j];
            py+=j*matriz[i][j];
        }
    }

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            desv_i += pow((i-px),2)*matriz[i][j];
            desv_j += pow((j-py),2)*matriz[i][j];
        }
    }

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            aux += ((i-px)*(j-py)*matriz[i][j])/(desv_i*desv_j);
        }
    }
    //atributos.insert("correlacao");
    auxValores.push_back(aux);

    return aux;
}

double MatrizCooCorrencia::homogeneidade()
{
    double aux = 0.0;
    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            aux += (1/(1+pow((i-j),2)))*matriz[i][j];
        }
    }
    //atributos.insert("homogeneidade");
    auxValores.push_back(aux);
    return aux;
}

double MatrizCooCorrencia::variancia_x()
{
    double aux = 0.0;
    double px, py;
    px = py = 0;

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            px+=matriz[i][j];
        }
    }

    px = px/pow(256,2);

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            aux += pow((i-px),2)*matriz[i][j];
        }
    }
    auxValores.push_back(aux);
    //atributos.insert("variância");
    return aux;
}

double MatrizCooCorrencia::variancia_y()
{
    double aux = 0.0;
    double py, desv_i, desv_j;
    py = desv_i = desv_j = 0;

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            py+=j*matriz[i][j];
        }
    }

    for(int i=0; i<256; i++){
        for(int j=0; j<256; j++){
            aux += pow((j-py),2)*matriz[i][j];
        }
    }

    return aux;
}

void MatrizCooCorrencia::geraDados(bool vari, string img, bool homo, bool entro, bool ener, bool cont, bool corre)
{
    int ang = 0;
    DadosImagens auxImage(img, buscaClasse(img));

    while (ang <= 135) {
        geraMatriz(img,ang,distancia);
        if(ener){
            auxImage.addAtributo(retornaAtributo("Energia",ang), energia());
        }
        if(entro){
            auxImage.addAtributo(retornaAtributo("Entropia", ang), entropia());
        }
        if(cont){
            auxImage.addAtributo(retornaAtributo("Contraste", ang),contraste());
        }

        if(corre){
            auxImage.addAtributo(retornaAtributo("Correlacao", ang),correlacao());
        }
        if(homo){
            auxImage.addAtributo(retornaAtributo("Homogeneidade", ang),homogeneidade());
        }
        if(vari){
            auxImage.addAtributo(retornaAtributo("Variancia", ang), variancia_x());
        }
        ang+=45;
    }
    images.push_back(auxImage);
}

void MatrizCooCorrencia::geraDados(vector<string> img, bool ener, bool entro, bool cont, bool corre, bool homo, bool vari)
{
    for(auto i:img) {
        geraDados(vari, i, homo, entro, ener, cont,corre);
    }
}

void MatrizCooCorrencia::geraMatriz(std::string imagem, int ang, int dist)
{
    zeraMatriz(matriz);
    Mat img = imread(imagem, CV_LOAD_IMAGE_GRAYSCALE);
    int cor1, cor2;
    str = "";

    for(int i=dist; i<=img.rows-(dist + 1); i++){
        for(int j=dist; j<=img.cols-(dist + 1); j++){
            switch (ang) {

            case 0:
                cor1 = img.at<uchar>(i,j);
                cor2 = img.at<uchar>(i,j+dist);
                break;

            case 45:
                cor1 = img.at<uchar>(i,j);
                cor2 = img.at<uchar>(i-dist,j+dist);
                break;

            case 90:
                cor1 = img.at<uchar>(i,j);
                cor2 = img.at<uchar>(i-dist,j);
                break;

            case 135:
                cor1 = img.at<uchar>(i,j);
                cor2 = img.at<uchar>(i-dist,j-dist);
                break;
            }
            matriz[cor1][cor2]++;
        }
    }

    norma_liza(dist, img);

}
