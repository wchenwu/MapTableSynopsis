
#include "const.h"
#include "clsTubeManager.h"
#include "clsOptimizer.h"
#include "clsVideoGenerator.h"


using namespace cv;
using namespace std;

int main(){

	//用于记录计算运行时间的起止时间
	int64 _timeStart, _timeEnd;
	//用于保存预处理文件目录
	string _preFilePath = constPreFilePath;

	_timeStart = GetTickCount();


	//生成Manager实例对象，并进行初始化
	clsTubeManager *pTubeManager = NULL;
	pTubeManager = new clsTubeManager(_preFilePath);
	if (pTubeManager != NULL)
		pTubeManager->initTubeManager();

	//生成优化类实例对象，并调用MCMC优化
	clsOptimizer  *pOptimizer = NULL;
	pOptimizer = new clsOptimizer(pTubeManager,constSizeLow);
	if (pOptimizer != NULL)
		pOptimizer->OnMapTableOptimize();

	//计算浓缩视频长度
	pTubeManager->computeSynopsisVideoLen();
	//打印优化后的信息
	pTubeManager->printTubeListInfo();

	//生成产生视频类实例对象，并开始生成视频
	clsVideoGenerator *pVideoGenerator = NULL;
	pVideoGenerator = new clsVideoGenerator(pTubeManager, _preFilePath);
	if (pVideoGenerator != NULL)
		pVideoGenerator->OnGenerateVideo();

	//结束时间
	_timeEnd = GetTickCount();

	cout << "RunTime is: " << (_timeEnd - _timeStart) / 1000 << endl;
}
