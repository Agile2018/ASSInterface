#pragma once

#include "ASSInterface/Recognition/Tracking.h"
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

		void detect(const cv::Mat& Image, std::vector<cv::Rect>& objects) CV_OVERRIDE
		{
			Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
		}

		virtual ~CascadeDetectorAdapter() CV_OVERRIDE
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
		virtual float** GetCoordinates() override;
		virtual float* GetColor() override;
		virtual const DetectSpecification& Get() const override { return detectSpecification; };
		virtual inline void SetWidth(int width) override { widthFrame = width; }
		virtual inline void SetHeight(int height) override { heightFrame = height; }
		virtual inline const std::vector<unsigned char> GetFlowData() const { return flowData; };
	private:
		virtual void CreateTemplate() override;
		virtual void Crop() override;
		virtual const CropSpecification& GetCropSpecification() const override { return cropSpecification; };
	private:
		CropSpecification cropSpecification;
		DetectSpecification detectSpecification;
		DetectionBasedTracker* Detector = nullptr;
		Mat referenceFrame;
		Mat grayFrame;
		std::vector<Rect> faces;
		int widthFrame = 0, heightFrame = 0;
		std::vector<unsigned char> flowData;
		
	};
}