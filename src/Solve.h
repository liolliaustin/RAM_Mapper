#ifndef __SOLVE_INCLUDED__
#define __SOLVE_INCLUDED__

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class Solve{
private:
	vector<vector<int> > finalResult;
public:
	int geometricAverage();

	void areaModel();

	vector<int> getFinalResult(int i){return finalResult[i];}

	void mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, bool LUTRAM_support, bool MTJ);

	vector<int> RAM8kconfiguration(int mode, int depthExponent, int widthExponent, int Width);
	vector<int> RAM128kconfiguration(int mode, int depthExponent, int widthExponent, int Width);
	vector<int> LUTRAMconfiguration(int mode, int depthExponent, int widthExponent, int Width);

	vector<int> returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent, int Width);

	float getArea(int circuit, int logic, int newlogic, int RAM128used, int RAM8used, int LUTRAMused);

};




#endif