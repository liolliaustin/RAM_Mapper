#include "Solve.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "math.h"

using namespace std;


void Solve::mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, bool LUTRAM_support, bool MTJ){
	float logic_block_area, RAM_area;
	int bits = 0, max_width = 0;
	int circuit = circuitDefs[0][0], logic = logicBlockCount[0], RAM_ID, Mode, Depth, Width, newlogic = 0, RAM8count=0, RAM8used=0, RAM128count=0, RAM128used=0, LUTRAMcount = 0, LUTRAMused = 0;
	int chose128 = 0, chose8 = 0, choseLUT = 0, count=0;
	
	for(int i=0; i<circuitDefs.size(); i++){

		if(circuit != circuitDefs[i][0]){
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
			newlogic = 0; 
			RAM8count=0; 
			RAM8used=0; 
			RAM128count=0;
			RAM128used=0; 
			LUTRAMcount = 0; 
			LUTRAMused = 0;
			circuit = circuitDefs[i][0];
			logic = logicBlockCount[circuit];
			
		}
		count++;
		RAM_ID = circuitDefs[i][1];
		Mode = circuitDefs[i][2];
		Depth = circuitDefs[i][3];
		Width = circuitDefs[i][4];

		int widthExponent = ceil(log2(Width));
		int depthExponent = ceil(log2(Depth));
		int multiplier = widthExponent + depthExponent;
		vector<int> resultVector;

		resultVector = returnLowest(newlogic, RAM8used, RAM128used, LUTRAMused, Mode, depthExponent, widthExponent);
		// cout << i << ": " ;
		// for(int k=0; k<resultVector.size(); k++)
		// 	cout << resultVector[k] << " ";
		// cout << endl;

		if(resultVector[0] == 1){
			RAM8used = resultVector[1];
			newlogic = resultVector[2];
			chose8++;
		}
		else if(resultVector[0] == 2){
			RAM128used = resultVector[1];
			newlogic = resultVector[2];
			chose128++;
		}
		else if(resultVector[0] == 0){
			LUTRAMused = resultVector[1];
			newlogic = resultVector[2];
			choseLUT++;
		}
		resultVector.clear();
		// cout << newlogic << endl;
		// if(multiplier >= 17){
		// 	RAM128used++;
		// 	RAM128count = newlogic/300;
		// 	if(RAM128used > RAM128count){
		// 		newlogic = RAM128used*300;
		// 		RAM128count = newlogic/300;
		// 	}
		// 	cout << depthExponent << " " << widthExponent << " " << Mode << endl;
		// 	chose128++;
		// 	// resultVector = RAM128kconfiguration(mode, depthExponent, widthExponent);
		// }
		// else if(depthExponent >= 8){
		// 	RAM8used++;
		// 	RAM8count = newlogic/10;
		// 	if(RAM8used > RAM8count){
		// 		newlogic += (RAM8used - RAM8count)*10;
		// 		RAM8count = newlogic/10;
		// 	}
		// 	chose8++;
		// 	cout << depthExponent << " " << widthExponent << " " << Mode << endl;
		// 	//resultVector = RAM8kconfiguration(Mode, depthExponent, widthExponent);
		// 	for(int j=0; j<resultVector.size(); j++)
		// 		cout << resultVector[j] <<" ";
		// 	//cout << endl;
		// }
		// else{
		// 	LUTRAMused++;
		// 	LUTRAMcount = newlogic/2;
		// 	if(LUTRAMused > LUTRAMcount){
		// 		newlogic += (LUTRAMused - LUTRAMcount)*2;
		// 		LUTRAMcount = newlogic/2;
		// 	}
		// 	choseLUT++;
		// 	// resultVector =fdb LUTRAMconfiguration(mode, depthExponent, widthExponent);
		// 	// LUTRAMcount++;
		// }



	}
	cout << "128k: " << chose128 << endl << "8k: " << chose8 << endl << "LUTRAM: " << choseLUT << endl << "Total: " << count << endl;
}

