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

//Stream
#include "ASSInterface/Streamer/Stream.h"

//Database
#include "ASSInterface/Database/Database.h"

//Tools

#include "ASSInterface/Utils/DateTime.h"
#include "ASSInterface/Utils/Base64.h"
#include "ASSInterface/Utils/TransformImage.h"

//Recognition

#include "ASSInterface/Recognition/Detection.h"
#include "ASSInterface/Recognition/Identification.h"
#include "ASSInterface/Recognition/Tracking.h"

//Configuration

#include "ASSInterface/Configuration/Configuration.h"
#include "ASSInterface/Utils/File.h"

