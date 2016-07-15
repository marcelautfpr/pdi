#ifndef ESTATISITICA_H
#define ESTATISITICA_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <sstream>
#include "dadosimagens.h"
#include "descritor.h"

using namespace std;
using namespace cv;


class Estatisitica : public Descritor
{
public:
    Estatisitica();

    Mat getImage() const;
    void setImage(const Mat &value);

    Scalar entropy();
    Scalar kurtosis();
    Scalar min();
    Scalar max();
    Scalar means();
    Scalar median();
    Scalar mode();
    Scalar skewness();
    Scalar standardDeviation();
    Scalar variance();
    void geraDados(vector<string> img, bool e, bool k, bool mi, bool ma, bool me, bool mo, bool sk, bool st, bool va);

private:

    Mat image;
};

#endif // ESTATISITICA_H