vector<int> Solve::returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent){
	vector<int> hit128, hit8, hitLUT, final;
	int minimum;
	bool checkLUT = true, RAM128free = false, RAM8free=false, LUTRAMfree = false;

	hit8 = RAM8kconfiguration(mode, depthExponent, widthExponent);
	hit128 = RAM128kconfiguration(mode, depthExponent, widthExponent);
	if(mode != 4){
		hitLUT = LUTRAMconfiguration(mode, depthExponent, widthExponent);
		minimum = min(min(hit8[1], hit128[1]), hitLUT[1]);
	}
	else
		minimum = min(hit8[1], hit128[1]);

	
	if(RAM8used + hit8[0] <= currentLogic/10 && minimum == hit8[1]){
		final.push_back(1);
		final.push_back(RAM8used + hit8[0]);
		final.push_back(currentLogic + ceil(minimum/10));
	}

	else if(RAM128used + hit128[0] <= currentLogic/300 && minimum == hit128[1]){
		final.push_back(2);
		final.push_back(RAM128used + hit128[0]);
		final.push_back(currentLogic + ceil(minimum/10));
	}
	else if(LUTRAMused + hitLUT[0] <= currentLogic/2 && mode != 4 && minimum == hitLUT[1]){
		final.push_back(0);
		final.push_back(LUTRAMused + hitLUT[0]);
		final.push_back(currentLogic + ceil(minimum/10));
	}
	else if(minimum == hit8[1]){
		final.push_back(1);
		final.push_back(RAM8used + hit8[0]);
		final.push_back(currentLogic + 10 + ceil(minimum/10));
	}
	else if(minimum == hit128[1]){
		final.push_back(2);
		final.push_back(RAM128used + hit128[0]);
		final.push_back(currentLogic + 300 + ceil(minimum/10));
	}
	else if (mode != 4 && minimum == hitLUT[1]){
		final.push_back(0);
		final.push_back(LUTRAMused + hitLUT[0]);
		final.push_back(currentLogic + 2 + ceil(minimum/10));
	}
	

	hit128.clear(); hit8.clear(); hitLUT.clear();

	return final;

}

