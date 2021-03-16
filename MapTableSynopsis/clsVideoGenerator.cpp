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
	string _tubeiFilePath;	//tubeid�ļ�Ŀ¼

	for (i = 0; i < pTubeManager->getTubeNumber(); i++)
	{
		_tubeiFilePath = pTubeManager->getPreFilePath() + "\\" + to_string(i + 1) + "\\";
		TubeFrameStitching(i, _tubeiFilePath);
		//cout << "tubeID: " << i + 1 << " stitching finished!" << endl;
	}
	cout << "Video Stitching finished!" << endl;

	string _videoname = pTubeManager->getTimeString() + ".avi";
	//��ͼ�����кϲ�����Ƶ
	MergeImagesToVideo(pImageSequence, 25, true, _videoname,pTubeManager->getSynVideoLength());

}

//��ʵ��
void clsVideoGenerator::TubeFrameStitching(int _tubeid, string _tubefilepath)
{
	int i, j;

	Rect _bbox;
	int _centerx, _centery;
	double _rsize;
	string _srcfile;
	string _maskfile;
	//ָ��tubeid��tube
	clsTube *pTube = pTubeManager->getTube(_tubeid);
	if (pTube == NULL)
		return;

	//��Ũ����Ƶ�е�֡��
	int _dstframeIndex = pTube->getDstFrameStart();
	int _tubeLen	   = pTube->getSrcTubeLength();
	//����
	Mat  bgImage = imread(backgroundFileName, 3);

	int _offsetframe;
	for (i = 0; i < _tubeLen; i++)
	{
		//֡ƫ����
		_offsetframe = i;

		//�����ǰ֡����û�����ݣ�������ͼ�񿽱�����ǰ֡ͼ��
		if (pImageSequence[_dstframeIndex].cols == 0)
			bgImage.copyTo(pImageSequence[_dstframeIndex]);

		if (!pTube->getSrcBboxRect(_offsetframe, _bbox))
			return;

		_centerx = _bbox.x + 0.5*_bbox.width;
		_centery = _bbox.y + 0.5*_bbox.height;
		_rsize = pTube->getSizeRatio(_offsetframe);
		_srcfile = srcFramesFilePath + to_string(pTube->getSrcFrameStart() + _offsetframe) + ".png";
		_maskfile = _tubefilepath + to_string(pTube->getSrcFrameStart() + _offsetframe) + ".png";
		//ͼ���ں�
		ImageStitching(pImageSequence[_dstframeIndex], _rsize, _centerx, _centery, _srcfile, _maskfile);

		//�����ο�
		rectangle(pImageSequence[_dstframeIndex], rectScale(_bbox, _rsize), Scalar(0, 0, 255));

		_dstframeIndex++;

	}

}


void clsVideoGenerator::ImageStitching(Mat& _dstimage, double _size_r, int _centerx, int _centery, string _srcfilename, string _maskfilename)
{
	//��ȡԭͼƬ��maskͼƬ
	Mat _srcImage = imread(_srcfilename, 3);
	Mat _maskImage = imread(_maskfilename, 0);


	//���￴һ�������壺
	//ǰ����������ԭͼ�����ͼ��ûʲô��˵��
	//���������������ѡȡʹ�õģ���Ҫ���ǹ涨���ŵı�����
	//dsize�൱��ֱ�ӹ涨�����ź�Ҫ�õ��Ĵ�С�����Ҫ�������ˣ������fx fy��Ĭ��ֵ���ǲ���Ҫ���õ�
	//fx fy�Ǹ�dsize���е�������Ϣ��������dsizeΪ0��ʱ��������ֵ�������ã���x y��������ű���
	//���һ��Ϊ��ֵ��ʽ������Ĭ�ϵ������Բ�ֵ����������ֵ��ʽ�������ص�
	/*CV_EXPORTS_W void resize( InputArray src, OutputArray dst,
	Size dsize, double fx = 0, double fy = 0,
	int interpolation = INTER_LINEAR );
	*/

	//������Ҫ����ͼƬ��С
	resize(_srcImage, _srcImage, Size(), _size_r, _size_r);
	resize(_maskImage, _maskImage, Size(), _size_r, _size_r);

	//������Ŀ��ͼƬ��ROI����ʵ���Ͼ���ԭͼƬ���ź������ͼƬ����
	//������������Ŀ��ͼƬ��ʲôλ�ã�����֤��
	Rect roi_rect = cv::Rect(_centerx*(1 - _size_r), _centery*(1 - _size_r), _srcImage.cols, _srcImage.rows);
	//ע��Mat::copyTo()������ʹ��
	//srcImage.copyto(dstImage, mask)��mask��Ϊһ����ģ�壬�����ĳ�����ص�(i, j)��ֵΪ1��ֻ����һͨ��������mask��ͨ�����ɣ�
	//���srcImage.at(i, j)����ֱֵ�Ӹ���dstImage.at(i, j)�������ֵΪ0��dstImage.at(i, j)��������ԭʼ����ֵ��
	//��ȡ_maskImage(i,j)���ص�Ϊ1��ԭͼƬ��(i,j)���ص㣬������_dstimage��roi_rect�����(i,j)���ص�
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
