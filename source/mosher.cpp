#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include <cstring>

#include "mosh.h"

int main(int argc, char* argv[])
{
	int headerSize = 7000;
	float noise = 0;
	float duplicate = 0;
	float move = 0;
	std::vector<std::string> filePaths;

	// Parse arguments
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			try
			{
				std::string argument = argv[i];
				if (argument.rfind("h=", 0) == 0) headerSize = std::stoi(argument.substr(2));
				else if (argument.rfind("n=", 0) == 0) noise = std::stof(argument.substr(2));
				else if (argument.rfind("d=", 0) == 0) duplicate = std::stof(argument.substr(2));
				else if (argument.rfind("m=", 0) == 0) move = std::stof(argument.substr(2));
				else filePaths.push_back(argument);
			}
			catch (const std::exception&)
			{
				std::cout << "Couldn't parse argument " << argv[i] << std::endl;
				return -1;
			}
		}
	}
	else
	{
		std::cout << "No arguments, running example" << std::endl;

		try
		{
			filePaths.emplace_back("example.avi");
		}
		catch (const std::exception&)
		{
			std::cout << "Example file not present" << std::endl;
			return -1;
		}

		headerSize = 7000;
		noise = 33;
		duplicate = 33;
		move = 33;
		printf("mosh %s h=%i n=%f d=%f m=%f\n\n", "example.avi", headerSize, noise, duplicate, move);
	}

	printf("Moshing\n    noise=%f\n    duplicate=%f\n    move=%f\nfiles\n", noise, duplicate, move);
	for (std::string file : filePaths) std::cout << "    " << file << std::endl;

	// No idea what the real header length is
	// Online it says all this stuff about it being 56 bytes or whatever,
	// but if you open video files there's multiple KBs of data at the
	// start where if you mess with it the video won't play back so <shrug>
	const int AVI_HEADER_LENGTH = 7000;

	for (std::string file : filePaths)
	{
		try
		{
			std::cout << "Moshing " << file << std::endl;
			std::ifstream input(file, std::ios::binary);

			char header[AVI_HEADER_LENGTH];
			input.read(header, AVI_HEADER_LENGTH);

			std::vector<char> bytes(
				(std::istreambuf_iterator<char>(input)),
				(std::istreambuf_iterator<char>()));

			if (bytes.size() == 0)
			{
				std::cout << "File " << file << " non-existent or too small for header size, skipping" << std::endl;
				continue;
			}

			input.close();

			mosh(bytes, 0, noise, duplicate, move);

			int fileExtensionPos = file.find_last_of(".");
			std::ofstream output(file.substr(0, fileExtensionPos) + "-moshed" + file.substr(fileExtensionPos, file.length()), std::ios::out | std::ios::binary);
			output.write((char*)header, AVI_HEADER_LENGTH);
			output.write((char*)&bytes[0], bytes.size() * sizeof(char));

			output.close();
		}
		catch (const std::exception& ex)
		{
			std::cout << "Couldn't mosh " << file << " " << ex.what() << std::endl;
		}
	}

	std::cout << "Moshing complete" << std::endl;

	return 0;
}
