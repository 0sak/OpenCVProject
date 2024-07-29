#include "Metric.h"


void Metric::readLabels() {
	std::string datasetLabelString;
	std::ifstream datasetLablesStream("dataset_labeling.txt");
	std::regex re(",");

	while (std::getline(datasetLablesStream, datasetLabelString, ';')) {
		label datasetLabel;
		datasetLabelString.erase(remove(datasetLabelString.begin(), datasetLabelString.end(), '\n'), datasetLabelString.end());
		std::sregex_token_iterator iter(datasetLabelString.begin(), datasetLabelString.end(), re, -1);
		std::sregex_token_iterator end;
		std::vector<std::string> subString;

		for (; iter != end; ++iter) {
			if (*iter == '\n') {
				continue;
			}
			subString.push_back(*iter);
		}
		
		datasetLabel.id = std::stoi(subString.at(0));
		datasetLabel.width = std::stof(subString.at(1));
		datasetLabel.heigth = std::stof(subString.at(2));
		datasetLabel.brightness = std::stof(subString.at(3));
		datasetLabel.numberplateCount = std::stoi(subString.at(4));

		for (int i = 5; i < datasetLabel.numberplateCount + 5; i++) {
			datasetLabel.characterCount.push_back(std::stoi(subString.at(i)));
		}

		int counter = 0;
		float x = 0, y = 0;

		for (int i = datasetLabel.numberplateCount + 5; i < datasetLabel.numberplateCount * 2 + datasetLabel.numberplateCount + 5; i++) {
			if (counter % 2 == 0) {
				counter = 0;
			}
			if (counter == 0)
				x = std::stof(subString.at(i));

			if (counter == 1) {
				y = std::stof(subString.at(i));

				coords newCoordinate;
				newCoordinate.x = x;
				newCoordinate.y = y;

				datasetLabel.coordinates.push_back(newCoordinate);
			}
			counter++;
		}
		labels.push_back(datasetLabel);
	}
	datasetLablesStream.close();
}

void Metric::printLabels() {
	for (size_t i = 0; i < labels.size(); i++)
	{
		std::cout <<"ID: " << labels.at(i).id << '\n' << "WIDTH: " << labels.at(i).width << '\n' << "HEIGHT: " << labels.at(i).heigth << '\n' << "BRIGHTNESS: " 
			<< labels.at(i).brightness << '\n' << "NUMBERPLATECOUNT: " << labels.at(i).numberplateCount<< '\n';
		std::cout << "CHARACTERCOUNT: " << std::endl;
		for (int j = 0; j < labels.at(i).characterCount.size(); j++) {
			std::cout << labels.at(i).characterCount.at(j) << '\n';
		}
		std::cout << "COORDINATES OF LICENSE PLATES ON PICTURE : " << std::endl;
		for (int z = 0; z < labels.at(i).coordinates.size(); z++) {
			std::cout << "X : " << labels.at(i).coordinates.at(z).x << " --- " << " Y: " << labels.at(i).coordinates.at(z).y << std::endl;
		}
	}
}

void Metric::printMetricOnePicture(int index) {
	std::string falsePositive = "FALSE";
	if (falsePositiveCounterPerPicture > 0) {
		falsePositive = "TRUE";
	}

	table << fort::header
		<< "ID" << "WIDTH" << "HEIGHT" << "BRIGHTNESS" << "LICENSEPLATESONPIC" << "RECOGNIZED" << "FALSEPOSITIVE?" << "FALSEPOSITIVECOUNTER" << fort::endr
		<< labels.at(index).id << labels.at(index).width << labels.at(index).heigth << labels.at(index).brightness << labels.at(index).numberplateCount << recognizedPerPicture << falsePositive << falsePositiveCounterPerPicture << fort::endr;

	for (int i = 0; i < recognizedCharactersPerLicenseplate.at(index).size(); i++) {
		table << fort::header
			<< "Recognized Characters " << recognizedCharactersPerLicenseplate.at(index).at(i);
	}
	table << fort::endr;

	for (int i = 0; i < labels.at(index).characterCount.size(); i++) {
		table << fort::header
			<< "Characters on license plate " + std::to_string(i+1)
			<< labels.at(index).characterCount.at(i);
	}
	table << fort::endr;

	if (index == (NUMBER_OF_IMAGES - 1)) {
		std::cout << table.to_string() << std::endl;
	}
}

void Metric::printRecognizedOutOfAll() {
	std::cout << "Recognized " << recognizedLicensePlates << " of " << numberOfLicensePlatesOnAllPictures << " possible license plates on all pictures (" << ((float)recognizedLicensePlates / (float)numberOfLicensePlatesOnAllPictures) * 100.0f << " %)" << std::endl;
	std::cout << "Recognized " << recognizedCharacters << " of " << totalAmountOfCharacters << " possible characters on license plates (" << ((float)recognizedCharacters / (float)totalAmountOfCharacters ) * 100.0f << " %)" << std::endl;
}

void Metric::setTotalAmountLicensePlatesOnPictures() {
	for (int i = 0; i < labels.size(); i++) {
		numberOfLicensePlatesOnAllPictures += labels.at(i).numberplateCount;
	}
}

void Metric::setTotalAmountOfCharactersOnPictures() {
	for (int i = 0; i < labels.size(); i++) {
		for (int j = 0; j < labels.at(i).characterCount.size(); j++) {
			totalAmountOfCharacters += labels.at(i).characterCount.at(j);
		}
	}
}
