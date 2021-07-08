#pragma once

#include <memory>

#ifdef _WIN32
/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define ASS_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define ASS_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define ASS_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		error "Unknown Apple platform!"
	#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define ASS_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define ASS_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection


// DLL support
#ifdef ASS_PLATFORM_WINDOWS
	#if ASS_DYNAMIC_LINK
		#ifdef ASS_BUILD_DLL
			#define ASS_API __declspec(dllexport)
		else
			#define ASS_API __declspec(dllimport)
		#endif
	#else
		#define ASS_API
	#endif
#else
	#error ASSInterface only supports Windows!
#endif // End of DLL support

#ifdef ASS_DEBUG
	#define ASS_ENABLE_ASSERTS
#endif

#ifdef ASS_ENABLE_ASSERTS
	#define ASS_ASSERT(x, ...) { if(!(x)) { ASS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ASS_CORE_ASSERT(x, ...) { if(!(x)) { ASS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define ASS_ASSERT(x, ...)
	#define ASS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define ASS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define MAX_SIZE_IMAGE 640
#define MAX_CHANNELS 4
#define NUM_TRACKED_OBJECTS	5
#define NUM_COORDINATES_X_IMAGE  4
#define COORDINATES_X_ALL_IMAGES 20
#define ENROLL_PERSON 0
#define IMPORT_FACE 1
#define CONTROL_ENTRY 2
#define CONTROL_ENTRY_TRACK0 3
#define CONTROL_ENTRY_TRACK1 4
#define CONTROL_ENTRY_TRACK2 5
#define ENROLL_WITH_TEMPLATES 6
#define ADD_TEMPLATES_OF_FILES 7
#define SET_TEMPLATE_OF_FILES 8
#define EDIT_PERSON 9
#define SIMILARITY_THRESHOLD_IDENTIFICATION 1
#define SIMILARITY_THRESHOLD_DEDUPLICATION 2
#define SIMILARITY_THRESHOLD_VERIFICATION 3
#define BEST_CANDIDATES 10
#define BATCH_TOTAL_SIZE  30
#define EMPTY_FACE 0
#define MB 1024*1024

namespace ASSInterface {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
