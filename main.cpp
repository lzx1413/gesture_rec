/**********************************************************************
    
  main
  Release Date: 2014/2/15
  Copyright (C) 2014	zuouoxin Lee
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

#include"Gesture_recognition.h"
#include"Gesture_detacte.h"
#include <QtWidgets/QApplication>
#include<QtWidgets>
#include<QMainWindow>
#include<qstring.h>
#include"info.h"


/********************************************/
int Target::H_max  = 40;
int Target::H_min = 0;
int Target::S_min = 30;

int Target::S_max = 170;
int Target::V_min = 0;
int Target::V_max = 255;
int Target::beta = 20;
double Target::alpha = 1.25;
float Target::SIMILAR = 0.12;
int TemplateNUm = 0;
int commandNum = 0;
int flag = 1;
QString Info = "HELLOW WORLD";

/***************************************************/
Mat mask,bg;
Mat  Target::mask_back; 
info myinfo(Info);

/************************************************/

/*************************************************/
void set_H_max(int i)
{
	Target::H_max = i;
}
void set_H_min(int i)
{
	Target::H_min= i;
}
void set_S_max(int i)
{
	Target::S_max = i;
}
void set_S_min(int i)
{
	Target::S_min = i;
}
void set_V_max(int i)
{
	Target::V_max = i;
}
void set_V_min(int i)
{
	Target::V_min = i;
}
void set_beta(int i)
{
	Target::beta = i;
}
void setTemplateNum(int i)
{
	TemplateNUm = i;
	qDebug()<<"The currentTemplate"<<i;
}
void setSIMILAR(float i)
{
	Target::SIMILAR = i;
}
void command_change(State &command)
  {
 	if(command == SPACE)
	{
	 if(  commandNum == 0)
		 command =  SPACE;
	 if(  commandNum == 1)
		command =ENTER;
	 if(  commandNum == 2)
		 command = SF1;
	}
	
}
void setLIMIT1(int  i)
{
	g_LIMIT1 = i;
}
void setLIMIT2(int  i)
{
	g_LIMIT2 = i;
}
void setCommandNum(int i)
{
	commandNum = i;
	qDebug()<<"The current function is"<<commandNum;
}
void setFlag(bool i)
{
	flag = i;

}
void setActionNum(int i)
{
    ActionNum = i;
}
void resetAll()
{
	 Target::H_max  = 40;
     Target::H_min = 0;
	 Target::S_min = 30;

	 Target::S_max = 170;
	 Target::V_min = 0;
	 Target::V_max = 255;
	 Target::beta = 20;
	 Target::alpha = 1.25;
	 Target::SIMILAR = 0.12;
	 TemplateNUm = 0;
	 commandNum = 0;
	 flag = 1;
	 Info = "HELLOW WORLD";
	 g_LIMIT1 = 90;//每两个手势中心变化的触发阈值
     g_LIMIT2 = 160;
     ActionNum = 40;
}
/***********************************************/


void saveTemplate(vector<vector<Point> >t,int Tnum)//此函数用于对获取的手势模板进行保存保存在template1.txt和template2.txt中
{
	if(Tnum == 0)
	{
		ofstream template_file1("template1.txt",ios::out);
		if(!template_file1)
		{
			cerr<<"File could be opened "<<endl;
			exit(1);
		}
			for(int i = 0;i<t[0].size();i++)
			{
				template_file1<<(int)t[0][i].x<<" " ;
				template_file1<<(int)t[0][i].y <<" ";
			}
	
		template_file1.close();
	}
		if(Tnum == 1)
		{
			ofstream template_file2("template2.txt",ios::out);
			if(!template_file2)
			{
				cerr<<"File could be opened "<<endl;
				exit(1);
			}
				for(int i = 0;i<t[1].size();i++)
				{
					template_file2<<(int)t[1][i].x<<" " ;
					template_file2<<(int)t[1][i].y <<" ";
				}
	
			template_file2.close();
		}
		if(Tnum == 2)
		{
			ofstream template_file3("template3.txt",ios::out);
			if(!template_file3)
			{
				cerr<<"File could be opened "<<endl;
				exit(1);
			}
				for(int i = 0;i<t[1].size();i++)
				{
					template_file3<<(int)t[1][i].x<<" " ;
					template_file3<<(int)t[1][i].y <<" ";
				}
	
			template_file3.close();
		}
}
/**********************************************************************/
	void readTemplate1(vector<Point>& a)//从template1.txt中读取模板1 
	{
		ifstream template_file_in1;
		template_file_in1.open("template1.txt",ios::in);
		if(!template_file_in1)
		{
			cerr<<"File can not be read"<<endl;
			exit(1);
		}
		int temp1;
		int temp2;
	   while(template_file_in1>>temp1>>temp2)
		{
			
			a.push_back(Point(temp1,temp2));
		}
	   template_file_in1.close();
	   cout<<a.size()<<endl;
	  
	}

	/*******************************************************************/
	void readTemplate2(vector<Point>& b)//从template2.txt中读取模板2 
	{
	    ifstream template_file_in2;
		template_file_in2.open("template2.txt",ios::in);
		if(!template_file_in2)
		{
			cerr<<"File can not be read"<<endl;
			exit(1);
		}
		int temp1;
		int temp2;
	    while(template_file_in2>>temp1>>temp2)
		{
			
			b.push_back(Point(temp1,temp2));
		}
	   template_file_in2.close();
	   cout<<b.size()<<endl;
	}




