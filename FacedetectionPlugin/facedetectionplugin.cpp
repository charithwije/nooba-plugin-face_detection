#include "facedetectionplugin.h"
#include <QtCore>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QString>

//QFile file("/home/charith/Programming/FYP/Plugins/out-files/out3.txt");

QFile file(QCoreApplication::applicationDirPath()+"/Detected_face_coordinates.txt");
QTextStream out(&file);

FacedetectionPlugin::FacedetectionPlugin()
{

}

FacedetectionPlugin::~FacedetectionPlugin()
{

}

bool FacedetectionPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{

    frame_number_local++;
    cv::cvtColor(in, out, CV_BGR2GRAY);
    //cv::imshow( "face---", out );

    b =face_cascade.load(lbpFaceCascadePath);

    if(!b)
    {
        qDebug("--(!)Error loading\n"); return false;
    }
    //if( !face_cascade2.load(haarFaceCascadePath)){ cout<<"--(!)Error loading\n"; return false; };

    if( !eyes_cascade.load(eyeCascadePath) )
    {
        qDebug("--(!)Error loading\n"); return false;
    }



    cv::equalizeHist( out, out );
    //  cv::cvtColor(out, out, CV_GRAY2BGR);


    //        //-- Detect faces
    face_cascade.detectMultiScale(out, faces );

    /*face_cascade2.detectMultiScale( out, faces2, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(10, 10), cv::Size(40, 40) );

        faces.insert( faces.end(), faces2.begin(), faces2.end() );

//        std::vector<cv::Rect>::iterator it;
//        it = std::unique (faces.begin(), faces.end());

//        faces.resize(std::distance(faces.begin(),it) );*/

    for( int i = 0; i < faces.size(); i++ )
    {
        cv::Rect face(faces[i].x ,faces[i].y, faces[i].width , faces[i].height );

        // write to output text filet

        bool result = FacedetectionPlugin::writeToFile(frame_number_local,faces[i].x ,faces[i].y, faces[i].width , faces[i].height);

        cv::rectangle(out,face,cv::Scalar(255,255,51),2);
        cv::Mat faceROI = out(faces[i] );
        std::vector<cv::Rect> eyes;

        // imshow( "face---", faceROI );
        // cv::waitKey(20);


        //-- In each face, detect eyes
        //eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(30, 30) );

        // to mark the eyes on a face

        //            for( int j = 0; j < eyes.size(); j++ )
        //            {
        //                cv::Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
        //                int radius =cvRound( (eyes[j].width + eyes[j].height)*0.25 );
        //                cv::circle( out, center, radius, cv::Scalar( 255, 200, 0 ), 1, 8, 0 );
        //            }
    }
    //  cv::cvtColor(out,out,CV_GRAY2BGR);
    //-- Show what you got
    //    cv::imshow( "face---",out);
    //    cv::waitKey(20);
    return true;
    file.close();
}


bool FacedetectionPlugin::writeToFile(int frameNoLocal, int x, int y, int width, int height )
{
    try{
        out << frameNoLocal <<","<< x << ","<< y << ","<< width << ","<< height <<"\n";
    }

    catch(QException e){

        debugMsg(e.what());
        return false;
    }
    return true;
}






bool FacedetectionPlugin::init()
{
    haarFaceCascadePath= "/home/charith/Programming/FYP/Plugins/data/haarcascades/haarcascade_frontalface_default.xml";
    lbpFaceCascadePath = "/home/charith/Programming/FYP/Plugins/data/lbpcascades/lbpcascade_frontalface.xml";
    eyeCascadePath = "/home/charith/Programming/FYP/Plugins/data/haarcascades/haarcascade_eye.xml";

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    file.open(QIODevice::Append | QIODevice::Text);

    out << "%"<<asctime(timeinfo)<<"%======= Face details (Frame number, x, y, width, height)======= \n";

    return true;
}






bool FacedetectionPlugin::release()
{
    return true;
}




PluginInfo FacedetectionPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
                "Facedetection Plugin",
                0,
                1,
                "Plugin Description goes here",
                "Plugin Creator");
    return pluginInfo;
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(FacedetectionPlugin, FacedetectionPlugin);
#endif
