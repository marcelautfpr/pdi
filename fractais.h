#ifndef FRACTAIS_H
#define FRACTAIS_H

#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <QPlainTextEdit>
#include <QString>

#include "dadosimagens.h"
#include "descritor.h"

using namespace std;
using namespace cv;

class Fractais : public Descritor
{
public:
    Fractais();
    Fractais(Mat img);

    Rect getRoi() const;
    void setRoi(const Rect &value);
    map<string, double> BoxCounting(Mat img, int d);
    double DiferentialBoxCounting(int r);
    map<string,double> sucolaridade(int d);
    void fractalImage(vector<string> img, int d, bool bxc, bool dboxc, bool sucolari);

private:

    Mat image;
    Rect roi;

    int calculaN(Mat img, int d);
    int pressMax(double c, int d);
    Mat recortaImagem(Mat image);
    double topDown(Mat image, int op, int p);
    double leftRigth(Mat image, int op, int p);
    double press(Mat aux, int op, int d);
};

#endif // FRACTAIS_H
