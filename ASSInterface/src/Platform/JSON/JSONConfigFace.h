#pragma once
#include <nlohmann/json.hpp>
#include "ASSInterface.h"

namespace ASSInterface {
	class JSONConfigFace : public Configuration {
	public:
		JSONConfigFace();
		virtual ~JSONConfigFace();
		virtual void ParseToObject() override;
		virtual void ParseToFile() override;
		virtual std::any GetParam(const char* name) override;
		virtual void SetParam(const char* name, std::any value) override;
		virtual void SetPath(std::string nameFile) override;
	private:
		virtual void SaveFile(std::string content) override;
	private:
		std::string path;
		std::string fileConfig = "face.txt";
		float minFaceSize = 18.0;
		float maxFaceSize = 400.0;
		int maxfacesDetect = 1;
		int confidenceThreshold = 600;
		int qualityModel = 60;
		int faceCropImage = 1;
		int speedAccurancyMode = 0;
		int faceTemplextspeedAccurancyMode = 0;
		int faceDetectionMode = 2;
		int faceExtractionMode = 2;
		int faceDetectionForced = 1;
		int ignoreMultipleFaces = 1;

		const std::string MAXFACESDETECT = "FACE_MAX_DETECT";
		const std::string MINFACE = "TRACK_MIN_FACE_SIZE";
		const std::string MAXFACE = "TRACK_MAX_FACE_SIZE";
		const std::string CONFIDENCE_THRESHOLD = "FACEDET_CONFIDENCE_THRESHOLD";
		const std::string SPEED_ACCURACY_MODE = "FACEDET_SPEED_ACCURACY_MODE";
		const std::string FACETMPLEXT_SPEED_ACCURACY_MODE = "FACETMPLEXT_SPEED_ACCURACY_MODE";
		const std::string FACE_CROP_IMAGE = "IFACE_CROP_IMAGE";		
		const std::string QUALITY_MODEL = "QUALITY_MODEL";
		const std::string CFG_IFACE_DETECTION_MODE = "CFG_IFACE_DETECTION_MODE";
		const std::string CFG_IFACE_EXTRACTION_MODE = "CFG_IFACE_EXTRACTION_MODE";
		const std::string CFG_IFACE_DETECT_FORCED = "CFG_IFACE_DETECT_FORCED";
		const std::string CFG_IFACE_IGNORE_MULTIPLE_FACES = "CFG_IFACE_IGNORE_MULTIPLE_FACES";

		mutable std::unordered_map<std::string, std::any> paramsFace;
		ASSInterface::Ref<ASSInterface::File> managerFile;

	};
}