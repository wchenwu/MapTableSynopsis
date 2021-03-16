#include "clsMapTable.h"
#include "opencv2/imgproc.hpp"



clsMapTable::clsMapTable()
{
	pTableData = NULL;
	cycleCount = 0;
	startFrameNumber = 0;
}


clsMapTable::~clsMapTable()
{
	if (pTableData != NULL)
		delete pTableData;
}


void clsMapTable::OnInitTable(int _width, int _height)
{
	int i,j,k;
	frameWidth	= _width;
	frameHeight = _height;
	tableFrameLength =constTableFrameLength;

	//创建三维数组
	pTableData = new int**[tableFrameLength];
	for (i = 0; i < tableFrameLength; i++)
	{
		pTableData[i] = new int*[frameHeight];
		for (j = 0; j < frameHeight; j++)
		{
			pTableData[i][j] = new int[frameWidth];
		}
	}

	//给三维数组赋值(置0)
	for (i = 0; i < tableFrameLength; i++)
		for (j = 0; j < frameHeight; j++)
			for (k = 0; k < frameWidth; k++)
				pTableData[i][j][k] = 0;

}

int clsMapTable::OnSearchPosForNewTube(Rect _rect)
{
	bool find = false;
	int i;
	//从起始位置开始查找
	for (i = startFrameNumber; i < tableFrameLength; i+=5)
	{
		if (!isOverlap(pTableData[i], _rect))
		{
			//修改Table表
			updateTableFrame(pTableData[i], _rect);
			return (i + cycleCount*tableFrameLength +1);
		}
			
	}
	//从0位置开始查找
	for (i = 0; i < startFrameNumber; i++)
	{
		if (!isOverlap(pTableData[i], _rect))
		{
			//修改Table表
			updateTableFrame(pTableData[i], _rect);

			return (i + (cycleCount + 1)*tableFrameLength+1);
		}
	}
	return -1;
}

double clsMapTable::OnComputeSizeRatio(int _synFrameNumber, Rect _rect)
{
	double _ratio =1.0,_count=0;
	int _tableFrameNumber;
	_tableFrameNumber = (_synFrameNumber-1)%tableFrameLength;
	
	int i, j;
	for (i = _rect.y; i < _rect.y + _rect.height; i++)
	{
		for (j = _rect.x; j < _rect.x + _rect.width; j++)
		{
			_count += pTableData[_tableFrameNumber][i][j];
		}
	}

	double _compareValue = _count / (_rect.width*_rect.height);
	double  alpha1= 0.05, alpha2=0.6,belta=0.8;
	if (_compareValue <= alpha1)
		_ratio = 1.0;
	else if (_compareValue <= alpha2)
		_ratio = 1.0 - _compareValue*belta;
	else
		_ratio = 1.0 - alpha2*belta;

	//update MapTable
	updateTableFrame(pTableData[_tableFrameNumber],_rect,_ratio);

	return _ratio;
}

//设计判定是否有区域重叠的算法
bool clsMapTable::isOverlap(int **_frameData, Rect _rect)
{
	int i, j;
	double _count=0;
	for (i = _rect.y; i < _rect.y + _rect.height; i++)
	{
		for (j = _rect.x; j < _rect.x + _rect.width; j++)
		{
			_count += _frameData[i][j];
		}
	}
	//cout << "_count: " << _count << endl;
	
	//重叠比小于一定阈值，则判定为不重叠，否则判定为重叠
	if (_count / (_rect.width*_rect.height) < constOverlapThreshold)
	{
		return false;
	}
	else
		return true;

	return false;

}

void clsMapTable::updateTableFrame(int **_frameData, Rect _rect, double _sizeRatio)
{
	Rect _newRect;
	if (_sizeRatio == 1.0)
		_newRect = _rect;
	else
		_newRect = rectScale(_rect, _sizeRatio);

	int i, j;
	for (i = _newRect.y; i < _newRect.y + _newRect.height; i++)
	{
		for (j = _newRect.x; j < _newRect.x + _newRect.width; j++)
		{
			_frameData[i][j] += 1;
		}
	}

	
	return;
}


Rect clsMapTable::rectScale(Rect _rect, double _size_r)
{
	_rect.x = _rect.x + 0.5*_rect.width - 0.5*_rect.width*_size_r;
	_rect.y = _rect.y + 0.5*_rect.height - 0.5*_rect.height*_size_r;

	_rect.width *= _size_r;
	_rect.height *= _size_r;
	return (_rect);
}