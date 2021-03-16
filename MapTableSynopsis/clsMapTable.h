#pragma once
#include "const.h"

using namespace cv;
using namespace std;

class clsMapTable
{
public:
	clsMapTable();
	virtual ~clsMapTable();

	//��ʼ��
	void OnInitTable(int _width,int _height);

	//Ϊ��TubeѰ�Һ��ʵ�λ��(���ص���Ũ����Ƶ�е�λ��)
	int OnSearchPosForNewTube(Rect _rect);
	
	//Ϊ����Tube��Ŀ��Ѱ�Һ��ʵ����ű���
	double  OnComputeSizeRatio(int _synFrameNumber,Rect _rect);

	//�ж��Ƿ��ص�
	bool  isOverlap(int **_frameData, Rect _rect);
	//����
	void  updateTableFrame(int **_frame, Rect _rect,double _sizeRatio=1.0);
	//���ο�����
	Rect rectScale(Rect _rect, double _size_r);

private:

	
	int frameWidth, frameHeight;		//MapTable֡�Ŀ�͸�
	int tableFrameLength;				//MapTable��֡��
	int startFrameNumber;				//MapTable�Ƚϵ���ʼ֡
	int cycleCount;						//ѭ������

	int ***pTableData;					//3d�������

};

