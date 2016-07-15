#ifndef KMEANS_H
#define KMEANS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <chrono>
#include <random>
#include <QPlainTextEdit>


using namespace std;
using namespace cv;

class Kmeans
{
public:
    Kmeans();
    Kmeans(string img);
    void clusters(int v);

    Mat getImage() const;
    void setImage(const Mat &value);


    vector<Vec3b> getCentroide() const;
    void setCentroide(const vector<Vec3b> &value);

    vector<vector<Vec3b> > getCluster() const;
    void setCluster(const vector<vector<Vec3b> > &value);
    Mat saveImg(string save);

private:
    Mat image;
    vector<Vec3b> centroide;
    vector<vector<Vec3b>> cluster;
    double distancia(Vec3b pt1, Vec3b pt2);
    bool atualizaCentroides();
    void inicialize(int v);
    void inGroup(int i, int j);
    void indexVec(vector<pair<Vec3b, Vec3b> > *vec);
    Vec3b buscaCor(Vec3b v);
};

#endif // KMEANS_H
