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

	int RAM8kconfiguration(int mode, int depth, int width);
	int RAM128kconfiguration(int mode, int depth, int width);

	int returnLowest(int currentLogic, int RAM8count, int RAM128count, int LUTRAMcount, int mode, int depthExponent, int widthExponent);

};




#endif