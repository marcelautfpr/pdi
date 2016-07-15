#include "kmeans.h"


Kmeans::Kmeans()
{

}

Kmeans::Kmeans(string img)
{
    image = imread(img);
}

Mat Kmeans::getImage() const
{
    return image;
}

void Kmeans::setImage(const Mat &value)
{
    image = value;
}

vector<Vec3b> Kmeans::getCentroide() const
{
    return centroide;
}

void Kmeans::setCentroide(const vector<Vec3b> &value)
{
    centroide = value;
}

vector<vector<Vec3b> > Kmeans::getCluster() const
{
    return cluster;
}

void Kmeans::setCluster(const vector<vector<Vec3b> > &value)
{
    cluster = value;
}



void Kmeans::inicialize(int v)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    minstd_rand0 aleatorio(seed);
    for(int i=0; i<v; i++){
        centroide.push_back(Vec3b(aleatorio()%256,aleatorio()%256,aleatorio()%256));
        vector<Vec3b> aux;
        cluster.push_back(aux);
    }
}

void Kmeans::inGroup(int i, int j)
{
    multimap<double, int> group;
    int k = 0;
    Vec3b p1 = image.at<Vec3b>(i,j);
    for(auto c:centroide){
        group.insert(pair<double,int>(distancia(p1,c),k++));
    }
    cluster[group.begin()->second].push_back(p1);
}

void Kmeans::indexVec(vector<pair<Vec3b,Vec3b>> *vec)
{
    int i=0;
    for(auto c:cluster){
        for(auto v:c){
            vec->push_back(make_pair(v,centroide[i]));
        }
        i++;
    }
}

Vec3b Kmeans::buscaCor(Vec3b v)
{

}

Mat  Kmeans::saveImg(string save)
{
    Mat aux(image.rows,image.cols, CV_8UC3);
    typedef cv::Point3_<uint8_t> Pixel;
    vector<pair<Vec3b,Vec3b>> v;
    indexVec(&v);

    image.forEach<Vec3b>([&aux, &v](Vec3b &p, const int * position)-> void{
        aux.at<Vec3b>(position[0],position[1]) = find_if(v.begin(), v.end(),[p](const pair<Vec3b,Vec3b>& a){
            return a.first == p;
        })->second;
    });


    vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(8);
    //save += "/imgKmeans.jpg";
    imwrite(save.c_str(), aux);
    cvtColor(aux,aux,CV_BGR2RGB);
    return aux;
}

void Kmeans::clusters(int v)
{
    inicialize(v);

    do{
        cluster.clear();
        for(auto a:centroide){
            vector<Vec3b> v;
            cluster.push_back(v);
        }
        for(int i=0; i<image.rows; i++){
            for(int j=0; j<image.cols; j++){
                inGroup(i, j);
            }
        }
    }while(atualizaCentroides());
    int z = v;
    int w = v+z;
}

double Kmeans::distancia(Vec3b pt1, Vec3b pt2)
{
    int i=0;
    double d = 0.0;
    for(uchar v:pt1.val){
        d += pow((uchar)pt2.val[i++]-v,2);
    }
    double aux = pow(d,(double)1/2);
    return aux;
}

bool Kmeans::atualizaCentroides()
{
    vector<Vec3b> aux;
    int i =0;
    for(auto cl:cluster){
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        for(Vec3b c:cl){
            x += (double)c.val[0]/(double)cl.size();
            y += (double)c.val[1]/(double)cl.size();
            z += (double)c.val[2]/(double)cl.size();
        }
        aux.push_back(Vec3b(x,y,z));
    }

    for(auto c:centroide){
        if(c != aux[i++]){
            centroide = aux;
            return true;
        }
    }
    return false;
}

