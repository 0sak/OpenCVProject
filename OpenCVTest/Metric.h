#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "fort.hpp"

const int NUMBER_OF_IMAGES = 71;

typedef struct coords {
	float x;
	float y;
} Coords;

typedef struct label {
	int id;
	float width;
	float heigth;
	float brightness;
	int numberplateCount;
	std::vector<int> characterCount;
	std::vector<Coords> coordinates;
} Label;

class Metric
{
public:
	int numberOfLicensePlatesOnAllPictures = 0;
	int recognizedLicensePlates = 0, recognizedPerPicture = 0, totalAmountOfCharacters = 0, recognizedCharacters = 0, falsePositiveCounterPerPicture = 0;
	std::vector<label> labels;
	std::vector<std::vector<int>> recognizedCharactersPerLicenseplate;
	fort::char_table table;

	/// @brief Read dataset_labeling.txt into vector of labels.
	void readLabels();

	/// @brief print read in labels vector
	void printLabels();

	/// @brief Print metric information for one picture that is analyzed.
	/// @param index
	void printMetricOnePicture(int index);

	/// @brief print Metric information for all pictures analyzed.
	void printRecognizedOutOfAll();

	/// @brief Iterates over labels vector and counts all license plates of all pictures
	void setTotalAmountLicensePlatesOnPictures();

	/// @brief Iterates over all labels and counts all possible characters on license plates.
	void setTotalAmountOfCharactersOnPictures();
};