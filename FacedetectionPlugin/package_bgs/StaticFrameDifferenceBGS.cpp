#include "StaticFrameDifferenceBGS.h"

StaticFrameDifferenceBGS::StaticFrameDifferenceBGS() : firstTime(true), enableThreshold(true), threshold(15), showOutput(true)
{
  std::cout << "StaticFrameDifferenceBGS()" << std::endl;
}

StaticFrameDifferenceBGS::~StaticFrameDifferenceBGS()
{
  std::cout << "~StaticFrameDifferenceBGS()" << std::endl;
}

void StaticFrameDifferenceBGS::process(const cv::Mat &img_input, cv::Mat &img_output,cv::Mat imgback)
{
  if(img_input.empty())
    return;

 // imgback.copyTo(mask);

//  if(img_background.empty())
//    img_input.copyTo(img_background);

  loadConfig();

  //if(firstTime)
  //  saveConfig();

//  cv::absdiff(img_input, mask, mask);


//  if(enableThreshold)
//    cv::threshold(mask, mask, threshold, 255, cv::THRESH_BINARY);

//  cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
//  cv::erode(mask,mask, element,cv::Point(-1,-1),5);
//  cv::dilate(mask,mask, element,cv::Point(-1,-1),1);


//  img_output.zeros(img_output.rows,img_output.cols,img_output.type());

  mask = abs(img_input - imgback) > threshold;

  if(mask.channels() == 3)
    //cv::cvtColor(mask, mask, CV_BGR2GRAY);

//img_output = img_input & mask;
  //img.copyTo(sharpened, lowContrastMask);
  img_input.copyTo(img_output,mask);
 // cv::imshow("Static Frame Difference", mask);


  firstTime = false;
}

void StaticFrameDifferenceBGS::saveConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("./config/StaticFrameDifferenceBGS.xml", 0, CV_STORAGE_WRITE);

  cvWriteInt(fs, "enableThreshold", enableThreshold);
  cvWriteInt(fs, "threshold", threshold);
  cvWriteInt(fs, "showOutput", showOutput);

  cvReleaseFileStorage(&fs);
}

void StaticFrameDifferenceBGS::loadConfig()
{
  CvFileStorage* fs = cvOpenFileStorage("./config/StaticFrameDifferenceBGS.xml", 0, CV_STORAGE_READ);

  enableThreshold = cvReadIntByName(fs, 0, "enableThreshold", true);
  threshold = cvReadIntByName(fs, 0, "threshold", 15);
  showOutput = cvReadIntByName(fs, 0, "showOutput", false);

  cvReleaseFileStorage(&fs);
}

