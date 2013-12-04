#include "facedetectionplugin.h"
#include "blobobject.h"
#include <QtCore>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QTime>

QFile file2(QCoreApplication::applicationDirPath()+"/initial_frame.png");

FacedetectionPlugin::FacedetectionPlugin()
{


}

FacedetectionPlugin::~FacedetectionPlugin()
{

}

bool FacedetectionPlugin::procFrame( const cv::Mat &in, cv::Mat &out, ProcParams &params )
{

    QTime t;
    frame_number_local++;
    img_mask = in.clone();

//    mat1 = cv::imread("/home/charith/Programming/FYP/Plugins/initialimg2.png",3);

    cv::cvtColor(in, out, CV_BGR2GRAY);

    //cv::imshow( "face---", out );
    // bgs.process(in,img_mask,mat1);
    //imshow( "img mask", img_mask );
    //  cv::waitKey(20);
    t.start();

    if(!b)
    {
        qDebug() << "--(!)Error loading\n" << Q_FUNC_INFO;
        return false;
    }
//    if( !face_cascade2.load(haarFaceCascadePath))
//    {
//        qDebug("--(!)Error loading\n");
//        return false;
//    }

//    if( !eyes_cascade.load(eyeCascadePath) )
//    {
//        qDebug("--(!)Error loading\n"); return false;
//    }



    cv::equalizeHist( out, out );
    //  cv::cvtColor(out, out, CV_GRAY2BGR);


    //        //-- Detect faces
    //face_cascade.detectMultiScale(out, faces,1.1,3,CV_HAAR_SCALE_IMAGE,cvSize(10,10),cvSize(50,50) );
    face_cascade.detectMultiScale(out, faces,1.1,3,CV_HAAR_SCALE_IMAGE,cv::Size(20, 20));

    //    face_cascade2.detectMultiScale( out, faces2, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(10, 10), cv::Size(40, 40) );

    //        faces.insert( faces.end(), faces2.begin(), faces2.end() );

    //        std::vector<cv::Rect>::iterator it;
    //        it = std::unique (faces.begin(), faces.end());

    //        faces.resize(std::distance(faces.begin(),it) );
    cv::cvtColor(out,out,CV_GRAY2BGR);
   // debugMsg(QString("time spent for process  %1").arg(t.elapsed()));

   // t.start();
    for( int i = 0; i < faces.size(); i++ )
    {
        cv::Rect face(faces[i].x ,faces[i].y, faces[i].width , faces[i].height );

        // write to output text filet

        result = FacedetectionPlugin::writeToFile(frame_number_local,faces[i].x ,faces[i].y, faces[i].width , faces[i].height);

        cv::putText(out, QString("(%1,%2)").arg(faces[i].x).arg(faces[i].y).toStdString(), cv::Point(faces[i].x,faces[i].y), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(250,255,0));
        cv::rectangle(out,face,cv::Scalar(255,255,51),2);
        faceROI = out(faces[i] );

        QImage passingImg=convertToQImage(faceROI);
        imgList.append(passingImg);
        //std::vetor<cv::Rect> eyes;

        /* imshow( "face", faceROI );  // showing the cropped face
        cv::waitKey(20);
        cv::moveWindow("face",5,5 ); // sets the position of the cropped face
        */

        //         IplImage copy=faceROI;
        //        IplImage* savingImg = &copy;

        QString s = QString(imgOutputValue+"/face_results%1.png").arg(frame_number_local);


        bool bw=  cv::imwrite(s.toStdString(),faceROI);

        NoobaPluginAPI::debugMsg(QString("detected face in frame %1 ,at (%2,%3)").arg(frame_number_local).arg(faces[i].x).arg(faces[i].y));

//        QString params="(%1,%2)";
//        params.arg(faces[i].x).arg(faces[i].y);



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
  //  debugMsg(QString("time spent for writing %1").arg(t.elapsed()));




   // passingStringList.append(QString("%1").arg(faces.size()));
   // passingStringList.append(QString("%1").arg(frame_number_local));
   // passingData.setStrList(passingStringList);
    QStringList list;
    list.append(QString("%1").arg(faces.size()));
    list.append(QString("%1").arg(frame_number_local));
    outputData(list,imgList);
  //  passingStringList.clear();
    // outputData(passingData);
    QImage img = convertToQImage(out);
    updateFrameViewer("Output",img);

    return true;
}




bool FacedetectionPlugin::processFrame(const cv::Mat &in)
{

    QTime t;
    frame_number_local++;
    img_mask = in.clone();

    cv::Mat out;

//    mat1 = cv::imread("/home/charith/Programming/FYP/Plugins/initialimg2.png",3);

    cv::cvtColor(in, out, CV_BGR2GRAY);

    //cv::imshow( "face---", out );
    // bgs.process(in,img_mask,mat1);
    //imshow( "img mask", img_mask );
    //  cv::waitKey(20);
    t.start();

    if(!b)
    {
        qDebug() << "--(!)Error loading\n" << Q_FUNC_INFO;
        return false;
    }
//    if( !face_cascade2.load(haarFaceCascadePath))
//    {
//        qDebug("--(!)Error loading\n");
//        return false;
//    }

//    if( !eyes_cascade.load(eyeCascadePath) )
//    {
//        qDebug("--(!)Error loading\n"); return false;
//    }



    cv::equalizeHist( out, out );
    //  cv::cvtColor(out, out, CV_GRAY2BGR);

    cv::Mat blobROI ;

    for(int i=0;i<FacedetectionPlugin::bloblist.size();i++){

        //cv::Rect temp(bloblist.at(i)->getBlobTopLeftX() ,bloblist.at(i)->getBlobTopLeftY(),bloblist.at(i)->getBlobWidth() , bloblist.at(i)->getBlobHeight());

        cv::Rect t(bloblist.at(i)->getBlobTopLeftX(),bloblist.at(i)->getBlobTopLeftY(), bloblist.at(i)->getBlobWidth(), bloblist.at(i)->getBlobHeight());
  //      QString t2=QString("x= %1 , y = %2, width= %3 , height = %4 ").arg(bloblist.at(i)->getBlobTopLeftX()).arg(bloblist.at(i)->getBlobTopLeftY()).arg(bloblist.at(i)->getBlobWidth()).arg(bloblist.at(i)->getBlobHeight());
      //  QString t1=QString("rows = %1 , cols = %2 ").arg(out.rows).arg(out.cols);
        //debugMsg(t2);
       // debugMsg(t1);

        if ((bloblist.at(i)->getBlobTopLeftX()+bloblist.at(i)->getBlobWidth()) <=704)
        {
            if((bloblist.at(i)->getBlobTopLeftY()+bloblist.at(i)->getBlobHeight()) <=640)
            {
                 blobROI = out(t);

                 face_cascade.detectMultiScale(blobROI, faces,1.1);

            }
        }
       cv::imshow("blob",blobROI);
        //cv::waitKey(10);
    }




    //        //-- Detect faces
    //face_cascade.detectMultiScale(out, faces,1.1,3,CV_HAAR_SCALE_IMAGE,cvSize(10,10),cvSize(50,50) );
    //face_cascade.detectMultiScale(out, faces,1.1);

    //    face_cascade2.detectMultiScale( out, faces2, 1.1, 2, CV_HAAR_SCALE_IMAGE, cv::Size(10, 10), cv::Size(40, 40) );

    //        faces.insert( faces.end(), faces2.begin(), faces2.end() );

    //        std::vector<cv::Rect>::iterator it;
    //        it = std::unique (faces.begin(), faces.end());

    //        faces.resize(std::distance(faces.begin(),it) );
    cv::cvtColor(out,out,CV_GRAY2BGR);
   // debugMsg(QString("time spent for process  %1").arg(t.elapsed()));

   // t.start();
    for( int i = 0; i < faces.size(); i++ )
    {
        cv::Rect face(faces[i].x ,faces[i].y, faces[i].width , faces[i].height );

        // write to output text filet

        result = FacedetectionPlugin::writeToFile(frame_number_local,faces[i].x ,faces[i].y, faces[i].width , faces[i].height);

        cv::putText(out, QString("(%1,%2)").arg(faces[i].x).arg(faces[i].y).toStdString(), cv::Point(faces[i].x,faces[i].y), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(250,255,0));
        cv::rectangle(out,face,cv::Scalar(255,255,51),2);
        cv::Mat faceROI = out(faces[i] );

        QImage passingImg=convertToQImage(faceROI);
        imgList.append(passingImg);

        //std::vetor<cv::Rect> eyes;

        /* imshow( "face", faceROI );  // showing the cropped face
        cv::waitKey(20);
        cv::moveWindow("face",5,5 ); // sets the position of the cropped face
        */

        //         IplImage copy=faceROI;
        //        IplImage* savingImg = &copy;

        QString s = QString(imgOutputValue+"/face_results%1.png").arg(frame_number_local);


        bool bw=  cv::imwrite(s.toStdString(),faceROI);

        NoobaPluginAPI::debugMsg(QString("detected face in frame %1 ,at (%2,%3)").arg(frame_number_local).arg(faces[i].x).arg(faces[i].y));

//        QString params="(%1,%2)";
//        params.arg(faces[i].x).arg(faces[i].y);



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
  //  debugMsg(QString("time spent for writing %1").arg(t.elapsed()));

   // passingStringList.append(QString("%1").arg(faces.size()));
   // passingStringList.append(QString("%1").arg(frame_number_local));
    //passingData.setStrList(passingStringList);

    QStringList list;
    list.append(QString("%1").arg(faces.size()));
    list.append(QString("%1").arg(frame_number_local));
    outputData(list,imgList);

    passingData.setImage(out);
  //  passingStringList.clear();
   // outputData(passingData);
    QImage img((uchar*)out.data, out.cols, out.rows, out.step1(), QImage::Format_RGB888);
    updateFrameViewer("Output",img);

    return true;

}


QImage FacedetectionPlugin::convertToQImage(const cv::Mat &cvImg)
{
    if (cvImg.channels()== 1){
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_Indexed8);
        return img;
    }
    else{
        QImage img((uchar*)cvImg.data, cvImg.cols, cvImg.rows, cvImg.step1(), QImage::Format_RGB888);
        return img;
    }
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
    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("config")){
        dir.mkdir("config");
    }
    dir.cd("config");


    frame_number_local=0;
    haarFaceCascadePath = dir.absoluteFilePath("haarcascade_frontalface_alt2.xml").toStdString();
    copyFile("://resources/haarcascades/haarcascade_frontalface_alt2.xml", dir.absoluteFilePath("haarcascade_frontalface_alt2.xml"));
    lbpFaceCascadePath = dir.absoluteFilePath("lbpcascade_frontalface.xml").toStdString();
    copyFile("://resources/lbpcascades/lbpcascade_frontalface.xml", dir.absoluteFilePath("lbpcascade_frontalface.xml"));
    eyeCascadePath = "/home/charith/Programming/FYP/Plugins/data/haarcascades/haarcascade_eye.xml";
    pedestrianCascade="/home/charith/Programming/FYP/Plugins/data/hogcascades/hogcascadepedestrians.xml";


    dir.cd("..");
    if(!dir.exists("faces")){
        dir.mkdir("faces");
    }
    dir.cd("faces");
    dir2.mkpath(dir.absolutePath());


    imgOutputPropertyName = "Detected Face Store";
    imgOutputValue=dir.absolutePath();
    imgOutCordinatesPropertyName = "Face Coordinates File";
    imgOutCordinatesValue = dir.absoluteFilePath("faceCoordinates.txt");



    createFilePathParam(imgOutputPropertyName,imgOutputValue,nooba::DirPath);
    createStringParam(imgOutCordinatesPropertyName,imgOutCordinatesValue,true);
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );



    file.open(QIODevice::Append | QIODevice::Text);

    b =face_cascade.load(haarFaceCascadePath);
    out.setDevice(&file);
    out << "%"<<asctime(timeinfo)<<"%======= Face details (Frame number, x, y, width, height)======= \n";

    createFrameViewer("Output");

    return true;
}

