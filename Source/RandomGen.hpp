//
//  RandomGen.hpp
//  CSE_168
//
//  Created by Clayton Anderson on 5/8/17.
//  Copyright © 2017 Clayton Anderson. All rights reserved.
//

#ifndef RandomGen_h
#define RandomGen_h

#include <climits>
#include <stdlib.h>
#include <iostream>

class RandomGen
{
public:
    RandomGen() {
        seed = rand();
        GenerateRandom(0,1);
    }
    RandomGen(unsigned long int in_seed) {
        seed = in_seed;
        GenerateRandom(0,1); // Skip first input
    }
    float GenerateRandom (float min, float max) {
        // Not thread safe!
        seed ^= seed << 21;
        seed ^= seed >> 35;
        seed ^= seed << 4;
        
		// TODO : NOT WORKING, REPEATS AFTER 4 iterations
        //return ((float)seed / (float)ULONG_MAX) * (max - min) + min;
		float ranf = ((float)rand()) / (float)RAND_MAX;
		return min + ranf * (max - min);
    }
private:
    unsigned long int seed;
    
};

#endif /* RandomGen_h */
