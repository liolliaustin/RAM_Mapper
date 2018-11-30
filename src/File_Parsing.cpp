#include <fstream>
#include <ctime>
#include <time.h>
#include <sys/time.h>
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

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

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

	std::ofstream outfile;
	vector<int> results;

	double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

	Utils * ut = new Utils();
	ut->get_configuration(argv[1], argv[2]);

	// cout << argv[3][0] << endl;

	if (strcmp(argv[3], "1") == 0){
		Solve set;
		set.mapCircuit(ut->getLogicalValues(), ut->getLBCount());

		double wall1 = get_wall_time();
	    double cpu1  = get_cpu_time();

	    cout << "Wall Time = " << wall1 - wall0 << endl;
	    cout << "CPU Time  = " << cpu1  - cpu0  << endl;

		for(int i=0; i< ut->getLogicalValues().size(); i++){
			results = set.getFinalResult(i);
			outfile.open("resultRAMS2.txt", std::ios_base::app);
			if(results[9] == 1)
	  			outfile << results[0] << " " << results[1] << " " << results[2] << " LW " << results[3] << " LD " << results[4] << " ID " << results[5] << " S " 
	  				<< results[6] << " P " << results[7] << " Type " << results[8] << " Mode ROM W " << results[10] << " D " << results[11] << "\n"; 
	  		else if(results[9] == 2)
	  			outfile << results[0] << " " << results[1] << " " << results[2] << " LW " << results[3] << " LD " << results[4] << " ID " << results[5] << " S " 
	  				<< results[6] << " P " << results[7] << " Type " << results[8] << " Mode SinglePort W " << results[10] << " D " << results[11] << "\n"; 
	  		else if(results[9] == 3)
	  			outfile << results[0] << " " << results[1] << " " << results[2] << " LW " << results[3] << " LD " << results[4] << " ID " << results[5] << " S " 
	  				<< results[6] << " P " << results[7] << " Type " << results[8] << " Mode SimpleDualPort W " << results[10] << " D " << results[11] << "\n"; 
	  		else if(results[9] == 4)
	  			outfile << results[0] << " " << results[1] << " " << results[2] << " LW " << results[3] << " LD " << results[4] << " ID " << results[5] << " S " 
	  				<< results[6] << " P " << results[7] << " Type " << results[8] << " Mode TrueDualPort W " << results[10] << " D " << results[11] << "\n"; 
	  		outfile.close();
		}
	}

	else if (strcmp(argv[3], "2") == 0){
		bool LUTRAM_support = false, MTJ = false, multiRAM = false;
		if (strcmp(argv[4], "false") == 0)
			LUTRAM_support = false;
		else if (strcmp(argv[4], "true") == 0)
			LUTRAM_support = true;
		if (strcmp(argv[5], "false") == 0)
			MTJ = false;
		else if (strcmp(argv[5], "true") == 0)
			MTJ = true;
		if (strcmp(argv[6], "false") == 0)
			multiRAM = false;
		else if (strcmp(argv[6], "true") == 0){
			multiRAM = true;
		}
		Solve set;
		for(int i=10; i<18; i++){
			cout << endl;
			set.areaModel(ut->getLogicalValues(), ut->getLBCount(), i, LUTRAM_support, MTJ, multiRAM);
		}
		double wall1 = get_wall_time();
	    double cpu1  = get_cpu_time();

	    cout << "Wall Time = " << wall1 - wall0 << endl;
	    cout << "CPU Time  = " << cpu1  - cpu0  << endl;
	}

	return (0);
	
}

