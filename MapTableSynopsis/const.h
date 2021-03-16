#pragma once
#include <iostream> 
#include <windows.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

//目标缩放的最小比例
const  double  constSizeLow = 0.5;
//源视频帧数
const int constSrcFrameNumber = 12000;
//3D Table的帧总数
const int constTableFrameLength = 1000;

//判定是否重叠的阈值
const double  constOverlapThreshold = 0.0001;

//tube与当前源视频帧号的关系
enum TubeStatus
{
	FIRST_FRAME = 1,
	NEXT_FRAME  = 2,
	NOT_EXIST	= 3
};

//源文件路径
const char constPreFilePath[80] = "D:\\01-VideoSynopsis\\TestVideo\\video4_tube";