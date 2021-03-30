#include "hzpch.h"
#include "JSONConfigFace.h"

namespace ASSInterface {
	JSONConfigFace::JSONConfigFace()
	{
		managerFile = ASSInterface::File::Create();

		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
		paramsFace[MAXFACESDETECT] = maxfacesDetect;
		paramsFace[MINFACE] = minFaceSize;
		paramsFace[MAXFACE] = maxFaceSize;
		paramsFace[CONFIDENCE_THRESHOLD] = confidenceThreshold;
		paramsFace[SPEED_ACCURACY_MODE] = speedAccurancyMode;
		paramsFace[FACETMPLEXT_SPEED_ACCURACY_MODE] = faceTemplextspeedAccurancyMode;
		paramsFace[FACE_CROP_IMAGE] = faceCropImage;
		paramsFace[QUALITY_MODEL] = qualityModel;
		paramsFace[CFG_IFACE_DETECTION_MODE] = faceDetectionMode;
		paramsFace[CFG_IFACE_EXTRACTION_MODE] = faceExtractionMode;
		paramsFace[CFG_IFACE_DETECT_FORCED] = faceDetectionForced;
		paramsFace[CFG_IFACE_IGNORE_MULTIPLE_FACES] = ignoreMultipleFaces;		
	}
	JSONConfigFace::~JSONConfigFace()
	{
	}
	void JSONConfigFace::ParseToObject()
	{
		std::string strJson = managerFile->ReadFile(path);
		if (!strJson.empty())
		{
			auto jdata = nlohmann::json::parse(strJson);

			for (nlohmann::json::iterator it = jdata.begin(); it != jdata.end(); ++it) {
				if (it.key() == MAXFACESDETECT) {
					maxfacesDetect = it.value();
					paramsFace[MAXFACESDETECT] = maxfacesDetect;
				}
				if (it.key() == MINFACE) {
					minFaceSize = it.value();
					paramsFace[MINFACE] = minFaceSize;
				}
				if (it.key() == MAXFACE) {
					maxFaceSize = it.value();
					paramsFace[MAXFACE] = maxFaceSize;
				}
				if (it.key() == CONFIDENCE_THRESHOLD) {
					confidenceThreshold = it.value();
					paramsFace[CONFIDENCE_THRESHOLD] = confidenceThreshold;
				}
				if (it.key() == SPEED_ACCURACY_MODE) {
					speedAccurancyMode = it.value();
					paramsFace[SPEED_ACCURACY_MODE] = speedAccurancyMode;
				}
				if (it.key() == FACETMPLEXT_SPEED_ACCURACY_MODE) {
					faceTemplextspeedAccurancyMode = it.value();
					paramsFace[FACETMPLEXT_SPEED_ACCURACY_MODE] = faceTemplextspeedAccurancyMode;
				}
				if (it.key() == FACE_CROP_IMAGE) {
					faceCropImage = it.value();
					paramsFace[FACE_CROP_IMAGE] = faceCropImage;
				}
				if (it.key() == QUALITY_MODEL) {
					qualityModel = it.value();
					paramsFace[QUALITY_MODEL] = qualityModel;
				}
				if (it.key() == CFG_IFACE_DETECTION_MODE) {
					faceDetectionMode = it.value();
					paramsFace[CFG_IFACE_DETECTION_MODE] = faceDetectionMode;
				}
				if (it.key() == CFG_IFACE_EXTRACTION_MODE) {
					faceExtractionMode = it.value();
					paramsFace[CFG_IFACE_EXTRACTION_MODE] = faceExtractionMode;
				}
				if (it.key() == CFG_IFACE_DETECT_FORCED) {
					faceDetectionForced = it.value();
					paramsFace[CFG_IFACE_DETECT_FORCED] = faceDetectionForced;
				}
				if (it.key() == CFG_IFACE_IGNORE_MULTIPLE_FACES) {
					ignoreMultipleFaces = it.value();
					paramsFace[CFG_IFACE_IGNORE_MULTIPLE_FACES] = ignoreMultipleFaces;
				}
			}
		}
	}
	void JSONConfigFace::ParseToFile()
	{
		maxfacesDetect = std::any_cast<int>(paramsFace[MAXFACESDETECT]);
		minFaceSize = std::any_cast<float>(paramsFace[MINFACE]);
		maxFaceSize = std::any_cast<float>(paramsFace[MAXFACE]);
		confidenceThreshold = std::any_cast<int>(paramsFace[CONFIDENCE_THRESHOLD]);
		speedAccurancyMode = std::any_cast<int>(paramsFace[SPEED_ACCURACY_MODE]);
		faceTemplextspeedAccurancyMode = std::any_cast<int>(paramsFace[FACETMPLEXT_SPEED_ACCURACY_MODE]);
		faceCropImage = std::any_cast<int>(paramsFace[FACE_CROP_IMAGE]);
		qualityModel = std::any_cast<int>(paramsFace[QUALITY_MODEL]);
		faceDetectionMode = std::any_cast<int>(paramsFace[CFG_IFACE_DETECTION_MODE]);
		faceExtractionMode = std::any_cast<int>(paramsFace[CFG_IFACE_EXTRACTION_MODE]);
		faceDetectionForced = std::any_cast<int>(paramsFace[CFG_IFACE_DETECT_FORCED]);
		ignoreMultipleFaces = std::any_cast<int>(paramsFace[CFG_IFACE_IGNORE_MULTIPLE_FACES]);


		nlohmann::json jParams = nlohmann::json::object({ {MAXFACESDETECT, maxfacesDetect},
			{MINFACE, minFaceSize}, {MAXFACE, maxFaceSize}, {CONFIDENCE_THRESHOLD, confidenceThreshold},
			{SPEED_ACCURACY_MODE, speedAccurancyMode}, {FACETMPLEXT_SPEED_ACCURACY_MODE, faceTemplextspeedAccurancyMode},
			{FACE_CROP_IMAGE, faceCropImage}, {QUALITY_MODEL, qualityModel}, {CFG_IFACE_DETECTION_MODE, faceDetectionMode}, 
			{CFG_IFACE_EXTRACTION_MODE, faceExtractionMode}, {CFG_IFACE_DETECT_FORCED, faceDetectionForced}, 
			{CFG_IFACE_IGNORE_MULTIPLE_FACES, ignoreMultipleFaces} });

		std::string jsonToString = jParams.dump();
		SaveFile(jsonToString);
	}

	std::any JSONConfigFace::GetParam(const char* name)
	{
		std::any result;
		if (paramsFace.find(name) != paramsFace.end())
			return paramsFace[name];
		return result;
	}

	void JSONConfigFace::SetParam(const char* name, std::any value)
	{
		paramsFace[name] = value;
	}
	void JSONConfigFace::SetPath(std::string nameFile)
	{
		fileConfig = nameFile;
		path = managerFile->GetFolderConfiguration() + "/" + fileConfig;
	}
	void JSONConfigFace::SaveFile(std::string content)
	{
		managerFile->DelFile(path);
		managerFile->WriteFile(path, content);
	}
}