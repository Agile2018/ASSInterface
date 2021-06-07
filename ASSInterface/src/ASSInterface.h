#pragma once

// For use ASSInterface applications

#include "ASSInterface/Core/Application.h"
#include "ASSInterface/Core/Timestep.h"
#include "ASSInterface/Core/Input.h"
#include "ASSInterface/Core/KeyCodes.h"
#include "ASSInterface/Core/MouseButtonCodes.h"
#include "ASSInterface/Core/Log.h"
#include "ASSInterface/Core/Layer.h"
#include "ASSInterface/ImGui/ImGuiLayer.h"

//----Renderer-----------------------

#include "ASSInterface/Renderer/Renderer.h"
#include "ASSInterface/Renderer/RenderCommand.h"
#include "ASSInterface/Renderer/Buffer.h"
#include "ASSInterface/Renderer/Shader.h"
#include "ASSInterface/Renderer/Texture.h"
#include "ASSInterface/Renderer/VertexArray.h"
#include "ASSInterface/Renderer/OrthographicCamera.h"
#include "ASSInterface/Renderer/OrthographicCameraController.h"
#include "ASSInterface/Renderer/Framebuffer.h"

//Database
#include "ASSInterface/Database/Database.h"

//Stream
#include "ASSInterface/Streamer/Stream.h"

//Tools

#include "ASSInterface/Utils/DateTime.h"
#include "ASSInterface/Utils/Base64.h"
#include "ASSInterface/Utils/TransformImage.h"
#include "ASSInterface/Utils/Math.h"

//Recognition
#include "ASSInterface/Recognition/Detection.h"
#include "ASSInterface/Recognition/Identification.h"
#include "ASSInterface/Recognition/Tracking.h"
#include "ASSInterface/Recognition/InitLibrary.h"
#include "ASSInterface/Recognition/ErrorRecognition.h"
#include "ASSInterface/Recognition/ErrorIdentification.h"
#include "ASSInterface/Recognition/ExecuteTask.h"

//Configuration

#include "ASSInterface/Configuration/Configuration.h"
#include "ASSInterface/Utils/File.h"

//Language
#include "ASSInterface/Core/IndexLabel.h"
#include "ASSInterface/Core/Label.h"

//Graphic processor
#include "ASSInterface/Hardware/GraphicProcessor.h"

//Debug

#include "ASSInterface/ImGui/ImGuiLog.h"

//Utils

#include "ASSInterface/Utils/avir.h"

//Trace error
//#include <ASSInterface/Debug/ErrorInstrumentor.h>