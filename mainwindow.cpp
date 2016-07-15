#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

vector<string> MainWindow::buscaImagens()
{
    QStringList arqs;
    //QDir diretorio(names,"*.jpg | *.png | *.bmp | *.tif");
    QDir diretorio(imgPath,"*.bmp");
    arqs = diretorio.entryList();
    vector<string> imgs;
    foreach (QString var, arqs) {
       imgs.push_back(imgPath.toStdString() +"/"+ var.toStdString());
    }

    return imgs;
}

void MainWindow::on_pushButton_Image_clicked()
{
    QFileDialog file;
    imgPath = file.getExistingDirectory(this,tr("Escolha a Pasta"), "C:/Imagens/Teste");
    QDir dir(imgPath);
    dir.mkdir("Descritores");
    dir.cd("Descritores");
    savePath = dir.path();
    dir.cdUp();

    ui->label_ImagePath->setText(imgPath);
}

void MainWindow::extraiCaracteristica(){
    QProgressDialog pross("Extraindo Caracteriticas...","Cancel",0, 5, this);
    pross.setWindowModality(Qt::WindowModal);
    pross.setValue(1);
    glcm();

    pross.setValue(2);
    fractais();

    pross.setValue(3);
    dadosEstatisicos();

    pross.setValue(4);
    dadosLBP();

    pross.setValue(5);
}

void MainWindow::glcm(){
    MatrizCooCorrencia glcm(ui->spinBox_Distancia->value());

    glcm.geraDados(buscaImagens(),ui->checkBox_Energia->isChecked(),ui->checkBox_Entropia->isChecked(),
                   ui->checkBox_Contraste->isChecked(),ui->checkBox_Correlacao->isChecked(),
                   ui->checkBox_Homogeneidade->isChecked(),ui->checkBox_Variancia_X->isChecked());
    glcm.normaliza();
    glcm.salvaDados(savePath.toStdString());
    glcm.imprimeArff("GLCM_Simpsons_Arff",savePath.toStdString());
    glcm.snns(savePath.toStdString());
    glcm.svm(savePath.toStdString());
}

void MainWindow::fractais(){
    Fractais fract;
    fract.fractalImage(buscaImagens(),ui->spinBox_Pressao->value(),ui->checkBox_BoxCounting->isChecked(),
                       ui->checkBox_DiferencialBC->isChecked(), ui->checkBox_Sucolaridade->isChecked());
    fract.normaliza();
    fract.salvaDados(savePath.toStdString());
    fract.imprimeArff("GLCM_Simpsons_Arff",savePath.toStdString());
    fract.snns(savePath.toStdString());
    fract.svm(savePath.toStdString());
}

void MainWindow::dadosEstatisicos(){
    Estatisitica est;
    est.geraDados(buscaImagens(),ui->checkBox_Entropy->isChecked(), ui->checkBox_Kurtosis->isChecked(),
                  ui->checkBox_Min->isChecked(), ui->checkBox_Max->isChecked(),ui->checkBox_Mean->isChecked(),
                  ui->checkBox_Mode->isChecked(),ui->checkBox_Skewness->isChecked(),
                  ui->checkBox_StandardDeviation->isChecked(), ui->checkBox_Variance->isChecked());

    est.normaliza();
    est.salvaDados(savePath.toStdString());
    est.imprimeArff("Simpsons", savePath.toStdString());
    est.svm(savePath.toStdString());
    est.snns(savePath.toStdString());
}

void MainWindow::dadosLBP(){
    if(ui->checkBox_LBP->isChecked()){
        QDir temp("c:/");
        temp.mkdir("temp");
        LPB lbp;
        lbp.geraDados(imgPath.toStdString(), savePath.toStdString());
        lbp.imprimeArff("Simpsons",savePath.toStdString());
        lbp.salvaDados(savePath.toStdString());
        lbp.snns(savePath.toStdString());
        lbp.svm(savePath.toStdString());
        temp.remove("c:/temp/ok.dados");
        temp.rmdir("c:/temp");
    }
}

void MainWindow::on_pushButton_Calcular_clicked()
{
    extraiCaracteristica();
}

