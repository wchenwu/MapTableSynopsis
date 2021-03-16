#pragma once
#include "const.h"
#include "clsTubeManager.h"

class clsVideoGenerator
{
public:
	clsVideoGenerator(clsTubeManager* _manager, string _filepath);
	virtual ~clsVideoGenerator();

	//������Ƶ
	void  OnGenerateVideo();
	//tube��ͼ��֡�ں�
	void  TubeFrameStitching(int _tubeid, string _filepath);
	//ͼ��ƴ��
	void  ImageStitching(Mat& _image, double _size_r, int _centerx, int _centery, string _srcfilename, string _maskfilename);
	//���ݱ�����rect��������
	Rect  rectScale(Rect  _rect, double _size_r);
	//��ͼ��ϲ�����Ƶ
	void  MergeImagesToVideo(Mat *_pFrames, int _fps, bool _iscolor, string _videoname,int _synopsisLen);

private:
	clsTubeManager *pTubeManager;
	//�洢ԭ��Ƶ֡ͼĿ¼
	string  srcFramesFilePath;
	//�����ļ���
	string  backgroundFileName;
	//ͼ������
	Mat *pImageSequence;

};

