#pragma once
#include <ASSInterface.h>

struct PersonSpecification {
	char name[32] = "";
	char lastname[32] = "";
	char document[32] = "";
	char date[32] = "";
	char idtemp[15] = "";
	char id[15] = "";
	char match[4] = "";
	char state[20] = "";
	char place[30] = "";
	char channel[2] = "";
	char type[20] = "";
	char type1[20] = "";
	uint32_t txtCapture = 0;
	uint32_t txtGallery = 0;
	uint32_t txtDocumentObverse = 0;
	uint32_t txtDocumentReverse = 0;
	std::vector<unsigned char> bufferCapture;	
	std::vector<unsigned char> bufferGallery;
	std::vector<unsigned char> bufferDocumentObverse;
	std::vector<unsigned char> bufferDocumentReverse;
	std::vector<char> templateFace;
	std::vector<char> faceSerialized;
	int size = 0;
	int cropWidth = 0;
	int cropHeight = 0;	
	int task = -1;
	int templates = 0;
};