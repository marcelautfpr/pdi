#ifndef MATRIZCONFUSAOSVM_H
#define MATRIZCONFUSAOSVM_H

#include <map>
#include <fstream>
#include <vector>

using namespace std;


class MatrizConfusaoSVM
{
public:
    MatrizConfusaoSVM();
    const vector<string> explode(const string &s, const char &c);
    double acuracia(string txt, string predict);
private:

};

#endif // MATRIZCONFUSAOSVM_H
