#include "Solve.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "math.h"

using namespace std;


void Solve::mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, bool LUTRAM_support, bool MTJ){
	vector<vector<int> > finalResult;
	vector<int> RAM_map;
	float logic_block_area, RAM_area, totalArea, areaSum;
	vector<float> areaVector;
	int bits = 0, max_width = 0, additionalLuts;
	int circuit = circuitDefs[0][0], logic = logicBlockCount[0], RAM_ID, Mode, Depth, Width, newlogic = 0, RAM8count=0, RAM8used=0, RAM128count=0, RAM128used=0, LUTRAMcount = 0, LUTRAMused = 0;
	int chose128 = 0, chose8 = 0, choseLUT = 0, count=0;
	
	for(int i=0; i<circuitDefs.size(); i++){
		if(circuit != circuitDefs[i][0]){
			// totalArea = getArea(circuit, logic, newlogic - logic, RAM128used, RAM8used, LUTRAMused);
			// areaVector.push_back(totalArea/100000000);
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused <<  endl;
			cout << "Total Area: " << totalArea << endl << endl;
			newlogic = logicBlockCount[circuit]; 
			RAM8count=0; 
			RAM8used=0; 
			RAM128count=0;
			RAM128used=0; 
			LUTRAMcount = 0; 
			LUTRAMused = 0;
			// count = 0;
			circuit = circuitDefs[i][0];
			logic = logicBlockCount[circuit];
			//break;
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

		resultVector = returnLowest(newlogic, RAM8used, RAM128used, LUTRAMused, Mode, depthExponent, widthExponent, Width);
		// cout << "depthExponent: " << depthExponent << endl;
		// cout << "widthExponent: " << widthExponent << endl;
		// cout << "Mode: " << Mode <<endl;
		// cout << "128k: " << chose128 << endl << "8k: " << chose8 << endl << "LUTRAM: " << choseLUT << endl;
		// if(circuit == 68){
		// 	for(int j=0; j<resultVector.size(); j++){
		// 		cout << resultVector[j] << " ";
		// 	}
		// 	cout << endl;
		// }
		
		// if(Depth >= pow(2,13))
		// 	cout << Depth << " " << Width <<endl;

		if(resultVector[0] == 2){
			RAM8used = resultVector[1];
			newlogic = resultVector[2];
			additionalLuts = resultVector[3];
			chose8++;
		}
		else if(resultVector[0] == 3){
			// cout << Depth << " " << Width <<endl;
			RAM128used = resultVector[1];
			newlogic = resultVector[2];
			additionalLuts = resultVector[3];
			chose128++;
		}
		else if(resultVector[0] == 1){
			LUTRAMused = resultVector[1];
			newlogic = resultVector[2];
			additionalLuts = resultVector[3];
			choseLUT++;
		}

		RAM_map.push_back(circuit); 
		RAM_map.push_back(RAM_ID); 
		RAM_map.push_back(additionalLuts); 
		RAM_map.push_back(Width);
		RAM_map.push_back(Depth);
		RAM_map.push_back(count);
		RAM_map.push_back(resultVector[4]);
		RAM_map.push_back(resultVector[5]);
		RAM_map.push_back(resultVector[0]);
		RAM_map.push_back(Mode);
		RAM_map.push_back(resultVector[6]);
		RAM_map.push_back(resultVector[7]);

		finalResult.push_back(RAM_map);

		RAM_map.clear();


		//cout << "Circuit " << circuit << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
		resultVector.clear();

		if(i == circuitDefs.size()-1){
			// totalArea = getArea(circuit, logic, newlogic - logic, RAM128used, RAM8used, LUTRAMused);
			// areaVector.push_back(totalArea/100000000);
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
			// cout << "Total Area: " << totalArea << endl << endl;

			// for(int j=0; j<areaVector.size(); j++){
			// 	areaSum += areaVector[j];
			// }
			// areaSum /= areaVector.size();
			// areaSum *= 100000000;
			// cout << "Final Area: " << areaSum << endl;
		}


		
	}
	this -> finalResult = finalResult;
	cout << "128k: " << chose128 << endl << "8k: " << chose8 << endl << "LUTRAM: " << choseLUT << endl << "Total: " << count << endl;
}

vector<int> Solve::returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent, int Width){
	vector<int> hit128, hit8, hitLUT, final;
	int minimum, LUTcost = 999999999, RAM8cost, RAM128cost, RAM8add = 0, RAM128add = 0, LUTRAMadd = 0;
	float RAM8available = floor(float(currentLogic)/10.0) - RAM8used;
	float RAM128available = floor(float(currentLogic)/300.0) - RAM128used;
	float LUTRAMavailable = float(currentLogic)/2.0 - LUTRAMused;

	hit8 = RAM8kconfiguration(mode, depthExponent, widthExponent, Width);
	hit128 = RAM128kconfiguration(mode, depthExponent, widthExponent, Width);
	if(mode != 4){
		hitLUT = LUTRAMconfiguration(mode, depthExponent, widthExponent, Width);

		if(LUTRAMavailable >= hitLUT[0]){
			LUTcost = hitLUT[1];
		}
		else{
			LUTRAMadd = 2*(hitLUT[0] - LUTRAMavailable);
			LUTcost = LUTRAMadd + hitLUT[1];
		}
	}
	if(RAM8available >= hit8[0]){
		RAM8cost = hit8[1];
	}
	else{
		RAM8add = 10*(hit8[0] - RAM8available);
		RAM8cost = RAM8add + hit8[1];
	}
	if(RAM128available >= hit128[0]){
		RAM128cost = hit128[1];
	}
	else{
		RAM128add = 300*(hit128[0] - RAM128available);
		RAM128cost = RAM128add + hit128[1];
	}

	minimum = min(min(RAM8cost, RAM128cost), LUTcost);
	//cout << "Available128: " << RAM128available << endl << RAM128cost << endl;
	

	if(minimum == RAM8cost){
		//cout << "currentLogic: " << currentLogic + ceil(hit8[1]/10) + RAM8add << endl << "RAM8cost: " << RAM8cost << endl << "RAM8used: " << RAM8used + hit8[0] << endl << "hit8: " << hit8[0] << endl << endl;
		final.push_back(2);
		final.push_back(RAM8used + hit8[0]);
		final.push_back(currentLogic + ceil(float(hit8[1])/10.0) + RAM8add);
		final.push_back(hit8[1]);
		final.push_back(hit8[2]);
		final.push_back(hit8[3]);
		final.push_back(hit8[4]);
		final.push_back(hit8[5]);
	}
	else if (minimum == RAM128cost){
		//cout << "currentLogic: " << currentLogic + ceil(hit128[1]/10) + RAM128add << endl << "RAM128cost: " << RAM128cost << endl << "RAM128used: " << RAM128used + hit128[0] << endl << "hit128: " << hit128[0] << endl<< endl;
		final.push_back(3);
		final.push_back(RAM128used + hit128[0]);
		final.push_back(currentLogic + ceil(float(hit128[1])/10.0) + RAM128add);
		final.push_back(hit128[1]);
		final.push_back(hit128[2]);
		final.push_back(hit128[3]);
		final.push_back(hit128[4]);
		final.push_back(hit128[5]);
	}

	else if(mode != 4 && minimum == LUTcost){
		//cout << "currentLogic: " << currentLogic + ceil(hitLUT[1]/10) + LUTRAMadd << endl << "LUTcost: " << LUTcost << endl << "LUTRAMused: " << LUTRAMused + hitLUT[0] << endl << "hitLUT: " << hitLUT[0] << endl << endl;
		final.push_back(1);
		final.push_back(LUTRAMused + hitLUT[0]);
		final.push_back(currentLogic + ceil(float(hitLUT[1])/10.0) + LUTRAMadd);
		final.push_back(hitLUT[1]);
		final.push_back(hitLUT[2]);
		final.push_back(hitLUT[3]);
		final.push_back(hitLUT[4]);
		final.push_back(hitLUT[5]);
	}
	
	
	hit128.clear(); hit8.clear(); hitLUT.clear();

	return final;

}

