#include "fractais.h"

Fractais::Fractais()
{
    Descritor::type = "Factais";
}

Fractais::Fractais(Mat img)
{
    Descritor::type = "Factais";
    image = img;
}

Rect Fractais::getRoi() const
{
    return roi;
}

void Fractais::setRoi(const Rect &value)
{
    roi = value;
}

int Fractais::calculaN(Mat img, int d)
{
    Mat aux;
    threshold(img,aux,200.0,255.0,THRESH_BINARY);
    //cvtColor(aux,aux,CV_BGR2GRAY);
    int n = 0;
    unsigned noZeros;
    for(int i=0; i<d; i++){
        for(int j=0; j<d; j++){
            setRoi(Rect(j*aux.cols/d,i*aux.rows/d,(int)aux.cols/d,aux.rows/d));
            noZeros = countNonZero(aux(roi));
            if(noZeros < aux(roi).total()){
                n++;
            }
        }
    }

    return n;
}

map<string,double> Fractais::BoxCounting(Mat img, int d)
{
    vector<Mat> aux;
    map<string, double> res;
    split(img,aux);
    int i = 0;
    for(auto im:aux){
        stringstream atrib;
        atrib << "BoxCounting_L" << ++i;
        //double dB;
        double n[2];
        n[0] = calculaN(im,d);
        n[1] = calculaN(im,d*2);
        double logD = log((double)(d*2)/d);
        double logN = log((n[1]/n[0]));
        res[atrib.str()] = (double)logN/logD;
    }

    return res;

}

double Fractais::DiferentialBoxCounting(int r)
{
    r = r*2;
    double min, max;
    int mr;
    double lr = 0;
    double pN = 0;
    double pD = 0;
    int q = (image.rows/r)*(image.cols/r);
    Mat aux;
    cvtColor(image,aux,CV_BGR2GRAY);

    for(int i=0; i<aux.rows/r;i++){
        for(int j=0; j<aux.cols/r;j++){
            setRoi(Rect(j*r,i*r,r,r));
            minMaxLoc(aux(roi),&min,&max);
            mr = (int)(ceilf(max/r) - ceilf(min/r)) + 1;
            pN += (double)pow(mr,2);
            pD += (double)mr/q;
        }
    }
    lr = (pN/q)/pow(pD,2);
    return lr;
}

int Fractais::pressMax(double c, int d)
{
    int pr = 0;
    for(int i=0; i<d; i++){
        pr += d * (c+i*d);
    }

    return pr;
}

Mat Fractais::recortaImagem(Mat image)
{
    Rect roi;
    int xMin, xMax, yMin, yMax;
    xMin = image.cols;
    yMin = image.rows;
    xMax = yMax = 0;

    for(int i=0; i<image.rows;i++){
        for(int j=0; j<image.cols;j++){
            int k = image.at<Vec3b>(i,j)[0];
            if(k < 230){
                if(i<yMin){
                    yMin = i;
                }
                if(j<xMin){
                    xMin = j;
                }
                if(i>yMax){
                    yMax = i;
                }
                if(j>xMax){
                    xMax = j;
                }
            }
        }
    }
    roi = Rect(xMin+3,yMin+3,(xMax-xMin)-3,(yMax-yMin)-3);
    return image(roi);
}

double Fractais::topDown(Mat image, int op, int p)
{
    Mat aux;
    cvtColor(image,image,CV_BGR2GRAY);
    threshold(image,image,200.0,255.0,THRESH_BINARY);
    cvtColor(image,aux,CV_GRAY2RGB);

    int r;
    switch (op) {
    case 1:
        r=image.rows-1;
        break;
    default:
        r = 0;
        break;
    }

    Vec3b cor(0,0,0);

    for(int i=0; i<aux.cols;i++){
        Vec3b c = aux.at<Vec3b>(r,i);
        if(c != cor){
            Point x(i,r);
            floodFill(aux, x, Scalar(0.0,0.0,0.0));
        }
    }

    cvtColor(aux,aux,CV_RGB2GRAY);
    absdiff(aux,image,aux);

    return press(aux,0,p);
}

double Fractais::leftRigth(Mat image, int op, int p)
{
    Mat aux;
    cvtColor(image,image,CV_BGR2GRAY);
    threshold(image,image,200.0,255.0,THRESH_BINARY);
    cvtColor(image,aux,CV_GRAY2RGB);

    int c;
    switch (op) {
    case 1:
        c = image.cols-1;
        break;
    default:
        c = 0;
        break;
    }

    Vec3b cor(0,0,0);
    for(int i=0; i<aux.rows;i++){
        if(aux.at<Vec3b>(i,c) != cor){
            floodFill(aux, Point(c,i), Scalar(0.0,0.0,0.0));
        }
    }

    cvtColor(aux,aux,CV_RGB2GRAY);
    absdiff(aux,image,aux);

    return press(aux,1, p);
}

double Fractais::press(Mat image, int op, int d)
{
    double c = (double)d/2;
    int pr = 0;
    switch (op) {
    case 1:
        pr = pressMax(c, image.cols/d);
        break;
    default:
        pr = pressMax(c, image.rows/d);
        break;
    }

    double   delta = 0;
    for(int i=0; i<(int)image.rows/d; i++){
        int op = 0;
        for(int j=0; j<(int)image.cols/d; j++){
            setRoi(Rect(j*d,i*d,d,d));
            op = countNonZero(image(roi))/image(roi).total();
            if(op){
                delta += op*(c+(j*d))/pr;
            }
        }
    }
    return delta;
}

map<string,double> Fractais::sucolaridade(int d)
{
    Mat aux;
    threshold(recortaImagem(image),aux,200.0,255,THRESH_BINARY);
    map<string,double> sucolari;
    sucolari["Sucolaridade_topDown"] = topDown(aux,0,d);
    sucolari["Sucolaridade_bottomUp"] = topDown(aux,1,d);
    sucolari["Sucolaridade_leftRigth"] = leftRigth(aux,0,d);
    sucolari["Sucolaridade_Rigthleft"] = leftRigth(aux,1,d);

    return sucolari;
}

void Fractais::fractalImage(vector<string> img, int d, bool bxc, bool dboxc, bool sucolari)
{
    for(auto s:img){

        DadosImagens dados;
        dados.setImagem(s);
        dados.setClasse(explode((*explode(s,'/').rbegin()),'_')[0]);

        image = imread(s);

        if(bxc){
            dados.addAtributo(BoxCounting(image,d*d));
        }
        if(dboxc){
            dados.addAtributo("DifBoxCounting",DiferentialBoxCounting(d*d));
        }
        if(sucolari){
            dados.addAtributo(sucolaridade(d));
        }
        images.push_back(dados);
    }
}
