#include "estatisitica.h"

Estatisitica::Estatisitica()
{
    type = "Estatisticos";
}

Mat Estatisitica::getImage() const
{
    return image;
}

void Estatisitica::setImage(const Mat &value)
{
    type = "Estatisticos";
    image = value;
}

Scalar Estatisitica::entropy()
{
    std::vector<cv::Mat> channels;
    cv::split( image, channels );

    int histSize = 256;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist0, hist1, hist2;

    cv::calcHist( &channels[0], 1, 0, cv::Mat(), hist0, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[1], 1, 0, cv::Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[2], 1, 0, cv::Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate );

    //frequency
    float f0=0, f1=0, f2=0;
    for (int i=0;i<histSize;i++)
    {
        f0+=hist0.at<float>(i);
        f1+=hist1.at<float>(i);
        f2+=hist2.at<float>(i);
    }

    //entropy
    cv::Scalar e;
    e.val[0]=0;
    e.val[1]=0;
    e.val[2]=0;
    // e0=0, e1=0, e2=0;
    float p0, p1, p2;

    for (int i=0;i<histSize;i++)
    {
        p0=abs(hist0.at<float>(i))/f0;
        p1=abs(hist1.at<float>(i))/f1;
        p2=abs(hist2.at<float>(i))/f2;
        if (p0!=0)
            e.val[0]+=-p0*log10(p0);
        if (p1!=0)
            e.val[1]+=-p1*log10(p1);
        if (p2!=0)
            e.val[2]+=-p2*log10(p2);
    }

    return e;
}

Scalar Estatisitica::kurtosis()
{
    cv::Scalar kurt,mean,stddev;
    kurt.val[0]=0;
    kurt.val[1]=0;
    kurt.val[2]=0;
    meanStdDev(image,mean,stddev,cv::Mat());
    int sum0, sum1, sum2;
    int N=image.rows*image.cols;
    float den0=0,den1=0,den2=0;

    for (int i=0;i<image.rows;i++)
    {
        for (int j=0;j<image.cols;j++)
        {
            sum0=image.ptr<uchar>(i)[3*j]-mean.val[0];
            sum1=image.ptr<uchar>(i)[3*j+1]-mean.val[1];
            sum2=image.ptr<uchar>(i)[3*j+2]-mean.val[2];

            kurt.val[0]+=sum0*sum0*sum0*sum0;
            kurt.val[1]+=sum1*sum1*sum1*sum1;
            kurt.val[2]+=sum2*sum2*sum2*sum2;
            den0+=sum0*sum0;
            den1+=sum1*sum1;
            den2+=sum2*sum2;
        }
    }

    kurt.val[0]= (kurt.val[0]*N*(N+1)*(N-1)/(den0*den0*(N-2)*(N-3)))-(3*(N-1)*(N-1)/((N-2)*(N-3)));
    kurt.val[1]= (kurt.val[1]*N/(den1*den1))-3;
    kurt.val[2]= (kurt.val[2]*N/(den2*den2))-3;

    return kurt;
}

Scalar Estatisitica::min()
{
    cv::Scalar min;
    min.val[0]=image.ptr<uchar>(0)[0];
    min.val[1]=image.ptr<uchar>(0)[1];
    min.val[2]=image.ptr<uchar>(0)[2];

    for (int i=0;i<image.rows;i++)
        for (int j=0;j<image.cols;j++)
        {
            if(min.val[0]>image.ptr<uchar>(i)[3*j])
                min.val[0]=image.ptr<uchar>(i)[3*j];
            if(min.val[1]>image.ptr<uchar>(i)[3*j+1])
                min.val[1]=image.ptr<uchar>(i)[3*j+1];
            if(min.val[2]>image.ptr<uchar>(i)[3*j+2])
                min.val[2]=image.ptr<uchar>(i)[3*j+2];
        }
    return min;
}

Scalar Estatisitica::max()
{
    cv::Scalar max;
    max.val[0]=image.ptr<uchar>(0)[0];
    max.val[1]=image.ptr<uchar>(0)[1];
    max.val[2]=image.ptr<uchar>(0)[2];

    for (int i=0;i<image.rows;i++)
        for (int j=0;j<image.cols;j++)
        {
            if(max.val[0]<image.ptr<uchar>(i)[3*j])
                max.val[0]=image.ptr<uchar>(i)[3*j];
            if(max.val[1]<image.ptr<uchar>(i)[3*j+1])
                max.val[1]=image.ptr<uchar>(i)[3*j+1];
            if(max.val[2]<image.ptr<uchar>(i)[3*j+2])
                max.val[2]=image.ptr<uchar>(i)[3*j+2];
        }
    return max;
}

Scalar Estatisitica::means()
{
    return mean(image);
}

Scalar Estatisitica::median()
{
    double m=(image.rows*image.cols)/2;
    int bin0=0, bin1=0, bin2=0;
    cv::Scalar med;
    med.val[0]=-1;
    med.val[1]=-1;
    med.val[2]=-1;
    int histSize = 256;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist0, hist1, hist2;
    std::vector<cv::Mat> channels;
    cv::split( image, channels );
    cv::calcHist( &channels[0], 1, 0, cv::Mat(), hist0, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[1], 1, 0, cv::Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[2], 1, 0, cv::Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate );

    for (int i=0; i<256 && ( med.val[0]<0 || med.val[1]<0 || med.val[2]<0);i++)
    {
        bin0=bin0+cvRound(hist0.at<float>(i));
        bin1=bin1+cvRound(hist1.at<float>(i));
        bin2=bin2+cvRound(hist2.at<float>(i));
        if (bin0>m && med.val[0]<0)
            med.val[0]=i;
        if (bin1>m && med.val[1]<0)
            med.val[1]=i;
        if (bin2>m && med.val[2]<0)
            med.val[2]=i;
    }

    return med;
}

