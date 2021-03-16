#include "clsTubeManager.h"
#include <ctime>
#include <fstream>
#include <algorithm> 

#pragma warning(disable:4996)

clsTubeManager::clsTubeManager(string _filePath)
{
	pTubeList = NULL;
	preFilePath = _filePath;

	srcVideoLength = 0;
	synVideoLength = 0;
}


clsTubeManager::~clsTubeManager()
{
	if (pTubeList != NULL)
		delete pTubeList;
}


void clsTubeManager::initTubeManager()
{
	//存储启动时间
	startTime = getTimeString();

	//从文件中读取每个tube的编号、起始帧号和终止帧号
	fstream _fileTubeFrame;
	_fileTubeFrame.open(preFilePath + "\\" + "tubeframe.txt");

	//读取文件中的tubeNumber
	_fileTubeFrame >> tubeNumber;
	cout << "tubeNumber: " << tubeNumber << endl;

	int i;

	//创建tubeList
	pTubeList = new clsTube[tubeNumber];

	int _tubeID, _startFrame, _endFrame;

	for (i = 0; i < tubeNumber; i++)
	{
		//读取文件中数据
		_fileTubeFrame >> _tubeID >> _startFrame >> _endFrame;
		//cout << "tubeID: " << _tubeID << " startF: " << _startFrame << " endF: " << _endFrame << endl;

		//对每个tube进行初始化
		pTubeList[i].OnInitTube(_tubeID, _startFrame, _endFrame, preFilePath);

	}
	//计算源视频帧长度
	srcVideoLength = computeSrcVideoLength();
	cout << "srcVideoLen: " << srcVideoLength << endl;

	//关闭文件
	_fileTubeFrame.close();

	//计算图片的宽度和高度
	computeImageWidthAndHeight();

}

string clsTubeManager::getTimeString()
{
	time_t _curTime;
	std::time(&_curTime);
	char _timeStr[64];
	strftime(_timeStr, sizeof(_timeStr), "%Y_%m_%d_%H_%M_%S", localtime(&_curTime));
	return _timeStr;
}

int clsTubeManager::getTubeNumber()
{
	return tubeNumber;
}

string clsTubeManager::getPreFilePath()
{
	return preFilePath;
}


int clsTubeManager::getSrcVideoLength()
{
	return srcVideoLength;
}

void clsTubeManager::setSynVideoLength(int _len)
{
	synVideoLength = _len;
}

int clsTubeManager::getSynVideoLength()
{
	return synVideoLength;
}

int clsTubeManager::computeSrcVideoLength()
{
	int i,len = 0;

	for (i = 0; i < tubeNumber; i++)
	{
		if (len < pTubeList[i].getSrcFrameEnd())
			len = pTubeList[i].getSrcFrameEnd();
		
	}
	return len;
}

bool clsTubeManager::getInstanceInfo(int _tubeid, int _srcframeid, enum TubeStatus& _status, Rect& _rect, int& _synframeid)
{
	if (_tubeid < 0 || _tubeid >= tubeNumber)
		return false;

	//get instance information from the tube
	pTubeList[_tubeid].getInstanceInfo(_srcframeid,_status,_rect,_synframeid);
	
	return true;

}

bool clsTubeManager::setDstFrameStart(int _tubeid, int _frameid)
{
	if (_tubeid < 0 || _tubeid >= tubeNumber)
		return false;

	pTubeList[_tubeid].setDstFrameStart(_frameid);
	return true;
}

bool clsTubeManager::setDstFrameSizeRatio(int _tubeid, int _synframeid, double _size_r)
{
	if (_tubeid < 0 || _tubeid >= tubeNumber)
		return false;

	pTubeList[_tubeid].setDstFrameSizeRatio(_synframeid, _size_r);

	return true;
}

void clsTubeManager::getImageWidthAndHeight(int& _width, int &_height)
{
	_width  = imageWidth;
	_height = imageHeight;
}

void clsTubeManager::computeImageWidthAndHeight()
{
	backgroundFileName = preFilePath + "\\0.png";
	Mat bgImage = imread(backgroundFileName, 3);

	imageWidth = bgImage.cols;
	imageHeight = bgImage.rows;

	cout << "width: " << imageWidth << " height: " << imageHeight << endl;

}

void clsTubeManager::printTubeListInfo()
{
	int i;
	for (i = 0; i < tubeNumber; i++)
	{
		//pTubeList[i].printFrameSizeRatio();
		//cout << "tubeid: " << i + 1 << " framestart: " << pTubeList[i].getDstFrameStart() << endl;
	}
	
	cout << " the synopsis len: " << synVideoLength << endl;


}

void clsTubeManager::computeSynopsisVideoLen()
{
	int len=0;
	for (int i = 0; i < tubeNumber; i++)
	{
		if (len < pTubeList[i].getDstFrameEnd())
			len = pTubeList[i].getDstFrameEnd();
	}

	synVideoLength = len;
}


clsTube* clsTubeManager::getTube(int _tubeid)
{
	if (_tubeid >= 0 && _tubeid<tubeNumber)
		return &pTubeList[_tubeid];
	return NULL;
}