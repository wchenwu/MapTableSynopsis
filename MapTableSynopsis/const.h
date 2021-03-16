#pragma once
#include <iostream> 
#include <windows.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//Ŀ�����ŵ���С����
const  double  constSizeLow = 0.5;
//Դ��Ƶ֡��
const int constSrcFrameNumber = 12000;
//3D Table��֡����
const int constTableFrameLength = 1000;

//�ж��Ƿ��ص�����ֵ
const double  constOverlapThreshold = 0.0001;

//tube�뵱ǰԴ��Ƶ֡�ŵĹ�ϵ
enum TubeStatus
{
	FIRST_FRAME = 1,
	NEXT_FRAME  = 2,
	NOT_EXIST	= 3
};

//Դ�ļ�·��
const char constPreFilePath[80] = "D:\\01-VideoSynopsis\\TestVideo\\video4_tube";