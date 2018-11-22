#ifndef __RAM_INCLUDED__
#define __RAM_INCLUDED__

#include "Solve.h"
#include "math.h"
#include <iostream>
using namespace std;

class RAM_8k{
private:
	int amount;
public:
	RAM_8k(int logic_block_amount, int orientation, int depth, int width){
		int amount = ceil(logic_block_amount/10);
		
	}


};

class LUTRAM{
private:
	int 
}


class RAM_128k{
private:
	int amount, depth, width;
public:
	RAM_128k(int logic_block_amount, int orientation, int depth, int width){
		int amount = ceil(logic_block_amount/300);
	}
};



#endif