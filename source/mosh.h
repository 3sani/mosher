#pragma once
#include <vector>
#include <random>

// Moshes the bytes in the given buffer randomly
// There are 3 algorithms, the absolute strength of which you control with parameters noise, duplicate, and shuffle
// 0 means that the algorithm won't be used, and at around 100 they have a decent chance of rendering the file unplayable
// although the exact volatility and effects depend a lot on the size and shape of the buffer. Experiment with the values
// 
// * noise simply fills some bytes with garbage
// * duplicate copies regions of bytes over the following data
// * move moves regions of bytes around the buffer
//
// No code was optimised during the production of this software

void mosh
(
	std::vector<char>& buffer,
	int seed = 0,
	float noise = 0,
	float duplicate = 0,
	float move = 0
);
