#pragma once
#include <ASSInterface.h>

struct PersonSpecification {
	char name[32] = "Name";
	char document[32] = "123456";
	char date[32] = "02102021";
	char id[15] = "1";
	char match[4] = "0%";
	char state[20] = "Autorized";
	char place[10] = "8vo";
	char channel[2] = "1";
	char type[20] = "Visitor";
	ASSInterface::Ref<ASSInterface::Texture2D> txtCapture;
	ASSInterface::Ref<ASSInterface::Texture2D> txtGallery;
	ASSInterface::Ref<ASSInterface::Texture2D> txtDocument;
	std::vector<unsigned char> bufferCapture;
	std::vector<unsigned char> bufferGallery;	
};