//worry about tdp
vector<int> Solve::RAM8kconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	vector<int> solveData;
	int depthHit=1, widthHit=1, hit = 0, widthSize=0, extraLogic=0, RAM8width, RAM8depth;
	if(depthExponent + widthExponent <= 13 && widthExponent <= 5 && mode < 4 || depthExponent + widthExponent <= 13 && widthExponent <= 4 && mode == 4){
		hit = 1;
		if(widthExponent == 5 && mode < 4 || widthExponent == 4 && mode ==4 || depthExponent < 8 && mode < 4 || depthExponent < 9 && mode == 4){
			RAM8width = pow(2.0,float(widthExponent));
			RAM8depth = pow(2.0,float(13-widthExponent));
		}
		else if(widthExponent < 5 && depthExponent >= 8 && mode < 4 || widthExponent < 4 && depthExponent >= 9 && mode == 4){
			RAM8width = pow(2.0,float(13-depthExponent));
			RAM8depth = pow(2.0,float(depthExponent));
		}
		else{
			if(mode < 4){
				RAM8width = 32;
				RAM8depth = 256;
			}
			else{
				RAM8width = 16;
				RAM8depth = 512;
			}
			
		}
		
	}
	else if(depthExponent <= 13 && depthExponent >= 8 && mode < 4 || depthExponent <= 13 && depthExponent >= 9 && mode == 4){
		hit = ceil(float(Width)/float(pow(2,(13-depthExponent))));
		widthHit = hit;
		RAM8width = pow(2.0,float(13-depthExponent));
		RAM8depth = pow(2,float(depthExponent));
	}
	else if(depthExponent < 8 && mode < 4 || depthExponent < 9 && mode == 4){
		if(mode < 4){
			hit = ceil(float(Width)/32.0);
			RAM8depth = 256;
			RAM8width = 32;
		}
		else{
			hit = ceil(float(Width)/16.0);
			RAM8depth = 512;
			RAM8width = 16;
		}
		widthHit = hit;
		
	}
	else if(depthExponent > 13){
		if(widthExponent <= 5 && mode < 4 || widthExponent<=4 && mode == 4){
			hit = pow(2.0,float((depthExponent - (13 - widthExponent))));
			depthHit = hit;
			RAM8width = pow(2.0,float(widthExponent));
			RAM8depth = pow(2.0, float(13 - widthExponent));
		}
			
		else if(widthExponent > 5 && mode < 4 || widthExponent > 4 && mode == 4){
			depthHit = pow(2.0,float(depthExponent - 13));
			widthHit = pow(2.0,float(widthExponent));
			if(depthHit > 16)
				hit = 99;
			else
				hit = depthHit*widthHit;

			RAM8depth = 8192;
			RAM8width = 1;
		}
		// else{
		// 	depthHit = pow(2,(depthExponent - 13));
		// 	if(mode < 4)
		// 		widthHit = pow(2,widthExponent-5);
		// 	else
		// 		widthHit = pow(2,widthExponent-4);
		// 	if(depthHit > 16)
		// 		hit = 99;
		// 	else
		// 		hit = depthHit*widthHit;
		// }

		if(depthHit == 2)
			extraLogic += 1;
		else
			extraLogic += depthHit;

		//multiplexers
		if(depthHit <=4)
			extraLogic += 1*pow(2.0,float(widthExponent));
		else if(depthHit <= 8)
			extraLogic += 3*pow(2.0,float(widthExponent));
		else if(depthHit <=16)
			extraLogic += 5*pow(2.0,float(widthExponent));
		else
			extraLogic += 99*pow(2.0,float(widthExponent));

		if(mode == 4)
			extraLogic *=2;

	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);
	solveData.push_back(depthHit);
	solveData.push_back(widthHit);
	solveData.push_back(RAM8width);
	solveData.push_back(RAM8depth);

	return solveData;
}

