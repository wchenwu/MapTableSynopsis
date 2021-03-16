#pragma once
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

class clsTube
{
public:
	clsTube();
	virtual ~clsTube();

	//初始化
	void OnInitTube(int _id, int _src_fs, int _src_fe, string _filePath);

	//从文件中读取每帧的Activity值
	void readFrameActivityValue(string _fileName);
	//从文件中读取每帧object的bbox框坐标值
	void readFrameBboxRect(string _fileName);

	//得到浓缩视频中tube的结束帧
	int  getDstFrameEnd();
	//得到浓缩视频中tube的起始帧
	int  getDstFrameStart();
	//设置Tube在浓缩视频中的起始帧号
	void setDstFrameStart(int _sframe);

	//得到Tube在源视频中的开始帧和结束帧
	int  getSrcFrameStart();
	int  getSrcFrameEnd();

	//得到_srcframeid所对应帧的Rect和_synframeid
	bool  getInstanceInfo(int _srcframeid, enum TubeStatus& _status, Rect& _rect, int& _synframeid);

	//设置某帧在浓缩视频的缩放比例
	bool  setDstFrameSizeRatio(int _synframeid, double _size_r);
	//得到size变化率
	double  getSizeRatio(int _frameid);

	//打印最终每帧缩放比例
	void  printFrameSizeRatio();
	//得到tubeLength
	int   getSrcTubeLength();

	//得到Bbox坐标值
	bool getSrcBboxRect(int _frameid, Rect &_rect);


private:

	int  tubeID;				//tube的编号(从1开始计数)	
	string  preFilePath;		//预处理文件目录
	int  srcFrameStart, srcFrameEnd;	//tube在源视频中的起始和终止帧(不变)	
	int  srcTubeLength;				    //tube源视频中总帧长(不变)
	int  dstFrameStart;					//tube在浓缩视频中的起始帧

	double	*pFrameSizeRatio;		//每帧的大小变化因子

	double  *pFrameActivityValue;	//每帧的活动能量值(已经计算好，从文件读取)
	Rect	*pFrameBboxRect;		//每帧object的bbox框坐标值(已经计算好，从文件读取)	
};

