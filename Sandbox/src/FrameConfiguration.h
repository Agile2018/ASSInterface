#pragma once

#include <ASSInterface.h>
#include "data/ConstantApplication.h"

class FrameConfiguration {
public:
	FrameConfiguration(ASSInterface::LanguageType language);
	~FrameConfiguration();
	void ShowParametersFaceProcessing(bool* p_open);
	void ShowParametersFaceTracking(bool* p_open);
	void ShowParametersEnrollmentProcessing(bool* p_open);
	void ShowParametersControlEntryMatch(bool* p_open);
	void ShowParametersChannel(bool* p_open);
	void ShowParametersBiometric(bool* p_open);
	void ShowParametersOnthefly(bool* p_open);
	void ShowParametersDatabase(bool* p_open);
	void ShowParametersLicence(bool* p_open);
	inline std::string GetNameDatabase() { return std::string(databaseName); }
	inline std::string GetConnectDatabase() { return std::string(connectString); }
	inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) { dbMongo = db; }
	inline void SetMessageStatus(std::string* message) { messageStatus = message; }
private:
	ASSInterface::LanguageType lg;	
	ASSInterface::Ref<ASSInterface::Configuration> configurationDB;
	ASSInterface::Ref<ASSInterface::Configuration> configurationChannel;
	ASSInterface::Ref<ASSInterface::Configuration> configurationGlobals;
	ASSInterface::Ref<ASSInterface::Configuration> configurationFace;
	ASSInterface::Ref<ASSInterface::Configuration> configurationTrack;
	ASSInterface::Ref<ASSInterface::Configuration> configurationEnroll;
	ASSInterface::Ref<ASSInterface::Configuration> configurationEntry;
	ASSInterface::Ref<ASSInterface::Configuration> configurationOnTheFly;

	ASSInterface::Ref<ASSInterface::Database> dbMongo;

	std::string* messageStatus;
	const char* yesOrno[2] = { "No", "Yes" };
	const char* numberChannels[4] = { "1", "2", "3", "4" };
	const char* numberSavedRecords[4] = { "1", "2", "3", "4" };
	const char* numberGallery[4] = { "1", "2", "3", "4" };
	char databaseName[20] = "";
	char connectString[100] = "";
	char channelAddress[80] = "";
	char channelName[50] = "";
	char idLicence[50] = "";
	char codeLicence[50] = "";
	const char* channel = NULL;
	const char* channelNumbers = NULL;
	const char* savedRecords = NULL;
	const char* galleries = NULL;
	int channelType = 0;
	int id = 0, thrNumber = 1, gpu = 0, mode = 0;
	const char* multithreadingMode[3] = { "Max parallel", "Min memory", "Single" };
	const char* cgpu = NULL;
	const char* threadManagement = NULL;
	float maxFace = 0.0f, minFace = 0.0f;
	int confidenceThreshold = 0, confidenceThresholdTrack = 0, qualityModel = 1, 
		maxDetected = 1, multipleFace = 1,
		speedDetect = 1, speedExtraction = 1, faceCrop = 1, detected = 1, extraction = 1, detectForced = 1;
	const char* speedDetectExtractionMode[4] = { "Mode default", "Mode balanced", "Mode accurate", "Mode fast" };
	const char* crop[5] = { "Token frontal", "Full frontal", "Full frontal extended", "Token not frontal", "Token frontal extended" };
	const char* modeDetected[4] = { "Mode fast", "Mode balanced", "Mode accurate", "Mode accurate server" };
	const char* modeExtraction[6] = { "Mode fast", "Mode balanced", "Mode accurate", "Mode accurate server", "Mode accurate wild", "Mode accurate visa" };
	const char* speedDetectDesc = NULL;
	const char* speedExtractionDesc = NULL;
	const char* faceCropDesc = NULL;
	const char* multipleFaceDesc = NULL;
	const char* detectedDesc = NULL;
	const char* extractionDesc = NULL;
	const char* detectForcedDesc = NULL;

	int discoveryFrecuence = 100, timeRedetect = 0, tracking = 0, speedTrack = 0, trackOptimus = 0;
	const char* modeTracking[3] = { "Mode default", "Mode liveness dot", "Mode object tracking" };
	const char* speedTracking[3] = { "Mode balanced", "Mode accurate", "Mode fast" };
	const char* optimizationTracking[4] = { "Disabled", "History long accurate", "History long fast", "History short" };
	bool deepTracking = false;
	const char* trackingDesc = NULL;
	const char* speedTrackDesc = NULL;
	const char* trackOptimusDesc = NULL;

	int detectionThreshold = 0, similarityThreshold = 0, maxTemplates = -1, minScore = 0, maxScore = 0,
		gallery = 0, deduplicate = 0, concatenate = 0, concatenateMode = 0;
	const char* concatenateOptions[2] = { "Auto", "Forced" };
	const char* galleryDesc = NULL;
	const char* deduplicateDesc = NULL;
	const char* concatenateDesc = NULL;
	const char* concatenateModeDesc = NULL;

	int idenSimilarityThreshold = 0, verySimilarityThreshold = 0, bestCadidates = 1, identificationSpeed = 0;
	int numberTemplates = 1, showNumberImages = 1;
	long dateInitLicence, dateEndLicence;
	ConstantApplication constantApplication;
private:
	void SetParametersDatabase();
	void SetParametersChannel();
	void SetParametersGlobals();
	void SetParametersFace();
	void SetParametersTrack();
	void SetParametersEnroll();
	void SetParametersEntry();
	void SetParametersOnTheFly();
	void PutToolTip(std::string description);
	void SaveParametersDatabase();
	void SaveParametersChannel();
	void SaveParameteresGlobals();
	void SaveParametersFace();
	void SaveParametersTrack();
	void SaveParametersEnroll();
	void SaveParametersEntry();
	void SaveParametersOnTheFly();
	void SaveParametersLicence();
	void DropDatabase();
	
};