vector<int> Solve::RAM128kconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	vector<int> solveData;
	int depthHit=1, widthHit=1, hit = 0, widthSize=0, extraLogic=0, RAM128width=0, RAM128depth=0;
	if(depthExponent + widthExponent <= 17 && widthExponent <= 7 && mode < 4 || depthExponent + widthExponent <= 17 && widthExponent <= 6 && mode == 4){
		hit = 1;
		if(widthExponent == 7 && mode < 4 || widthExponent == 6 && mode ==4 || depthExponent < 10 && mode < 4 || depthExponent < 11 && mode == 4){
			RAM128width = pow(2.0,float(widthExponent));
			RAM128depth = pow(2.0,float(17-widthExponent));
		}
		else if(widthExponent < 7 && depthExponent >= 10 && mode < 4 || widthExponent < 6 && depthExponent >= 11 && mode == 4){
			RAM128width = pow(2.0,float(17-depthExponent));
			RAM128depth = pow(2.0,float(depthExponent));
		}
		else{
			if(mode < 4){
				RAM128width = 128;
				RAM128depth = 1024;
			}
			else{
				RAM128width = 64;
				RAM128depth = 2048;
			}
			
		}
	}
	else if(depthExponent <= 14 && depthExponent >= 10 && mode < 4 || depthExponent <= 14 && depthExponent >= 11 && mode == 4){
		hit = ceil(float(Width)/float(pow(2.0,float(17-depthExponent))));
		widthHit = hit;
		RAM128width = pow(2.0,float(17-depthExponent));
		RAM128depth = pow(2.0, float(depthExponent));
	}
	else if(depthExponent < 10 && mode < 4 || depthExponent < 11 && mode == 4){
		if(mode < 4){
			hit = ceil(float(Width)/128.0);
			RAM128width = 128;
			RAM128depth = 1024;
		}
		else{
			hit = ceil(float(Width)/64.0);
			RAM128width = 64;
			RAM128depth = 2048;
		}
		widthHit = hit;
	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);
	solveData.push_back(depthHit);
	solveData.push_back(widthHit);
	solveData.push_back(RAM128width);
	solveData.push_back(RAM128depth);

	return solveData;

}