/************************************************************/
	void readTemplate3(vector<Point>& c)//从template2.txt中读取模板2 
	{
	    ifstream template_file_in3;
		template_file_in3.open("template3.txt",ios::in);
		if(!template_file_in3)
		{
			cerr<<"File can not be read"<<endl;
			exit(1);
		}
		int temp1;
		int temp2;
	    while(template_file_in3>>temp1>>temp2)
		{
			
			c.push_back(Point(temp1,temp2));
		}
	   template_file_in3.close();
	   cout<<c.size()<<endl;
	}



	/******************************************************************************/


void getTemplates()//需要进一步丰富操作
{
	Target t1;
	vector<vector<Point> >T;
	VideoCapture templa(0);
   	Mat frame;
	for(int i = 0;i<20;i++)
	{
		templa>>frame;
		qDebug()<<TemplateNUm;	
		t1.setRawFame(frame);
		t1.showRawFrame();
		t1.getHSVmask();
		t1.getContours();
		t1.showContours();
		int c = waitKey(0);
		if(c =='y')
		{
			T.push_back(t1.contours[0]);
			cout<<"get template "<<T.size()<<endl;
		}
		if(c =='q')
			break;

		t1.contours.clear();
	}
	    if(T.size()>0)
		{
			saveTemplate(T,TemplateNUm);
			cout<<"templates have been saved"<<T[0]<<endl<<T[1]<<endl;
		}
		t1.contours.clear();
		
}
/*********************************************************/

 void MyLine( Mat img, vector<Point>scr )  //画多边形
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
	/********************************************/
	//背景差分部分//
     void get_background()//利用背景差分
	{
		
		int count=0;
		VideoCapture capture;
		capture.open(0);
		Mat fram;
		int framNum=0;
		Mat dstTemp01, dstTemp02;

			for(int i = 0;i<100;i++)
			{
		    	capture>>fram;
                resize(fram,fram,Size(480,320),0,0,CV_INTER_LINEAR);
			    Mat dstTemp1(fram.rows, fram.cols, CV_8UC1);
	            Mat dstTemp2(fram.rows, fram.cols, CV_8UC1);
                inRange(fram, Scalar(Target::H_min,Target::S_min,Target::V_min), Scalar(Target::H_max,Target::S_max,Target::V_max), dstTemp1);  //40
                inRange(fram, Scalar(156,30,0), Scalar(180,170,256), dstTemp2);  
	            bitwise_or(dstTemp1, dstTemp2, bg);
				int morph_elem = 2;
	        	int morph_size = 2;
		        Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) ); 
        
                morphologyEx(bg, bg, MORPH_OPEN, element);  
	            morphologyEx(bg, bg, MORPH_OPEN, element); 
                //dilate(mask, mask, element);  
                morphologyEx(bg, bg, MORPH_CLOSE, element); 
	            morphologyEx(bg, bg, MORPH_CLOSE, element); 

				if(i == 0)
				{
					Target::mask_back = bg;
				}
				if(i>0)
				{
					bitwise_or(Target::mask_back, bg, Target::mask_back);
				}
			//	imshow("111",bg);
				//waitKey(0);
				
			}
			int  height= Target::mask_back.rows;                      
            int  width= Target::mask_back.cols* Target::mask_back.channels();
			for(int m= 0; m< height; m++)  
				{  
                    for(int j=0; j< width; j++)  
                    {  
                        Target::mask_back.at<uchar>(m, j)= 255- Target::mask_back.at<uchar>(m, j);   // 每一个像素反转  
                    }  
                }  
  
			imshow("background",Target::mask_back);
		
		    char key=(char)waitKey(2);
			switch(key)
			{
			case 27:
				break;
			
			}
	
	}
	




	 


