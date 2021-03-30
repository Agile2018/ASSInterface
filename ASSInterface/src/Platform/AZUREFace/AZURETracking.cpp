#include "hzpch.h"
#include "AZURETracking.h"

namespace ASSInterface {
	AZURETracking::AZURETracking()
	{
		transformImage = ASSInterface::TransformImage::Create();
	}
	AZURETracking::~AZURETracking()
	{
	}
	void AZURETracking::Init()
	{

	}
	void AZURETracking::Detect(std::vector<unsigned char> data)
	{
		std::vector<unsigned char> dataWithformat = transformImage->ImenCode(data, heightFrame, widthFrame);

		web::uri_builder builder(baseUrl); //conversions::to_string_t
		builder.append_path(U("/face/v1.0/detect?"));
		builder.append_query(U("returnFaceId=true"), U("&"), U("returnFaceLandmarks=false"));

		http_client client(builder.to_string());
		//web::http::method mtd = methods::POST;
		web::http::http_request msg(methods::POST);

		msg.headers().add(field_ctt, value_ctt);
		msg.headers().add(field_ocp, value_ocp);
		msg.set_body(dataWithformat);
		pplx::task<void> requestTask = client.request(msg)
			.then([=](http_response response) {                
				ASS_INFO("Received response status code: {0}", response.status_code());
				return response.extract_json().get();
			})
			.then([this](web::json::value value){

				GetResponse(value);
				
			});

		try
		{
			requestTask.wait();
		}
		catch (const std::exception& e)
		{
			ASS_ERROR("Error exception: {0}", e.what());
			ClearCoordinates();
		}
	}
	float* AZURETracking::GetCoordinates()
	{
		return rectangleFace;
	}
	float* AZURETracking::GetColor()
	{
		return nullptr;
	}
	void AZURETracking::CreateTemplate()
	{
	}
	void AZURETracking::Crop()
	{
	}

    void AZURETracking::SetCoordinates() {
        rectangleFace[0] = left;
        rectangleFace[1] = top;
        rectangleFace[2] = width;
        rectangleFace[3] = height;
    }

	void AZURETracking::ClearCoordinates() {
		rectangleFace[0] = 0.0f;
		rectangleFace[1] = 0.0f;
		rectangleFace[2] = 0.0f;
		rectangleFace[3] = 0.0f;

	}

	void AZURETracking::GetResponse(web::json::value jresponse)
	{		
		if (jresponse.is_array() && jresponse.size() > 0) {
			
			for (int i = 0; i < jresponse.size(); i++)
			{
				
				auto objParent = jresponse[i];
				auto objDefinite = objParent.as_object();
				
				for (auto iterInner = objDefinite.cbegin(); iterInner != objDefinite.cend(); ++iterInner)
				{
					
					auto& propertyName = iterInner->first;
					auto& propertyValue = iterInner->second;

					if (propertyValue.is_object())
					{
						wchar_t jheight[] = L"height";
						wchar_t jleft[] = L"left";
						wchar_t jtop[] = L"top";
						wchar_t jwidth[] = L"width";

						auto objCoordinates = propertyValue.as_object();
						
						for (auto iter = objCoordinates.cbegin(); iter != objCoordinates.cend(); ++iter) {
							
							auto& coordinatesName = iter->first;
							auto& coordinatesValue = iter->second;

							if (wcscmp(coordinatesName.c_str(), jheight) != 0) {
								
								height = (float)coordinatesValue.as_integer();
							}

							if (wcscmp(coordinatesName.c_str(), jleft) != 0) {
								
								left = (float)coordinatesValue.as_integer();
							}
							if (wcscmp(coordinatesName.c_str(), jtop) != 0) {
								
								top = (float)coordinatesValue.as_integer();
							}
							if (wcscmp(coordinatesName.c_str(), jwidth) != 0) {
								
								width = (float)coordinatesValue.as_integer();
							}

						}

						SetCoordinates();
						
					}
				}

			}
		}
		else
		{
			ClearCoordinates();
		}
	}
}