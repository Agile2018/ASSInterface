#pragma once
#include "hzpch.h"

#include "ASSInterface/Core/Core.h"


namespace ASSInterface {

	class ASS_API Stream {
	public:
		virtual ~Stream() = default;
		virtual void Run() = 0;
		virtual void FinishLoop() = 0;
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual int GetChannels() const = 0;
		virtual int GetSizeImage() const = 0;
		virtual int GetFPS() const = 0;
		virtual int GetElapse() const = 0;
		virtual int GetSourceFlow() const = 0;
		virtual float GetSizeHeightTexture() const = 0;
		virtual std::vector<unsigned char> GetDataTexture() = 0;
		virtual std::vector<unsigned char> GetDataRaw() = 0;
	private:
		virtual char* DescriptionFlow(int optionFlow, std::string descriptionFlow) = 0;
		virtual std::string ConvertBackslashToSlash(std::string input) = 0;
	};

	class ASSStreamer : public Stream {
	public:
		static Ref<ASSStreamer> Create(int channel);
	};
}