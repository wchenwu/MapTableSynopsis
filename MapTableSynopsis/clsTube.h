#pragma once
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

class clsTube
{
public:
	clsTube();
	virtual ~clsTube();

	//��ʼ��
	void OnInitTube(int _id, int _src_fs, int _src_fe, string _filePath);

	//���ļ��ж�ȡÿ֡��Activityֵ
	void readFrameActivityValue(string _fileName);
	//���ļ��ж�ȡÿ֡object��bbox������ֵ
	void readFrameBboxRect(string _fileName);

	//�õ�Ũ����Ƶ��tube�Ľ���֡
	int  getDstFrameEnd();
	//�õ�Ũ����Ƶ��tube����ʼ֡
	int  getDstFrameStart();
	//����Tube��Ũ����Ƶ�е���ʼ֡��
	void setDstFrameStart(int _sframe);

	//�õ�Tube��Դ��Ƶ�еĿ�ʼ֡�ͽ���֡
	int  getSrcFrameStart();
	int  getSrcFrameEnd();

	//�õ�_srcframeid����Ӧ֡��Rect��_synframeid
	bool  getInstanceInfo(int _srcframeid, enum TubeStatus& _status, Rect& _rect, int& _synframeid);

	//����ĳ֡��Ũ����Ƶ�����ű���
	bool  setDstFrameSizeRatio(int _synframeid, double _size_r);
	//�õ�size�仯��
	double  getSizeRatio(int _frameid);

	//��ӡ����ÿ֡���ű���
	void  printFrameSizeRatio();
	//�õ�tubeLength
	int   getSrcTubeLength();

	//�õ�Bbox����ֵ
	bool getSrcBboxRect(int _frameid, Rect &_rect);


private:

	int  tubeID;				//tube�ı��(��1��ʼ����)	
	string  preFilePath;		//Ԥ�����ļ�Ŀ¼
	int  srcFrameStart, srcFrameEnd;	//tube��Դ��Ƶ�е���ʼ����ֹ֡(����)	
	int  srcTubeLength;				    //tubeԴ��Ƶ����֡��(����)
	int  dstFrameStart;					//tube��Ũ����Ƶ�е���ʼ֡

	double	*pFrameSizeRatio;		//ÿ֡�Ĵ�С�仯����

	double  *pFrameActivityValue;	//ÿ֡�Ļ����ֵ(�Ѿ�����ã����ļ���ȡ)
	Rect	*pFrameBboxRect;		//ÿ֡object��bbox������ֵ(�Ѿ�����ã����ļ���ȡ)	
};

