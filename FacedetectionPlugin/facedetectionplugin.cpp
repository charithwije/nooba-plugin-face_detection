#include "facedetectionplugin.h"
#include <QtCore>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QString>

//QFile file("/home/charith/Programming/FYP/Plugins/out-files/out3.txt");


QFile file2(QCoreApplication::applicationDirPath()+"/initial_frame.png");

FacedetectionPlugin::FacedetectionPlugin()
{


}

FacedetectionPlugin::~FacedetectionPlugin()
{

}

bool FacedetectionPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{

    frame_number_local++;
    img_mask = in.clone();

   mat1 = cv::imread("/home/charith/Programming/FYP/Plugins/initialimg2.png",3);

    cv::cvtColor(in, out, CV_BGR2GRAY);

    //cv::imshow( "face---", out );

   // bgs.process(in,img_mask,mat1);

     //imshow( "img mask", img_mask );
     //  cv::waitKey(20);

    b =face_cascade.load(haarFaceCascadePath);

    if(!b)
    {
        qDebug("--(!)Error loading\n"); return false;
    }
    if( !face_cascade2.load(haarFaceCascadePath))
    {
        qDebug("--(!)Error loading\n");
        return false;
    }

    if( !eyes_cascade.load(eyeCascadePath) )
    {
        qDebug("--(!)Error loading\n"); return false;
    }



    cv::equalizeHist( out, out );
    //  cv::cvtColor(out, out, CV_GRAY2BGR);


    //        //-- Detect faces
    //face_cascade.detectMultiScale(out, faces,1.1,3,CV_HAAR_SCALE_IMAGE,cvSize(10,10),cvSize(50,50) );
    face_cascade.detectMultiScale(out, faces,1.1);

//    face_cascade2.detectMultiScale( out, faces2, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(10, 10), cv::Size(40, 40) );

//        faces.insert( faces.end(), faces2.begin(), faces2.end() );

//        std::vector<cv::Rect>::iterator it;
//        it = std::unique (faces.begin(), faces.end());

//        faces.resize(std::distance(faces.begin(),it) );
cv::cvtColor(out,out,CV_GRAY2BGR);

    for( int i = 0; i < faces.size(); i++ )
    {
        cv::Rect face(faces[i].x ,faces[i].y, faces[i].width , faces[i].height );

        // write to output text filet

        result = FacedetectionPlugin::writeToFile(frame_number_local,faces[i].x ,faces[i].y, faces[i].width , faces[i].height);

        cv::putText(out, QString("(%1,%2)").arg(faces[i].x).arg(faces[i].y).toStdString(), cv::Point(faces[i].x,faces[i].y), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
        cv::rectangle(out,face,cv::Scalar(255,255,51),2);
        cv::Mat faceROI = out(faces[i] );

        //std::vetor<cv::Rect> eyes;

        imshow( "face---", faceROI );
        cv::waitKey(20);

        //         IplImage copy=faceROI;
        //        IplImage* savingImg = &copy;

        QString s = QString(outputPath+"/face_results%1.png").arg(frame_number_local);


        cv::imwrite(s.toStdString(),faceROI);

        NoobaPluginAPI::debugMsg(QString("detected face %1,at (%2,%3)").arg(frame_number_local).arg(faces[i].x).arg(faces[i].y));


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

    return true;


}


bool FacedetectionPlugin::writeToFile(int frameNoLocal, int x, int y, int width, int height )
{

    try{
        out << frameNoLocal <<","<< x << ","<< y << ","<< width << ","<< height <<"\n";
        out.flush();
    }

    catch(QException e){

        debugMsg(e.what());
        return false;
    }
    return true;
}


bool FacedetectionPlugin::init()
{
    frame_number_local=0;
    haarFaceCascadePath= "/home/charith/Programming/FYP/Plugins/data/haarcascades/haarcascade_frontalface_alt2.xml";
    lbpFaceCascadePath = "/home/charith/Programming/FYP/Plugins/data/lbpcascades/lbpcascade_frontalface.xml";
    eyeCascadePath = "/home/charith/Programming/FYP/Plugins/data/haarcascades/haarcascade_eye.xml";
    pedestrianCascade="/home/charith/Programming/FYP/Plugins/data/hogcascades/hogcascadepedestrians.xml";

    createStringParam("outputImagePath","Detected Faces",true);
    createStringParam("outputImageCordinatesPath","Dtected Face Cordinates",true);


    file.setFileName(QCoreApplication::applicationDirPath()+"/face_cordinates");

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    file.open(QIODevice::Append | QIODevice::Text);

    out.setDevice(&file);
    out << "%"<<asctime(timeinfo)<<"%======= Face details (Frame number, x, y, width, height)======= \n";

    return true;
}

void FacedetectionPlugin::onStringParamChanged(const QString& varName, const QString& val){

  //  qDebug("signal came");

    if (varName.compare("outputImagePath") == 0) {
        outputPath=val;
    }

    if (varName.compare("outputImageCordinatesPath") == 0) {
        qDebug("in second");
    outCordinates = val;

    }


}




bool FacedetectionPlugin::release()
{

    file.close();
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
