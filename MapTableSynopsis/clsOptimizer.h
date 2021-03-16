#pragma once
#include "const.h"
#include "clsTubeManager.h"
#include "clsMapTable.h"


class clsOptimizer
{
public:
	clsOptimizer(clsTubeManager *_pManager, double _sizelow);
	virtual ~clsOptimizer();

	void OnMapTableOptimize();

private:

	clsTubeManager *pTubeManager;		
	double  sizeLow;

	clsMapTable  *pMapTable;

};

