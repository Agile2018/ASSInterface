workspace "ASSInterface"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "ASSInterface/vendor/GLFW/include"
IncludeDir["Glad"] = "ASSInterface/vendor/Glad/include"
IncludeDir["ImGui"] = "ASSInterface/vendor/imgui"
IncludeDir["glm"] = "ASSInterface/vendor/glm"
IncludeDir["Rx"] = "ASSInterface/vendor/Rx/include"
IncludeDir["SOIL2"] = "ASSInterface/vendor/SOIL2/src/SOIL2"
IncludeDir["boost"] = "ASSInterface/vendor/boost"
IncludeDir["opencv"] = "ASSInterface/vendor/opencv/include"
IncludeDir["nlohmann_json"] = "ASSInterface/vendor/nlohmann_json/include"
IncludeDir["TBB"] = "ASSInterface/vendor/TBB/include"
IncludeDir["CUDA"] = "ASSInterface/vendor/CUDA/include"
IncludeDir["IDKitSDK"] = "ASSInterface/vendor/Innovatrics/IDKitSDK/include"
IncludeDir["IFaceSDK"] = "ASSInterface/vendor/Innovatrics/IFaceSDK/include"

group "Dependencies"
	include "ASSInterface/vendor/GLFW"
	include "ASSInterface/vendor/Glad"
	include "ASSInterface/vendor/imgui"
	include "ASSInterface/vendor/SOIL2"	
group ""


project "ASSInterface"
	location "ASSInterface"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzpch.h"
	pchsource "ASSInterface/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/Rx/include/**.hpp",
		"%{prj.name}/vendor/mongo-c-driver/include/libmongoc-1.0/**.h",
		"%{prj.name}/vendor/mongo-c-driver/include/libbson-1.0/**.h",
		"%{prj.name}/vendor/mongo-cxx-driver/install/include/bsoncxx/v_noabi/bsoncxx/**.hpp",
		"%{prj.name}/vendor/mongo-cxx-driver/install/include/mongocxx/v_noabi/mongocxx/**.hpp",
		"%{prj.name}/vendor/opencv/include/opencv2/**.h",
		"%{prj.name}/vendor/opencv/include/opencv2/**.hpp",
		"%{prj.name}/vendor/nlohmann_json/include/nlohmann/**.hpp",
		"%{prj.name}/vendor/TBB/include/tbb/**.h",
		"%{prj.name}/vendor/TBB/include/oneapi/**.h",
		"%{prj.name}/vendor/TBB/include/oneapi/tbb/**.h",
		"%{prj.name}/vendor/CUDA/include/**.h",
		"%{prj.name}/vendor/CUDA/include/**.hpp",
		"%{prj.name}/vendor/Innovatrics/IDKitSDK/include/**.h",
		"%{prj.name}/vendor/Innovatrics/IFaceSDK/include/**.h"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",				
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.SOIL2}",
		"%{IncludeDir.opencv}",
		"%{IncludeDir.Rx}",
		"%{IncludeDir.boost}",
		"%{IncludeDir.nlohmann_json}",
		"%{IncludeDir.TBB}",
		"%{IncludeDir.CUDA}",
		"%{IncludeDir.IDKitSDK}",
		"%{IncludeDir.IFaceSDK}",
		"%{prj.name}/vendor/GStreamer/include",
		"%{prj.name}/vendor/GStreamer/include/glib-2.0",
		"%{prj.name}/vendor/GStreamer/include/gstreamer-1.0",
		"%{prj.name}/vendor/GStreamer/lib/glib-2.0/include",	
		"%{prj.name}/vendor/mongo-c-driver/include/libmongoc-1.0",
		"%{prj.name}/vendor/mongo-c-driver/include/libbson-1.0",
		"%{prj.name}/vendor/mongo-cxx-driver/install/include/bsoncxx/v_noabi",
		"%{prj.name}/vendor/mongo-cxx-driver/install/include/mongocxx/v_noabi"		
	}

	libdirs 
	{ 
		"ASSInterface/vendor/opencv/x64/vc16/lib",
		"ASSInterface/vendor/TBB/lib"
	}

	links
	{
		"GLFW",		
		"Glad",
		"ImGui",
		"SOIL2",
		"opengl32.lib",		
		"ASSInterface/vendor/GStreamer/lib/gobject-2.0.lib",
		"ASSInterface/vendor/GStreamer/lib/glib-2.0.lib",
		"ASSInterface/vendor/GStreamer/lib/gstreamer-1.0.lib",
		"ASSInterface/vendor/GStreamer/lib/gstapp-1.0.lib",
		"ASSInterface/vendor/mongo-cxx-driver/install/lib/bsoncxx.lib",
		"ASSInterface/vendor/mongo-cxx-driver/install/lib/mongocxx.lib",
		"ASSInterface/vendor/CUDA/lib/cudart_static.lib",
		"ASSInterface/vendor/Innovatrics/IDKitSDK/lib/idkit.lib",
		"ASSInterface/vendor/Innovatrics/IFaceSDK/lib/iface.lib"
	}

	filter "system:windows"				
		systemversion "latest"

		defines
		{
			"ASS_PLATFORM_WINDOWS",
			"ASS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "ASS_DEBUG"
		runtime "Debug"
		symbols "on"		

		links
		{						
			"ASSInterface/vendor/TBB/lib/tbb12_debug.lib"
		}

	filter "configurations:Release"
		defines "ASS_RELEASE"
		runtime "Release"
		optimize "on"		

		links
		{						
			"ASSInterface/vendor/TBB/lib/tbb12.lib"
		}

	filter "configurations:Dist"
		defines "ASS_DIST"
		runtime "Release"
		optimize "on"		

		links
		{						
			"ASSInterface/vendor/TBB/lib/tbb12.lib"
		}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"	
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"ASSInterface/vendor/spdlog/include",		
		"ASSInterface/src",
		"ASSInterface/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.SOIL2}",
		"%{IncludeDir.Rx}",
		"%{IncludeDir.JSONCPP}",
		"%{IncludeDir.TBB}",
		"%{IncludeDir.CUDA}",
		"%{IncludeDir.IDKitSDK}",
		"%{IncludeDir.IFaceSDK}",
		"ASSInterface/vendor/opencv/include"
	}	

	links
	{
		"ASSInterface"
	}

	filter "system:windows"			
		systemversion "latest"		

	filter "configurations:Debug"
		defines "ASS_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"ASSInterface/vendor/opencv/x64/vc16/lib/opencv_world451d.lib",
			"ASSInterface/vendor/TBB/lib/tbb_debug.lib",
			"ASSInterface/vendor/TBB/lib/tbb12_debug.lib"
		}

	filter "configurations:Release"
		defines "ASS_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"ASSInterface/vendor/opencv/x64/vc16/lib/opencv_world451.lib",
			"ASSInterface/vendor/TBB/lib/tbb.lib",
			"ASSInterface/vendor/TBB/lib/tbb12.lib"
		}

	filter "configurations:Dist"
		defines "ASS_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"ASSInterface/vendor/opencv/x64/vc16/lib/opencv_world451.lib",
			"ASSInterface/vendor/TBB/lib/tbb.lib",
			"ASSInterface/vendor/TBB/lib/tbb12.lib"
		}