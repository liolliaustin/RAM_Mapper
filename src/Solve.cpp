#include "Solve.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


void Solve::mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount){
	int circuit = circuitDefs[0][0], logic = logicBlockCount[0], RAM_ID, Mode, Depth, Width;
	for(int i=0; i<circuitDefs.size(); i++){
		if(circuit != circuitDefs[i][0]){
			circuit = circuitDefs[i][0];
			logic = logicBlockCount[circuit];
		}

		RAM_ID = circuitDefs[i][1];
		Mode = circuitDefs[i][2];
		Depth = circuitDefs[i][3];
		Width = circuitDefs[i][4];

		


	}
}