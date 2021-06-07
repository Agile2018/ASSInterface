#pragma once

#include "ASSInterface.h"
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;
using namespace cv;

namespace ASSInterface {

	class CascadeDetectorAdapter : public DetectionBasedTracker::IDetector
	{
	public:
		CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
			IDetector(),
			Detector(detector)
		{
			CV_Assert(detector);
		}

		void detect(const cv::Mat& Image, std::vector<cv::Rect>& objects) //CV_OVERRIDE
		{
			Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
		}

		virtual ~CascadeDetectorAdapter() //CV_OVERRIDE
		{}

	private:
		CascadeDetectorAdapter();
		cv::Ptr<cv::CascadeClassifier> Detector;
	};


	class OpenCVTracking : public Tracking {
	public:
		OpenCVTracking();
		~OpenCVTracking();
		virtual void Init() override;
		virtual void Detect(std::vector<unsigned char> data) override;
		virtual float* GetCoordinates() override;
		virtual float* GetColor() override;
		virtual inline const Rx::observable<DetectSpecification> Get() const { return Rx::observable<DetectSpecification>(); };
		virtual inline void SetWidth(int width) override { widthFrame = width; }
		virtual inline void SetHeight(int height) override { heightFrame = height; }
		virtual inline const std::vector<unsigned char> GetFlowData() const { return flowData; };
		virtual void Terminate() override;
		virtual inline void SetElapseFrame(int elapse) override { elapseFrame = elapse; };
		virtual inline bool IsTrackFinish() override { return true; };
		virtual inline void SetTask(int value) override { task = value; };
		virtual inline int GetTask() override { return 0; };
	private:
		virtual void CreateTemplate(void* face, DetectSpecification& specDetect) override;
		virtual void Crop(void* face, CropSpecification& specCrop) override;
				
	private:		
		DetectionBasedTracker* Detector = nullptr;
		Mat referenceFrame;
		Mat grayFrame;
		std::vector<Rect> faces;
		int widthFrame = 0, heightFrame = 0, elapseFrame, task;
		std::vector<unsigned char> flowData;
		
	};
}