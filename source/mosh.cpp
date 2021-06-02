#include "mosh.h"

void mosh(std::vector<char>& buffer, int seed, float noise, float duplicate, float move)
{
	const int bufferSize = buffer.size();
	std::default_random_engine rng(seed);
	std::uniform_real_distribution<float> float_dist(0.0, 1.0);

	if (noise > 0)
	{
		// Randomly fill bytes with garbage
		std::uniform_int_distribution<int> byte_dist(0, 255);
		float noiseProbability = 0.00000000001 * std::pow(noise, 4);
		for (int i = 0; i < bufferSize; i++)
		{
			if (float_dist(rng) < noiseProbability) buffer[i] = static_cast<unsigned char>(byte_dist(rng));
		}
	}

	if (duplicate > 0)
	{
		// Randomly duplicate regions
		std::uniform_int_distribution<int> jump_dist(0, bufferSize / (2 * duplicate));
		std::uniform_int_distribution<int> copy_dist(0, duplicate * bufferSize / 1000);
		float duplicateProbability = 0.002 * duplicate;
		int tracker = 0;
		int copyLength = copy_dist(rng);
		while (tracker + 2 * copyLength < bufferSize)
		{
			if (float_dist(rng) < duplicateProbability)
			{
				for (int i = 0; i < copyLength; i++)
				{
					buffer[tracker + copyLength] = buffer[tracker++];
				}
			}

			// Chance to move to the next region
			// If not, the same region will get duplicated multiple times ie ABC -> AAA
			if (float_dist(rng) > duplicateProbability * 0.25) {
				tracker += jump_dist(rng);
				copyLength = copy_dist(rng);
			}
		}
	}

	if (move > 0)
	{
		// Randomly move regions from the end of the file over the start
		std::uniform_int_distribution<int> jump_dist(0, bufferSize / (2 * move));
		std::uniform_int_distribution<int> copy_dist(0, move * bufferSize / 1000);
		float moveProbability = 0.002 * move;
		int copyLength = copy_dist(rng);
		int copyFromTracker = bufferSize - copyLength;
		int copyToTracker = 0;
		while (copyToTracker + copyLength < bufferSize && copyFromTracker > 0 && copyFromTracker + copyLength < bufferSize)
		{
			if (float_dist(rng) < moveProbability)
			{
				for (int i = 0; i < copyLength; i++)
				{
					buffer[copyToTracker++] = buffer[copyFromTracker + i];
				}
			}
			copyFromTracker -= jump_dist(rng);
			copyToTracker += jump_dist(rng);
			copyLength = copy_dist(rng);
		}
	}
}
