#pragma once
#include "hzpch.h"
#include "ASSInterface.h"
//#include "ASSInterface/Core/Core.h"
#include "ASSInterface/Recognition/Identification.h"

namespace ASSInterface {

	class ASS_API Stream {
	public:
		virtual ~Stream() = default;
		virtual void Run() = 0;
		virtual void FinishLoop() = 0;
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual inline int GetWidth() const = 0;
		virtual inline int GetHeight() const = 0;
		virtual inline int GetChannels() const = 0;
		virtual inline int GetSizeImage() const = 0;
		virtual inline int GetFPS() const = 0;
		virtual inline int GetElapse() const = 0;
		virtual int GetSourceFlow() const = 0;
		virtual float GetSizeHeightTexture() const = 0;
		virtual std::vector<unsigned char> GetDataTexture() = 0;
		virtual inline const std::string GetNameWindow() = 0;
		virtual float* GetCoordinates() = 0;
		virtual inline bool IsCorrectStart() = 0;
		virtual inline bool IsCorrectDescription() = 0;
		virtual void InitTracking() = 0;
		virtual inline void SetTask(int value) = 0;
		virtual inline int GetTask() = 0;
		virtual inline void SetDatabase(ASSInterface::Ref<ASSInterface::Database> db) = 0;		
		virtual inline const Rx::observable<IdentitySpecification> GetIdentify() const = 0;		
		virtual std::any GetExecuteTask() = 0; 
		virtual inline void CloseConnection() = 0;
		virtual inline void ResetTask() = 0;
		virtual inline void ResetTrack() = 0;
	private:
		virtual char* DescriptionFlow(int optionFlow, std::string descriptionFlow) = 0;
		virtual std::string ConvertBackslashToSlash(std::string input) = 0;
	};

	class ASSStreamer : public Stream {
	public:		
		static Ref<ASSStreamer> Create(int channel);
	};
}