void MainWindow::on_pB_selectDir_clicked()
{
    QFileDialog file;
    selectPath = file.getExistingDirectory(this,tr("Escolha a Pasta"), savePath);
    ui->label_selectPath->setText(selectPath);
    QDir dir(selectPath, "*.dados");
    ui->listWidget->addItems(dir.entryList());
    ui->listWidget->sortItems();
}

void MainWindow::on_buttonBox_accepted()
{
    if(!ui->listWidget->selectedItems().empty()){
        QString auxPath = selectPath + "/" + ui->listWidget->currentItem()->text();
        Classificadores cls(auxPath.toStdString());
        if(ui->radioButton_Knn->isChecked()){
            cls.knn(ui->spinBox_Vizinhos->value(),selectPath.toStdString());
        }else if(ui->radioButton_Perceptron->isChecked()){
            cls.percetron(ui->spinBox_Eras->value(),selectPath.toStdString());
        }
    }
}

void MainWindow::on_buttonBox_rejected()
{
    this->close();
}

void MainWindow::kmeansImage()
{
//    MatrizConfusaoSVM svm;
//    svm.acuracia("C:/Users/Marcela/Documents/8p/PDI/libsvm-3.21/tools/testeLbpFelibSVM.txt","C:/Users/Marcela/Documents/8p/PDI/libsvm-3.21/Resultados/testeLbpFelibSVMLbpFelibSVM.txt.predict");
    QPixmap pix;
    savePath = QFileDialog::getSaveFileName(this,tr("Escolha seu arquivo"), savePath,"Imagens *.jpg; *.png; *.tif; *.bmp");
    if(!savePath.isEmpty()){
        Kmeans med(kmeansPath.toStdString());
        int x = ui->spinBox_Clusters->value();
        med.clusters(x);
        Mat auxImg = med.saveImg(savePath.toStdString());
        QImage img = QImage(auxImg.data,auxImg.cols, auxImg.rows, auxImg.step, QImage::Format_RGB888);
        QSize s(200, 200*((double)img.height()/(double)img.width()));

        pix = QPixmap::fromImage(img.scaled(s));
        ui->label_ImgKmeans->setPixmap(pix);
        ui->label_ImgKmeans->resize(pix.size());
        ui->label_ImgKmeans->setAlignment(Qt::AlignRight);
    }
}

void MainWindow::on_pB_selectFileKmeans_clicked()
{
    kmeansPath = QFileDialog::getOpenFileName(this,tr("Escolha seu arquivo"), savePath,"Imagens *.jpg; *.png; *.tif; *.bmp");
    ui->label_selectFileKmeans->setText(kmeansPath);
    QImage imgAux(kmeansPath);
    QSize s(200, 200*((double)imgAux.height()/(double)imgAux.width()));
    QPixmap pix = QPixmap::fromImage(imgAux.scaled(s));
    ui->label_ImgOriginal->setPixmap(pix);
    ui->label_ImgOriginal->resize(pix.size());
    ui->label_ImgOriginal->setAlignment(Qt::AlignLeft);
}

void MainWindow::on_buttonBox_2_accepted()
{
    kmeansImage();
}

void MainWindow::on_buttonBox_2_rejected()
{
    this->close();
}

void MainWindow::on_pB_SvmTeste_clicked()
{
    svmTeste = QFileDialog::getOpenFileName(this,tr("Escolha seu arquivo"), savePath,"Arquivos *.txt");
    ui->label_svmTeste->setText(svmTeste);
}

void MainWindow::on_pB_predict_clicked()
{
    svmPredict = QFileDialog::getOpenFileName(this,tr("Escolha seu arquivo"), savePath,"Arquivos *.predict");
    ui->label_svmPredict->setText(svmPredict);
}

void MainWindow::on_buttonBox_3_accepted()
{
    MatrizConfusaoSVM confusao;
    double acuracia = confusao.acuracia(svmTeste.toStdString(),svmPredict.toStdString());
    ui->textEdit->append("Acurária: " + QString::number(acuracia) + "%");
}
