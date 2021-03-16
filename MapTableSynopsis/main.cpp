
#include "const.h"
#include "clsTubeManager.h"
#include "clsOptimizer.h"
#include "clsVideoGenerator.h"


using namespace cv;
using namespace std;

int main(){

	//���ڼ�¼��������ʱ�����ֹʱ��
	int64 _timeStart, _timeEnd;
	//���ڱ���Ԥ�����ļ�Ŀ¼
	string _preFilePath = constPreFilePath;

	_timeStart = GetTickCount();


	//����Managerʵ�����󣬲����г�ʼ��
	clsTubeManager *pTubeManager = NULL;
	pTubeManager = new clsTubeManager(_preFilePath);
	if (pTubeManager != NULL)
		pTubeManager->initTubeManager();

	//�����Ż���ʵ�����󣬲�����MCMC�Ż�
	clsOptimizer  *pOptimizer = NULL;
	pOptimizer = new clsOptimizer(pTubeManager,constSizeLow);
	if (pOptimizer != NULL)
		pOptimizer->OnMapTableOptimize();

	//����Ũ����Ƶ����
	pTubeManager->computeSynopsisVideoLen();
	//��ӡ�Ż������Ϣ
	pTubeManager->printTubeListInfo();

	//���ɲ�����Ƶ��ʵ�����󣬲���ʼ������Ƶ
	clsVideoGenerator *pVideoGenerator = NULL;
	pVideoGenerator = new clsVideoGenerator(pTubeManager, _preFilePath);
	if (pVideoGenerator != NULL)
		pVideoGenerator->OnGenerateVideo();

	//����ʱ��
	_timeEnd = GetTickCount();

	cout << "RunTime is: " << (_timeEnd - _timeStart) / 1000 << endl;
}
