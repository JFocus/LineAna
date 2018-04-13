#include <iostream>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <math.h>

using namespace cv;
using namespace std;


int main(int argc, char **argv) {
    
    ofstream VanishingPointRecord ("VanishingPointRecord.txt");
    
    
    string dataset_dir  = "/home/jfoucs/MYGraduationProject/Data";
    ifstream fin ( dataset_dir+"/associate.txt" );
    if ( !fin )
    {
        cout<<"please generate the associate file called associate.txt!"<<endl;
        return 1;
    }
    vector<string> rgb_files ;
    vector<double> rgb_times ;
    while ( !fin.eof() )
    {
        string rgb_time, rgb_file;
        fin>>rgb_time>>rgb_file;
        rgb_times.push_back ( atof ( rgb_time.c_str() ) );
        rgb_files.push_back ( rgb_file );

        if ( fin.good() == false )
            break;
    }   
    
    for ( int i=0; i<(rgb_files.size()-1); i++ )
    {
	cv::Mat srcImage = imread(rgb_files[i]);  
	cv::Mat midImage,dstImage;  
	vector<Point> CrossPoint;
	Canny(srcImage, midImage, 50, 200, 3);
	cvtColor(midImage,dstImage, CV_GRAY2BGR);
	
	
	vector<Vec2f> lines;
	HoughLines(midImage, lines, 1, CV_PI/180, 360, 0, 0 );  
      
	cout << "line number  " <<std::to_string(i)<< "  is  " << lines.size() << endl;
	for( size_t i = 0; i < lines.size(); i++ )  
	{  
	    
	    float rho = lines[i][0], theta = lines[i][1];  
	    Point pt1, pt2;  
	    double a = cos(theta), b = sin(theta);  
	    double x0 = a*rho, y0 = b*rho;  
	    pt1.x = cvRound(x0 + 1000*(-b));  
	    pt1.y = cvRound(y0 + 1000*(a));  
	    pt2.x = cvRound(x0 - 1000*(-b));  
	    pt2.y = cvRound(y0 - 1000*(a));  
	    line( dstImage, pt1, pt2, Scalar(55,100,195), 1, CV_AA);  
	}
	
	for( size_t i = 0; i < lines.size() ; i++)
	{
	  float rho0 = lines[i][0], theta0 = lines[i][1];
	  double Sintheta0 = sin(theta0), Costheta0 = cos(theta0);
	  for(size_t j = i + 1; j < lines.size(); j++)
	  {
	    float rho1 = lines[j][0], theta1 = lines[j][1];
	    double Sintheta1 = sin(theta1), Costheta1 = cos(theta1);
	    double S0_S1 = Sintheta0/Sintheta1;
	    double C0_C1 = Costheta0 / Costheta1;
	    int x = cvRound((rho1*S0_S1 -rho0 ) / (Costheta1 * S0_S1 - Costheta0));
	    int y = cvRound((rho0 - rho1*C0_C1 ) / ( Sintheta0 - Sintheta1*C0_C1 ));
	    CrossPoint.push_back(Point_<int>(x , y)); 
	  }
	}
	cv::Mat img_CrossPoint = dstImage.clone();
	
	
	for(size_t i = 0 ; i < CrossPoint.size(); i++)
	{
	  cv::circle(img_CrossPoint, CrossPoint[i], 10, cv::Scalar(0, 240, 0), 1);
	  if(VanishingPointRecord.is_open())
	  {
	    VanishingPointRecord << "number" << std::to_string(i) << "  " << CrossPoint[i].x << "  " <<CrossPoint[i].y << "\n";            
	  } 	  
	}
	
	
	cout << "vanishing point number  " << std::to_string(i) << " : "<< CrossPoint.size() << endl;
	cout << "vanishing point location  " << std::to_string(i) << "  is\n" << CrossPoint << endl;
	
	
	string CrossPointimg = "CrossPointpicture" + std::to_string(i) + ".png";
	string originalpicture = "originalpicture" + std::to_string(i) + ".png";
	string cannypicture = "cannypicture" + std::to_string(i) + ".png";
	string linepicture = "linepicture" + std::to_string(i) + ".png";
	
	
	imwrite(CrossPointimg , img_CrossPoint);
	imwrite(originalpicture , srcImage);  
	imwrite(cannypicture, midImage);        
	imwrite(linepicture, dstImage);  
	
	if(VanishingPointRecord.is_open())
	{
	  VanishingPointRecord << "\n\n\n\n\n\n";
	}
    }
    VanishingPointRecord.close();
    
    return 0;
}
