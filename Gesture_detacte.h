/**********************************************************************
    
  Gesture_detacte
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
#include"Target.h"
#include"qt_windows.h"
#include<stdexcept>
#include<qdebug>
#include"info.h"

enum State{INITIAL,LEFT,RIGHT,UP,DOWN,ESC,SPACE,MOUSE,AUTO,ENTER,SF1,SF2,SF3};//定义8种手势信号
 int g_LIMIT1 = 90;//每两个手势中心变化的触发阈值
 int g_LIMIT2 = 160;
 int ActionNum = 40;
Target T1;
vector <Point> background_center;//背景轮廓的中心
vector<Point> TT1;
vector<Point> TT2;
vector<Point> TT3;//读取模板所用


VideoCapture temp(0);
extern QString Info;
extern info myinfo;

/**************************************************************************/

void sizeChange(const float new_dis,const float old_dis)
{
	cout<<(double)(new_dis/old_dis)<<endl;
	if((double)(new_dis/old_dis)>1)
	{
		double change = (new_dis/old_dis)*120;
		
		mouse_event(MOUSEEVENTF_WHEEL,0,0,change,GetMessageExtraInfo());
	}
		cout<<(double)(new_dis/old_dis)<<endl;
	if((double)(new_dis/old_dis)<1)
	{
		double change =-(1/((new_dis/old_dis)))*120;
			
		mouse_event(MOUSEEVENTF_WHEEL,0,0,change,GetMessageExtraInfo());
	}
}


/********************************************************************************************************************/

State stateAnalyze (Point new_point,Point old_point,int new_falg ,int old_flag,State lastState ,int space_times )throw(runtime_error)//对每两次次的手势信息进行分析得出命令返回信号
{
        	 std::cout<<lastState<<endl;
	 if(space_times == 3)
	 {
		return ESC;
	 }
                 	 if((abs(old_point.x-new_point.x)>g_LIMIT2)||(abs(old_point.y-new_point.y)>g_LIMIT1))//
	 {
		if((abs(old_point.x-new_point.x)>g_LIMIT2)&&(abs(old_point.y-new_point.y)>g_LIMIT1))
			return INITIAL;
		else if((new_point.x-old_point.x)>g_LIMIT2)
			return LEFT;
		else if((new_point.x-old_point.x)<-g_LIMIT2)
			return RIGHT;
		else if((new_point.y-old_point.y)>g_LIMIT1)
			return DOWN;
        else if((new_point.y-old_point.y)<-g_LIMIT1)
			return UP;
	 }
	
	
		
		else if((old_flag==1)&&(new_falg==1)&&(lastState == INITIAL))
		{
			return SPACE;
		}
		else  if((old_flag==2)&&(new_falg==2)&&(lastState == INITIAL))
		{
			
			return MOUSE;
		}
			else  if((old_flag==3)&&(new_falg==3)&&(lastState == INITIAL))
		{
			
			return AUTO;
		}
		  
		
		else
		{
			//qDebug("INITIAL");
			return INITIAL;
		}
	 }


/***********************************************************************/


