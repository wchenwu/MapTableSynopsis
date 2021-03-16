#include "clsVideoGenerator.h"
#include "opencv2/imgproc.hpp"


clsVideoGenerator::clsVideoGenerator(clsTubeManager* _manager, string _filepath)
{
	pTubeManager = _manager;
	srcFramesFilePath = _filepath + "\\outputs\\input\\";
	backgroundFileName = _filepath + "\\0.png";

	pImageSequence = NULL;

	pImageSequence = new Mat[constSrcFrameNumber];
}


clsVideoGenerator::~clsVideoGenerator()
{
	if (pImageSequence != NULL)
		delete pImageSequence;
}

void clsVideoGenerator::OnGenerateVideo()
{
	int i;
	string _tubeiFilePath;	//tubeid文件目录

	for (i = 0; i < pTubeManager->getTubeNumber(); i++)
	{
		_tubeiFilePath = pTubeManager->getPreFilePath() + "\\" + to_string(i + 1) + "\\";
		TubeFrameStitching(i, _tubeiFilePath);
		//cout << "tubeID: " << i + 1 << " stitching finished!" << endl;
	}
	cout << "Video Stitching finished!" << endl;

	string _videoname = pTubeManager->getTimeString() + ".avi";
	//将图像序列合并成视频
	MergeImagesToVideo(pImageSequence, 25, true, _videoname,pTubeManager->getSynVideoLength());

}

//待实现
void clsVideoGenerator::TubeFrameStitching(int _tubeid, string _tubefilepath)
{
	int i, j;

	Rect _bbox;
	int _centerx, _centery;
	double _rsize;
	string _srcfile;
	string _maskfile;
	//指向tubeid的tube
	clsTube *pTube = pTubeManager->getTube(_tubeid);
	if (pTube == NULL)
		return;

	//在浓缩视频中的帧号
	int _dstframeIndex = pTube->getDstFrameStart();
	int _tubeLen	   = pTube->getSrcTubeLength();
	//背景
	Mat  bgImage = imread(backgroundFileName, 3);

	int _offsetframe;
	for (i = 0; i < _tubeLen; i++)
	{
		//帧偏移量
		_offsetframe = i;

		//如果当前帧序列没有数据，将背景图像拷贝到当前帧图像
		if (pImageSequence[_dstframeIndex].cols == 0)
			bgImage.copyTo(pImageSequence[_dstframeIndex]);

		if (!pTube->getSrcBboxRect(_offsetframe, _bbox))
			return;

		_centerx = _bbox.x + 0.5*_bbox.width;
		_centery = _bbox.y + 0.5*_bbox.height;
		_rsize = pTube->getSizeRatio(_offsetframe);
		_srcfile = srcFramesFilePath + to_string(pTube->getSrcFrameStart() + _offsetframe) + ".png";
		_maskfile = _tubefilepath + to_string(pTube->getSrcFrameStart() + _offsetframe) + ".png";
		//图像融合
		ImageStitching(pImageSequence[_dstframeIndex], _rsize, _centerx, _centery, _srcfile, _maskfile);

		//画矩形框
		rectangle(pImageSequence[_dstframeIndex], rectScale(_bbox, _rsize), Scalar(0, 0, 255));

		_dstframeIndex++;

	}

}


void clsVideoGenerator::ImageStitching(Mat& _dstimage, double _size_r, int _centerx, int _centery, string _srcfilename, string _maskfilename)
{
	//读取原图片和mask图片
	Mat _srcImage = imread(_srcfilename, 3);
	Mat _maskImage = imread(_maskfilename, 0);


	//这里看一函数定义：
	//前两个参数：原图，输出图。没什么好说的
	//第三四五个参数是选取使用的，主要就是规定缩放的倍数，
	//dsize相当于直接规定了缩放后要得到的大小，这个要是设置了，后面的fx fy有默认值，是不需要设置的
	//fx fy是跟dsize并行的缩放信息，就是在dsize为0的时候，这两个值必须设置，表x y方向的缩放倍率
	//最后一个为插值方式，这里默认的是线性插值，还有许多插值方式，各有特点
	/*CV_EXPORTS_W void resize( InputArray src, OutputArray dst,
	Size dsize, double fx = 0, double fy = 0,
	int interpolation = INTER_LINEAR );
	*/

	//根据需要调整图片大小
	resize(_srcImage, _srcImage, Size(), _size_r, _size_r);
	resize(_maskImage, _maskImage, Size(), _size_r, _size_r);

	//生成在目标图片的ROI区域，实际上就是原图片缩放后的整个图片区域
	//？？？拷贝到目标图片的什么位置，待考证！
	Rect roi_rect = cv::Rect(_centerx*(1 - _size_r), _centery*(1 - _size_r), _srcImage.cols, _srcImage.rows);
	//注意Mat::copyTo()函数的使用
	//srcImage.copyto(dstImage, mask)，mask作为一个掩模板，如果在某个像素点(i, j)其值为1（只看第一通道，所以mask单通道即可）
	//则把srcImage.at(i, j)处的值直接赋给dstImage.at(i, j)，如果其值为0则dstImage.at(i, j)处保留其原始像素值。
	//提取_maskImage(i,j)像素点为1的原图片中(i,j)像素点，拷贝到_dstimage的roi_rect区域的(i,j)像素点
	_srcImage.copyTo(_dstimage(roi_rect), _maskImage);
}

Rect clsVideoGenerator::rectScale(Rect _rect, double _size_r)
{
	_rect.x = _rect.x + 0.5*_rect.width - 0.5*_rect.width*_size_r;
	_rect.y = _rect.y + 0.5*_rect.height - 0.5*_rect.height*_size_r;

	_rect.width *= _size_r;
	_rect.height *= _size_r;
	return (_rect);
}

void clsVideoGenerator::MergeImagesToVideo(Mat *_pFrames, int _fps, bool _iscolor, string _videoname,int _synopsisLen)
{
	int i, _count = 0, _nodataCount = 0;
	Mat bgImage = imread(backgroundFileName, 3);
	//('D', 'I', 'V', 'X')
	VideoWriter writer(_videoname, VideoWriter::fourcc('M', 'J', 'P', 'G'), _fps, Size(bgImage.cols, bgImage.rows), _iscolor);
	for (i = 0; i < _synopsisLen; i++)
	{
		if (_pFrames[i].cols == 0)
		{
			_nodataCount++;
			bgImage.copyTo(_pFrames[i]);
		}

		else
		{
			_count++;
		}

		writer.write(_pFrames[i]);
	}

	cout << "count:" << _count << " no data:" << _nodataCount << endl;

	writer.write(bgImage);

}
