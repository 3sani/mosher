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

	for (std::string file : filePaths)
	{
		try
		{
			std::cout << "Moshing " << file << std::endl;
			std::ifstream input(file, std::ios::binary);

			std::vector<char> bytes(
				(std::istreambuf_iterator<char>(input)),
				(std::istreambuf_iterator<char>()));

			input.close();

			if (bytes.size() == 0)
			{
				std::cout << "File " << file << " empty, skipping file" << std::endl;
				continue;
			}

			if (bytes.size() < headerSize)
			{
				std::cout << "File " << file << " smaller than header, skipping file" << std::endl;
				continue;
			}

			// Move header to its own vector
			std::vector<char> header;
			header.insert(header.begin(), std::make_move_iterator(bytes.begin()), std::make_move_iterator(bytes.begin() + headerSize));
			bytes.erase(bytes.begin(), bytes.begin() + headerSize);

			mosh(bytes, 0, noise, duplicate, move);

			// Write to output file
			int fileExtensionPos = file.find_last_of(".");
			std::ofstream output(file.substr(0, fileExtensionPos) + "-moshed" + file.substr(fileExtensionPos, file.length()), std::ios::out | std::ios::binary);

			// Combine vectors
			bytes.insert(bytes.begin(), header.begin(), header.end());
			//output.write((char*)&header[0], header.size() * sizeof(char));
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
