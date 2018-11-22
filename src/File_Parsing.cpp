#include <fstream>
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
			int currentBits, maxBits=0, val1, val2, out1, out2;
			while(fgets(line2, 256, fp2) != NULL){

				if(Count2 >= 2){
					char * new_value = strtok(line2, "	");
					
					//cout << atoi(new_value) << endl;

					while (new_value != NULL){
						switch(Count3){
							case 2:
								if(new_value == "SimpleDualPort")
									current.push_back(3);
								else if(new_value == "ROM")
									current.push_back(1);
								else if(new_value == "SinglePort")
									current.push_back(2);
								else
									current.push_back(4);
								break;
							case 3:
								current.push_back(atoi(new_value));
								val1 = atoi(new_value);
								break;
							case 4:
								current.push_back(atoi(new_value));
								val2 = atoi(new_value);
								break;
							default:
								current.push_back(atoi(new_value));
								break;
						}
						
						Count3++;
						new_value = strtok(NULL, "	");
						
					}

					currentBits = val1*val2;
					if(currentBits > maxBits){
						maxBits = currentBits;
						out1 = val1;
						out2 = val2;
					}

					
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

			cout <<"Max Bits = " << maxBits << endl << out1 << " by " << out2 << endl;
			
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
	
	//cout << argv[1] << " " << argv[2];

	return (0);
	
}

