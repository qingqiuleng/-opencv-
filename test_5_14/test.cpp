
/*#include <string.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <stdio.h>

using namespace std;
using namespace cv;
void Video_to_Image(string filename)
{
	VideoCapture capture(filename);//获取VideoCapture 对象
	if (!capture.isOpened())
	{
		cout << "open video error";
	}
	//capture.get十分强大 可以获得帧的很多信息
	int frame_width = (int)capture.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)capture.get(CAP_PROP_FRAME_HEIGHT);
	int frame_number = capture.get(CAP_PROP_FRAME_COUNT);
	cout << "frame_width is " << frame_width << endl;
	cout << "frame_height is " << frame_height << endl;
	cout << "frame_number is " << frame_number << endl;
	srand((unsigned)time(NULL)); //时间点
	long frameToStart = rand() % frame_number; //取  最大帧数之内的 随机数
	cout << "帧开始的地方" << frameToStart << endl;
	//capture.set(CAP_PROP_POS_FRAMES, frameToStart);//从此时的帧数开始获取帧

	/*Mat frame;//Mat对象 其实就是图像对象
	char image_name[100];
	if (!capture.read(frame))
	{
		cout << "读取视频失败" << endl;
	}
	imshow("che", frame);//显示
	sprintf(image_name, "%s%s", "imageNew1", ".jpg");//保存的图片名
	imwrite(image_name, frame); //写入前面是path + name不要忘了后缀后面是帧
	capture.release(); //释放对象  //注释

	Mat frame;
	namedWindow("Movie Player");

	double position = 0.0;
	//capture.set(CV_CAP_PROP_POS_FRAMES, position);

	//设置播放到哪一帧，这里设置为第0帧

	while (1)
	{
		//读取视频帧
		if (!capture.read(frame))
			break;

		imshow("Movie Player", frame);
		//获取按键值
		char c = waitKey(1);//150的延时的播放速度特别慢，是上一帧播放完了等150ms再进行播放下一帧
		if (c == 27) //按ESC跳出播放
			break;
	}
	capture.release();
}
int main()
{
	string filename = "D:\\Users\\xuetao_yuan\\source\\repos\\souce\\data\\test.mp4";
	Video_to_Image(filename);
	waitKey(0);
	return 0;
}*/



#include<opencv2/opencv.hpp>
#include<opencv2/tracking.hpp>
#include <fstream>
#include<iostream>
#include<String.h>
#include <windows.h>
using namespace cv;
using namespace std;

String ConfigFileRead()
{
	printf("开始读取视频和保存图片的路径\n");
	ifstream infile;
	String con_path("");
	char buf[256];
	GetCurrentDirectory(256, buf);
	con_path.append(buf);
	con_path += "\\config.ini";
	
	//infile.open("D:\\Users\\xuetao_yuan\\source\\repos\\souce\\test_5_14\\test_5_14\\config.ini");   //将文件流对象与文件连接起来 
	infile.open(con_path);
	assert(infile.is_open());                   //检查数据合法性
						  
	String ret_str(""),tmp_str("");
	while (getline(infile, tmp_str))       //第一参数:文件,第二参数:读入地方,第三参数:终止字符
	{
		ret_str += tmp_str + "+";
	}
	infile.close();             //关闭文件输入流 

	printf("完成读取视频和保存图片的路径\n");
	return ret_str;
}

void get_face_fromvideo()
{
	CascadeClassifier faceCascader, eyeCascader; //级联分类器

	//加载OpenCv的人脸特征分析器
	//String filename1 = "D:\\Users\\xuetao_yuan\\source\\repos\\souce\\data1\\haarcascade_frontalface_alt2.xml";
	String filename1("");
	char buf[256];
	GetCurrentDirectory(256, buf);
	filename1.append(buf);
	filename1 += "\\haarcascade_frontalface_alt2.xml";

	String filename = ConfigFileRead();
	size_t pos = filename.find("+");
	String filename_video = filename.substr(0, pos);
	String filename_save = filename.substr(pos+1, filename.length()-2-pos);
	if (filename_video.empty() || filename_save.empty())
	{
		return;
	}
	printf("开始加载模型文件：%s\n", filename1);
	if (!faceCascader.load(filename1))
	{
		printf("can not load the face feature data \n");
		system("pause");
		return;
	}

	//namedWindow("input", WINDOW_AUTOSIZE);
	//namedWindow("output", WINDOW_AUTOSIZE);
	VideoCapture capture;

	//读取视频文件
	//capture.open("D:\\Users\\xuetao_yuan\\source\\repos\\souce\\data1\\test.mp4");
	capture.open(filename_video);
	if (!capture.isOpened())
	{
		printf("can not load the video \n");
		return;
	}
	Mat frame, gray, faceROI, src;
	std::vector<Rect> faces, eyes;
	int k = 0;
	double video_frames_num = capture.get(7);
	printf("视频一共有：%f帧\n", video_frames_num);
	//return;
	//循环读取（把每一帧信息放到frame中）
	double cur_frames = 0;
	while (capture.read(frame))
	{

		//double video_frames = capture.get(0);
		printf("当百分比为：%f%%\n", 100*(cur_frames / video_frames_num));
		cur_frames++;

		printf("当百分比为\n");
		src = frame;
		//颜色转换，形成新的矩阵gray
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		//直方图均衡化
		equalizeHist(gray, gray);

		faceCascader.detectMultiScale(gray, faces, 1.2, 3, 0, Size(100, 100));
		for (int i = 0; i < faces.size(); i++)
		{
			Rect roi;
			roi.x = faces[static_cast<int>(i)].x + 2;
			roi.y = faces[static_cast<int>(i)].y + 2;
			roi.width = faces[static_cast<int>(i)].width - 2;
			roi.height = faces[static_cast<int>(i)].height - 2;
			//人脸所在的感兴趣区域
			faceROI = frame(roi);
			rectangle(frame, faces[static_cast<int>(i)], Scalar(0, 255, 0), 1, 8, 0);
			k++;
			//将人脸图像的大小统一调整为150*150
			resize(faceROI, faceROI, Size(150, 150));

			//输出文件路径			
			//sprintf(filename, "D:\\Users\\xuetao_yuan\\source\\repos\\souce\\data2\\facePicture%02d.jpg", k);
			
			String str_buf = filename_save + "\\";
			char filename_buf[100];
			sprintf(filename_buf, "%02d.jpg", k);
			str_buf.append(filename_buf);
			imwrite(str_buf, faceROI);
		}
		char q = waitKey(1);
		if (q == 27)
		{
			break;
		}
		//imshow("input", frame);
		//imshow("output", faceROI);
	}

    waitKey(0);
	capture.release();
}

int main()
{
	printf("开始进入main函数\n");
	get_face_fromvideo();
	printf("执行完毕!\n");
	system("pause");
	return 0;
}