vector<int> Solve::LUTRAMconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	vector<int> solveData;
	int hit=0, extraLogic =0, depthHit = 1, widthHit = 1, LUTwidth, LUTdepth;
	if(depthExponent <= 5){
		hit = ceil(float(Width)/20.0);
		widthHit = hit;
		LUTwidth = 20;
		LUTdepth = 32;
	}
	else if(depthExponent == 6){
		hit = ceil(float(Width)/10.0);
		widthHit = hit;
		LUTwidth = 10;
		LUTdepth = 64;
	}

	else{
		depthHit = pow(2.0, float(depthExponent - 6));
		widthHit = ceil(float(Width)/10.0);
		hit = depthHit*widthHit;
		LUTwidth = 10;
		LUTdepth = 64;

		if(depthHit == 2)
			extraLogic += 1;
		else
			extraLogic += depthHit;

		//multiplexers
		if(depthHit <=4)
			extraLogic += 1*pow(2.0, float(widthExponent));
		else if(depthHit <= 8)
			extraLogic += 3*pow(2.0,float(widthExponent));
		else if(depthHit <=16)
			extraLogic += 5*pow(2.0,float(widthExponent));
		else
			extraLogic = 99*pow(2.0,float(widthExponent)); 
	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);
	solveData.push_back(depthHit);
	solveData.push_back(widthHit);
	solveData.push_back(LUTwidth);
	solveData.push_back(LUTdepth);


	return solveData;
}

float Solve::getArea(int circuit, int logic, int newlogic, int RAM128used, int RAM8used, int LUTRAMused){
	//figure this area thing out
	float area, requiredLogicTiles = newlogic + logic + LUTRAMused;
	area = requiredLogicTiles*(35000+40000)/2 + RAM8used*(9000 + 5*8192 + 90*sqrt(8192) + 600*2*32) + RAM128used*(9000 + 5*131072 + 90*sqrt(131072) + 600*2*128);
	return area;
}

// if(LUTRAM_support == false)
// 			logic_block_area = 35000;
// 		else
// 			logic_block_area = 40000;

// 		if(MTJ == true)
// 			RAM_area = 9000 + 1.25*bits + 90*sqrt(bits) + 600*2*max_width;
// 		else
// 			RAM_area = 9000 + 5*bits + 90*sqrt(bits) + 600*2*max_width;



