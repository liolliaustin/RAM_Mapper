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

	void mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount);

	vector<int> RAM8kconfiguration(int mode, int depthExponent, int widthExponent, int Width);
	vector<int> RAM128kconfiguration(int mode, int depthExponent, int widthExponent, int Width);
	vector<int> LUTRAMconfiguration(int mode, int depthExponent, int widthExponent, int Width);

	vector<int> returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent, int Width);

	float getArea(int newlogic, int RAMused, int maxWidth, int bits);

	void areaModel(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, int BlockRAMsizeExponent, bool LUTRAM_support);
	vector<int> returnAmount(int newLogic, int Mode, int BlockRAMsizeExponent, int maxWidthExponent, int widthExponent, int depthExponent, int RAM8used, int LBpRB);

};




#endif