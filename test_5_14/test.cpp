
/*#include <string.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <stdio.h>

using namespace std;
using namespace cv;
void Video_to_Image(string filename)
{
	VideoCapture capture(filename);//��ȡVideoCapture ����
	if (!capture.isOpened())
	{
		cout << "open video error";
	}
	//capture.getʮ��ǿ�� ���Ի��֡�ĺܶ���Ϣ
	int frame_width = (int)capture.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = (int)capture.get(CAP_PROP_FRAME_HEIGHT);
	int frame_number = capture.get(CAP_PROP_FRAME_COUNT);
	cout << "frame_width is " << frame_width << endl;
	cout << "frame_height is " << frame_height << endl;
	cout << "frame_number is " << frame_number << endl;
	srand((unsigned)time(NULL)); //ʱ���
	long frameToStart = rand() % frame_number; //ȡ  ���֡��֮�ڵ� �����
	cout << "֡��ʼ�ĵط�" << frameToStart << endl;
	//capture.set(CAP_PROP_POS_FRAMES, frameToStart);//�Ӵ�ʱ��֡����ʼ��ȡ֡

	/*Mat frame;//Mat���� ��ʵ����ͼ�����
	char image_name[100];
	if (!capture.read(frame))
	{
		cout << "��ȡ��Ƶʧ��" << endl;
	}
	imshow("che", frame);//��ʾ
	sprintf(image_name, "%s%s", "imageNew1", ".jpg");//�����ͼƬ��
	imwrite(image_name, frame); //д��ǰ����path + name��Ҫ���˺�׺������֡
	capture.release(); //�ͷŶ���  //ע��

	Mat frame;
	namedWindow("Movie Player");

	double position = 0.0;
	//capture.set(CV_CAP_PROP_POS_FRAMES, position);

	//���ò��ŵ���һ֡����������Ϊ��0֡

	while (1)
	{
		//��ȡ��Ƶ֡
		if (!capture.read(frame))
			break;

		imshow("Movie Player", frame);
		//��ȡ����ֵ
		char c = waitKey(1);//150����ʱ�Ĳ����ٶ��ر���������һ֡�������˵�150ms�ٽ��в�����һ֡
		if (c == 27) //��ESC��������
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
	printf("��ʼ��ȡ��Ƶ�ͱ���ͼƬ��·��\n");
	ifstream infile;
	String con_path("");
	char buf[256];
	GetCurrentDirectory(256, buf);
	con_path.append(buf);
	con_path += "\\config.ini";
	
	//infile.open("D:\\Users\\xuetao_yuan\\source\\repos\\souce\\test_5_14\\test_5_14\\config.ini");   //���ļ����������ļ��������� 
	infile.open(con_path);
	assert(infile.is_open());                   //������ݺϷ���
						  
	String ret_str(""),tmp_str("");
	while (getline(infile, tmp_str))       //��һ����:�ļ�,�ڶ�����:����ط�,��������:��ֹ�ַ�
	{
		ret_str += tmp_str + "+";
	}
	infile.close();             //�ر��ļ������� 

	printf("��ɶ�ȡ��Ƶ�ͱ���ͼƬ��·��\n");
	return ret_str;
}

void get_face_fromvideo()
{
	CascadeClassifier faceCascader, eyeCascader; //����������

	//����OpenCv����������������
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
	printf("��ʼ����ģ���ļ���%s\n", filename1);
	if (!faceCascader.load(filename1))
	{
		printf("can not load the face feature data \n");
		system("pause");
		return;
	}

	//namedWindow("input", WINDOW_AUTOSIZE);
	//namedWindow("output", WINDOW_AUTOSIZE);
	VideoCapture capture;

	//��ȡ��Ƶ�ļ�
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
	printf("��Ƶһ���У�%f֡\n", video_frames_num);
	//return;
	//ѭ����ȡ����ÿһ֡��Ϣ�ŵ�frame�У�
	double cur_frames = 0;
	while (capture.read(frame))
	{

		//double video_frames = capture.get(0);
		printf("���ٷֱ�Ϊ��%f%%\n", 100*(cur_frames / video_frames_num));
		cur_frames++;

		printf("���ٷֱ�Ϊ\n");
		src = frame;
		//��ɫת�����γ��µľ���gray
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		//ֱ��ͼ���⻯
		equalizeHist(gray, gray);

		faceCascader.detectMultiScale(gray, faces, 1.2, 3, 0, Size(100, 100));
		for (int i = 0; i < faces.size(); i++)
		{
			Rect roi;
			roi.x = faces[static_cast<int>(i)].x + 2;
			roi.y = faces[static_cast<int>(i)].y + 2;
			roi.width = faces[static_cast<int>(i)].width - 2;
			roi.height = faces[static_cast<int>(i)].height - 2;
			//�������ڵĸ���Ȥ����
			faceROI = frame(roi);
			rectangle(frame, faces[static_cast<int>(i)], Scalar(0, 255, 0), 1, 8, 0);
			k++;
			//������ͼ��Ĵ�Сͳһ����Ϊ150*150
			resize(faceROI, faceROI, Size(150, 150));

			//����ļ�·��			
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
	printf("��ʼ����main����\n");
	get_face_fromvideo();
	printf("ִ�����!\n");
	system("pause");
	return 0;
}