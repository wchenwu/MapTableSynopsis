#pragma once

#include "const.h"
#include "clsTube.h"
using namespace std;



class clsTubeManager
{
public:
	clsTubeManager(string _filePath);
	virtual ~clsTubeManager();

	//��ʼ��
	void initTubeManager();

	//�õ���ǰʱ���ַ���
	string getTimeString();

	//�õ�tube������ 
	int getTubeNumber();
	//�õ�Ԥ�����ļ�Ŀ¼
	string getPreFilePath();
	//����Ũ������Ƶ����
	void setSynVideoLength(int _len);
	//�õ�Ũ������Ƶ����
	int getSynVideoLength();
	//�õ�Դ��Ƶ����
	int getSrcVideoLength();

	//����Դ��Ƶ����
	int computeSrcVideoLength();

	//�õ�Ŀ����Ϣ
	bool getInstanceInfo(int _tubeID, int _srcframeID,enum TubeStatus& _status,Rect& _rect,int& _synframeID);

	//����tube��dst��ʼ֡��Ϣ
	bool setDstFrameStart(int _tubeid,int _frameid);
	
	//����tube����Ũ����Ƶ�е����ű���
	bool setDstFrameSizeRatio(int _tubeid, int _synframeid, double _size_r);

	//����ͼƬ�Ŀ�Ⱥ͸߶�
	void computeImageWidthAndHeight();
	//�õ�ͼƬ�Ŀ�Ⱥ͸߶�
	void getImageWidthAndHeight(int& _width,int &_height);

	//��ӡŨ�������Ϣ
	void printTubeListInfo();

	//����Ũ����Ƶ����
	void computeSynopsisVideoLen();
	//�õ�tube
	clsTube* getTube(int _tubeid);


private:

	int  tubeNumber;		//tube����
	clsTube  *pTubeList;	//tube�б�

	string  preFilePath;	//Ԥ�����ļ�Ŀ¼
	string  startTime;		//����ʱ�䣬������־�ļ���

	int   srcVideoLength;
	int   synVideoLength;

	//ͼƬ�Ŀ�Ⱥ͸߶�
	int   imageWidth, imageHeight;

	//�����ļ���
	string  backgroundFileName;
};

