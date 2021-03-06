#include "Solve.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "math.h"

using namespace std;

//create vector for final file creation
void Solve::mapCircuit(vector<vector<int> > circuitDefs, vector<int> logicBlockCount){
	vector<vector<int> > finalResult;
	vector<int> RAM_map;
	float logic_block_area, RAM_area, totalArea, areaSum;
	vector<float> areaVector;
	int bits = 0, max_width = 0, additionalLuts;
	int circuit = circuitDefs[0][0], logic = logicBlockCount[0], RAM_ID, Mode, Depth, Width, newlogic = 0, RAM8count=0, RAM8used=0, RAM128count=0, RAM128used=0, LUTRAMcount = 0, LUTRAMused = 0;
	int chose128 = 0, chose8 = 0, choseLUT = 0, count=0;
	
	for(int i=0; i<circuitDefs.size(); i++){
		if(circuit != circuitDefs[i][0]){
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused <<  endl;
			cout << "Total Area: " << totalArea << endl << endl;
			newlogic = logicBlockCount[circuit]; 
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

		resultVector = returnLowest(newlogic, RAM8used, RAM128used, LUTRAMused, Mode, depthExponent, widthExponent, Width);
		
		if(resultVector[0] == 2){
			RAM8used = resultVector[1];
			newlogic = resultVector[2];
			additionalLuts = resultVector[3];
			chose8++;
		}
		else if(resultVector[0] == 3){
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

		resultVector.clear();

		if(i == circuitDefs.size()-1){
			
			cout << "Circuit " << circuit << " original size: " << logic << endl << "New logic: " << newlogic << endl << "128k: " << RAM128used << endl << "8k: " << RAM8used << endl << "LUTRAM: " << LUTRAMused << endl << endl;
			
		}


		
	}
	this -> finalResult = finalResult;
	cout << "128k: " << chose128 << endl << "8k: " << chose8 << endl << "LUTRAM: " << choseLUT << endl << "Total: " << count << endl;
}

//return lowest cost to implement on type of RAM (128, 8 LUTRAM)
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
	if(mode != 4 && minimum == LUTcost){
		final.push_back(1);
		final.push_back(LUTRAMused + hitLUT[0]);
		final.push_back(currentLogic + ceil(float(hitLUT[1])/10.0) + LUTRAMadd);
		final.push_back(hitLUT[1]);
		final.push_back(hitLUT[2]);
		final.push_back(hitLUT[3]);
		final.push_back(hitLUT[4]);
		final.push_back(hitLUT[5]);
	}

	else if(minimum == RAM8cost){
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
		final.push_back(3);
		final.push_back(RAM128used + hit128[0]);
		final.push_back(currentLogic + ceil(float(hit128[1])/10.0) + RAM128add);
		final.push_back(hit128[1]);
		final.push_back(hit128[2]);
		final.push_back(hit128[3]);
		final.push_back(hit128[4]);
		final.push_back(hit128[5]);
	}
	
	
	hit128.clear(); hit8.clear(); hitLUT.clear();

	return final;

}

//Determine Hit, cost and other parameters of instantiated 8kRAM
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

//Determine Hit, cost and other parameters of instantiated 128kRAM
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

//Determine Hit, cost and other parameters of instantiated LUTRAM
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

//return area based flags
float Solve::getArea(int newlogic, int RAMused, int maxWidth, int bits, int RAM2used, int maxWidth2, int bits2, bool LUTRAM_support, bool MTJ){
	float area;
	if(LUTRAM_support == false && MTJ == false)
		area = newlogic*35000/2 + RAMused*(9000 + 5*bits + 90*sqrt(bits) + 600*2*maxWidth) + RAM2used*(9000 + 5*bits2 + 90*sqrt(bits2) + 600*2*maxWidth2);
	else if(LUTRAM_support == true && MTJ == false)
		area = newlogic*(40000 + 35000)/2 + RAMused*(9000 + 5*bits + 90*sqrt(bits) + 600*2*maxWidth) + RAM2used*(9000 + 5*bits2 + 90*sqrt(bits2) + 600*2*maxWidth2);
	else if(LUTRAM_support == false && MTJ == true)
		area = newlogic*35000/2 + RAMused*(9000 + 1.25*bits + 90*sqrt(bits) + 600*2*maxWidth) + RAM2used*(9000 + 1.25*bits2 + 90*sqrt(bits2) + 600*2*maxWidth2);
	else if(LUTRAM_support == true && MTJ == true)
		area = newlogic*(40000 + 35000)/2 + RAMused*(9000 + 1.25*bits + 90*sqrt(bits) + 600*2*maxWidth) + RAM2used*(9000 + 1.25*bits2 + 90*sqrt(bits2) + 600*2*maxWidth2);;

	return area;
}

//based on flags explore specific architecture
void Solve::areaModel(vector<vector<int> > circuitDefs, vector<int> logicBlockCount, int BlockRAMsizeExponent, bool LUTRAM_support, bool MTJ, bool multiRAM){
	float minimumArea = 10000000000000;
	int minMaxWidth = 0, minLBpRB = 0, minMaxWidth2=0;

	if(multiRAM == false){
		for(int j = -2; j < 3; j++){
			for(int k = 10; k<=50; k+=10){
				int circuit = circuitDefs[0][0], logic = logicBlockCount[0], newLogic = logicBlockCount[0], RAM_ID, Mode, Depth, Width, RAMused=0;
				int LBpRB = k, LUTRAMused=0;
				int BRAMsize = pow(2.0, float(BlockRAMsizeExponent));
				int maxWidthExponent = BlockRAMsizeExponent/2+j;
				float areaCircuit=0, areaResult=0;

				vector<int> ramResult, LUTResult;
				vector<float> areaVector;

				for(int i=0; i<circuitDefs.size(); i++){
					// cout << newLogic << endl;
					if(circuit != circuitDefs[i][0]){
						
						areaCircuit = getArea(newLogic, RAMused, pow(2.0, float(maxWidthExponent)), BRAMsize, 0, 0, 0, LUTRAM_support, MTJ);
						areaVector.push_back(areaCircuit/100000000.0);
						//cout << "Circuit: " << circuit << endl << "RAMs Used: " << RAMused << endl << "Previous Logic: " << logic << endl << "newLogic: " << newLogic << endl << "Area: " << areaCircuit << endl << endl;
						circuit = circuitDefs[i][0];
						logic = logicBlockCount[circuit];
						newLogic = logicBlockCount[circuit];
						RAMused = 0;
					}

					RAM_ID = circuitDefs[i][1];
					Mode = circuitDefs[i][2];
					Depth = circuitDefs[i][3];
					Width = circuitDefs[i][4];
					int widthExponent = ceil(log2(Width));
					int depthExponent = ceil(log2(Depth));

					if(LUTRAM_support == false){
					
						ramResult = returnAmount(newLogic, Mode, BlockRAMsizeExponent, maxWidthExponent, widthExponent, depthExponent, RAMused, LBpRB);

						RAMused = ramResult[0];
						newLogic += ramResult[1];

						//cout << "RAMs Used: " << RAMused << endl << "newLogic: " << newLogic << endl;

						ramResult.clear();
					}
					else{
						if(Mode < 4)
							LUTResult = LUTRAMconfiguration(Mode, depthExponent, widthExponent, Width);
						ramResult = returnAmount(newLogic, Mode, BlockRAMsizeExponent, maxWidthExponent, widthExponent, depthExponent, RAMused, LBpRB);

						if(Mode < 4 && LUTResult[1] == 0)
							LUTRAMused += LUTResult[0];
						else if(ramResult[1] == 0)
							RAMused = ramResult[0];
						else if(Mode < 4 && LUTResult[1] < ramResult[1]){
							newLogic += ceil(float(LUTResult[1])/10.0);
							LUTRAMused += LUTResult[0];
						}
						else{
							RAMused = ramResult[0];
							newLogic += ramResult[1];
						}

					}

					if(i == circuitDefs.size()-1){
						//cout << "Circuit: " << circuit << endl << "RAMs Used: " << RAMused << endl << "Previous Logic: " << logic << endl << "newLogic: " << newLogic << endl;
						areaCircuit = getArea(newLogic, RAMused, pow(2.0, float(maxWidthExponent)), 0, 0, 0, BRAMsize, LUTRAM_support, MTJ);
						areaVector.push_back(areaCircuit/100000000.0);
					}

				}
				
				for(int i=0; i<areaVector.size(); i++){
					//cout << areaVector[i] << endl;
					areaResult += areaVector[i];
				}
				areaResult /= areaVector.size();
				areaResult *= 100000000.0;

				if(areaResult < minimumArea){
					minMaxWidth = pow(2.0, float(maxWidthExponent));
					minimumArea = areaResult;
					minLBpRB = LBpRB;
				}
				
				areaVector.clear();
			}
		}
		cout << minMaxWidth << " " << minLBpRB << endl;
		cout << "Final Area " << pow(2.0, float(BlockRAMsizeExponent)) << " maxWidthExponent (" << minMaxWidth << "): " << minimumArea << endl;
	}
	else{
		int minRAM2;
		for(int RAM2exponent = 10; RAM2exponent <= 17; RAM2exponent++){
			for(int j = -2; j < 3; j++){
				for(int k = 10; k<=50; k+=10){
					int circuit = circuitDefs[0][0], logic = logicBlockCount[0], newLogic = logicBlockCount[0], RAM_ID, Mode, Depth, Width, RAMused=0, RAM2used = 0;
					int LBpRB = k, LUTRAMused=0;
					int BRAMsize = pow(2.0, float(BlockRAMsizeExponent));
					int BRAMsize2 = pow(2.0, float(RAM2exponent));
					int maxWidthExponent = BlockRAMsizeExponent/2+j;
					int maxWidthExponent2 = RAM2exponent/2+j;
					float areaCircuit=0, areaResult=0;

					vector<int> ramResult, LUTResult, ram2Result;
					vector<float> areaVector;

					for(int i=0; i<circuitDefs.size(); i++){
						// cout << newLogic << endl;
						if(circuit != circuitDefs[i][0]){
							
							areaCircuit = getArea(newLogic, RAMused, pow(2.0, float(maxWidthExponent)), BRAMsize, RAM2used, pow(2.0, float(maxWidthExponent2)), BRAMsize2, LUTRAM_support, MTJ);
							areaVector.push_back(areaCircuit/100000000.0);
							//cout << "Circuit: " << circuit << endl << "RAMs Used: " << RAMused << endl << "Previous Logic: " << logic << endl << "newLogic: " << newLogic << endl << "Area: " << areaCircuit << endl << endl;
							circuit = circuitDefs[i][0];
							logic = logicBlockCount[circuit];
							newLogic = logicBlockCount[circuit];
							RAMused = 0;
							RAM2used = 0;
						}

						RAM_ID = circuitDefs[i][1];
						Mode = circuitDefs[i][2];
						Depth = circuitDefs[i][3];
						Width = circuitDefs[i][4];
						int widthExponent = ceil(log2(Width));
						int depthExponent = ceil(log2(Depth));

						if(LUTRAM_support == false){
						
							ramResult = returnAmount(newLogic, Mode, BlockRAMsizeExponent, maxWidthExponent, widthExponent, depthExponent, RAMused, LBpRB);

							RAMused = ramResult[0];
							newLogic += ramResult[1];

							//cout << "RAMs Used: " << RAMused << endl << "newLogic: " << newLogic << endl;

							ramResult.clear();
						}
						else{
							if(Mode < 4)
								LUTResult = LUTRAMconfiguration(Mode, depthExponent, widthExponent, Width);
							ramResult = returnAmount(newLogic, Mode, BlockRAMsizeExponent, maxWidthExponent, widthExponent, depthExponent, RAMused, LBpRB);
							ram2Result = returnAmount(newLogic, Mode, RAM2exponent, maxWidthExponent2, widthExponent, depthExponent, RAM2used, LBpRB);

							if(Mode < 4 && LUTResult[1] == 0)
								LUTRAMused += LUTResult[0];
							else if(ramResult[1] == 0)
								RAMused = ramResult[0];
							else if(ram2Result[1] == 0)
								RAM2used = ram2Result[0];
							else if(Mode < 4 && LUTResult[1] < ramResult[1] && LUTResult[1] < ram2Result[1]){
								newLogic += ceil(float(LUTResult[1])/10.0);
								LUTRAMused += LUTResult[0];
							}
							else if(ramResult[0] - RAMused < ram2Result[0] - RAM2used){
								RAMused = ramResult[0];
								newLogic += ramResult[1];
							}
							else{
								RAM2used = ram2Result[0];
								newLogic += ram2Result[1];
							}

						}

						if(i == circuitDefs.size()-1){
							//cout << "Circuit: " << circuit << endl << "RAMs Used: " << RAMused << endl << "Previous Logic: " << logic << endl << "newLogic: " << newLogic << endl;
							areaCircuit = getArea(newLogic, RAMused, pow(2.0, float(maxWidthExponent)), BRAMsize, RAM2used, pow(2.0, float(maxWidthExponent2)), BRAMsize2, LUTRAM_support, MTJ);
							areaVector.push_back(areaCircuit/100000000.0);
						}

					}
					
					for(int i=0; i<areaVector.size(); i++){
						//cout << areaVector[i] << endl;
						areaResult += areaVector[i];
					}
					areaResult /= areaVector.size();
					areaResult *= 100000000.0;

					if(areaResult < minimumArea){
						minMaxWidth = pow(2.0, float(maxWidthExponent));
						minMaxWidth2 = pow(2.0, float(maxWidthExponent2));
						minRAM2 = pow(2.0, float(RAM2exponent));
						minimumArea = areaResult;
						minLBpRB = LBpRB;
					}
					
					areaVector.clear();
				}
			}
		}
		cout << "Max width RAM 1: " << minMaxWidth << endl << "Max width RAM 2: " << minMaxWidth2 << endl << minLBpRB << endl;
		cout << "Final Area " << pow(2.0, float(BlockRAMsizeExponent)) << " maxWidthExponent (" << minMaxWidth << ") and "; 
		cout << minRAM2 << " maxWidthExponent (" << minMaxWidth2 << "): " << minimumArea << endl;

	}
	
	//cout << maxWidthExponent << endl;
}

//Map RAM type passed
vector<int> Solve::returnAmount(int newLogic, int mode, int BlockRAMsizeExponent, int maxWidthExponent, int widthExponent, int depthExponent, int RAMused, int LBpRB){
	vector<int> returnVector;
	int available = newLogic/LBpRB - RAMused, extraLogic = 0, depthHit=0, newvals =0;

	if(depthExponent + widthExponent <= BlockRAMsizeExponent && widthExponent <= maxWidthExponent && mode < 4 || depthExponent + widthExponent <= BlockRAMsizeExponent && widthExponent < (maxWidthExponent-1) && mode == 4){
		RAMused += 1;
	}
	else if(depthExponent <= BlockRAMsizeExponent && depthExponent >= (BlockRAMsizeExponent - maxWidthExponent) && mode <4){
		RAMused += ceil(pow(2.0, float(widthExponent - maxWidthExponent)));
	}
	else if(depthExponent <= BlockRAMsizeExponent && depthExponent >= (BlockRAMsizeExponent - maxWidthExponent + 1) && mode==4){
		RAMused += ceil(pow(2.0, float(widthExponent - maxWidthExponent + 1)));
	}
	else if(depthExponent < (BlockRAMsizeExponent - maxWidthExponent) && mode < 4){
		if(widthExponent <= maxWidthExponent)
			RAMused += 1;
		else
			RAMused += ceil(pow(2.0, float(widthExponent - maxWidthExponent)));
	}

	else if(depthExponent < (BlockRAMsizeExponent - maxWidthExponent -1) && mode == 4){
		if(widthExponent <= (maxWidthExponent -1))
			RAMused += 1;
		else
			RAMused += ceil(pow(2.0, float(widthExponent - maxWidthExponent + 1)));
	}

	else if(depthExponent > BlockRAMsizeExponent){
		if(widthExponent <= maxWidthExponent && mode < 4 || widthExponent <= (maxWidthExponent -1) && mode == 4)
			RAMused += ceil(pow(2.0, float(depthExponent - BlockRAMsizeExponent)));
		
		else{
			if(mode < 4)
				RAMused += ceil(pow(2.0, float(depthExponent - BlockRAMsizeExponent)))*ceil(pow(2.0, float(widthExponent - maxWidthExponent)));
			else
				RAMused += ceil(pow(2.0, float(depthExponent - BlockRAMsizeExponent)))*ceil(pow(2.0, float(widthExponent - maxWidthExponent + 1)));
		}
		
		depthHit = ceil(pow(2.0, float(depthExponent - BlockRAMsizeExponent)));

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

	if(available <=0){
		newvals += LBpRB*RAMused;
	}
	if(extraLogic != 0)
		newvals += ceil(float(extraLogic)/10.0);

	returnVector.push_back(RAMused);
	returnVector.push_back(newvals);

	return returnVector;
	
}
