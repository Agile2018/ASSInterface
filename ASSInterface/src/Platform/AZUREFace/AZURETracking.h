#pragma once
#include "ASSInterface.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features

namespace ASSInterface {
	class AZURETracking : public Tracking {
	public:
		AZURETracking();
		~AZURETracking();
		virtual void Init() override;
		virtual void Detect(std::vector<unsigned char> data) override;
		virtual float* GetCoordinates() override;
		virtual float* GetColor() override;
		virtual const DetectSpecification& Get() const override { return detectSpecification; }
		virtual inline void SetWidth(int width) override { widthFrame = width; }
		virtual inline void SetHeight(int height) override { heightFrame = height; }
		virtual inline const std::vector<unsigned char> GetFlowData() const { return flowData; }

	private:
		virtual void CreateTemplate() override;
		virtual void Crop() override;
		virtual const CropSpecification& GetCropSpecification() const override { return cropSpecification; }
		void SetCoordinates();
		void GetResponse(web::json::value jresponse);
		void ClearCoordinates();
	private:
		web::uri baseUrl = uri(U("https://hiddenodds.cognitiveservices.azure.com"));
		//const std::string baseUrl = "https://hiddenodds.cognitiveservices.azure.com";
		
		const utility::string_t field_ctt = U("Content-Type");
		const utility::string_t value_ctt = U("application/octet-stream"); //application/json
		const utility::string_t field_ocp = U("Ocp-Apim-Subscription-Key");
		const utility::string_t value_ocp = U("5be74a0072fc4a3090cefd34d9a24163");
		float height = 0.0f, left = 0.0f, top = 0.0f, width = 0.0f;
		CropSpecification cropSpecification;
		DetectSpecification detectSpecification;
		int widthFrame = 0, heightFrame = 0;
		std::vector<unsigned char> flowData;
		float rectangleFace[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		ASSInterface::Ref<ASSInterface::TransformImage> transformImage;
	};
}