void postCommand(const State command )//对各种手势信号进行快捷键响应
{
	if(command  == LEFT )
	{
		std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<left"<<endl;//镜像，没有flip过来，所以这里注意点。
		Info = "LEFT";
		qDebug("LEFT");
        keybd_event(VK_LEFT,(BYTE)0, 0 ,0);  
        keybd_event(VK_LEFT, (BYTE)0, KEYEVENTF_KEYUP,0); 
		 Sleep(500);
		
	}
	if(command  == RIGHT )
	{
		std::cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>right"<<endl; 
		Info = "RIGHT";
		qDebug("RIGHT");
        keybd_event(VK_RIGHT,(BYTE)0, 0 ,0);  
        keybd_event(VK_RIGHT, (BYTE)0, KEYEVENTF_KEYUP,0); 
		 Sleep(500);
	}
	if(command  == UP)
	{
		 std::cout<<"upAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa"<<endl;
		 Info = "UP";
		 qDebug("UP");
		 for(int i = 0;i<ActionNum;i++)
		 {
			 keybd_event(VK_UP,(BYTE)0, 0 ,0);  
			 keybd_event(VK_UP, (BYTE)0, KEYEVENTF_KEYUP,0);
			 Sleep(10);
			 cout<<i<<endl;
		 }
		  Sleep(500);
		 
	}
	if(command  == DOWN )
	{
		 std::cout<<"downVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl ;
		 Info = "DOWN";
		 qDebug("DOWN");
		 for(int i = 0 ;i<ActionNum;i++)
		 {
			 keybd_event(VK_DOWN,(BYTE)0, 0 ,0);

			 keybd_event(VK_DOWN, (BYTE)0, KEYEVENTF_KEYUP,0); 
			 Sleep(10);

		 }
		 Sleep(500);
	}
	if(command  == MOUSE )//TODO
	{
		cout <<"MOUSE"<<endl;
		Info= "MOUSE";
		//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,GetMessageExtraInfo()); 
		Mat frame ;
		Point old_point ;
		Point new_point;
		POINT pt; 
		int old_area = 1;
		int new_area = 1;
	    int times = 0;
		bool leftbutton_on = false;
	    T1.setTemplate(TT1,TT2,TT3);
		for(int i  = 0;;i++)
		{
			temp>>frame;
		  
			T1.setRawFame(frame);
			T1.showRawFrame();
			T1.getHSVmask();
			T1.getContours();
			T1.setCenter();
			T1.setFlag();
			T1.getContourArea();
			cout<<T1.flag<<endl;
			if(i == 0 )
			{
				old_point = T1.center1;
				old_area = T1.getContourArea();
			}
			if(i>0)
			{   
				new_area = T1.getContourArea();
				new_point =  T1.center1;
			}
			if(T1.flag == 1)
			{
			  leftbutton_on = true;
			//  waitKey(0);
			  mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,GetMessageExtraInfo()); 

			}
			if(leftbutton_on&&(i>10))
			{
				if((double(new_area)/double(old_area)>1.05)||(double(new_area)/double(old_area)<0.95))
				sizeChange(new_area,old_area);
			}

		    if((abs(old_point.x-new_point.x)>2)||(abs(old_point.x-new_point.x)>2))//鼠标控制
			{
				 GetCursorPos(&pt);
				 cout<<pt.x<<"__"<<pt.y<<endl;
				 GetCursorPos(&pt);
				 cout<<pt.x<<"__"<<pt.y<<endl;
				 SetCursorPos(pt.x-2*(new_point.x-old_point.x),pt.y+2*(new_point.y-old_point.y));
				// if((new_point.x!=0)&&(new_point.y!=0))
				// SetCursorPos((1365/new_point.x),(720/new_point.y));
			}

			if((abs(old_point.x-new_point.x)<5)&&(abs(old_point.x-new_point.x)<5))
			{
				times = times+1;
			}
			if((abs(old_point.x-new_point.x)>5)||(abs(old_point.x-new_point.x)>5))
			{
				times =0;
			}

			if(times == 15)
			{
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,GetMessageExtraInfo()); 
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,GetMessageExtraInfo());
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo());
			    times = 0;
			}
            if(T1.contours.size()  == 0)
			{
				T1.contours.clear();
				if(leftbutton_on == true)
					leftbutton_on  = false;
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,GetMessageExtraInfo()); 
				break;
			}
			old_point = new_point;
			old_area = new_area;
			if((new_point.x == 1365)&&(new_point.y == 0))
				throw runtime_error("this is the bug");
			T1.contours.clear();
		
			//waitKey(10);
		}
		
	}	
	if(command  == SPACE )
	{
		 cout <<"SPACE"<<endl; 
		 Info = "SPACE";
		 keybd_event(VK_SPACE,(BYTE)0, 0 ,0);  
		 qDebug("SPACE");
         keybd_event(VK_SPACE, (BYTE)0, KEYEVENTF_KEYUP,0);
		 Sleep(1000);
		
	}
	if(command  == ESC )
	{
		cout <<"ESC"<<endl;
		qDebug("ESC");
		Info = "ESC";
		keybd_event(VK_MENU ,(BYTE)0, 0 ,0);  
		keybd_event(VK_F4 ,(BYTE)0, 0 ,0);
		Sleep(100);
        keybd_event(VK_F4, (BYTE)0, KEYEVENTF_KEYUP,0);
	    keybd_event(VK_MENU, (BYTE)0, KEYEVENTF_KEYUP,0);
		//waitKey(0);
	}
	/*if(command  == CONFIRM )
	{
		cout <<"CONFIRM"<<endl;
	
		keybd_event(VK_RETURN ,(BYTE)0, 0 ,0);
		Sleep(100);
        keybd_event(VK_RETURN, (BYTE)0, KEYEVENTF_KEYUP,0);	waitKey(0);
	}*/
	if(command == AUTO)
	{
	 cout<<"auto********************************"<<endl;
	 waitKey(0);
	}
	if(command  == INITIAL )

	{
	 std::cout<<"no GESTURE "<<endl;
	// Info ="No Gesture";
	}
	if(command == ENTER)
	{
		keybd_event(VK_RETURN ,(BYTE)0, 0 ,0);
		Info = "ENTER";
		Sleep(100);
        keybd_event(VK_RETURN, (BYTE)0, KEYEVENTF_KEYUP,0);	
		qDebug("ENTER");
	}
	if(command == SF1)
	{
		Info ="Open Music";
		keybd_event(VK_LWIN ,(BYTE)0, 0 ,0);
		keybd_event('M' ,(BYTE)0, 0 ,0);
		Sleep(100);
        keybd_event('M', 0, KEYEVENTF_KEYUP ,0);
        keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP,0);
		qDebug("open music player");
	}
}



/***********************************************************************************************/











