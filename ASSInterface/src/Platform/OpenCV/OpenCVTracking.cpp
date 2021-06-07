#include "hzpch.h"
#include "OpenCVTracking.h"

namespace ASSInterface {

	OpenCVTracking::OpenCVTracking()
	{
		
	}
	OpenCVTracking::~OpenCVTracking()
	{
	}

	void OpenCVTracking::Init()
	{
		std::string cascadeFrontalfilename = "assets/data/haarcascades/haarcascade_frontalface_default.xml";
		cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
		cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
		if (cascade->empty())
		{
			ASS_ERROR("Error: Cannot load {0}", cascadeFrontalfilename.c_str());
			
		}

		cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
		cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
		if (cascade->empty())
		{
			ASS_ERROR("Error: Cannot load {0}", cascadeFrontalfilename.c_str());
		}

		DetectionBasedTracker::Parameters params;
		
		Detector = new DetectionBasedTracker(MainDetector, TrackingDetector, params);

		if (!Detector->run())
		{
			ASS_ERROR("Error: Detector initialization failed ");
		}
	}		

	void OpenCVTracking::Detect(std::vector<unsigned char> data)
	{		
		//clock_t timeStart1 = clock();

		/*Mat reference(cv::Mat(cv::Size(widthFrame, heightFrame),
			CV_8UC3, (char*)&data[0], cv::Mat::AUTO_STEP));*/
		void* face = nullptr;
		DetectSpecification specDetect;
		cv::Mat reference = cv::imdecode(data, cv::IMREAD_COLOR);

		cv::cvtColor(reference, grayFrame, COLOR_BGR2GRAY);
		Detector->process(grayFrame);
		Detector->getObjects(faces);					

		for (size_t i = 0; i < faces.size(); i++)
		{
			cv::rectangle(reference, faces[i], Scalar(0, 255, 0), 4);
		}
		
		referenceFrame = reference.clone();
		CreateTemplate(face, specDetect);



		/*clock_t duration1 = clock() - timeStart1;
		int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);
		ASS_INFO("Time Opencv Detect: {0}", durationMs1);*/
		
	}	

	float* OpenCVTracking::GetCoordinates()
	{
		return nullptr;
	}
	float* OpenCVTracking::GetColor()
	{
		return nullptr;
	}

	void OpenCVTracking::Terminate()
	{
	}

	void OpenCVTracking::CreateTemplate(void* face, DetectSpecification& specDetect)
	{
		flowData.clear();
		int params[3] = { 0 };
		params[0] = cv::IMWRITE_JPEG_QUALITY;
		params[1] = 100;
		if (!referenceFrame.empty())
		{
			bool code = cv::imencode(".jpg", referenceFrame,
				flowData, std::vector<int>(params, params + 2));

		}
		
	}

	void OpenCVTracking::Crop(void* face, CropSpecification& specCrop)
	{
	}


}