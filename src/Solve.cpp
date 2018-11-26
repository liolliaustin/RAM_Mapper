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
		// for(int j=0; j<resultVector.size(); j++){
		// 	cout << resultVector[j] << " ";
		// }
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

		//cout << "Circuit " << circuit << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
		resultVector.clear();

		if(i == circuitDefs.size()-1){
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
		}

		
	}
	cout << "128k: " << chose128 << endl << "8k: " << chose8 << endl << "LUTRAM: " << choseLUT << endl << "Total: " << count << endl;
}

vector<int> Solve::returnLowest(int currentLogic, int RAM8used, int RAM128used, int LUTRAMused, int mode, int depthExponent, int widthExponent, int Width){
	vector<int> hit128, hit8, hitLUT, final;
	int minimum, LUTcost = 999999999, RAM8cost, RAM128cost, RAM8add = 0, RAM128add = 0, LUTRAMadd = 0;

	int RAM8available = currentLogic/10 - RAM8used;
	int RAM128available = currentLogic/300 - RAM128used;
	int LUTRAMavailable = currentLogic/2 - LUTRAMused;


	hit8 = RAM8kconfiguration(mode, depthExponent, widthExponent, Width);
	hit128 = RAM128kconfiguration(mode, depthExponent, widthExponent, Width);
	if(mode != 4){
		hitLUT = LUTRAMconfiguration(mode, depthExponent, widthExponent, Width);
		if(LUTRAMavailable <=0){
			LUTcost = 2*hitLUT[0] + hitLUT[1];
			LUTRAMadd = 2*hitLUT[0];
		}
		else if(LUTRAMavailable > LUTRAMused + hitLUT[0]){
			LUTcost = hitLUT[1];
		}
		else{
			LUTcost = 2*(hitLUT[0] + LUTRAMused - LUTRAMavailable) + hitLUT[1];
			LUTRAMadd = 2*(hitLUT[0] + LUTRAMused - LUTRAMavailable);
		}
		//minimum = min(min(hit8[1], hit128[1]), hitLUT[1]);
	}
	// else
	// 	minimum = min(hit8[1], hit128[1]);
	if(RAM8available <= 0){
		RAM8add = 10*(RAM8used + hit8[0]) - currentLogic;
		RAM8cost = 10*hit8[0] + hit8[1];
	}
	else if(RAM8available > RAM8used + hit8[0]){
		RAM8cost = hit8[1];
	}
	else{
		RAM8add = 10*((RAM8used + hit8[0]) - RAM8available) - currentLogic;
		RAM8cost = 10*(RAM8available - (RAM8used + hit8[0])) + hit8[1];
	}
	if(RAM128available <=0){
		RAM128cost = 300*hit128[0] + hit128[1];
		RAM128add = 300*(RAM128used + hit128[0]) - currentLogic;
	}
	else if(RAM128available > RAM128used + hit128[0]){
		RAM128cost = hit128[1];
	}
	else{
		RAM128add = 300*((RAM128used + hit128[0]) - RAM128available) - currentLogic;
		RAM128cost = 300*(RAM128used + hit128[0] - RAM128available) + hit128[1];
	}

	//cout << "RAM8cost: " << RAM8cost << endl << "RAM128cost: " << RAM128cost << endl << "LUTcost: " << LUTcost << endl;
	minimum = min(min(RAM8cost, RAM128cost), LUTcost);
	//cout << "Available128: " << RAM128available << endl << RAM128cost << endl;

	if(minimum == RAM8cost){
		//cout << "currentLogic: " << currentLogic + ceil(hit8[1]/10) + RAM8add << endl << "RAM8cost: " << RAM8cost << endl << "RAM8used: " << RAM8used + hit8[0] << endl << "hit8: " << hit8[0] << endl << endl;
		final.push_back(1);
		final.push_back(RAM8used + hit8[0]);
		final.push_back(currentLogic + ceil(hit8[1]/10) + RAM8add);
	}
	else if (minimum == RAM128cost){
		//cout << "currentLogic: " << currentLogic + ceil(hit128[1]/10) + RAM128add << endl << "RAM128cost: " << RAM128cost << endl << "RAM128used: " << RAM128used + hit128[0] << endl << "hit128: " << hit128[0] << endl<< endl;
		final.push_back(2);
		final.push_back(RAM128used + hit128[0]);
		final.push_back(currentLogic + ceil(hit128[1]/10) + RAM128add);
	}
	else if(mode != 4 && minimum == LUTcost){
		//cout << "currentLogic: " << currentLogic + ceil(hitLUT[1]/10) + LUTRAMadd << endl << "LUTcost: " << LUTcost << endl << "LUTRAMused: " << LUTRAMused + hitLUT[0] << endl << "hitLUT: " << hitLUT[0] << endl << endl;
		final.push_back(0);
		final.push_back(LUTRAMused + hitLUT[0]);
		final.push_back(currentLogic + ceil(hitLUT[1]/10) + LUTRAMadd);
	}
	
	hit128.clear(); hit8.clear(); hitLUT.clear();

	return final;

}

