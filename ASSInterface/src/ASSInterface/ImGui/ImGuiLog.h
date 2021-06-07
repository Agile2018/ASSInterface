#pragma once

namespace ASSInterface {
	class ImGuiLog {
	public:
		static void Init();
		static void Clear();
		static void AddLog(const char* fmt, ...);
		static void Draw(const char* title, bool* p_open = NULL);
		static const char* Categories(int index);
	};
}