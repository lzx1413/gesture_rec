/**********************************************************************
    
 Target
  Release Date: 2014/2/15
  Copyright (C) 2014	zuoxin Lee
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
#pragma once
#include"stdafx.h"
 
	//相似度限制用于轮廓匹配时
class Target
{
private:
	Mat rawframe;//原始图像
	Mat HSV_mask;//HSV二值化图像
	int contour_area;//轮廓面积
	int contour_length;//轮廓周长
	Mat mask0;
	Rect rection;
 


	
public:
	Target():contour_area(0) ,contour_length(0),flag(0)//构造函数
	{
	  namedWindow("HSV_mask",1);
	  namedWindow("rawframe",1);
	//  rawframe = Mat::zeros(320, 480,CV_32FC3); 
	//  HSV_mask = Mat::zeros(320, 480,CV_32FC3); 

	}
	
	int flag;//是否和模板匹配及哪个模板匹配 
	vector<vector<Point> > contours;//包含轮廓
    Point2f center1;//轮廓中心1若只有一个轮廓则为此中心
	Point2f center2;//轮廓中心2
	float distance;//两个轮廓时两轮廓的中心距离用于控制图片大小变化

	void setRawFame(Mat frame);
	void showRawFrame();
	void getHSVmask();
	void getContours();
	void showContours();
    void setCenter();
	void setFlag();
    void setTemplate(vector<Point>&a,vector<Point>&b,vector<Point>&c);
    void getDistance();
 // void foregound();

	int getContourArea();;
    vector<vector<Point> >Templates;

	static int H_max ;
	static int S_max;
	static int H_min;
	static int S_min;
	static int V_min;
	static int V_max;//HSV的范围
	static double alpha ; /**< Simple contrast control 1-3 */  
    static  int beta; /**< Simple brightness control 0-100 */
	static Mat mask_back;
    static float SIMILAR;
	//静态手势模板（暂时只有三个）




	inline Mat opening(const Mat src)//开运算使轮廓平滑完整
{
       Mat mask = src;
       int morph_elem = 2;
	   int morph_size = 2;
	    Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );  
       // erode(mask, mask, element);  
       morphologyEx(mask, mask, MORPH_OPEN, element);  
	      morphologyEx(mask, mask, MORPH_OPEN, element); 
     //   dilate(mask, mask, element);  
       morphologyEx(mask, mask, MORPH_CLOSE, element); 
	     morphologyEx(mask, mask, MORPH_CLOSE, element); 
		return mask;
       
};
	inline void MyLine( Mat img, vector<Point>scr )  //画多边形
{  
  int thickness = 2;  
  int lineType = 8;  
  for(int i = 1;i<scr.size()-1;i++)
  {
	  line( img,  
		scr[i-1],  
		scr[i],  
		Scalar( 255, 0, 0 ),  
		thickness,  
		lineType ); 
  }
    line( img,  
		scr[scr.size()-1],  
		scr[0],  
		Scalar( 255, 0, 0 ),  
		thickness,  
		lineType ); 
}

};
