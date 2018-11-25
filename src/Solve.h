#ifndef __SOLVE_INCLUDED__
#define __SOLVE_INCLUDED__

#include <iostream>
#include <algorithm>
#include <vector>
#include "RAM.h"
using namespace std;

class Solve{
public:
	int geometricAverage();

	void areaModel();

	void mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, bool LUTRAM_support, bool MTJ);

	vector<int> RAM8kconfiguration(int mode, int depth, int width);
	vector<int> RAM128kconfiguration(int mode, int depth, int width);
	vector<int> LUTRAMconfiguration(int mode, int depth, int width);

	vector<int> returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent);

};




#endif