#pragma once

#include <ASSInterface.h>
#include "glib.h"
#include "gst/gst.h"
#include "gst/app/app.h"
#include <SOIL2.h>
#include "rxcpp/rx.hpp"
#include "tbb/parallel_invoke.h"

namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;

}

namespace ASSInterface {
	class ASGStreamer : public ASSStreamer
	{
	public:
		ASGStreamer(int channel);
		virtual ~ASGStreamer();
		virtual void Run() override;
		virtual void FinishLoop() override;
		virtual void Play() override;
		virtual void Stop() override;
		virtual int GetWidth() const override { return m_Width; }
		virtual int GetHeight() const override { return m_Height; }
		virtual int GetChannels() const override { return m_Channels; };
		virtual int GetSizeImage() const override { return m_SizeImage; };
		virtual int GetFPS() const override { return m_Fps; };
		virtual int GetElapse() const override { return m_Elapse; };
		virtual int GetSourceFlow() const override { return optionSourceFlow; };
		virtual float GetSizeHeightTexture() const override { return sizeHeightTexture; };
		virtual std::vector<unsigned char> GetDataTexture() override;
		virtual std::vector<unsigned char> GetDataRaw() override;
		virtual inline const std::string GetNameWindow() override { return nameWindow; };
		virtual float* GetCoordinates() override { return trackingOpenCV->GetCoordinates(); };
	private:
		virtual char* DescriptionFlow(int optionFlow, std::string descriptionFlow) override;
		virtual std::string ConvertBackslashToSlash(std::string input) override;
		static GstFlowReturn CaptureGstBuffer(GstAppSink* appsink, gpointer /*data*/);
		static gboolean MessageCallback(GstBus* bus, GstMessage* message, gpointer data);		
		void ObserverFlow();
		void InitDimensionsImage(std::tuple<int, int, int, int> dimensionsImage);
		void SetDataImage(unsigned char* data, int size, int channel);
		void BranchTracking(std::vector<unsigned char> data);
		void BranchVideo(unsigned char* data, int size);
		void BranchVideo(std::vector<unsigned char> data);
	private:
		ASSInterface::Ref<ASSInterface::Configuration> configuration;		
		GstElement* pipeline = nullptr;
		GMainLoop* loop = nullptr;
		GstElement* source = nullptr;
		GstBus* bus = nullptr;
		char* description = nullptr;
		int m_Width = 0, m_Height = 0, m_Channels = 0, m_Fps = 30, m_SizeImage = 0, m_Elapse = 0;
		float sizeHeightTexture = 0.0f;
		std::vector<unsigned char> dataImage;
		std::vector<unsigned char> dataRawImage;
		std::mutex mtxTexture;
		std::mutex mtxRaw;
		int optionSourceFlow = 0;
		int indexChannel = 1;
		bool isInitChannel = false;
		std::string nameWindow = "Video";
		ASSInterface::Ref<ASSInterface::Tracking> trackingOpenCV;
		//ASSInterface::Ref<ASSInterface::Tracking> trackingAzureFace;

		bool isFinishTrack = true;
	};
}