//worry about tdp
vector<int> Solve::RAM8kconfiguration(int mode, int depthExponent, int widthExponent){
	vector<int> solveData;
	int hit = 0, widthSize=0, extraLogic=0;
	if(depthExponent + widthExponent <= 13 && widthExponent <= 5 && mode < 4 || depthExponent + widthExponent <= 13 && widthExponent <= 4 && mode == 4){
		hit = 1;
	}
	else if(depthExponent <= 13 && depthExponent >= 8 && mode < 4 || depthExponent <= 13 && depthExponent >= 9 && mode == 4){
		hit = widthExponent - (13-depthExponent);
	}
	else if(depthExponent < 8){
		if(mode < 4)
			hit = widthExponent - 5;
		else
			hit = widthExponent - 4;
	}
	else if(depthExponent > 13){
		if(widthExponent <= 5 && mode < 4 || widthExponent<=4 && mode == 4)
			hit = depthExponent - (13 - widthExponent);
			
		else if(widthExponent > 5 && mode < 4)
			hit = (depthExponent - 8)*(widthExponent - 5);
		
		else if(widthExponent > 4 && mode == 4)
			hit = (depthExponent - 9)*(widthExponent - 4);
		else
			hit = 99;

		if(hit == 2)
			extraLogic += 1;
		else
			extraLogic += hit;

		//multiplexers
		if(hit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(hit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(hit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic += 99*pow(2,widthExponent);

	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;
}

vector<int> Solve::RAM128kconfiguration(int mode, int depthExponent, int widthExponent){
	vector<int> solveData;
	int hit = 0, widthSize=0, extraLogic=0;
	if(depthExponent + widthExponent <= 17 && widthExponent <= 7 && mode < 4 || depthExponent + widthExponent <= 17 && widthExponent <= 6 && mode == 4){
		hit = 1;
	}
	else if(depthExponent <= 17 && depthExponent >= 10 && mode < 4 || depthExponent <= 17 && depthExponent >= 11 && mode == 4){
		hit = widthExponent - (17-depthExponent);
	}
	else if(depthExponent < 10){
		if(mode < 4)
			hit = widthExponent - 7;
		else
			hit = widthExponent - 6;
	}

	else if(depthExponent > 17){
		if(widthExponent <= 7 && mode < 4 || widthExponent<=6 && mode == 4)
			hit = depthExponent - (17 - widthExponent);

		else if(widthExponent > 7 && mode < 4)
			hit = (depthExponent - 10)*(widthExponent - 7);
		
		else if(widthExponent > 6 && mode == 4)
			hit = (depthExponent - 11)*(widthExponent - 6);
		else
			hit = 99;
		
		//decoders
		if(hit == 2)
			extraLogic += 1;
		else
			extraLogic += hit;

		//multiplexers
		if(hit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(hit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(hit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic = 99*pow(2,widthExponent);

	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;
}

vector<int> Solve::LUTRAMconfiguration(int mode, int depthExponent, int widthExponent){
	vector<int> solveData;
	int hit=0, extraLogic =0;
	if(depthExponent <= 5)
		hit = ceil(pow(2,widthExponent)/20);
	else if(depthExponent == 6)
		hit = ceil(pow(2,widthExponent)/10);
	else{
		hit = (depthExponent - 6)*ceil(pow(2,widthExponent)/10);

		if(hit == 2)
			extraLogic += 1;
		else
			extraLogic += hit;

		//multiplexers
		if(hit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(hit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(hit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic = 99*pow(2,widthExponent); 
	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;
}

	// vector<int> RAMdata;
	// bool increaseWidth = false, increaseDepth = false;
	// int amount = 1, amountDepth = 1, amountWidth = 1, widthDifference=0;


	// if(depthExponent > 13){
	// 	amountDepth = depthExponent - 13;
	// 	RAMdata.push_back(amountDepth);
	// 	int differenceWidth = depthExponent-13;
	// 	amountWidth = pow(2,(widthExponent-differenceWidth));
	// 	RAMdata.push_back(amountWidth);
	// }
	// else if(widthExponent + depthExponent > 13 && depthExponent > 8 && mode < 4){
	// 	int differenceWidth = 13 - depthExponent;
	// 	amountWidth = pow(2,(widthExponent-differenceWidth));
	// 	RAMdata.push_back(amountDepth);
	// 	RAMdata.push_back(amountWidth);

	// }
	// else if(widthExponent + depthExponent > 13 && depthExponent > 9 && mode == 4){
	// 	int differenceWidth = 13 - depthExponent;
	// 	amountWidth = pow(2,(widthExponent-differenceWidth));
	// 	RAMdata.push_back(amountDepth);
	// 	RAMdata.push_back(amountWidth);
	// }

	// return RAMdata;


// if(Mode == 4){
// 			fullSupport = false;
// 			useLUTRAMs = false;
// 		}

// 		/*some depth min set by chosen ram*/
// 		if(Depth > maxPhysicalDepth){
// 			dec = ceil(Depth/maxPhysicalDepth);
// 			muxSelectAmount = ceil(log2(dec));
// 			if(dec == 2)
// 				dec = 1;

// 			additionalLUTs += dec;
// 		}

// int additionalLUTs = 0, maxPhysicalDepth, dec, muxSelectAmount;

// bool fullSupport = true, LUTRAM_configuration = false, useLUTRAMs = LUTRAM_support;
// 		int LUTRAM_depth, LUTRAM_width;

// 		if(LUTRAM_support == true){
// 			if(LUTRAM_configuration == true){
// 				LUTRAM_depth = 64;
// 				LUTRAM_width = 10;
// 			}
// 			else{
// 				LUTRAM_depth = 32;
// 				LUTRAM_width = 20;
// 			}
// 		}

// if(LUTRAM_support == false)
// 			logic_block_area = 35000;
// 		else
// 			logic_block_area = 40000;

// 		if(MTJ == true)
// 			RAM_area = 9000 + 1.25*bits + 90*sqrt(bits) + 600*2*max_width;
// 		else
// 			RAM_area = 9000 + 5*bits + 90*sqrt(bits) + 600*2*max_width;



// if(mode < 4 && widthExponent > 5 | mode == 4 && widthExponent > 4)
// 		increaseWidth = true;


	

	

	

// 	if(difference > 0){
// 		int normalDepthExponent = pow(2,13-widthExponent);
// 		if(normalDepthExponent < depthExponent)
// 			increaseDepth = true;
// 	}

// 	if(13 - difference - widthExponent == depthExponent)
// 		RAMdata.push_back(depth);