void FacedetectionPlugin::onStringParamChanged(const QString& varName, const QString& val){

    //  qDebug("signal came");

    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("faces")){
        dir.mkdir("faces");
    }
    dir.cd("faces");

    if (varName.compare(imgOutputPropertyName) == 0) {
        imgOutputValue=val;
        //dir.mkpath(imgOutputValue);
    }

    if (varName.compare(imgOutCordinatesPropertyName) == 0) {

        imgOutCordinatesValue = val;
        file.close();
        file.setFileName(imgOutCordinatesValue);

        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        file.open(QIODevice::Append | QIODevice::Text);

        out.setDevice(&file);
  //  out << "%"<<asctime(timeinfo)<<"%======= Face details (Frame number, x, y, width, height)======= \n";

    }


}

void FacedetectionPlugin::onFilePathParamChanged(const QString &varName, const QString &val)
{

    if (varName.compare(imgOutputPropertyName) == 0) {
        imgOutputValue=val;
        //dir.mkpath(imgOutputValue);
    }
}

void FacedetectionPlugin::copyFile(const QString &sourcePath, const QString &destPath)
{
    QFile in(sourcePath);
    if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray b = in.readAll();
    in.close();
    QFile out(destPath);

    if (!out.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    out.write(b.data(), qstrlen(b.data()));
    out.close();
}




bool FacedetectionPlugin::release()
{

    file.close();
    return true;
}

void FacedetectionPlugin::inputData(const QStringList &strList, QList<QImage> imageList){


    cv::Mat frame(imageList.at(0).height(),imageList.at(0).width(),CV_8UC3,(uchar*)imageList.at(0).bits(),imageList.at(0).bytesPerLine());

int size = strList.size();

    foreach(QString str,strList){
        //  debugMsg(str);
       int i=0;
        QList<QString> parameters1 = str.split(" ");
        QList<QString> parameters2 = parameters1.at(1).split(",");
        minX = parameters2.at(4);
        minY = parameters2.at(5);
        maxX = parameters2.at(6);
        maxY = parameters2.at(7);

        int width = maxX.toInt()-minX.toInt();
        int height = maxY.toInt()-minY.toInt();

        QString t=QString("width= %1 , height = %2 ").arg(width).arg(height);

        debugMsg(t);

        FacedetectionPlugin::bloblist.append(new blobobject(minX.toInt(),maxY.toInt(),maxX.toInt()-minX.toInt(),maxY.toInt()-minY.toInt()));

    }

    processFrame(frame);

    QString deb=QString("%1,%2,%3,%4").arg(minX).arg(minY).arg(maxX).arg(maxY);
   // debugMsg(deb);
}


PluginInfo FacedetectionPlugin::getPluginInfo() const
{
    PluginInfo pluginInfo(
                "Facedetection Plugin",
                0,
                1,
                "This plugin detects faces from a video feed",
                "Charith Wijenayake");
    return pluginInfo;
}


// see qt4 documentation for details on the macro (Qt Assistant app)
// Mandatory  macro for plugins in qt4. Made obsolete in qt5
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(FacedetectionPlugin, FacedetectionPlugin);
#endif
