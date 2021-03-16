#pragma once

#include "const.h"
#include "clsTube.h"
using namespace std;



class clsTubeManager
{
public:
	clsTubeManager(string _filePath);
	virtual ~clsTubeManager();

	//初始化
	void initTubeManager();

	//得到当前时间字符串
	string getTimeString();

	//得到tube的数量 
	int getTubeNumber();
	//得到预处理文件目录
	string getPreFilePath();
	//设置浓缩后视频长度
	void setSynVideoLength(int _len);
	//得到浓缩后视频长度
	int getSynVideoLength();
	//得到源视频长度
	int getSrcVideoLength();

	//计算源视频长度
	int computeSrcVideoLength();

	//得到目标信息
	bool getInstanceInfo(int _tubeID, int _srcframeID,enum TubeStatus& _status,Rect& _rect,int& _synframeID);

	//设置tube的dst起始帧信息
	bool setDstFrameStart(int _tubeid,int _frameid);
	
	//设置tube的在浓缩视频中的缩放比例
	bool setDstFrameSizeRatio(int _tubeid, int _synframeid, double _size_r);

	//计算图片的宽度和高度
	void computeImageWidthAndHeight();
	//得到图片的宽度和高度
	void getImageWidthAndHeight(int& _width,int &_height);

	//打印浓缩后的信息
	void printTubeListInfo();

	//计算浓缩视频长度
	void computeSynopsisVideoLen();
	//得到tube
	clsTube* getTube(int _tubeid);


private:

	int  tubeNumber;		//tube数量
	clsTube  *pTubeList;	//tube列表

	string  preFilePath;	//预处理文件目录
	string  startTime;		//启动时间，用作标志文件名

	int   srcVideoLength;
	int   synVideoLength;

	//图片的宽度和高度
	int   imageWidth, imageHeight;

	//背景文件名
	string  backgroundFileName;
};