/************************************************************************/
int Gesture_recognition()
{
	    Mat frame; //获取的原始图像
		Target target1;
		Target target2;//用于手势信号确定的两个手势
		int space_times = 0;//连续信号SPACE的个数，用于触发ESC
		Point  new_point_temp;//新的手势中心
		Point  old_point_temp;//上一个手势中心
		int new_flag_temp = 0;//新的手势模板属性
		int old_flag_temp = 0;//上个手势模板属性
		float new_distance = 0;
		float old_distance = 0;
		int size1 = 0;
		int size2 = 0;//两个手势类的轮廓数用于信号判断
	
		VideoCapture cap(0); // open the default camera  
		Sleep(100);//等待摄像头响
        if(!cap.isOpened()) // check if we succeeded  
            return -1;  

		readTemplate1(TT1);
		readTemplate2(TT2);
		readTemplate1(TT3);
		std::cout<<TT1<<endl<<endl<<endl<<TT2<<endl;//显示两个模板的数组
		target1.setTemplate(TT1,TT2,TT3);
		target2.setTemplate(TT1,TT2,TT3);
		Mat tt = Mat::zeros(320, 480,CV_8UC3);
	    MyLine(tt,TT1);
		imshow("Template1",tt);
		State g;//手势信号
		State lastState = INITIAL ;//上一次获取的手势信号用于手势识别
		//get_background();
		for(int i = 0;;i++)
		{

			qDebug()<<g_LIMIT1;
			myinfo.setInfo(Info);
		    myinfo.send();
		    POINT pt;
		   // GetCursorPos(&pt);
	      //  cout<<pt.x<<"__"<<pt.y<<endl;  
			cap >> frame; // get a new frame from camera
			
    		resize(frame,frame,Size(480,320),0,0,CV_INTER_LINEAR);
		    frame.convertTo(frame, -1, Target::alpha,-Target::beta);
			//imshow("raw",frame);
			
			if((i%2==0))//利用i的奇偶性两个手势类交替计算复制
			{
			   
				target1.setRawFame(frame);
				target1.showRawFrame();
//		target1.foregound();
				target1.getHSVmask();
				target1.getContours();
				target1.setCenter();
				target1.setFlag();
				target1.getDistance();
				size1 = target1.contours.size();
			}

			else
			{
           
			    target2.setRawFame(frame);
				//target2.showRawFrame();
		//target2.foregound();
				target2.getHSVmask();
				target2.getContours();
				target2.showContours();
				target2.setFlag();
				target2.setCenter();
				target2.getDistance();
				size2 = target2.contours.size();
			}
		
			if(i>0&&(size1==1)&&(size2==1))//两个帧中均只有一个轮廓时的情况
			{	
				if(i>0&&(i%2 == 1))
				{
					new_point_temp = target2.center1;
					new_flag_temp = target2.flag;
					old_point_temp = target1.center1;
					old_flag_temp = target1.flag;
				
				}
				if(i>0&&(i%2 == 0))
				{
					new_point_temp = target1.center1;
					new_flag_temp = target1.flag;
					old_point_temp = target2.center1;
					old_flag_temp = target2.flag;
				
				}
				try
				{
				   g = stateAnalyze (new_point_temp,old_point_temp,new_flag_temp ,old_flag_temp,lastState,space_times);
				}
				catch(runtime_error &error)
				{
					cout<<"Exception occured"<<error.what()<<endl;
					waitKey();
				}
				if((g == INITIAL)&&(lastState == SPACE)||(g ==SPACE)&&(lastState ==INITIAL))
				{
					space_times++;
				}
				else
				{
					space_times = 0;
				}
				
 			    command_change(g);
    			lastState = g;

 				postCommand(g);
			}
			if((size1==2)&&(size2==2))
			{
				if(i>0&&(i%2 == 1))
				{
					new_distance = target2.distance;
					old_distance = target1.distance;
				}
				if(i>0&&(i%2 == 0))
				{
					new_distance = target1.distance;
					old_distance = target2.distance;
				}
				sizeChange(new_distance,old_distance);
						
			}
			cout<<old_distance<<"--"<<new_distance<<endl;
			if((target1.contours.size()>=1)||(target2.contours.size()>=1)) 
			{
				target1.contours.clear();
				target2.contours.clear();
			}//轮廓信息清除进行下次计算
		int c = waitKey(1);//与两帧的时间间隔有关
		if(c==27)
			break;
		if(flag = 0)
			break;
		if( c=='p')
			waitKey(0);

		std::cout<<"processing"<<i<<endl;
		}
      
}
	