Scalar Estatisitica::mode()
{
    double m=(image.rows*image.cols)/2;
    int bin0=0, bin1=0, bin2=0;
    cv::Scalar mode;
    mode.val[0]=-1;
    mode.val[1]=-1;
    mode.val[2]=-1;
    int histSize = 256;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist0, hist1, hist2;
    std::vector<cv::Mat> channels;
    cv::split( image, channels );
    cv::calcHist( &channels[0], 1, 0, cv::Mat(), hist0, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[1], 1, 0, cv::Mat(), hist1, 1, &histSize, &histRange, uniform, accumulate );
    cv::calcHist( &channels[2], 1, 0, cv::Mat(), hist2, 1, &histSize, &histRange, uniform, accumulate );

    for (int i=0; i<256 ;i++)
    {
        if (bin0<cvRound(hist0.at<float>(i)))
        {
            bin0=cvRound(hist0.at<float>(i));
            mode.val[0]=i;
        }
        if (bin1<cvRound(hist1.at<float>(i)))
        {
            bin1=cvRound(hist1.at<float>(i));
            mode.val[1]=i;
        }
        if (bin2<cvRound(hist2.at<float>(i)))
        {
            bin2=cvRound(hist2.at<float>(i));
            mode.val[2]=i;
        }
    }

    return mode;
}

Scalar Estatisitica::skewness()
{
    cv::Scalar skewness,mean,stddev;
    skewness.val[0]=0;
    skewness.val[1]=0;
    skewness.val[2]=0;
    meanStdDev(image,mean,stddev,cv::Mat());
    int sum0, sum1, sum2;
    float den0=0,den1=0,den2=0;
    int N=image.rows*image.cols;

    for (int i=0;i<image.rows;i++)
    {
        for (int j=0;j<image.cols;j++)
        {
            sum0=image.ptr<uchar>(i)[3*j]-mean.val[0];
            sum1=image.ptr<uchar>(i)[3*j+1]-mean.val[1];
            sum2=image.ptr<uchar>(i)[3*j+2]-mean.val[2];

            skewness.val[0]+=sum0*sum0*sum0;
            skewness.val[1]+=sum1*sum1*sum1;
            skewness.val[2]+=sum2*sum2*sum2;
            den0+=sum0*sum0;
            den1+=sum1*sum1;
            den2+=sum2*sum2;
        }
    }

    skewness.val[0]=skewness.val[0]*sqrt(N)/(den0*sqrt(den0));
    skewness.val[1]=skewness.val[1]*sqrt(N)/(den1*sqrt(den1));
    skewness.val[2]=skewness.val[2]*sqrt(N)/(den2*sqrt(den2));

    return skewness;
}

Scalar Estatisitica::standardDeviation()
{
    cv::Scalar mean,stddev;

    meanStdDev(image,mean,stddev,cv::Mat());

    return stddev;
}

Scalar Estatisitica::variance()
{
    Scalar variancie = standardDeviation();
    int i = 0;
    for(auto v:variancie.val){
        variancie.val[i++] = v*v;
    }
    return variancie;
}

void Estatisitica::geraDados(vector<string> img,bool e, bool k, bool mi, bool ma, bool me, bool mo, bool sk, bool st, bool va)
{
    for(auto s:img){
        DadosImagens dados;
        image = imread(s);
        dados.setImagem(s);
        dados.setClasse(explode(*explode(s,'/').rbegin(),'_')[0]);
        if(e){
            Scalar en = entropy();
            dados.addAtributo("entropia_01",en.val[0]);
            dados.addAtributo("entropia_02",en.val[1]);
            dados.addAtributo("entropia_03",en.val[2]);
        }
        if(k){
            Scalar kurt = kurtosis();
            dados.addAtributo("kurtosis_01",kurt.val[0]);
            dados.addAtributo("kurtosis_02",kurt.val[1]);
            dados.addAtributo("kurtosis_03",kurt.val[2]);
        }
        if(mi){
            Scalar m = min();
            dados.addAtributo("min_01",m.val[0]);
            dados.addAtributo("min_02",m.val[1]);
            dados.addAtributo("min_03",m.val[2]);
        }
        if(ma){
            Scalar m = max();
            dados.addAtributo("max_01",m.val[0]);
            dados.addAtributo("max_02",m.val[1]);
            dados.addAtributo("max_03",m.val[2]);
        }
        if(me){
            Scalar m = median();
            dados.addAtributo("median_01",m.val[0]);
            dados.addAtributo("median_02",m.val[1]);
            dados.addAtributo("median_03",m.val[2]);
        }
        if(mo){
            Scalar m = mode();
            dados.addAtributo("moda_01",m.val[0]);
            dados.addAtributo("moda_02",m.val[1]);
            dados.addAtributo("moda_03",m.val[2]);
        }
        if(sk){
            Scalar ske = skewness();
            dados.addAtributo("skewness_01",ske.val[0]);
            dados.addAtributo("skewness_02",ske.val[1]);
            dados.addAtributo("skewness_03",ske.val[2]);
        }

        if(st){
            Scalar stdD = standardDeviation();
            dados.addAtributo("standardDeviation_01",stdD.val[0]);
            dados.addAtributo("standardDeviation_02",stdD.val[1]);
            dados.addAtributo("standardDeviation_03",stdD.val[2]);
        }
        if(va){
            Scalar vari = variance();
            dados.addAtributo("variance_01",vari.val[0]);
            dados.addAtributo("variance_02",vari.val[1]);
            dados.addAtributo("variance_03",vari.val[2]);
        }
        images.push_back(dados);
    }
}
