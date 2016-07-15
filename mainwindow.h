#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "matrizcoocorrencia.h"
#include "fractais.h"
#include "estatisitica.h"
#include "kmeans.h"
#include "knn.h"
#include "classificadores.h"
#include "lpb.h"
#include "matrizconfusaosvm.h"

#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QDir>
#include <QFileInfoList>
#include <vector>
#include <string>
#include <list>
#include <cstdlib>
#include <QString>
#include <QStringList>
#include <QFileSystemModel>
#include <QProgressDialog>
#include <QPixmap>
#include <QSize>
#include <QImage>
#include <QFileDialog>

using namespace std;
using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void extraiCaracteristica();
    void glcm();
    void fractais();
    void dadosEstatisicos();
    void dadosLBP();

private slots:
    void on_pushButton_Image_clicked();

    void on_pushButton_Calcular_clicked();

    void on_pB_selectDir_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pB_selectFileKmeans_clicked();

    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

    void on_pB_SvmTeste_clicked();

    void on_pB_predict_clicked();

    void on_buttonBox_3_accepted();

private:
    Ui::MainWindow *ui;
    QString imgPath;
    QString savePath;
    QString selectPath;
    QString kmeansPath;
    QString svmTeste;
    QString svmPredict;
    vector<string> buscaImagens();
    void kmeansImage();
};

#endif // MAINWINDOW_H