int main(int argc, char *argv[])  
    {
		//Gesture_recognition();
		QApplication a(argc, argv);
		QMainWindow Gesture;
		//Gesture,resize(900);
		QWidget mainWindow;
	//	mainWindow.resize(900,600);
		Gesture.setCentralWidget(&mainWindow);
		mainWindow.setWindowTitle("Function");

		//QHBoxLayout *layout = new QHBoxLayout;
		//PushButton start("start",&mainWindow);
		//QSlider LightingSet(Qt::Horizontal,&mainWindow);
		//QLCDNumber LightingDisplay(&mainWindow);
		//LightingSet.setRange(0,100);
	    //QObject::connect(&start,&QPushButton::clicked,Gesture_recognition);
		//QObject::connect(&LightingSet,&QSlider::valueChanged,set_beta);
		//QObject::connect(&LightingSet,SIGNAL(QSlider::valueChanged),&LightingDisplay,SLOT      (QLCDNumber::display(int)));
		//layout->addWidget(&start);
		//layout->addWidget(&LightingSet);
		//layout->addWidget(&LightingDisplay);
		//mainWindow.setLayout(layout);
	
	QLabel *labelFunction = new QLabel("Function",&mainWindow);
	QLabel *labelTemplate = new QLabel("Template",&mainWindow);
	QComboBox *comboTemplate = new QComboBox;
	comboTemplate->setEditable(true);
	comboTemplate->insertItem(0,"Template 1");
	comboTemplate->insertItem(1,"Template 2");
	comboTemplate->insertItem(2,"Template 3");

	QComboBox *comboFunction = new QComboBox;
	comboFunction->setEditable(true);
	comboFunction->insertItem(0,"Function 1");
	comboFunction->insertItem(1,"Function 2");
	comboFunction->insertItem(2,"Function 3");

	QPushButton start("start",&mainWindow);
	
	QLabel *label = new QLabel("NOTE：Next Frame please press Space;\nSave please press Y;\nExit Setting please press Q",&mainWindow);
	label->setGeometry(QRect(328, 240, 329, 27*4));  //四倍行距
	label->setWordWrap(true);
	label->setAlignment(Qt::AlignTop);
	QObject::connect(&start,&QPushButton::clicked, getTemplates);

	QGridLayout *layoutmiddle = new QGridLayout;
	layoutmiddle->addWidget(labelTemplate,0,0);
	layoutmiddle->addWidget(comboTemplate,0,1,1,3);
	layoutmiddle->addWidget(labelFunction,1,0);
	layoutmiddle->addWidget(comboFunction,1,1,1,3);
	layoutmiddle->addWidget(&start,2,0,1,2);
	layoutmiddle->addWidget(label,3,0,1,4);

	//labelshow->setText(QString::cout);
	
	
	
	void (QComboBox::*functionChanged)(int)=&QComboBox::currentIndexChanged;
	QObject::connect(comboFunction, functionChanged,setCommandNum);
	void (QComboBox::*templateChanged)(int)=&QComboBox::currentIndexChanged;
	QObject::connect(comboTemplate, templateChanged,setTemplateNum);
	

	QLabel *labelInfShow = new QLabel(&mainWindow);
	QFont lbl_font;  
    lbl_font.setPointSize(16);
	labelInfShow->setFont(lbl_font);
	labelInfShow->setText(Info);
	labelInfShow->setAlignment( Qt::AlignVCenter);
	QObject::connect(&myinfo,&info::information,labelInfShow,&QLabel::setText);
	

	
	QPushButton Run("Run",&mainWindow);
	QPushButton quit("quit",&mainWindow);
	QGridLayout *layoutRight = new QGridLayout;
	layoutRight->addWidget(labelInfShow,0,0);
	layoutRight->addWidget(&quit,1,0);
	layoutRight->addWidget(&Run,1,1);
	QObject::connect(&quit,&QPushButton::clicked,&QApplication::quit);
	QObject::connect(&Run,&QPushButton::clicked,Gesture_recognition);




	QLabel *labellightsetting = new QLabel("Light Setting",&mainWindow);
	QSpinBox *spinboxlightsetting = new QSpinBox(&mainWindow);
	QSlider *sliderlightsetting = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxlightsetting->setRange(0,100);
	sliderlightsetting->setRange(0,100);

	QLabel *labelhmin = new QLabel("H_min",&mainWindow);
	QSpinBox *spinboxhmin = new QSpinBox(&mainWindow);
	QSlider *sliderhmin = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxhmin->setRange(0,20);
	sliderhmin->setRange(0,20);

	QLabel *labelhmax = new QLabel("H_max",&mainWindow);
	QSpinBox *spinboxhmax = new QSpinBox(&mainWindow);
	QSlider *sliderhmax = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxhmax->setRange(0,60);
	sliderhmax->setRange(0,60);

	QLabel *labelvmin = new QLabel("V_min",&mainWindow);
	QSpinBox *spinboxvmin = new QSpinBox(&mainWindow);
	QSlider *slidervmin = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxvmin->setRange(0,20);
	slidervmin->setRange(0,20);

	QLabel *labelvmax = new QLabel("V_max",&mainWindow);
	QSpinBox *spinboxvmax = new QSpinBox(&mainWindow);
	QSlider *slidervmax = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxvmax->setRange(0,255);
	slidervmax->setRange(0,255);

	QLabel *labelsmin = new QLabel("S_min",&mainWindow);
	QSpinBox *spinboxsmin = new QSpinBox(&mainWindow);
	QSlider *slidersmin = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxsmin->setRange(0,40);
	slidersmin->setRange(0,40);

	QLabel *labelsmax = new QLabel("S_max",&mainWindow);
	QSpinBox *spinboxsmax = new QSpinBox(&mainWindow);
	QSlider *slidersmax = new QSlider(Qt::Horizontal,&mainWindow);
	spinboxsmax->setRange(0,180);
	slidersmax->setRange(0,180);

    QLabel *label_LIMIT2 = new QLabel("LIMIT2",&mainWindow);
	QSpinBox *spinbox_LIMIT2 = new QSpinBox(&mainWindow);
	QSlider *slider_LIMIT2 = new QSlider(Qt::Horizontal,&mainWindow);
	spinbox_LIMIT2->setRange(100,200);
	slider_LIMIT2->setRange(100,200);



    QLabel *label_LIMIT1 = new QLabel("LIMIT1",&mainWindow);
	QSpinBox *spinbox_LIMIT1 = new QSpinBox(&mainWindow);
	QSlider *slider_LIMIT1 = new QSlider(Qt::Horizontal,&mainWindow);
	spinbox_LIMIT1->setRange(40,150);
	slider_LIMIT1->setRange(40,150);


	/**********************SIMILAR***********************/
	QLabel *label_SIMILAR = new QLabel("SIMILAR",&mainWindow);
	QSpinBox *spinbox_SIMILAR = new QSpinBox(&mainWindow);
	QSlider *slider_SIMILAR = new QSlider(Qt::Horizontal,&mainWindow);
	spinbox_SIMILAR->setRange(0,20);
	slider_SIMILAR->setRange(0,20);

	

	QLabel *label_ActionNum = new QLabel("ActionNum",&mainWindow);
	QSpinBox *spinbox_ActionNum = new QSpinBox(&mainWindow);
	QSlider *slider_ActionNum = new QSlider(Qt::Horizontal,&mainWindow);
	spinbox_ActionNum->setRange(0,50);
	slider_ActionNum->setRange(0,50);

	//QPushButton resetAll("resetAll",&mainWindow);
	//QObject::connect(&start,&QPushButton::clicked, getTemplates);
	

	QGridLayout *layoutleft = new QGridLayout;
	layoutleft->addWidget(labellightsetting,0,0);
	layoutleft->addWidget(spinboxlightsetting,0,1);
	layoutleft->addWidget(sliderlightsetting,0,2);

	layoutleft->addWidget(labelhmin,1,0);
	layoutleft->addWidget(spinboxhmin,1,1);
	layoutleft->addWidget(sliderhmin,1,2);

	layoutleft->addWidget(labelhmax,2,0);
	layoutleft->addWidget(spinboxhmax,2,1);
	layoutleft->addWidget(sliderhmax,2,2);

	layoutleft->addWidget(labelvmin,3,0);
	layoutleft->addWidget(spinboxvmin,3,1);
	layoutleft->addWidget(slidervmin,3,2);

	layoutleft->addWidget(labelvmax,4,0);
	layoutleft->addWidget(spinboxvmax,4,1);
	layoutleft->addWidget(slidervmax,4,2);

	layoutleft->addWidget(labelsmin,5,0);
	layoutleft->addWidget(spinboxsmin,5,1);
	layoutleft->addWidget(slidersmin,5,2);

	layoutleft->addWidget(labelsmax,6,0);
	layoutleft->addWidget(spinboxsmax,6,1);
	layoutleft->addWidget(slidersmax,6,2);

	layoutleft->addWidget(label_SIMILAR,7,0);
	layoutleft->addWidget(spinbox_SIMILAR,7,1);
	layoutleft->addWidget(slider_SIMILAR,7,2);

	layoutleft->addWidget(label_LIMIT1,8,0);
	layoutleft->addWidget(spinbox_LIMIT1,8,1);
	layoutleft->addWidget(slider_LIMIT1,8,2);

	layoutleft->addWidget(label_LIMIT2,9,0);
	layoutleft->addWidget(spinbox_LIMIT2,9,1);
	layoutleft->addWidget(slider_LIMIT2,9,2);

	layoutleft->addWidget(label_ActionNum,10,0);
	layoutleft->addWidget(spinbox_ActionNum,10,1);
	layoutleft->addWidget(slider_ActionNum,10,2);


	


	QObject::connect(sliderlightsetting, &QSlider::valueChanged,spinboxlightsetting, &QSpinBox::setValue);
	QObject::connect(sliderlightsetting,&QSlider::valueChanged,set_beta);
	QObject::connect(spinboxlightsetting,&QSpinBox::setValue,set_beta);
	void (QSpinBox::*spinboxlightsettingSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxlightsetting, spinboxlightsettingSignal,sliderlightsetting, &QSlider::setValue);
	spinboxlightsetting->setValue(20);

	QObject::connect(sliderhmin, &QSlider::valueChanged,spinboxhmin, &QSpinBox::setValue);
	QObject::connect(sliderhmin,&QSlider::valueChanged,set_H_min);
	QObject::connect(spinboxhmin,&QSpinBox::setValue,set_H_min);
	void (QSpinBox::*spinboxhminSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxhmin, spinboxhminSignal,sliderhmin, &QSlider::setValue);
	spinboxhmin->setValue(0);

	QObject::connect(sliderhmax, &QSlider::valueChanged,spinboxhmax, &QSpinBox::setValue);
	QObject::connect(sliderhmax,&QSlider::valueChanged,set_H_max);
	QObject::connect(spinboxhmax,&QSpinBox::setValue,set_H_max);
	void (QSpinBox::*spinboxhmaxSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxhmax, spinboxhmaxSignal,sliderhmax, &QSlider::setValue);
	spinboxhmax->setValue(40);

	QObject::connect(slidervmin, &QSlider::valueChanged,spinboxvmin, &QSpinBox::setValue);
	QObject::connect(slidervmin,&QSlider::valueChanged,set_V_min);
	QObject::connect(spinboxvmin,&QSpinBox::setValue,set_V_min);
	void (QSpinBox::*spinboxvminSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxvmin, spinboxvminSignal,slidervmin, &QSlider::setValue);
	spinboxvmin->setValue(0);

	QObject::connect(slidervmax, &QSlider::valueChanged,spinboxvmax, &QSpinBox::setValue);
	QObject::connect(slidervmax,&QSlider::valueChanged,set_V_max);
	QObject::connect(spinboxvmax,&QSpinBox::setValue,set_V_max);
	void (QSpinBox::*spinboxvmaxSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxvmax, spinboxvmaxSignal,slidervmax, &QSlider::setValue);
	spinboxvmax->setValue(250);

	QObject::connect(slidersmin, &QSlider::valueChanged,spinboxsmin, &QSpinBox::setValue);
	QObject::connect(slidersmin,&QSlider::valueChanged,set_S_min);
	QObject::connect(spinboxsmin,&QSpinBox::setValue,set_S_min);
	void (QSpinBox::*spinboxsminSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxsmin, spinboxsminSignal,slidersmin, &QSlider::setValue);
	spinboxsmin->setValue(30);

	QObject::connect(slidersmax, &QSlider::valueChanged,spinboxsmax, &QSpinBox::setValue);
	QObject::connect(slidersmax,&QSlider::valueChanged,set_S_max);
	QObject::connect(spinboxsmax,&QSpinBox::setValue,set_S_max);
	void (QSpinBox::*spinboxsmaxSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinboxsmax, spinboxsmaxSignal,slidersmax, &QSlider::setValue);
	spinboxsmax->setValue(170);

	QObject::connect(slider_ActionNum, &QSlider::valueChanged,spinbox_ActionNum, &QSpinBox::setValue);
	QObject::connect(slider_ActionNum,&QSlider::valueChanged, setActionNum);
	QObject::connect(spinbox_ActionNum,&QSpinBox::setValue, setActionNum);
	void (QSpinBox::*spinbox_ActionNumSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinbox_ActionNum, spinbox_ActionNumSignal,slider_ActionNum, &QSlider::setValue);
	spinbox_ActionNum->setValue(40);

	QObject::connect(slider_SIMILAR, &QSlider::valueChanged,spinbox_SIMILAR, &QSpinBox::setValue);
	QObject::connect(slider_SIMILAR,&QSlider::valueChanged, setSIMILAR);
	QObject::connect(spinbox_SIMILAR,&QSpinBox::setValue, setSIMILAR);
	void (QSpinBox::*spinbox_SIMILARSignal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinbox_SIMILAR, spinbox_SIMILARSignal,slider_SIMILAR, &QSlider::setValue);
	spinbox_SIMILAR->setValue(12);

	QObject::connect(slider_LIMIT1, &QSlider::valueChanged,spinbox_LIMIT1, &QSpinBox::setValue);
	QObject::connect(slider_LIMIT1,&QSlider::valueChanged, setLIMIT1);
	QObject::connect(spinbox_LIMIT1,&QSpinBox::setValue, setLIMIT1);
	void (QSpinBox::*spinbox_LIMIT1signal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinbox_LIMIT1, spinbox_LIMIT1signal,slider_LIMIT1, &QSlider::setValue);
	spinbox_LIMIT1->setValue(90);

	QObject::connect(slider_LIMIT2, &QSlider::valueChanged,spinbox_LIMIT2, &QSpinBox::setValue);
	QObject::connect(slider_LIMIT2,&QSlider::valueChanged, setLIMIT1);
	QObject::connect(spinbox_LIMIT2,&QSpinBox::setValue, setLIMIT1);
	void (QSpinBox::*spinbox_LIMIT2signal)(int)=&QSpinBox::valueChanged;
	QObject::connect(spinbox_LIMIT2, spinbox_LIMIT2signal,slider_LIMIT2, &QSlider::setValue);
	spinbox_LIMIT2->setValue(160);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	//mainLayout->setMargin(15);
	//mainLayout->setSpacing(10);
	mainLayout->addLayout(layoutleft);
	mainLayout->addLayout(layoutmiddle);
	mainLayout->addLayout(layoutRight);
	//mainLayout->setSizeConstraint(QLayout::SetFixedSize);




	mainWindow.setLayout(mainLayout);
	mainWindow.show();
	Gesture.show();



	return a.exec();       

}
