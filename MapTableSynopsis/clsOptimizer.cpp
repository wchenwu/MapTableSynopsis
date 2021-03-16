#include "clsOptimizer.h"



clsOptimizer::clsOptimizer(clsTubeManager *_pManager, double _sizelow)
{
	pTubeManager = _pManager;
	sizeLow = _sizelow;

	int _width, _height;
	pTubeManager->getImageWidthAndHeight(_width, _height);
	pMapTable = new clsMapTable();
	if (pMapTable != NULL)
		pMapTable->OnInitTable(_width,_height);

}


clsOptimizer::~clsOptimizer()
{
	if (pMapTable != NULL)
		delete pMapTable;
}

void clsOptimizer::OnMapTableOptimize()
{
	int i,j;
	Rect _rect;
	int  _synFrameID;
	enum TubeStatus  _status;
	int    _newTubePos;
	double _sizeratio;

	//i-- src video frame number
	
	for (i = 1; i <= pTubeManager->getSrcVideoLength(); i++)
	{
		for (j = 0; j < pTubeManager->getTubeNumber(); j++)
		{
			pTubeManager->getInstanceInfo(j,i,_status,_rect,_synFrameID);	
			
			switch(_status)
			{
			case FIRST_FRAME:

				_newTubePos = pMapTable->OnSearchPosForNewTube(_rect);	
				pTubeManager->setDstFrameStart(j, _newTubePos);
				cout << "tubeid: " << j+1 << " i: "<<i<<" pos: " << _newTubePos << endl;
				break;
			case NEXT_FRAME:
				_sizeratio = pMapTable->OnComputeSizeRatio(_synFrameID, _rect);
				pTubeManager->setDstFrameSizeRatio(j, _synFrameID, _sizeratio);
				break;
			case NOT_EXIST:
				break;
			}		
		}
	}
}
