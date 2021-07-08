#pragma once

#include <ASSInterface.h>
#include "glib.h"
#include "gst/gst.h"
#include "gst/app/app.h"
#include <SOIL2.h>
//#include "tbb/parallel_invoke.h"

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
		virtual inline int GetWidth() const override { return m_Width; }
		virtual inline int GetHeight() const override { return m_Height; }
		virtual inline int GetChannels() const override { return m_Channels; }
		virtual inline int GetSizeImage() const override { return m_SizeImage; }
		virtual inline int GetFPS() const override { return m_Fps; }
		virtual inline int GetElapse() const override { return m_Elapse; }
		virtual int GetSourceFlow() const override { return optionSourceFlow; }
		virtual float GetSizeHeightTexture() const override { return sizeHeightTexture; }
		virtual std::vector<unsigned char> GetDataTexture() override;
		virtual inline const std::string GetNameWindow() override { return nameWindow; }
		virtual float* GetCoordinates() override;
		virtual inline bool IsCorrectStart() override { return correctStart; };
		virtual inline bool IsCorrectDescription() override { return correctDescription; }
		virtual void InitTracking() override;
		virtual inline void SetTask(int value) override { trackingInno->SetTask(value); }
		virtual inline int GetTask() override { return trackingInno->GetTask(); };
		virtual inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) 
			override { innoTask->SetDatabase(db); }
		virtual inline const Rx::observable<ASSInterface::IdentitySpecification> GetIdentify()
			const override { return observableSpecIdentify; }
		virtual std::any GetExecuteTask() override { return innoTask; };
		virtual inline void CloseConnection() override { innoTask->CloseConnection(); }
		virtual inline void ResetTask() override { innoTask->ResetParameters(); };
		virtual inline void ResetTrack() override { trackingInno->ResetParams(); };
	private:
		virtual char* DescriptionFlow(int optionFlow, std::string descriptionFlow) override;
		virtual std::string ConvertBackslashToSlash(std::string input) override;
		static GstFlowReturn CaptureGstBuffer(GstAppSink* appsink, gpointer /*data*/);
		static gboolean MessageCallback(GstBus* bus, GstMessage* message, gpointer data);		
		Rx::subject<ASSInterface::IdentitySpecification> specSubject;
		Rx::observable<ASSInterface::IdentitySpecification> observableSpecIdentify = specSubject.get_observable();
		Rx::subscriber<ASSInterface::IdentitySpecification> shootSpecIdentity = specSubject.get_subscriber();
		void ObserverFlow();
		void InitDimensionsImage(std::tuple<int, int, int, int> dimensionsImage);
		void SetDataImage(unsigned char* data, int size, int channel);
		void BranchTracking(std::vector<unsigned char> data);
		void BranchVideo(unsigned char* data, int size);
		void BranchVideo(std::vector<unsigned char> data);
		void ClearCoordinatesImage(int indexTracked);
		void ClearAllCoordinatesImage();
		void ObserverTracking();
		void ObserverIdentify();
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
		std::mutex mtxTexture;		
		int optionSourceFlow = 0;
		int indexChannel = 1;
		bool isInitChannel = false;
		bool isInitTracking = false;
		std::string nameWindow = "Video";		
		bool correctStart = false;
		bool correctDescription = false;
		ASSInterface::Ref<ASSInterface::File> managerFile;
		ASSInterface::Ref<ASSInterface::Tracking> trackingInno;
		ASSInterface::Ref<ASSInterface::ExecuteTask> innoTask;
		//ASSInterface::Ref<ASSInterface::Tracking> trackingOpenCV;		
		float imageCoordinatesFollowed[COORDINATES_X_ALL_IMAGES] = {};
	};
}