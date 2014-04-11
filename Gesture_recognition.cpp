#include"Gesture_recognition.h"


void saveTemplate(vector<vector<Point> >t)//此函数用于对获取的手势模板进行保存保存在template1.txt和template2.txt中
{
		ofstream template_file1("template3.txt",ios::out);
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
			
	/*	ofstream template_file2("template2.txt",ios::out);
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
	
		template_file2.close();*/
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
	Target target1;
	vector<vector<Point> >T;
	VideoCapture templa;
	Mat frame;
	templa>>frame;
	for(;;)
	{
			
		target1.setRawFame(frame);
		target1.showRawFrame();
		target1.getHSVmask();
		target1.getContours();
		target1.showContours();
		int c = waitKey(0);
		if(c =='y')
		{
			T.push_back(target1.contours[0]);
			cout<<"get template "<<T.size()<<endl;
		}
		if(c =='q')
			break;

		target1.contours.clear();
	}
		saveTemplate(T);
		cout<<"templates have been saved"<<T[0]<<endl<<T[1]<<endl;
		target1.contours.clear();
		
}
