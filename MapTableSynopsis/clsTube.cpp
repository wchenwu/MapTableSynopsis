#include "const.h"
#include <fstream>
#include "clsTube.h"
#include <iostream>
#include <iomanip>



clsTube::clsTube()
{
	tubeID = -1;
	dstFrameStart = -1;

	pFrameSizeRatio		= NULL;
	pFrameActivityValue = NULL;
	pFrameBboxRect		= NULL;
}


clsTube::~clsTube()
{
	delete pFrameActivityValue;
	delete pFrameSizeRatio;
	delete pFrameBboxRect;
}

void clsTube::OnInitTube(int _id, int _src_fs, int _src_fe, string _filePath)
{
	int i;
	tubeID = _id;
	srcFrameStart = _src_fs;
	srcFrameEnd = _src_fe;
	srcTubeLength = srcFrameEnd - srcFrameStart + 1;

	preFilePath = _filePath;


	//根据tube的原长度，为activity和bbox分配空间
	pFrameSizeRatio = new double[srcTubeLength];
	pFrameActivityValue = new double[srcTubeLength];
	pFrameBboxRect = new Rect[srcTubeLength];

	//初始化sizeRatio为1
	for (i = 0; i < srcTubeLength; i++)
	{
		pFrameSizeRatio[i] = 1.0;
	}

	//读取文件中每帧Activity值
	string _fileActivityValue;
	_fileActivityValue = preFilePath + "\\" + to_string(tubeID) + "\\" + to_string(tubeID) + "value.txt";
	//cout << _fileActivityValue << endl;
	readFrameActivityValue(_fileActivityValue);

	//读取文件中每帧bbox坐标值
	string _fileBboxRect;
	_fileBboxRect = preFilePath + "\\" + to_string(tubeID) + "\\" + to_string(tubeID) + "node.txt";
	//cout << _fileBboxRect << endl;
	readFrameBboxRect(_fileBboxRect);


}

void clsTube::readFrameActivityValue(string _fileName)
{
	fstream _fileActivity;
	_fileActivity.open(_fileName, ios::in);
	if (!_fileActivity)
		cerr << "readFrameActivityValue文件打开错误" << endl;

	int i, j;
	int _id;
	for (i = srcFrameStart, j = 0; i < srcFrameEnd + 1; i++, j++)
	{
		_fileActivity >> _id;
		if (_id != i)
			cerr << "readFrameActivityValue value.txt值有问题" << endl;
		_fileActivity >> pFrameActivityValue[j];
	}

	_fileActivity.close();

}
void clsTube::readFrameBboxRect(string _fileName)
{
	fstream _fileBbox;
	_fileBbox.open(_fileName, ios::in);
	if (!_fileBbox)
		cerr << "readFrameBboxRect文件打开错误" << endl;

	int i, j;
	int _id;
	int _x, _y, _width, _height;
	for (i = srcFrameStart, j = 0; i < srcFrameEnd + 1; i++, j++)
	{
		_fileBbox >> _id;
		if (_id != i)
			cerr << "readFrameBboxRect node.txt值有问题" << endl;
		_fileBbox >> _x >> _y >> _width >> _height;

		pFrameBboxRect[j] = Rect(_x, _y, _width, _height);
	}

	_fileBbox.close();
}

int clsTube::getDstFrameEnd()
{
	return dstFrameStart + srcTubeLength;
}
int clsTube::getDstFrameStart()
{
	return dstFrameStart;
}

void clsTube::setDstFrameStart(int _sframe)
{
	dstFrameStart = _sframe;
}

int clsTube::getSrcFrameEnd()
{
	return srcFrameEnd;
}

int clsTube::getSrcFrameStart()
{
	return srcFrameStart;
}

bool clsTube::getInstanceInfo(int _srcframeid, enum TubeStatus& _status, Rect& _rect, int& _synframeid)
{
	if (_srcframeid < srcFrameStart || _srcframeid >srcFrameEnd)
	{
		_status = NOT_EXIST;
	}
	else if (_srcframeid == srcFrameStart)
	{
		_status = FIRST_FRAME;
		_rect = pFrameBboxRect[_srcframeid - srcFrameStart];

	}
	else
	{
		_status = NEXT_FRAME;
		_rect = pFrameBboxRect[_srcframeid - srcFrameStart];

		_synframeid = dstFrameStart + (_srcframeid - srcFrameStart);
	}	

	return true;
}

bool clsTube::setDstFrameSizeRatio(int _synframeid, double _size_r)
{
	int _frameid = _synframeid - dstFrameStart;
	if (_frameid <0 || _frameid >srcTubeLength)
		return false;

	pFrameSizeRatio[_frameid] = _size_r;

	return true;
}


void clsTube::printFrameSizeRatio()
{
	int i;
	for (i = 0; i < srcTubeLength; i++)
		cout <<"tubeID: "<<tubeID<<" frameid: "<<i+1<<" SizeRatio: "<< pFrameSizeRatio[i] << endl;
}

int clsTube::getSrcTubeLength()
{
	return srcTubeLength;
}

//返回false，表示没有找到对应值
bool clsTube::getSrcBboxRect(int _frameid, Rect& _rect)
{
	if (_frameid < 0 || _frameid >= srcTubeLength)
		return false;
	_rect = pFrameBboxRect[_frameid];

	return true;
}

double clsTube::getSizeRatio(int _frameid)
{
	if (_frameid >= 0 && _frameid < srcTubeLength)
		return pFrameSizeRatio[_frameid];
	return 1;
}