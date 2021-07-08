#include "hzpch.h"
#include "InnoDetection.h"

namespace ASSInterface {
	InnoDetection::InnoDetection(int channel)
	{
		indexChannel = channel;
		errorIFace = ASSInterface::ErrorRecognition::CreateIFace();
		transImage = ASSInterface::TransformImage::Create();
		GetParamsDetectFace();
	}
	InnoDetection::InnoDetection()
	{		
		errorIFace = ASSInterface::ErrorRecognition::CreateIFace();
	}
	InnoDetection::~InnoDetection()
	{
	}
	void InnoDetection::BuildTemplatesFromPersonFiles(std::vector<std::string> files)
	{
		if (!isSetParameters)
		{
			SetParameters();
		}
		LoadBufferImages(files);
		if (!bufferOfImages.empty())
		{			
			BuildSpecificationForIdentify(ENROLL_WITH_TEMPLATES);
		}
	}

	void InnoDetection::BuildTemplatesFromPersonFiles(std::vector<std::string> files, 
		int task, int id, int index)
	{
		if (!isSetParameters)
		{
			SetParameters();
		}
		LoadBufferImages(files);
		if (!bufferOfImages.empty())
		{
			BuildSpecificationForIdentify(task, id, index);
		}
	}

	void InnoDetection::Import(std::vector<std::string> files)
	{
		if (!isSetParameters)
		{
			SetParameters();
		}

		importDetect.clear();

		std::random_device rd;
		std::mt19937 g(rd());

		std::shuffle(files.begin(), files.end(), g);

		ParallelApply(files, files.size());
				
		shootSpecConcurrentDetected.on_next(importDetect);

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], quantity: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0),
			ASSInterface::DateTime::Now().c_str(), indexChannel, (int)importDetect.size(),
			"Upload for identification");

	}
	void InnoDetection::ResetParameters()
	{
		GetParamsDetectFace();
		isSetParameters = false;
	}

	float InnoDetection::ICAOFeature(void* face) {
		
			//Default
			/*Param EyeGaze : EYE_GAZE: < -400; 10000 > && EYE_STATUS_R && EYE_STATUS_L && FACE_CONFIDENCE
			Param Mouth : MOUTH_STATUS: < 0; 10000 > && YAW && PITCH && FACE_CONFIDENCE
			Param Pitch : PITCH: < -5500; 4500 > && FACE_CONFIDENCE
			Param Yaw : YAW: < -5100; 5000 > && FACE_CONFIDENCE
			Param Roll : ROLL: < -5000; 5000 > && FACE_CONFIDENCE*/

		bool result = true;
		float median = -1.0f;
		int errorCode = IFACE_OK;
		float scoreEyeGaze, scoreMouth, scorePitch, scoreYaw, scoreRoll;

		errorCode = IFACE_SetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_EYE_GAZE, "EYE_GAZE:<-4000;10000>&&EYE_STATUS_L:<-3000;10000>&&EYE_STATUS_R:<-3000;10000>&&FACE_CONFIDENCE");
		CheckError("InnoDetection::ICAOFeature", errorCode);
		
		errorCode = IFACE_SetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_MOUTH_STATUS, "MOUTH_STATUS:<-4000;10000>&&FACE_CONFIDENCE");
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_SetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_PITCH, "PITCH:<-9000;10000>&&FACE_CONFIDENCE");
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_SetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_YAW,
			"YAW:<-7000;8000>&&FACE_CONFIDENCE");
		CheckError("InnoDetection::ICAOFeature", errorCode);
		
		errorCode = IFACE_SetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_ROLL,
			"ROLL:<-7000;7000>&&FACE_CONFIDENCE");
		CheckError("InnoDetection::ICAOFeature", errorCode);

		IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatusPitch;
		IFACE_FaceAttributeRangeStatus faceAttributeRangeStatusPitch;
		IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatusYaw;
		IFACE_FaceAttributeRangeStatus faceAttributeRangeStatusYaw;
		IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatusRoll;
		IFACE_FaceAttributeRangeStatus faceAttributeRangeStatusRoll;
		IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatusEyeGaze;
		IFACE_FaceAttributeRangeStatus faceAttributeRangeStatusEyeGaze;
		IFACE_FaceAttributeDependenciesStatus faceAttributeDependenciesStatusMouth;
		IFACE_FaceAttributeRangeStatus faceAttributeRangeStatusMouth;
		bool icaoComplianceStatusEyeGaze = false;
		bool icaoComplianceStatusMouth = false;
		bool icaoComplianceStatusPitch = false;
		bool icaoComplianceStatusYaw = false;
		bool icaoComplianceStatusRoll = false;

		errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_YAW, &scoreYaw);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeRangeStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_YAW, &faceAttributeRangeStatusYaw);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeDependenciesStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_YAW, &faceAttributeDependenciesStatusYaw);
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_PITCH, &scorePitch);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeRangeStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_PITCH, &faceAttributeRangeStatusPitch);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeDependenciesStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_PITCH, &faceAttributeDependenciesStatusPitch);
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_ROLL, &scoreRoll);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeRangeStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_ROLL, &faceAttributeRangeStatusRoll);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeDependenciesStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_ROLL, &faceAttributeDependenciesStatusRoll);
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_EYE_GAZE, &scoreEyeGaze);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeRangeStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_EYE_GAZE, &faceAttributeRangeStatusEyeGaze);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeDependenciesStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_EYE_GAZE, &faceAttributeDependenciesStatusEyeGaze);
		CheckError("InnoDetection::ICAOFeature", errorCode);

		errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_MOUTH_STATUS, &scoreMouth);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeRangeStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_MOUTH_STATUS, &faceAttributeRangeStatusMouth);
		CheckError("InnoDetection::ICAOFeature", errorCode);
		errorCode = IFACE_GetFaceAttributeDependenciesStatus(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_MOUTH_STATUS, &faceAttributeDependenciesStatusMouth);
		CheckError("InnoDetection::ICAOFeature", errorCode);

				
		if (faceAttributeRangeStatusEyeGaze == IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE &&
			faceAttributeDependenciesStatusEyeGaze == IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK)
		{
			icaoComplianceStatusEyeGaze = true;
		}

		if (faceAttributeRangeStatusMouth == IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE &&
			faceAttributeDependenciesStatusMouth == IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK)
		{
			icaoComplianceStatusMouth = true;
		}

		if (faceAttributeRangeStatusYaw == IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE &&
			faceAttributeDependenciesStatusYaw == IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK)
		{
			icaoComplianceStatusYaw = true;
		}

		if (faceAttributeRangeStatusPitch == IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE &&
			faceAttributeDependenciesStatusPitch == IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK)
		{
			icaoComplianceStatusPitch = true;
		}

		if (faceAttributeRangeStatusRoll == IFACE_FACE_ATTRIBUTE_RANGE_STATUS_IN_RANGE &&
			faceAttributeDependenciesStatusRoll == IFACE_FACE_ATTRIBUTE_DEPENDENCIES_STATUS_OK)
		{
			icaoComplianceStatusRoll = true;
		}

		const unsigned int maxParamSize = 1000;
		char parameterValueEyeGaze[maxParamSize];
		char parameterValueMouth[maxParamSize];
		char parameterValuePitch[maxParamSize];
		char parameterValueYaw[maxParamSize];
		char parameterValueRoll[maxParamSize];
		
		errorCode = IFACE_GetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_EYE_GAZE, 
			parameterValueEyeGaze, maxParamSize);
		errorCode = IFACE_GetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_MOUTH_STATUS,
			parameterValueMouth, maxParamSize);
		errorCode = IFACE_GetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_PITCH,
			parameterValuePitch, maxParamSize);
		errorCode = IFACE_GetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_YAW,
			parameterValueYaw, maxParamSize);
		errorCode = IFACE_GetParam(faceHandler, IFACE_PARAMETER_FACE_ATTRIBUTE_COND_ROLL,
			parameterValueRoll, maxParamSize);

		std::string depEyeGaze = FaceAttributeDependenciesStatusToString(faceAttributeDependenciesStatusEyeGaze);
		std::string depMouth = FaceAttributeDependenciesStatusToString(faceAttributeDependenciesStatusMouth);
		std::string depPitch = FaceAttributeDependenciesStatusToString(faceAttributeDependenciesStatusPitch);
		std::string depYaw = FaceAttributeDependenciesStatusToString(faceAttributeDependenciesStatusYaw);
		std::string depRoll = FaceAttributeDependenciesStatusToString(faceAttributeDependenciesStatusRoll);


		std::size_t found = depEyeGaze.find("not fulfilled");
		if (found != std::string::npos) depEyeGaze = "Not OK";
		else depEyeGaze = "OK";
		
		found = depMouth.find("not fulfilled");
		if (found != std::string::npos) depMouth = "Not OK";
		else depMouth = "OK";

		found = depPitch.find("not fulfilled");
		if (found != std::string::npos) depPitch = "Not OK";
		else depPitch = "OK";

		found = depYaw.find("not fulfilled");
		if (found != std::string::npos) depYaw = "Not OK";
		else depYaw = "OK";

		found = depRoll.find("not fulfilled");
		if (found != std::string::npos) depRoll = "Not OK";
		else depRoll = "OK";

		std::string rangeEyeGaze = FaceAttributeRangeStatusToString(faceAttributeRangeStatusEyeGaze);
		std::string rangeMouth = FaceAttributeRangeStatusToString(faceAttributeRangeStatusMouth);
		std::string rangePitch = FaceAttributeRangeStatusToString(faceAttributeRangeStatusPitch);
		std::string rangeYaw = FaceAttributeRangeStatusToString(faceAttributeRangeStatusYaw);
		std::string rangeRoll = FaceAttributeRangeStatusToString(faceAttributeRangeStatusRoll);


		found = rangeEyeGaze.find("not compliant");
		if (found != std::string::npos) {
			std::size_t foundBad = rangeEyeGaze.find("(");
			rangeEyeGaze = rangeEyeGaze.substr(foundBad);
			
		}
		else
		{
			rangeEyeGaze = "In range";
		}

		found = rangeMouth.find("not compliant");
		if (found != std::string::npos) {
			std::size_t foundBad = rangeMouth.find("(");
			rangeMouth = rangeMouth.substr(foundBad);

		}
		else
		{
			rangeMouth = "In range";
		}


		found = rangePitch.find("not compliant");
		if (found != std::string::npos) {
			std::size_t foundBad = rangePitch.find("(");
			rangePitch = rangePitch.substr(foundBad);

		}
		else
		{
			rangePitch = "In range";
		}

		found = rangeYaw.find("not compliant");
		if (found != std::string::npos) {
			std::size_t foundBad = rangeYaw.find("(");
			rangeYaw = rangeYaw.substr(foundBad);

		}
		else
		{
			rangeYaw = "In range";
		}

		found = rangeRoll.find("not compliant");
		if (found != std::string::npos) {
			std::size_t foundBad = rangeRoll.find("(");
			rangeRoll = rangeRoll.substr(foundBad);

		}
		else
		{
			rangeRoll = "In range";
		}


		std::string complianceEyeGaze = IcaoFinalComplianceToString(icaoComplianceStatusEyeGaze);
		std::string complianceMouth = IcaoFinalComplianceToString(icaoComplianceStatusMouth);
		std::string compliancePitch = IcaoFinalComplianceToString(icaoComplianceStatusPitch);
		std::string complianceYaw = IcaoFinalComplianceToString(icaoComplianceStatusYaw);
		std::string complianceRoll = IcaoFinalComplianceToString(icaoComplianceStatusRoll);

		
		found = complianceEyeGaze.find("not compliant");
		if (found != std::string::npos) {
			complianceEyeGaze = "Not-valid";
			result = false;
		}
		else complianceEyeGaze = "Valid";

		found = complianceMouth.find("not compliant");
		if (found != std::string::npos) {
			complianceMouth = "Not-valid";
			result = false;
		}
		else complianceMouth = "Valid";

		found = compliancePitch.find("not compliant");
		if (found != std::string::npos) {
			compliancePitch = "Not-valid";
			result = false;
		}
		else compliancePitch = "Valid";

		found = complianceYaw.find("not compliant");
		if (found != std::string::npos) {
			complianceYaw = "Not-valid";
			result = false;
		}
		else complianceYaw = "Valid";

		found = complianceRoll.find("not compliant");
		if (found != std::string::npos) {
			complianceRoll = "Not-valid";
			result = false;
		}
		else complianceRoll = "Valid";


		ASS_INFO("--------------------------------------------------------------------------");
		ASS_INFO("Param EyeGaze : {0}", parameterValueEyeGaze);
		ASS_INFO("Param Mouth : {0}", parameterValueMouth);
		ASS_INFO("Param Pitch : {0}", parameterValuePitch);
		ASS_INFO("Param Yaw : {0}", parameterValueYaw);
		ASS_INFO("Param Roll : {0}", parameterValueRoll);
		ASS_INFO("--------------------------------------------------------------------------");

		ASS_INFO("Feature   Score/value         Dependencies    Range compliance   |  ICAO Status");
		ASS_INFO("--------------------------------------------------------------------------------------------------");
		ASS_INFO("EYE_GAZE  {0}                 {1}             {2}                |  {3}", 
			scoreEyeGaze, depEyeGaze, rangeEyeGaze, complianceEyeGaze);
		ASS_INFO("MOUTH     {0}                 {1}             {2}                |  {3}",
			scoreMouth, depMouth, rangeMouth, complianceMouth);
		ASS_INFO("PITCH     {0}                 {1}             {2}                |  {3}",
			scorePitch, depPitch, rangePitch, compliancePitch);
		ASS_INFO("YAW     {0}                 {1}             {2}                |  {3}",
			scoreYaw, depYaw, rangeYaw, complianceYaw);
		ASS_INFO("ROLL     {0}                 {1}             {2}                |  {3}",
			scoreRoll, depRoll, rangeRoll, complianceRoll);

		if (result)
		{
			std::vector<float> icaoScore;
			icaoScore.push_back(scoreEyeGaze);
			icaoScore.push_back(scoreMouth);
			icaoScore.push_back(scorePitch);
			icaoScore.push_back(scoreYaw);
			icaoScore.push_back(scoreRoll);

			std::sort(icaoScore.rbegin(), icaoScore.rend());
			median = icaoScore[2];
		}

		return median;

	}

	std::vector<void*> InnoDetection::Detect(std::vector<float>& icaoScore)
	{
		int errorCode, numberFaces = maxDetected;
		std::vector<void*> facesDetected;

		for (int i = 0; i < bufferOfImages.size(); i++) {
			void** faceTemp = new void* [maxDetected];
			
			for (int j = 0; j < maxDetected; j++) {
				errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faceTemp[j]));
				CheckError("InnoDetection::Detect", errorCode);
			}

			std::vector<unsigned char> buffer = bufferOfImages[i];

			errorCode = IFACE_DetectFaces(&buffer[0],
				dimensionsImages[i][0], dimensionsImages[i][1],
				minFace, maxFace, faceHandler, &numberFaces, faceTemp);
			CheckError("InnoDetection::Detect", errorCode);

			buffer.clear();

			if (numberFaces != EMPTY_FACE)
			{
				for (int j = 0; j < numberFaces; j++) {

					void* face = faceTemp[j];

					//float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
					//float faceConfidence;

					//errorCode = IFACE_GetFaceBasicInfo(face, faceHandler,
					//	&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
					//CheckError("InnoDetection::Detect", errorCode);														

					//// get eye distances
					//float eyeDist;
					//errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_EYE_DISTANCE, &eyeDist);
					//CheckError("InnoDetection::Detect", errorCode);

					
					float icao = ICAOFeature(face);
					if (icao != -1.0f)
					{
						void* faceClone = nullptr;

						errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &faceClone);
						CheckError("InnoDetection::Detect", errorCode);
						errorCode = IFACE_CloneEntity(face, faceClone);
						CheckError("InnoDetection::Detect", errorCode);
						if (errorCode == IFACE_OK)
						{
							facesDetected.push_back(faceClone);
							icaoScore.push_back(icao);
						}
						
					}
					else
					{						
						ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], ICAO_Feature: [%s], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
							ASSInterface::DateTime::Now().c_str(), indexChannel, "False", "Rejected");
					}
					
				}

			}
			for (int j = 0; j < maxDetected; j++) {
				errorCode = IFACE_ReleaseEntity(faceTemp[j]);
				CheckError("InnoDetection::Detect", errorCode);
			}

			delete[] faceTemp;
		}

		return facesDetected;
	}
	void InnoDetection::Crop(void* face, CropSpecification& specCrop)
	{
		int cropWidth = -1, cropHeight = -1, cropLength = -1, errorCode = -1;

		switch (faceCrop)
		{
		case 0:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 1:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 2:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 3:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 4:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		default:
			break;
		}

		unsigned char* cropImageData = new unsigned char[cropLength];

		switch (faceCrop)
		{
		case 0:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 1:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 2:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 3:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		case 4:
			errorCode = IFACE_GetFaceCropImage(face, faceHandler,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoDetection::Crop", errorCode);
			break;
		default:
			break;
		}

		if (cropImageData != NULL)
		{
			int sizeImage;
			errorCode = IFACE_SaveImageToMemory(cropImageData, cropWidth, cropHeight,
				3, IFACE_IMAGE_SAVE_TYPE_JPG, &sizeImage, NULL);
			CheckError("InnoDetection::Crop", errorCode);
			unsigned char* cropImageFormat = new unsigned char[sizeImage];
			errorCode = IFACE_SaveImageToMemory(cropImageData, cropWidth, cropHeight,
				3, IFACE_IMAGE_SAVE_TYPE_JPG, &sizeImage, cropImageFormat);
			CheckError("InnoDetection::Crop", errorCode);

			//IFACE_SaveImage("ino.jpg", cropWidth, cropHeight, 3, cropImageData);

			if (errorCode == IFACE_OK) {
				specCrop.length = sizeImage;
				specCrop.width = cropWidth;
				specCrop.height = cropHeight;
				specCrop.cropData.assign(cropImageFormat, cropImageFormat + sizeImage);

			}

			delete[] cropImageFormat;
		}

		delete[] cropImageData;
	}
	void InnoDetection::SetParameters()
	{
		int errorCode;

		errorCode = IFACE_CreateFaceHandler(&faceHandler);
		CheckError("InnoDetection::SetParameters", errorCode);

		switch (speedExtraction)
		{
		case 0:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
				IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_DEFAULT);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 1:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
				IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_BALANCED);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 2:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
				IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 3:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
				IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		default:
			break;
		}

		switch (speedDetect)
		{
		case 0:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
				IFACE_FACEDET_SPEED_ACCURACY_MODE_DEFAULT);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 1:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
				IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 2:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
				IFACE_FACEDET_SPEED_ACCURACY_MODE_ACCURATE);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		case 3:
		{
			errorCode = IFACE_SetParam(faceHandler,
				IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
				IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
			CheckError("InnoDetection::SetParameters", errorCode);
			break;
		}
		default:
			break;
		}

		errorCode = IFACE_SetParam(faceHandler,
			IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
			std::to_string(confidenceThreshold).c_str());
		CheckError("InnoDetection::SetParameters", errorCode);

		isSetParameters = true;
	}
	void InnoDetection::ParallelApply(std::vector<std::string> files, size_t n)
	{		
		parallel_for(blocked_range<size_t>(0, n),
			[=](const blocked_range<size_t>& r) {
				for (size_t i = r.begin(); i != r.end(); ++i)
					BuildTemplateOfFile(files[i]);
			}
		);
	}
	void InnoDetection::GetParamsDetectFace()
	{
		std::string nameFile = "face" + std::to_string(indexChannel) + ".txt";
		configurationFace = ASSInterface::Configuration::CreateConfigFace();
		configurationFace->SetPath(nameFile);
		configurationFace->ParseToObject();

		std::any anyMaxDetect = configurationFace->GetParam("FACE_MAX_DETECT");
		std::any anyMinFace = configurationFace->GetParam("TRACK_MIN_FACE_SIZE");
		std::any anyMaxFace = configurationFace->GetParam("TRACK_MAX_FACE_SIZE");
		std::any anyConfidence = configurationFace->GetParam("FACEDET_CONFIDENCE_THRESHOLD");
		std::any anySpeedDetect = configurationFace->GetParam("FACEDET_SPEED_ACCURACY_MODE");
		std::any anySpeedTemplate = configurationFace->GetParam("FACETMPLEXT_SPEED_ACCURACY_MODE");
		std::any anyCrop = configurationFace->GetParam("IFACE_CROP_IMAGE");
		std::any anyModel = configurationFace->GetParam("QUALITY_MODEL");		

		maxDetected = std::any_cast<int>(anyMaxDetect);		
		minFace = std::any_cast<float>(anyMinFace);
		maxFace = std::any_cast<float>(anyMaxFace);
		confidenceThreshold = std::any_cast<int>(anyConfidence);
		speedDetect = std::any_cast<int>(anySpeedDetect);
		speedExtraction = std::any_cast<int>(anySpeedTemplate);
		faceCrop = std::any_cast<int>(anyCrop);
		qualityModel = std::any_cast<int>(anyModel);
		
	}
	void InnoDetection::CheckError(std::string function, int errorCode)
	{
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE(function.c_str(), errorIFace->GetError(errorCode).c_str());
		}
	}
	unsigned char* InnoDetection::LoadFileImage(std::string fileImage, 
		int* width, int* height, int* length)
	{
		int errorCode = IFACE_LoadImage(fileImage.c_str(), width, height, length, NULL);
		CheckError("InnoDetection::LoadFileImage", errorCode);
		if (errorCode != IFACE_OK)
		{			
			return NULL;
		}

		unsigned char* imageData = new unsigned char[*length];
		errorCode = IFACE_LoadImage(fileImage.c_str(), width, height, length, imageData);
		CheckError("InnoDetection::LoadFileImage", errorCode);
		if (errorCode != IFACE_OK)
		{			
			return NULL;
		}

		return imageData;		
	}
	void InnoDetection::LoadBufferImages(std::vector<std::string> files)
	{
		int lenght = 0, width = 0, height = 0, channels = 0, count = 0;

		for (int i = 0; i < files.size(); i++)
		{
			unsigned char* rawImage = NULL;

			if (transImage->GetSize(files[i].c_str(), &width, &height)) {

				if (width <= MAX_SIZE_IMAGE) {
					rawImage = LoadFileImage(files[i], &width, &height, &lenght);
					
				}
				else
				{				
					unsigned char* tempImage = LoadFileImage(files[i], &width, &height, &lenght);
					
					rawImage = transImage->Resize(tempImage, &width, &height, &lenght);
															
					//IFACE_SaveImage("ino1.jpg", width, height, 3, rawImage);
					
				}	

				if (rawImage != NULL && count < BATCH_TOTAL_SIZE) {

					std::vector<unsigned char> bufferImage;
					bufferImage.assign(rawImage, rawImage + lenght);
					bufferOfImages.push_back(bufferImage);
					std::vector<int> sizeImage = { width, height, lenght };
					dimensionsImages.push_back(sizeImage);
					count++;
					delete[] rawImage;

					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], file name: [%s], width: [%03d], height: [%03d], result: [%s]\n",
						ASSInterface::ImGuiLog::Categories(0),
						ASSInterface::DateTime::Now().c_str(), indexChannel, files[i].c_str(),
						width, height, "Load");
				}
				else
				{
					ASS_ERROR_PROFILE_SCOPE("InnoDetection::LoadBufferImages",
						"File corrupt.");
				}
			}					

		}
	}
	unsigned char* InnoDetection::LoadImageMemory(std::vector<unsigned char> buffer, 
		int* width, int* height, int* lenght)
	{
		int errorCode;
		const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
		if (imgData == NULL) {
			return NULL;
		}

		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
			height, lenght, NULL);
		CheckError("InnoDetection::LoadImageMemory", errorCode);
		if (errorCode != IFACE_OK) {			
			return NULL;
		}
		unsigned char* flowImage = new unsigned char[*lenght];
		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
			height, lenght, flowImage);
		CheckError("InnoDetection::LoadImageMemory", errorCode);

		if (errorCode != IFACE_OK) {			
			return NULL;
		}

		return flowImage;
	}
	std::vector<DetectSpecification> InnoDetection::GetBatchTemplates(std::vector<void*> faces)
	{
		int errorCode = 0, templateSize = 0;		
		int facesDetected = (int)faces.size();
		char** batchTemplates = new char* [facesDetected];
		std::vector<DetectSpecification> templateDetection;

		errorCode = IFACE_CreateTemplateBatch(faceHandler,
			facesDetected, faces.data(),
			&templateSize, NULL);
		CheckError("InnoDetection::GetBatchTemplates", errorCode);

		if (errorCode == IFACE_OK)
		{
			for (int i = 0; i < facesDetected; ++i)
			{
				batchTemplates[i] = new char[templateSize];
			}

			errorCode = IFACE_CreateTemplateBatch(faceHandler,
				facesDetected, faces.data(),
				&templateSize, batchTemplates);
			CheckError("InnoDetection::GetBatchTemplates", errorCode);
			if (errorCode == IFACE_OK) {
				
				for (int i = 0; i < facesDetected; ++i)
				{
					DetectSpecification specDetect;

					specDetect.sizeTemplate = templateSize;					
					specDetect.templateData.assign(batchTemplates[i], 
						batchTemplates[i] + templateSize);
					templateDetection.push_back(specDetect);
				}
			}
			
		}
		
		for (int i = 0; i < facesDetected; ++i)
		{
			delete[] batchTemplates[i];
		}

		delete[] batchTemplates;

		return templateDetection;
	}
	std::vector<std::vector<int>> InnoDetection::ValidateQuality(
		std::vector<DetectSpecification>& detectTemplates)
	{
		int errorCode = 0, majorVersion = 0, 
			minorVersion = 0, quality = 0;
		std::vector<std::vector<int>> validatedTemplates;

		for (int i = 0; i < detectTemplates.size(); i++) {

			errorCode = IFACE_GetTemplateInfo(faceHandler,
				&detectTemplates[i].templateData[0], &majorVersion, 
				&minorVersion, &quality);
			
			if (quality > qualityModel) {
				detectTemplates[i].quality = quality;
				std::vector<int> orderQuality = { quality, i };
				validatedTemplates.push_back(orderQuality);
			}
			else
			{
				ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_quality: [%03d], quality: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
					ASSInterface::DateTime::Now().c_str(), indexChannel, qualityModel, quality, "Rejected");
			}
		}

		if (!validatedTemplates.empty())
		{
			std::sort(validatedTemplates.rbegin(), validatedTemplates.rend());
		}

		return validatedTemplates;
	}

	std::vector<CropSpecification> InnoDetection::CropValidateTemplates(
		std::vector<void*> faces, std::vector<std::vector<int>> validate)
	{
		std::vector<CropSpecification> cropValid;
		for (int i = 0; i < validate.size(); i++)
		{
			CropSpecification specCrop;			
			int index = validate[i][1];
			Crop(faces[index], specCrop);
			cropValid.push_back(specCrop);
		}
		return cropValid;
	}

	std::vector<char> InnoDetection::SerializeEntity(std::vector<void*> faces, int index)
	{
		int errorCode = IFACE_OK, serializedEntitySize = 0;
		std::vector<char> entSerialize;

		errorCode = IFACE_SerializeEntity(faces[index], NULL, &serializedEntitySize);
		CheckError("InnoDetection::SerializeEntity", errorCode);
		char* serializedEntity = new char[serializedEntitySize];
		errorCode = IFACE_SerializeEntity(faces[index], serializedEntity, &serializedEntitySize);
		CheckError("InnoDetection::SerializeEntity", errorCode);
		if (errorCode == IFACE_OK)
		{
			entSerialize.assign(serializedEntity, serializedEntity + serializedEntitySize);
		}

		return entSerialize;
	}

	std::vector<char> InnoDetection::SerializeEntity(void* face)
	{
		int errorCode = IFACE_OK, serializedEntitySize = 0;
		std::vector<char> entSerialize;

		errorCode = IFACE_SerializeEntity(face, NULL, &serializedEntitySize);
		CheckError("InnoDetection::SerializeEntity", errorCode);
		char* serializedEntity = new char[serializedEntitySize];
		errorCode = IFACE_SerializeEntity(face, serializedEntity, &serializedEntitySize);
		CheckError("InnoDetection::SerializeEntity", errorCode);
		if (errorCode == IFACE_OK)
		{
			entSerialize.assign(serializedEntity, serializedEntity + serializedEntitySize);
		}

		return entSerialize;
	}

	void InnoDetection::SendValidDetection(std::vector<DetectSpecification> templatesGet, 
		std::vector<std::vector<int>> valid, 
		std::vector<CropSpecification> cropValid, int task, 
		std::vector<char> faceSerialized, std::vector<float> icaoScore, int id, int idx)
	{
		std::vector<DetectSpecification> detectValid;
		for (int i = 0; i < valid.size(); i++)
		{
			DetectSpecification specDetected;
			int index = valid[i][1];
			specDetected.quality = templatesGet[index].quality;
			specDetected.sizeTemplate = templatesGet[index].sizeTemplate;
			specDetected.task = task;
			specDetected.id = id;
			specDetected.indexFace = idx;
			specDetected.medianICAO = icaoScore[index];
			specDetected.templateData.assign(&templatesGet[index].templateData[0],
				&templatesGet[index].templateData[0] + templatesGet[index].templateData.size());
			specDetected.cropSpec.height = cropValid[i].height;
			specDetected.cropSpec.width = cropValid[i].width;
			specDetected.cropSpec.length = cropValid[i].length;
			specDetected.cropSpec.cropData.assign(&cropValid[i].cropData[0],
				&cropValid[i].cropData[0] + cropValid[i].cropData.size());
			if (i == 0 && !faceSerialized.empty() && task != SET_TEMPLATE_OF_FILES)
			{
				specDetected.faceSerialized.assign(&faceSerialized[0],
					&faceSerialized[0] + faceSerialized.size());
			}



			detectValid.push_back(specDetected);
		}

		ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], quantity: [%02d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(0),
			ASSInterface::DateTime::Now().c_str(), indexChannel, (int)valid.size(), 
			"Upload for identification");

		if (!detectValid.empty())
		{
			shootSpecDetected.on_next(detectValid);
			detectValid.clear();
		}
		
	}

	void InnoDetection::BuildSpecificationForIdentify(int task, int id, int index)
	{
		std::vector<float> icaoScore;
		std::vector<void*> faces = Detect(icaoScore);

		if (faces.size() > 0)
		{
			std::vector<DetectSpecification> detectTemplates = GetBatchTemplates(faces);
			if (!detectTemplates.empty())
			{
				std::vector<std::vector<int>> validateTemplates = ValidateQuality(
					detectTemplates);
				if (!validateTemplates.empty())
				{
					std::vector<CropSpecification> cropValid = CropValidateTemplates(
						faces, validateTemplates);

					std::vector<char> faceSerialized = SerializeEntity(faces, validateTemplates[0][1]);

					SendValidDetection(detectTemplates, validateTemplates, 
						cropValid, task, faceSerialized, icaoScore, id, index);

					cropValid.clear();
					faceSerialized.clear();
				}

				validateTemplates.clear();
			}
			detectTemplates.clear();
			for (int j = 0; j < faces.size(); j++) {
				int errorCode = IFACE_ReleaseEntity(faces[j]);
				CheckError("InnoDetection::BuildSpecificationForIdentify", errorCode);
			}
			faces.clear();
		}
		icaoScore.clear();
		bufferOfImages.clear();
		dimensionsImages.clear();
	}
	void InnoDetection::BuildTemplateOfFile(std::string file)
	{
		int errorCode = 0, lenght = 0, width = 0, height = 0, numberFaces = maxDetected;
		unsigned char* rawImage = NULL;

		if (transImage->GetSize(file.c_str(), &width, &height)) {

			if (width <= MAX_SIZE_IMAGE) {
				rawImage = LoadFileImage(file, &width, &height, &lenght);

			}
			else
			{
				unsigned char* tempImage = LoadFileImage(file, &width, &height, &lenght);
				rawImage = transImage->Resize(tempImage, &width, &height, &lenght);
				
			}
			
			if (rawImage != NULL) {
				void** faceTemp = new void* [maxDetected];

				for (int j = 0; j < maxDetected; j++) {
					errorCode = IFACE_CreateEntity(IFACE_ENTITY_TYPE_FACE, &(faceTemp[j]));
					CheckError("InnoDetection::BuildTemplateOfFile", errorCode);
				}

				errorCode = IFACE_DetectFaces(rawImage,
					width, height, minFace, maxFace, faceHandler, &numberFaces, faceTemp);
				CheckError("InnoDetection::BuildTemplateOfFile", errorCode);

				if (numberFaces != EMPTY_FACE) {
					for (int j = 0; j < numberFaces; j++) {

						void* face = faceTemp[j];

						//float rightEyeX, rightEyeY, leftEyeX, leftEyeY;
						//float faceConfidence;

						//errorCode = IFACE_GetFaceBasicInfo(face, faceHandler,
						//	&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceConfidence);
						//CheckError("InnoDetection::BuildTemplateOfFile", errorCode);

						//// get eye distances
						//float eyeDist;
						//errorCode = IFACE_GetFaceAttribute(face, faceHandler, IFACE_FACE_ATTRIBUTE_ID_EYE_DISTANCE, &eyeDist);
						//CheckError("InnoDetection::BuildTemplateOfFile", errorCode);

						float icao = ICAOFeature(face);
						
						if (icao != -1.0f)
						{
							int templateSize;
							errorCode = IFACE_CreateTemplate(face, faceHandler, 0, &templateSize, NULL);
							CheckError("InnoDetection::BuildTemplateOfFile", errorCode);
							if (errorCode == IFACE_OK) {
								char* templateData = new char[templateSize];
								errorCode = IFACE_CreateTemplate(face, faceHandler, 0,
									&templateSize, templateData);
								CheckError("InnoDetection::BuildTemplateOfFile", errorCode);
								if (errorCode == IFACE_OK) {
									int  majorVersion, minorVersion, quality;
									errorCode = IFACE_GetTemplateInfo(faceHandler,
										templateData, &majorVersion, &minorVersion, &quality);

									if (quality > qualityModel) {

										CropSpecification specCrop;
										Crop(face, specCrop);

										std::vector<char> faceSerialized = SerializeEntity(face);

										DetectSpecification specDetected;
										specDetected.quality = quality;
										specDetected.sizeTemplate = templateSize;
										specDetected.task = IMPORT_FACE;
										specDetected.medianICAO = icao;
										specDetected.templateData.assign(templateData,
											templateData + templateSize);
										specDetected.cropSpec.height = specCrop.height;
										specDetected.cropSpec.width = specCrop.width;
										specDetected.cropSpec.length = specCrop.length;
										specDetected.cropSpec.cropData.assign(
											&specCrop.cropData[0],
											&specCrop.cropData[0] + specCrop.cropData.size());

										if (!faceSerialized.empty())
										{
											specDetected.faceSerialized.assign(&faceSerialized[0],
												&faceSerialized[0] + faceSerialized.size());
										}

										importDetect.push_back(specDetected);

										faceSerialized.clear();
									}
									else
									{
										ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_quality: [%03d], quality: [%03d], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1), ASSInterface::DateTime::Now().c_str(), indexChannel, qualityModel, quality, "Rejected");
									}

								}
								delete[] templateData;
							}
						}
						else
						{
							ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], ICAO_Feature: [%s], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
								ASSInterface::DateTime::Now().c_str(), indexChannel, "False", "Rejected");
						}

					}
				}
				else
				{
					ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], file name: [%s], width: [%03d], height: [%03d], result: [%s]\n",
						ASSInterface::ImGuiLog::Categories(1),
						ASSInterface::DateTime::Now().c_str(), indexChannel, file.c_str(),
						width, height, "Not face.");
				}

				for (int j = 0; j < maxDetected; j++) {
					errorCode = IFACE_ReleaseEntity(faceTemp[j]);
					CheckError("InnoDetection::Detect", errorCode);
				}

				delete[] faceTemp;
			}
			

			delete[] rawImage;
		}
		
	}

}