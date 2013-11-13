#ifndef FACEDETECTIONPLUGIN_H
#define FACEDETECTIONPLUGIN_H

#include "facedetectionplugin_global.h"
#include "noobapluginapi.h"
#include "package_bgs/StaticFrameDifferenceBGS.h"

#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QtGui/QImage>





class FACEDETECTIONPLUGIN_EXPORT FacedetectionPlugin: public NoobaPluginAPI
{
    Q_OBJECT
    Q_INTERFACES(NoobaPluginAPI)
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "nooba.plugins.qt5.facedetection-plugin" FILE "facedetectionPlugin.json")
#endif

public:
    FacedetectionPlugin();
    ~FacedetectionPlugin();

    bool procFrame(const cv::Mat &in, cv::Mat &out, ProcParams &params);
    bool init();
    bool release();
    bool writeToFile(int,int,int,int,int);

    PluginInfo getPluginInfo() const;

private:

       // QFile file;

        std::vector<cv::Rect> faces;
        std::vector<cv::Rect> faces2;
        std::vector<cv::Rect> faces3;

        cv::CascadeClassifier face_cascade;
        cv::CascadeClassifier face_cascade2;
        cv::CascadeClassifier eyes_cascade;
        cv::Mat img_mask;
        cv::Mat mat1 ;
        StaticFrameDifferenceBGS bgs;

      //  QTextStream out;
        QImage img;

        int frame_number_local;
        std::string haarFaceCascadePath;
        std::string lbpFaceCascadePath;
        std::string eyeCascadePath;
        std::string pedestrianCascade;

        bool b;


    /**
     * These functions will be called when the parameters are changed by the
     * user.
     */
//    void onIntParamChanged(const QString& varName, int val);
//    void onDoubleParamChanged(const QString& varName, double val);
//    void onStringParamChanged(const QString& varName, const QString& val);
//    void onMultiValParamChanged(const QString& varName, const QString& val);


};

#endif // FACEDETECTIONPLUGIN_H
