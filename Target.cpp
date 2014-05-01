/**********************************************************************
    
 Target
  Release Date: 2014/2/15
  Copyright (C) 2014	Zuoxin Lee
                        email: lzx1444@gmail.com
                       

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
**********************************************************************/
#include"Target.h"
void Target::setRawFame(Mat scr)
{
	rawframe = scr;
	medianBlur(rawframe,rawframe, 5);//中值滤波去除噪点
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);//
	filter2D(rawframe, rawframe, rawframe.depth(), kernel);//
}

/*****************************************************************************/
void Target::showRawFrame()
{   
	//createTrackbar("lighting","rawframe",&beta,100,0);
	imshow("rawframe",rawframe);
	waitKey(5);
}




/*****************************************************************************/

void Target::getHSVmask()
{
	
	Mat HSV_frame;
	//cvtColor(rawframe,HSV_frame,COLOR_BGR2HSV);//转成HSV
	//createTrackbar("H_max","HSV_mask",&H_max,100,0);
	////createTrackbar("V_max","HSV_mask",&V_max,255,0);
	//createTrackbar("V_min","HSV_mask",&V_min,20,0);
	//createTrackbar("S_min","HSV_mask",&S_min,40,0);
	//createTrackbar("S_max","HSV_mask",&S_max,180,0);

	
	
   // imshow("HSV_mask", HSV_mask);
   /// waitKey(5);
	//rawframe.convertTo(rawframe,CV_8UC3,255);
	//Mat yuv,dst;
	cvtColor(rawframe,HSV_frame,CV_BGR2HSV);
	//inRange(HSV_frame,Scalar(0.03*255,0.2*255,0),Scalar(0.28*255,255,0.8*255),HSV_mask);
	//cvtColor(rawframe,HSV_frame,CV_BGR2YCrCb);
	//imshow("HSV",HSV_frame);
	Mat dstTemp1(rawframe.rows, rawframe.cols, CV_8UC1);
	Mat dstTemp2(rawframe.rows, rawframe.cols, CV_8UC1);
    inRange(HSV_frame, Scalar(H_min,S_min,V_min), Scalar(H_max,S_max,V_max), dstTemp1);  //40
    inRange(HSV_frame, Scalar(156,30,0), Scalar(180,170,256), dstTemp2);  
	bitwise_or(dstTemp1, dstTemp2, HSV_mask);
	//dst.setTo(Scalar::all(0));
	//imshow("mask_back",mask_back);
	//waitKey(0);
   // bitwise_and(HSV_mask,mask_back,HSV_mask);
	HSV_mask =opening(opening(HSV_mask));
	line(HSV_mask,Point(240,0),Point(240,320),Scalar(255,0,0));
	line(HSV_mask,Point(0,160),Point(480,160),Scalar(160,0,0));
    imshow("HSV_mask",HSV_mask);

}


/*****************************************************************************/
void Target::showContours()
{
	Mat rawframe1 = rawframe;	
	if(contours.size()==1)
	{
	    //MyLine(rawframe1,contours);
		Rect r1 = boundingRect(Mat(contours[0]));
		rectangle(rawframe1,r1,Scalar(255,0,0),2);
		imshow("contour",rawframe);
		rection = r1;
	}
	else if (contours.size() ==0)
	{
		cout<<"no coutours"<<endl;
	}
	else if(contours.size()==2)
	{
		drawContours(rawframe1,contours, -1, Scalar(255,0,255), 3/*, 8, hierarchy*/);  
		Rect r1 = boundingRect(Mat(contours[0]));
		rectangle(rawframe1,r1,Scalar(255,0,0),2);
		Rect r2 = boundingRect(Mat(contours[1]));
		rectangle(rawframe1,r2,Scalar(0,255,0),2);
		imshow("contour",rawframe);
		
	}
	else 
		cout <<"too many  Targets =="<<endl;


};


/*****************************************************************************/
void Target::getContours()
{
	Mat src =opening( HSV_mask);
	vector<vector<Point> > contours_temp;
	vector<Vec4i> hierarchy;
    findContours(src, contours_temp, hierarchy, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
	if(contours_temp.size()>0)//TODO其他方法筛选轮廓
	{
		double bigarea = 0;
		for(int i = 0;i<contours_temp.size();i++)
		 {
		   double area =contourArea(contours_temp[i]);
		   if(area>1500)//面积大于1500的被选取
		   {

				contours.push_back(contours_temp[i]) ;
			

		   }
        }
	}
	
}


/*****************************************************************************/
void Target::setCenter()
{
	float radius;
	if(contours.size()==1)
	{
	   minEnclosingCircle(Mat(contours[0]),center1,radius); 
	 
	}
	if(contours.size()==2) 
	{
	  minEnclosingCircle(Mat(contours[0]),center1,radius); 
	  minEnclosingCircle(Mat(contours[1]),center2,radius); 
	
	}
	 
	// cout<<center<<endl;
	
};


/*****************************************************************************/
 void Target::setFlag()
 {
	 if(Templates.size() == 0)
	 {
		cout<<"there are no templates!!"<<endl;
		waitKey(0);
	 }
	     flag = 0;
		 if(rection.y!=0)
		 {
				if(((rection.x /rection.y)>0.99)&&((rection.x /rection.y)<1.1))
				flag = 2;
		 }
  	if(contours.size()==1)
	{
        if(Templates.size()>0)
		{  
			int i;
			for(i = 0;i<Templates.size();i++)
			{
				float m = matchShapes(contours[0],Templates[i],CV_CONTOURS_MATCH_I1,0);
				if(m<float(float(SIMILAR)/100))
				{
					flag = i+1;
					break;
				}
		
			}
			if(i==Templates.size())
				flag = 0;
			if(flag == 1)
			{
				if((center1.x>240)&&(center1.y<160))
					flag = 2;
			}
		}
	}
 }


/*****************************************************************************/
 void Target::setTemplate(vector<Point>&a,vector<Point>&b,vector<Point>&c)
 {
	 Templates.push_back(a);
	 Templates.push_back(b);
	 Templates.push_back(c);
 }

 

 /*******************************************************************************/
 void Target:: getDistance()
 {
	if(contours.size()==2)
		distance = sqrt((center1.x-center2.x)*(center1.x-center2.x)+(center1.y-center2.y)*(center1.y-center2.y));
	else 
		distance = 0;
 }

 /***********************************************************************************/
 int Target::getContourArea()
 {
	 if(contours.size()==1)
	   return  contourArea(contours[0]);
 }


 /**************************************************************************************/
