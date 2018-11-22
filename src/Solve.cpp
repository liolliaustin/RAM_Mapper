#include "Solve.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "math.h"

using namespace std;


void Solve::mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, bool LUTRAM_support, bool MTJ){
	float logic_block_area, RAM_area;
	int bits = 0, max_width = 0;
	
	int circuit = circuitDefs[0][0], logic = logicBlockCount[0], RAM_ID, Mode, Depth, Width;
	for(int i=0; i<circuitDefs.size(); i++){
		
		bool fullSupport = true, LUTRAM_configuration = false, useLUTRAMs = LUTRAM_support;
		int LUTRAM_depth, LUTRAM_width;

		if(LUTRAM_support == false)
			logic_block_area = 35000;
		else
			logic_block_area = 40000;

		if(MTJ == true)
			RAM_area = 9000 + 1.25*bits + 90*sqrt(bits) + 600*2*max_width;
		else
			RAM_area = 9000 + 5*bits + 90*sqrt(bits) + 600*2*max_width;


		if(circuit != circuitDefs[i][0]){
			circuit = circuitDefs[i][0];
			logic = logicBlockCount[circuit];
		}

		if(LUTRAM_support == true){
			if(LUTRAM_configuration == true){
				LUTRAM_depth = 64;
				LUTRAM_width = 10;
			}
			else{
				LUTRAM_depth = 32;
				LUTRAM_width = 20;
			}
		}
		int additionalLUTs = 0, maxPhysicalDepth, dec, muxSelectAmount;

		RAM_ID = circuitDefs[i][1];
		Mode = circuitDefs[i][2];
		Depth = circuitDefs[i][3];
		Width = circuitDefs[i][4];

		if(Mode == 4){
			fullSupport = false;
			useLUTRAMs = false;
		}

		/*some depth min set by chosen ram*/
		if(Depth > maxPhysicalDepth){
			dec = ceil(Depth/maxPhysicalDepth);
			muxSelectAmount = ceil(log2(dec));
			if(dec == 2)
				dec = 1;

			additionalLUTs += dec;
		}



	}
}


vector<int> Solve::RAM8kconfiguration(int mode, int depth, int width){
	vector<int> RAMdata;
	bool increaseWidth = false, increaseDepth = false;
	int amount;

	int widthExponent = ceil(log2(width));
	int depthExponent = ceil(log2(depth));

	if(mode < 4 && widthExponent > 5 | mode == 4 && widthExponent > 4)
		increaseWidth = true;

	if(depthExponent > 13)
		increaseDepth = true;
	else{
		int differenceWidth = 13 - depthExponent;
		if(differenceWidth < widthExponent){
			increaseWidth = true;
			amount = pow(2,(widthExponent-differenceWidth));
		}
	}
	

	

	

	if(difference > 0){
		int normalDepthExponent = pow(2,13-widthExponent);
		if(normalDepthExponent < depthExponent)
			increaseDepth = true;
	}

	if(13 - difference - widthExponent == depthExponent)
		RAMdata.push_back(depth);

	
	


}