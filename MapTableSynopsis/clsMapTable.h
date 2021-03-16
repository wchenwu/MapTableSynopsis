#pragma once
#include "const.h"

using namespace cv;
using namespace std;

class clsMapTable
{
public:
	clsMapTable();
	virtual ~clsMapTable();

	//初始化
	void OnInitTable(int _width,int _height);

	//为新Tube寻找合适的位置(返回的是浓缩视频中的位置)
	int OnSearchPosForNewTube(Rect _rect);
	
	//为已有Tube的目标寻找合适的缩放比例
	double  OnComputeSizeRatio(int _synFrameNumber,Rect _rect);

	//判断是否重叠
	bool  isOverlap(int **_frameData, Rect _rect);
	//更新
	void  updateTableFrame(int **_frame, Rect _rect,double _sizeRatio=1.0);
	//矩形框缩放
	Rect rectScale(Rect _rect, double _size_r);

private:

	
	int frameWidth, frameHeight;		//MapTable帧的宽和高
	int tableFrameLength;				//MapTable总帧长
	int startFrameNumber;				//MapTable比较的起始帧
	int cycleCount;						//循环次数

	int ***pTableData;					//3d表的数据

};