//worry about tdp
vector<int> Solve::RAM8kconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	vector<int> solveData;
	int depthHit=99, widthHit=99, hit = 0, widthSize=0, extraLogic=0;
	if(depthExponent + widthExponent <= 13 && widthExponent <= 5 && mode < 4 || depthExponent + widthExponent <= 13 && widthExponent <= 4 && mode == 4){
		hit = 1;
	}
	else if(depthExponent <= 13 && depthExponent >= 8 && mode < 4 || depthExponent <= 13 && depthExponent >= 9 && mode == 4){
		hit = ceil(Width/pow(2,(13-depthExponent)));
	}
	else if(depthExponent < 8 && mode < 4 || depthExponent < 9 && mode == 4){
		if(mode < 4)
			hit = ceil(Width/32);
		else
			hit = ceil(Width/16);
	}
	else if(depthExponent > 13){
		if(widthExponent <= 5 && mode < 4 || widthExponent<=4 && mode == 4)
			hit = pow(2,(depthExponent - (13 - widthExponent)));
			
		else if(widthExponent > 5 && mode < 4 || widthExponent > 4 && mode == 4){
			depthHit = pow(2,(depthExponent - 13));
			widthHit = pow(2,widthExponent);
			if(depthHit > 16)
				hit = 99;
			else
				hit = depthHit*widthHit;
		}
		else
			hit = 99;

		if(depthHit == 2)
			extraLogic += 1;
		else
			extraLogic += depthHit;

		//multiplexers
		if(depthHit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(depthHit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(depthHit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic += 99*pow(2,widthExponent);

	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;
}

vector<int> Solve::RAM128kconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	//cout << "gothere" << endl;
	vector<int> solveData;
	int depthHit=99, widthHit=99, hit = 0, widthSize=0, extraLogic=0;
	if(depthExponent + widthExponent <= 17 && widthExponent <= 7 && mode < 4 || depthExponent + widthExponent <= 17 && widthExponent <= 6 && mode == 4){
		hit = 1;
	}
	else if(depthExponent <= 17 && depthExponent >= 10 && mode < 4 || depthExponent <= 17 && depthExponent >= 11 && mode == 4){
		//cout << "gothere2" << endl;
		hit = ceil(Width/pow(2,(17-depthExponent)));
	}
	else if(depthExponent < 10 && mode < 4 || depthExponent < 11 && mode == 4){
		if(mode < 4)
			hit = ceil(Width/128);
		else
			hit = ceil(Width/64);
	}
	else if(depthExponent > 17){
		if(widthExponent <= 7 && mode < 4 || widthExponent<=6 && mode == 4)
			hit = pow(2,(depthExponent - (17 - widthExponent)));
			
		else if(widthExponent > 7 && mode < 4 || widthExponent > 6 && mode == 4){
			depthHit = pow(2,(depthExponent - 17));
			widthHit = pow(2,widthExponent);
			if(depthHit > 16)
				hit = 99;
			else
				hit = depthHit*widthHit;
		}
		else
			hit = 99;

		if(depthHit == 2)
			extraLogic += 1;
		else
			extraLogic += depthHit;

		//multiplexers
		if(depthHit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(depthHit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(depthHit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic += 99*pow(2,widthExponent);

	}

	//cout << "hit: " << hit << endl << "extraLogic: " << extraLogic << endl;

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;

}

vector<int> Solve::LUTRAMconfiguration(int mode, int depthExponent, int widthExponent, int Width){
	vector<int> solveData;
	int hit=0, extraLogic =0, depthHit = 99, widthHit = 99;
	if(depthExponent <= 5)
		hit = ceil(Width/20);
	else if(depthExponent == 6)
		hit = ceil(Width/10);
	else{
		depthHit = pow(2,depthExponent - 6);
		widthHit = ceil(Width/10);
		hit = depthHit*widthHit;

		if(depthHit == 2)
			extraLogic += 1;
		else
			extraLogic += depthHit;

		//multiplexers
		if(depthHit <=4)
			extraLogic += 1*pow(2,widthExponent);
		else if(depthHit <= 8)
			extraLogic += 3*pow(2,widthExponent);
		else if(depthHit <=16)
			extraLogic += 5*pow(2,widthExponent);
		else
			extraLogic = 99*pow(2,widthExponent); 
	}

	solveData.push_back(hit); 
	solveData.push_back(extraLogic);

	return solveData;
}



// if(LUTRAM_support == false)
// 			logic_block_area = 35000;
// 		else
// 			logic_block_area = 40000;

// 		if(MTJ == true)
// 			RAM_area = 9000 + 1.25*bits + 90*sqrt(bits) + 600*2*max_width;
// 		else
// 			RAM_area = 9000 + 5*bits + 90*sqrt(bits) + 600*2*max_width;



