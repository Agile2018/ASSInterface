#include "hzpch.h"
#include "InnoTracking.h"

namespace ASSInterface {

	InnoTracking::InnoTracking(int channel)
	{
		indexChannel = channel;
		errorIFace = ASSInterface::ErrorRecognition::CreateIFace();
		GetParametersTracking();
		GetParamsDetectFace();
	}
	InnoTracking::~InnoTracking()
	{
		flowData.clear();
	}
	void InnoTracking::Init()
	{
		int errorCode;

		errorCode = IFACE_CreateFaceHandler(&faceHandlerTracking);
		CheckError("InnoTracking::Init", errorCode);

		switch (speedExtraction)
		{
			case 0:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
					IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_DEFAULT);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 1:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
					IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_BALANCED);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}				
			case 2:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
					IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_ACCURATE);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}				
			case 3:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACETMPLEXT_SPEED_ACCURACY_MODE,
					IFACE_FACETMPLEXT_SPEED_ACCURACY_MODE_FAST);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}				
			default:
				break;
		}

		switch (speedDetect)
		{
			case 0:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
					IFACE_FACEDET_SPEED_ACCURACY_MODE_DEFAULT);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}			
			case 1:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
					IFACE_FACEDET_SPEED_ACCURACY_MODE_BALANCED);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 2:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
					IFACE_FACEDET_SPEED_ACCURACY_MODE_ACCURATE);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 3:
			{
				errorCode = IFACE_SetParam(faceHandlerTracking,
					IFACE_PARAMETER_FACEDET_SPEED_ACCURACY_MODE,
					IFACE_FACEDET_SPEED_ACCURACY_MODE_FAST);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			default:
				break;
		}

		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_FACEDET_CONFIDENCE_THRESHOLD,
			std::to_string(confidenceThresholdTrack).c_str());
		CheckError("InnoTracking::Init", errorCode);

		/*errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_TRACK_MAX_FACE_SIZE,
			std::to_string(maxFace).c_str());
		CheckError("10 InnoTracking::Init", errorCode);

		errorCode = IFACE_SetParam(faceHandlerTracking,
			IFACE_PARAMETER_TRACK_MIN_FACE_SIZE,
			std::to_string(minFace).c_str());
		CheckError("11 InnoTracking::Init", errorCode);*/

		errorCode = IFACE_CreateObjectHandler(&objectHandler, faceHandlerTracking);
		CheckError("InnoTracking::Init", errorCode);

		for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
		{
			errorCode = IFACE_CreateObject(&objects[i]);
			CheckError("InnoTracking::Init", errorCode);
		}

		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_FACE_DISCOVERY_FREQUENCE_MS,
			std::to_string(discoveryFrecuence).c_str());
		CheckError("InnoTracking::Init", errorCode);

		switch (tracking)
		{
			case 0:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_TRACKING_MODE,
					IFACE_TRACK_TRACKING_MODE_DEFAULT);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 1:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_TRACKING_MODE,
					IFACE_TRACK_TRACKING_MODE_LIVENESS_DOT);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 2:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_TRACKING_MODE,
					IFACE_TRACK_TRACKING_MODE_OBJECT_TRACKING);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			default:
				break;
		}

		switch (speedTrack)
		{
			case 0:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
					IFACE_TRACK_SPEED_ACCURACY_MODE_ACCURATE);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 1:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
					IFACE_TRACK_SPEED_ACCURACY_MODE_BALANCED);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 2:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_SPEED_ACCURACY_MODE,
					IFACE_TRACK_SPEED_ACCURACY_MODE_FAST);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			default:
				break;
		}

		switch (trackOptimus)
		{
			case 0:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
					IFACE_TRACK_MOTION_OPTIMIZATION_DISABLED);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 1:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
					IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_ACCURATE);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 2:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
					IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_LONG_FAST);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			case 3:
			{
				errorCode = IFACE_SetParam(objectHandler,
					IFACE_PARAMETER_TRACK_MOTION_OPTIMIZATION,
					IFACE_TRACK_MOTION_OPTIMIZATION_HISTORY_SHORT);
				CheckError("InnoTracking::Init", errorCode);
				break;
			}
			default:
				break;
		}

		errorCode = IFACE_SetParam(objectHandler,
			IFACE_PARAMETER_TRACK_DEEP_TRACK,
			(deepTracking) ? "true": "false");
		CheckError("24 InnoTracking::Init", errorCode);

		flagFinish = false;
		countFrameTracking = 0;
	}
	void InnoTracking::Detect(std::vector<unsigned char> data)
	{
		ASS_PROFILE_FUNCTION();
		
		int width, height, errorCode;
		flagFinish = true;

		unsigned char* rawImageData = LoadImageFromMemory(data, &width, &height);		

		if (rawImageData != NULL) {
			long secuence = countFrameTracking * elapseFrame;

			try
			{
				errorCode = IFACE_TrackObjects(objectHandler, rawImageData,
					width, height, secuence, NUM_TRACKED_OBJECTS, objects);
				CheckError("InnoTracking::Init", errorCode);

				ResetCoordinates();
				TrackObjectState();
				
			}
			catch (const std::exception& ex)
			{
				ASS_ERROR_PROFILE_SCOPE("InnoTracking::Detect", ex.what());
			}
			delete[] rawImageData;
		}
		flagFinish = false;
		data.clear();
		
	}
	float* InnoTracking::GetCoordinates()
	{
		return imageCoordinatesFollowed;
	}
	float* InnoTracking::GetColor()
	{
		return colorRectangle;
	}	
	void InnoTracking::Terminate()
	{
		int errorCode;

		for (int i = 0; i < NUM_TRACKED_OBJECTS; i++)
		{
			try
			{
				if (objects[i] != NULL)
				{
					errorCode = IFACE_ReleaseEntity(objects[i]);
					CheckError("InnoTracking::Terminate", errorCode);
				}

			}
			catch (const std::exception& e)
			{
				ASS_ERROR_PROFILE_SCOPE("InnoTracking::Terminate", e.what());
				
			}

		}

		errorCode = IFACE_ReleaseEntity(objectHandler);
		CheckError("InnoTracking::Terminate", errorCode);

		errorCode = IFACE_ReleaseEntity(faceHandlerTracking);
		CheckError("InnoTracking::Terminate", errorCode);
	}
	void InnoTracking::CreateTemplate(void* face, DetectSpecification& specDetect)
	{
		int errorCode, templateSize;		

		errorCode = IFACE_CreateTemplate(face, faceHandlerTracking, 0, &templateSize, NULL);
		CheckError("InnoTracking::CreateTemplate", errorCode);
		if (errorCode == IFACE_OK) {
			char* templateData = new char[templateSize];
			errorCode = IFACE_CreateTemplate(face, faceHandlerTracking, 0,
				&templateSize, templateData);
			CheckError("InnoTracking::CreateTemplate", errorCode);
			if (errorCode == IFACE_OK) {
				int majorVersion, minorVersion, quality;
				errorCode = IFACE_GetTemplateInfo(faceHandlerTracking,
					templateData, &majorVersion, &minorVersion, &quality);					
				CheckError("InnoTracking::CreateTemplate", errorCode);
				
				if (quality > qualityModel) {
					specDetect.sizeTemplate = templateSize;
					specDetect.quality = quality;
					specDetect.templateData.assign(templateData, templateData + templateSize);				
				}
			}
			
			delete[] templateData;
		}
	}
	void InnoTracking::Crop(void* face, CropSpecification& specCrop)
	{
		int cropWidth = -1, cropHeight = -1, cropLength = -1, errorCode = -1;

		switch (faceCrop)
		{
		case 0:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 1:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 2:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 3:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 4:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		default:
			break;
		}

		unsigned char* cropImageData = new unsigned char[cropLength];
		
		switch (faceCrop)
		{
		case 0:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 1:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 2:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_FULL_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 3:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_NOT_FRONTAL,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		case 4:
			errorCode = IFACE_GetFaceCropImage(face, faceHandlerTracking,
				IFACE_FACE_CROP_METHOD_TOKEN_FRONTAL_EXTENDED,
				&cropWidth, &cropHeight, &cropLength, cropImageData);
			CheckError("InnoTracking::Crop", errorCode);
			break;
		default:
			break;
		}

		if (cropImageData != NULL)
		{
			int sizeImage;
			errorCode = IFACE_SaveImageToMemory(cropImageData, cropWidth, cropHeight,
				3, IFACE_IMAGE_SAVE_TYPE_JPG, &sizeImage, NULL);
			CheckError("InnoTracking::Crop", errorCode);
			unsigned char* cropImageFormat = new unsigned char[sizeImage];
			errorCode = IFACE_SaveImageToMemory(cropImageData, cropWidth, cropHeight,
				3, IFACE_IMAGE_SAVE_TYPE_JPG, &sizeImage, cropImageFormat);
			CheckError("InnoTracking::Crop", errorCode);

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
	void InnoTracking::GetParametersTracking()
	{
		std::string nameFile = "track" + std::to_string(indexChannel) + ".txt";
		configurationTrack = ASSInterface::Configuration::CreateConfigTrack();
		configurationTrack->SetPath(nameFile);
		configurationTrack->ParseToObject();

		std::any anyDiscovery = configurationTrack->GetParam("TRACK_FACE_DISCOVERY_FREQUENCE_MS");
		std::any anyTimeDelta = configurationTrack->GetParam("COUNT_REDETECT_TIME_DELTA");
		std::any anyTracking = configurationTrack->GetParam("TRACK_TRACKING_MODE");
		std::any anySpeedTrack = configurationTrack->GetParam("TRACK_SPEED_ACCURACY_MODE");
		std::any anyOptimus = configurationTrack->GetParam("TRACK_MOTION_OPTIMIZATION");
		std::any anyDeep = configurationTrack->GetParam("TRACK_DEEP_TRACK");
		std::any anyConfidence = configurationTrack->GetParam("TRACK_CONFIDENCE_THRESHOLD");

		discoveryFrecuence = std::any_cast<int>(anyDiscovery);
		timeRedetect = std::any_cast<int>(anyTimeDelta);
		tracking = std::any_cast<int>(anyTracking);
		speedTrack = std::any_cast<int>(anySpeedTrack);
		trackOptimus = std::any_cast<int>(anyOptimus);
		deepTracking = std::any_cast<bool>(anyDeep);
		confidenceThresholdTrack = std::any_cast<int>(anyConfidence);

	}
	void InnoTracking::GetParamsDetectFace()
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
		std::any anyDetection = configurationFace->GetParam("CFG_IFACE_DETECTION_MODE");
		std::any anyExtraction = configurationFace->GetParam("CFG_IFACE_EXTRACTION_MODE");
		std::any anyForced = configurationFace->GetParam("CFG_IFACE_DETECT_FORCED");
		std::any anyMultipleFaces = configurationFace->GetParam("CFG_IFACE_IGNORE_MULTIPLE_FACES");

		maxDetected = std::any_cast<int>(anyMaxDetect);
		minFace = std::any_cast<float>(anyMinFace);
		maxFace = std::any_cast<float>(anyMaxFace);
		confidenceThreshold = std::any_cast<int>(anyConfidence);
		speedDetect = std::any_cast<int>(anySpeedDetect);
		speedExtraction = std::any_cast<int>(anySpeedTemplate);
		faceCrop = std::any_cast<int>(anyCrop);
		qualityModel = std::any_cast<int>(anyModel);
		detected = std::any_cast<int>(anyDetection);
		extraction = std::any_cast<int>(anyExtraction);
		detectForced = std::any_cast<int>(anyForced);
		multipleFace = std::any_cast<int>(anyMultipleFaces);

	}
	void InnoTracking::CheckError(std::string function, int errorCode)
	{
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE(function.c_str(), errorIFace->GetError(errorCode).c_str());
		}
	}
	unsigned char* InnoTracking::LoadImageFromMemory(std::vector<unsigned char> buffer, int* width, int* height)
	{
		int length, errorCode;
		const char* imgData = reinterpret_cast<const char*> (&buffer[0]);
		if (imgData == NULL) {
			return NULL;
		}

		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
			height, &length, NULL);
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoTracking::LoadImageFromMemory", errorIFace->GetError(errorCode).c_str());
			return NULL;
		}

		unsigned char* rawImage = new unsigned char[length];
		errorCode = IFACE_LoadImageFromMemory(imgData, (unsigned int)buffer.size(), width,
			height, &length, rawImage);
		if (errorCode != IFACE_OK) {
			ASS_ERROR_PROFILE_SCOPE("InnoTracking::LoadImageFromMemory", errorIFace->GetError(errorCode).c_str());
			return NULL;
		}

		return rawImage;
	}
	void InnoTracking::ResetCoordinates()
	{
		long countFrames = countFrameTracking - 1;
		residueCountFrames = countFrames % refreshDiscovery;
		if (residueCountFrames == 0)
		{
			ClearAllCoordinatesImage();
		}
	}
	void InnoTracking::ClearAllCoordinatesImage()
	{
		for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
			ClearCoordinatesImage(i);
		}
	}
	void InnoTracking::ClearCoordinatesImage(int indexTracked)
	{
		int index = indexTracked * NUM_COORDINATES_X_IMAGE;
		imageCoordinatesFollowed[index] = 0.0f;
		imageCoordinatesFollowed[index + 1] = 0.0f;
		imageCoordinatesFollowed[index + 2] = 0.0f;
		imageCoordinatesFollowed[index + 3] = 0.0f;
	}
	void InnoTracking::TrackObjectState()
	{
		int errorCode;

		for (int trackedObjectIndex = 0; trackedObjectIndex < NUM_TRACKED_OBJECTS;
			trackedObjectIndex++)
		{

			float bbX, bbY, bbWidth, bbHeight;
			IFACE_TrackedObjectState trackedState;

			errorCode = IFACE_GetObjectState(objects[trackedObjectIndex],
				objectHandler, &trackedState);
			CheckError("InnoTracking::TrackObjectState", errorCode);

			if (trackedState == IFACE_TRACKED_OBJECT_STATE_CLEAN) {

				errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
				CheckError("InnoTracking::TrackObjectState", errorCode);
				continue;
			}

			switch (trackedState)
			{
			case IFACE_TRACKED_OBJECT_STATE_TRACKED:

				float objectScore;
				
				errorCode = IFACE_GetObjectBoundingBox(objects[trackedObjectIndex],
					objectHandler, &bbX, &bbY, &bbWidth, &bbHeight);
				CheckError("InnoTracking::TrackObjectState", errorCode);

				SetCoordinatesImage(bbX, bbY, bbWidth, bbHeight, trackedObjectIndex);							

				errorCode = IFACE_GetObjectScore(objects[trackedObjectIndex],
					objectHandler, &objectScore);
				CheckError("InnoTracking::TrackObjectState", errorCode);

				SetColorRectangle(objectScore, trackedObjectIndex);

				break;
			case IFACE_TRACKED_OBJECT_STATE_COVERED:
				errorCode = IFACE_GetObjectBoundingBox(objects[trackedObjectIndex],
					objectHandler, &bbX, &bbY, &bbWidth, &bbHeight);
				CheckError("InnoTracking::TrackObjectState", errorCode);

				SetCoordinatesImage(bbX, bbY, bbWidth, bbHeight, trackedObjectIndex);
				break;
			case IFACE_TRACKED_OBJECT_STATE_SUSPEND:
				ClearCoordinatesImage(trackedObjectIndex);
				errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
				CheckError("InnoTracking::TrackObjectState", errorCode);
				break;
			case IFACE_TRACKED_OBJECT_STATE_LOST:
				ClearCoordinatesImage(trackedObjectIndex);
				errorCode = IFACE_CleanObject(objects[trackedObjectIndex], objectHandler);
				CheckError("InnoTracking::TrackObjectState", errorCode);

				void* newObj;
				errorCode = IFACE_CreateObject(&newObj);
				objects[trackedObjectIndex] = newObj;
				break;
			case IFACE_TRACKED_OBJECT_STATE_CLEAN:

				break;
			}

			if (trackedState == IFACE_TRACKED_OBJECT_STATE_TRACKED) {
				if (task == ENROLL_PERSON ||
					task == IMPORT_FACE ||
					task == CONTROL_ENTRY || 
					task == CONTROL_ENTRY_TRACK0 ||
					task == CONTROL_ENTRY_TRACK1)
				{
					DoHomework(trackedObjectIndex);
				}

			}
		}
		countFrameTracking++;
	}
	void InnoTracking::SetCoordinatesImage(float x, float y, float width, float height, int indexTracked)
	{
		int index = indexTracked * NUM_COORDINATES_X_IMAGE;
		imageCoordinatesFollowed[index] = x;
		imageCoordinatesFollowed[index + 1] = y;
		imageCoordinatesFollowed[index + 2] = width;
		imageCoordinatesFollowed[index + 3] = height;
	}
	void InnoTracking::SetColorRectangle(float score, int indexTracked)
	{
		float percentageScore = ((float)score * 100.0f) / 10000.0f;
		float variationColor = (1.0f * percentageScore) / 100.0f;
		float color = (1.0f - variationColor);
		colorRectangle[indexTracked] = color;
	}
	void InnoTracking::DoHomework(int indexTracked)
	{
		if (residueCountFrames == 0) {
			if (task == ENROLL_PERSON) task = -1;
			
			std::thread tdt(&InnoTracking::CreateFace, this, indexTracked);
			tdt.detach();
		}
	}
	void InnoTracking::CreateFace(int indexTracked)
	{
		int errorCode;
		void* face = nullptr;
		
		errorCode = IFACE_CreateFace(&face);

		errorCode = IFACE_GetFaceFromObject(objects[indexTracked],
			objectHandler, face, IFACE_TRACKED_OBJECT_FACE_TYPE_LAST_DISCOVERY);
		CheckError("InnoTracking::CreateFace", errorCode);

		float rightEyeX, rightEyeY, leftEyeX, leftEyeY, faceScore;

		errorCode = IFACE_GetFaceBasicInfo(face, faceHandlerTracking,
			&rightEyeX, &rightEyeY, &leftEyeX, &leftEyeY, &faceScore);
		CheckError("InnoTracking::CreateFace", errorCode);
			
		float distEyes = (leftEyeX - rightEyeX);
		
		if (errorCode == IFACE_OK && distEyes >= minFace && distEyes <= maxFace  
			&& faceScore >= confidenceThreshold) {			
			BuildSpecificationForIdentify(face);			
		}
		else
		{
			ASSInterface::ImGuiLog::AddLog("[%s], date: [%s], channel: [%01d], prm_min_face: [%.2f], face size: [%.2f], prm_confidence: [%05d], confidence: [%.2f], result: [%s]\n", ASSInterface::ImGuiLog::Categories(1),
				ASSInterface::DateTime::Now().c_str(), indexChannel, minFace, distEyes, confidenceThreshold, faceScore, "Rejected");
		}
		errorCode = IFACE_ReleaseEntity(face);
		CheckError("InnoTracking::CreateFace", errorCode);

	}
	void InnoTracking::BuildSpecificationForIdentify(void* face)
	{		
		CropSpecification specCrop;
		DetectSpecification specDetect;


		Crop(face, specCrop);
		CreateTemplate(face, specDetect);
		if (specDetect.sizeTemplate != 0 && !specCrop.cropData.empty())
		{			
			specDetect.cropSpec.length = specCrop.length;
			specDetect.cropSpec.width = specCrop.width;
			specDetect.cropSpec.height = specCrop.height;		
			specDetect.cropSpec.cropData.assign(&specCrop.cropData[0],
				&specCrop.cropData[0] + specCrop.cropData.size());		
			if (task == -1)
			{
				specDetect.task = ENROLL_PERSON;
			}
			else
			{
				specDetect.task = task;
			}
			
			shootSpecDetect.on_next(specDetect);
						
		}
		
	}
}