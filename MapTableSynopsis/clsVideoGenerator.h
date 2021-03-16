#pragma once
#include "const.h"
#include "clsTubeManager.h"

class clsVideoGenerator
{
public:
	clsVideoGenerator(clsTubeManager* _manager, string _filepath);
	virtual ~clsVideoGenerator();

	//生成视频
	void  OnGenerateVideo();
	//tube中图像帧融合
	void  TubeFrameStitching(int _tubeid, string _filepath);
	//图像拼接
	void  ImageStitching(Mat& _image, double _size_r, int _centerx, int _centery, string _srcfilename, string _maskfilename);
	//根据比例对rect进行缩放
	Rect  rectScale(Rect  _rect, double _size_r);
	//将图像合并成视频
	void  MergeImagesToVideo(Mat *_pFrames, int _fps, bool _iscolor, string _videoname,int _synopsisLen);

private:
	clsTubeManager *pTubeManager;
	//存储原视频帧图目录
	string  srcFramesFilePath;
	//背景文件名
	string  backgroundFileName;
	//图像序列
	Mat *pImageSequence;

};

