#include <fstream>
#include <typeinfo>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector> 
#include <queue>
#include <deque>
#include "Solve.h"
using namespace std;

class Utils {
private:
	vector<int> numLogicBlocks;
	vector<vector<int> > logicalValues;

public:

	void get_configuration(char * file_name, char * file_name_2) {
		FILE *fp, *fp2;
		char line[256];
		char line2[256];
		int Count=0, Count2=0, Count3=0;
		fp = fopen(file_name, "r+");
		fp2 = fopen(file_name_2, "r+"); 

		vector<int> numLogicBlocks;
		

		if(fp){

			while(fgets(line, 256, fp) != NULL){
				if(Count >= 1){
					char * value = strtok(line, "	");
					
					while (value != NULL){
						
						if(atoi(value) > 68)	
							numLogicBlocks.push_back(atoi(value));
						
						value = strtok(NULL, "	");
					}
					
				}	
				Count++;
			}
			
			this -> numLogicBlocks = numLogicBlocks;

			fclose(fp);
			
		}
		else{
			cout << "Couldn't open file 1." << endl;
			exit(-1);
		}
		// for(unsigned int i=0; i<numLogicBlocks.size(); i++){
		// 	cout << numLogicBlocks[i] << endl;
		// }

// 1= ROM, 2=SinglePort, 3=SimpleDualPort, 4=TrueDualPort
		if(fp2){
			vector<vector<int> >logicalValues;
			vector<int> current;
			int currentBits, maxBits=0, tdpCount = 0, ROMCount = 0, SDPcount =0, SinPcount = 0;
			while(fgets(line2, 256, fp2) != NULL){

				if(Count2 >= 2){
					char * new_value = strtok(line2, "	");
					bool doit = false;

					while (new_value != NULL){
						switch(Count3){
							case 2:
								if(new_value[0] == 'S' && new_value[2] == 'm'){
									current.push_back(3);
								}
								else if(new_value[0] == 'R'){
									current.push_back(1);
								}
								else if(new_value[0] == 'S' && new_value[2] == 'n'){
									current.push_back(2);
								}
								else{
									current.push_back(4);
								}
								break;
							case 3:
								current.push_back(atoi(new_value));
								if(atoi(new_value) == 8192){
									doit = true;
									tdpCount++;
								}
								else if(atoi(new_value) >= 4096)
									SDPcount++;
								else if(atoi(new_value) >= 2048)
									ROMCount++;
								break;
							case 4:
								if(doit == true){
									//cout << atoi(new_value) << endl;
								}
								current.push_back(atoi(new_value));
								break;
							default:
								current.push_back(atoi(new_value));
								break;
						}
						
						Count3++;
						new_value = strtok(NULL, "	");

						
					}


					doit = false;
					logicalValues.push_back(current);
					current.clear();
					Count3 = 0;

				}	
				Count2++;

			}

			// for(int i=0; i<logicalValues.size(); i++){
			// 	for(int j=0; j<logicalValues[i].size(); j++){
			// 		cout << logicalValues[i][j] << " ";
			// 	}
			// 	cout << endl;
			// }

			//cout << endl <<"SDPcount = " << SDPcount << endl << "TrueDualPortcount = " << tdpCount << endl << "SinPcount = " << SinPcount << endl << "ROMs = " << ROMCount << endl;
			
			fclose(fp2);
			this -> logicalValues = logicalValues;
			
		}
		else{
			cout << "Couldn't open file 2." << endl;
			exit(-1);
			return;
		}
		
	}

	vector<int> getLBCount(){return numLogicBlocks;}
	vector<vector<int> > getLogicalValues(){return logicalValues;}

};

int main(int argc, char * argv[]) {
	
	Utils * ut = new Utils();
	ut->get_configuration(argv[1], argv[2]);

	Solve set;
	set.mapCircuit(ut->getLogicalValues(), ut->getLBCount(), false, false);
	
	//cout << argv[1] << " " << argv[2];

	return (0);
	
}

