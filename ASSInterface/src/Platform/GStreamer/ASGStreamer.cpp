#include "hzpch.h"
#include <fstream>
#include "ASGStreamer.h"


namespace ASSInterface {

	Rx::subject<std::tuple<int, int, int, int>> dimensionsImageSubject;
	Rx::observable<std::tuple<int, int, int, int>> observableDimensionsImage = dimensionsImageSubject.get_observable();
	Rx::subscriber<std::tuple<int, int, int, int>> shootDimensionsImage = dimensionsImageSubject.get_subscriber();

	Rx::subjects::subject<std::tuple<unsigned char*, int, int>> imageDataSubject;
	Rx::observable<std::tuple<unsigned char*, int, int>> observableImageData = imageDataSubject.get_observable();
	Rx::subscriber<std::tuple<unsigned char*, int, int>> shootImageData = imageDataSubject.get_subscriber();

	ASGStreamer::ASGStreamer(int channel)
	{
		indexChannel = channel;
		ObserverFlow();
		managerFile = ASSInterface::File::Create();			
		trackingInno = ASSInterface::Tracking::CreateInnovatrics(indexChannel);
		innoTask = ASSInterface::ExecuteTask::CreateInnoTask(indexChannel);
		std::string descriptionFlow;
		std::string nameFile = "video" + std::to_string(indexChannel) + ".txt";
		std::string pathFile = managerFile->GetFolderConfiguration() + "/" + nameFile;

		if (managerFile->IsFileExists(pathFile)) {	

			configuration = ASSInterface::Configuration::CreateConfigVideo(nameFile);
			configuration->ParseToObject();
			std::any anyLbl = configuration->GetParam("label");
			nameWindow = std::any_cast<std::string>(anyLbl);
			std::any anySrc = configuration->GetParam("sourceFlow");
			if (anySrc.has_value())
			{
				optionSourceFlow = std::any_cast<int>(anySrc);
				switch (optionSourceFlow)
				{
				case 1:
					descriptionFlow = std::any_cast<std::string>(configuration->GetParam("ipCamera"));
					break;
				case 2:
					descriptionFlow = std::any_cast<std::string>(configuration->GetParam("fileVideo"));
					break;
				case 3:
					descriptionFlow = std::any_cast<std::string>(configuration->GetParam("deviceVideo"));
					break;
				default:
					break;
				}

				if (!descriptionFlow.empty())
				{
					description = DescriptionFlow(optionSourceFlow, descriptionFlow);
					ASS_INFO("Description flow: {0}", description);
					correctDescription = true;
				
				}
				else
				{
					ASS_ERROR("Description flow empty");
					const char* err = "Description flow empty";
					ASS_ERROR_PROFILE_SCOPE("ASGStreamer::ASGStreamer", err);
				}

			}
			else
			{
				ASS_ERROR("File config empty");
				const char* err = "File config empty";
				ASS_ERROR_PROFILE_SCOPE("ASGStreamer::ASGStreamer", err);				
			}
		}
		else
		{
			ASS_ERROR("File not exists.");
			const char* err = "File not exists.";
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::ASGStreamer", err);						
		}

	}
	ASGStreamer::~ASGStreamer()
	{
		trackingInno->Terminate();
		innoTask->CloseConnection();
	}

	void ASGStreamer::Run()
	{
		ASS_PROFILE_FUNCTION();
		
		gst_init(nullptr, nullptr);

		/*if (optionSourceFlow == 3)
		{
			source = gst_element_factory_make("ksvideosrc", "source");
			gst_element_set_state(source, GST_STATE_READY);
		}*/
		

		GError* gError = nullptr;
		pipeline = gst_parse_launch(description, &gError);
		if (gError) {
			ASS_ERROR("Could not construct pipeline: {0}", gError->message);			
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::Run", gError->message);
			g_error_free(gError);
			correctStart = false;
			return;
			//exit(-1);
		}

		GstElement* sink = gst_bin_get_by_name(GST_BIN(pipeline), "sink");

		gst_app_sink_set_emit_signals((GstAppSink*)sink, true);
		gst_app_sink_set_drop((GstAppSink*)sink, true);
		gst_app_sink_set_max_buffers((GstAppSink*)sink, indexChannel);

		GstAppSinkCallbacks* appsink_callbacks = (GstAppSinkCallbacks*)malloc(sizeof(GstAppSinkCallbacks));
		appsink_callbacks->eos = nullptr;
		appsink_callbacks->new_preroll = nullptr;
		appsink_callbacks->new_sample = &ASGStreamer::CaptureGstBuffer;
		gst_app_sink_set_callbacks(GST_APP_SINK(sink), appsink_callbacks, (gpointer)nullptr, free);
		
		bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
		gst_bus_add_watch(bus, &ASGStreamer::MessageCallback, nullptr);
		gst_object_unref(bus);

		GstStateChangeReturn ret;
		ret = gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
		if (ret == GST_STATE_CHANGE_FAILURE) {
			correctStart = false;
			ASS_ERROR("Unable to set the pipeline to the playing state.");
			const char* err = "Unable to set the pipeline to the playing state.";
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::Run", err);
			gst_object_unref(pipeline);
			return;
			//exit(-1);
		}
		
		loop = g_main_loop_new(nullptr, false);
		
		g_main_loop_run(loop);

		gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_NULL);
		gst_object_unref(GST_OBJECT(pipeline));
		
	}

	void ASGStreamer::FinishLoop()
	{
		if (pipeline != NULL)
		{

			if (gst_element_send_event(GST_ELEMENT(pipeline), gst_event_new_eos()))
			{
				ASS_INFO("Finish pipeline");
				
				
			}

		}
	}

	void ASGStreamer::Play()
	{
		GstState cur_state;
		if (pipeline != NULL)
		{
			gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
			if (cur_state == GST_STATE_PAUSED) {
				gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
			}
		}
	}

	void ASGStreamer::Stop()
	{
		GstState cur_state;
		if (pipeline != NULL)
		{
			gst_element_get_state(GST_ELEMENT(pipeline), &cur_state, NULL, 0);
			if (cur_state == GST_STATE_PLAYING) {
				gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
			}
		}
	}

	std::vector<unsigned char> ASGStreamer::GetDataTexture()
	{
		std::vector<unsigned char> dataTemp;
		if (mtxTexture.try_lock()) {
			dataTemp = dataImage;
			mtxTexture.unlock();
		}
		return dataTemp;
	}	

	float* ASGStreamer::GetCoordinates()
	{
		
		float* coordinatesFace = trackingInno->GetCoordinates();		
		float highAdjusted = sizeHeightTexture * 2.0f;

		ClearAllCoordinatesImage();
		
		for (int i = 0; i < COORDINATES_X_ALL_IMAGES; i += 4) {
			
			if (coordinatesFace[i] != 0) {
				
				float x = coordinatesFace[i];
				float y = coordinatesFace[i + 1];
				float w = coordinatesFace[i + 2];
				float h = coordinatesFace[i + 3];

				float xc = (x + (w / 2.0f));
				float yc = (y + (h / 2.0f));

				float px = ((xc * 100.0f) / (float)m_Width);
				float py = ((yc * 100.0f) / (float)m_Height);

				float xr = (px * 2.0f) / 100.0f;
				float yr = (py * highAdjusted) / 100.0f;

				if (xr < 1.0f)
				{
					xr = (1.0f - xr) * -1.0f;
				}

				if (xr >= 1.0f)
				{
					xr = xr - 1.0f;
				}

				if (yr < sizeHeightTexture)
				{
					yr = (sizeHeightTexture - yr);
				}

				if (yr >= sizeHeightTexture)
				{
					yr = (yr - sizeHeightTexture) * -1.0f;
				}

				float prw = ((w / 2.0f) * 100.0f / (float)m_Width);
				float prh = ((h / 2.0f) * 100.0f / (float)m_Height);
				float ratioWidth = (prw * 2.0f) / 100.0f;
				float ratioHeight = (prh * highAdjusted) / 100.0f;

				imageCoordinatesFollowed[i] = xr;
				imageCoordinatesFollowed[i + 1] = yr;
				imageCoordinatesFollowed[i + 2] = ratioWidth;
				imageCoordinatesFollowed[i + 3] = ratioHeight;

			}

		}
		
		return imageCoordinatesFollowed;
	}

	void ASGStreamer::InitTracking()
	{
		
		trackingInno->SetWidth(m_Width);
		trackingInno->SetHeight(m_Height);
		trackingInno->SetElapseFrame(m_Elapse);
		trackingInno->Init();		
		ObserverTracking();
		ObserverIdentify();
		//Temp
		/*trackingOpenCV = ASSInterface::Tracking::CreateOpenCV();
		trackingOpenCV->SetWidth(m_Width);
		trackingOpenCV->SetHeight(m_Height);
		trackingOpenCV->Init();*/
		isInitTracking = true;
	}

	char* ASGStreamer::DescriptionFlow(int optionFlow, std::string descriptionFlow)
	{
		ASS_PROFILE_FUNCTION();

		gchar* description = nullptr;
		
		switch (optionFlow) {
		case 1: // IP CAMERA
			description = g_strdup_printf(
				"rtspsrc location=%s "
				"! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
				"! decodebin ! videoconvert "
				"! video/x-raw, format=(string)I420 "
				"! jpegenc quality=100 "
				"! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true",
				descriptionFlow.c_str()
			); 

			//Temp OpenCV
			/*description = g_strdup_printf(
				"rtspsrc location=%s "
				"! application/x-rtp, payload=96 ! rtph264depay ! h264parse ! avdec_h264 "
				"! decodebin ! videoconvert "
				"! video/x-raw, format=(string)BGR "
				"! appsink name=sink emit-signals=true sync=false max-buffers=1 drop=true",
				descriptionFlow.c_str()
			);*/

			break;
		case 2: // FILE		

			//description = g_strdup_printf(

			//	"filesrc location=%s "
			//	"! decodebin ! videoconvert "
			//	"! video/x-raw, format=(string)YUY2 "
			//	"! jpegenc quality=100 "
			//	"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
			//	ConvertBackslashToSlash(descriptionFlow).c_str() // I420  , width=(int)1024, height=(int)768  ! videoscale method=0
			//);

			description = g_strdup_printf(

				"filesrc location=%s "
				"! decodebin ! videoconvert "
				"! video/x-raw, format=(string)I420 "	
				"! jpegenc quality=100 "
				"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
				ConvertBackslashToSlash(descriptionFlow).c_str() //BGR  ! aspectratiocrop aspect-ratio=16/11
			);

			break;
		case 3: // CAMERA   
			/*description = g_strdup_printf(
				"v4l2src device=%s "
				"! decodebin ! videoconvert "
				"! video/x-raw, format=(string)I420, width=(int)640, height=(int)480 "
				"! jpegenc "
				"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
				descriptionFlow.c_str()
			);*/
			description = g_strdup_printf(
				"ksvideosrc device-index=%s "
				"! decodebin ! videoconvert "
				"! video/x-raw, format=(string)YUY2, width=(int)640, height=(int)480 "
				"! jpegenc quality=100 "
				"! appsink name=sink emit-signals=true sync=true max-buffers=1 drop=true",
				descriptionFlow.c_str()
			); // device-path 
			break;

		}
		return description;
	}

	std::string ASGStreamer::ConvertBackslashToSlash(std::string input)
	{
		std::string output;
		for (auto character : input)
		{
			switch (character)
			{
			case '\\':
				output.push_back('/');
				break;			
			default:
				output.push_back(character);
				break;
			}
			
		}
		return "\"" + output + "\"";
	}

	GstFlowReturn ASGStreamer::CaptureGstBuffer(GstAppSink* appsink, gpointer)
	{
		ASS_PROFILE_FUNCTION();

		int currentChannel = gst_app_sink_get_max_buffers(appsink);

		GstSample* sample = gst_app_sink_pull_sample(appsink);
		if (sample == NULL) {
			ASS_ERROR("Null gst_app_sink_pull_sample.");
			const char* err = "Null gst_app_sink_pull_sample.";
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::CaptureGstBuffer", err);
			return GstFlowReturn::GST_FLOW_ERROR;
		}

		GstBuffer* buffer = gst_sample_get_buffer(sample);
		GstCaps* caps = gst_sample_get_caps(sample);
		//ASS_INFO("Caps flow of image: {0}", gst_caps_to_string(caps));
		GstStructure* structure = gst_caps_get_structure(caps, 0);
		int width = g_value_get_int(gst_structure_get_value(structure, "width"));
		int height = g_value_get_int(gst_structure_get_value(structure, "height"));
		int numerator = gst_value_get_fraction_numerator(gst_structure_get_value(structure, "framerate"));
		int denominator = gst_value_get_fraction_denominator(gst_structure_get_value(structure, "framerate"));
		//const gchar* name_ = gst_structure_get_name(structure);
		//std::string name = std::string(name_);
		//ASS_INFO("Name Structure: {0}", name);

		int frameRate = numerator / ((denominator == 0) ? 1 : denominator);
		auto tupleDimensions = std::make_tuple(width, height, frameRate, currentChannel);
		shootDimensionsImage.on_next(tupleDimensions);			
					
		GstMapInfo map_info;

		if (!gst_buffer_map((buffer), &map_info, GST_MAP_READ)) {
			gst_buffer_unmap((buffer), &map_info);
			gst_sample_unref(sample);
			ASS_ERROR("False gst_buffer_map.");
			const char* err = "False gst_buffer_map.";
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::CaptureGstBuffer", err);
			return GstFlowReturn::GST_FLOW_ERROR;
		}

		if (map_info.data != NULL) {

			auto tupleImageData = std::make_tuple(map_info.data, (int)map_info.size, currentChannel);
			shootImageData.on_next(tupleImageData);

		}
		gst_buffer_unmap(buffer, &map_info);
		gst_sample_unref(sample);
		
		return GstFlowReturn::GST_FLOW_OK;
	}

	gboolean ASGStreamer::MessageCallback(GstBus* bus, GstMessage* message, gpointer data)
	{
		switch (GST_MESSAGE_TYPE(message)) {
		case GST_MESSAGE_ERROR: {
			GError* err;
			gchar* debug;

			gst_message_parse_error(message, &err, &debug);
			ASS_ERROR("Error: {0}", err->message);
			ASS_ERROR_PROFILE_SCOPE("ASGStreamer::MessageCallback", err->message);
			g_error_free(err);
			g_free(debug);
			break;
		}
		case GST_MESSAGE_EOS:
			//g_main_loop_quit(loop);			
			ASS_INFO("End of stream");
			break;
		default:
			break;
		}

		return true;
	}
		
	void ASGStreamer::ObserverFlow()
	{
		auto dimensionsObservable = observableDimensionsImage.map([](std::tuple<int, int, int, int> dimensionsImage) {
			return dimensionsImage;
			});

		auto subscriptionDimensions = dimensionsObservable.subscribe([this](std::tuple<int, int, int, int> dimensionsImage) {
			InitDimensionsImage(dimensionsImage);

		});

		subscriptionDimensions.clear();

		auto imageDataObservable = observableImageData.map([](std::tuple<unsigned char*, int, int> dataImg) {
			return dataImg;
			});

		auto subscriptionImageData = imageDataObservable.subscribe([this](std::tuple<unsigned char*, int, int> dataImg) {

			SetDataImage(std::get<0>(dataImg), std::get<1>(dataImg), std::get<2>(dataImg));
		});

		subscriptionImageData.clear();
	}

	void ASGStreamer::InitDimensionsImage(std::tuple<int, int, int, int> dimensionsImage)
	{
		
		if (!isInitChannel && indexChannel == std::get<3>(dimensionsImage))
		{
			isInitChannel = true;
			correctStart = true;
			m_Width = std::get<0>(dimensionsImage);
			m_Height = std::get<1>(dimensionsImage);
			m_Fps = std::get<2>(dimensionsImage);

			if (m_Fps != 0)
			{
				m_Elapse = 1000 / m_Fps;
			}
			else
			{
				m_Fps = 30;
				m_Elapse = 1000 / m_Fps;
			}

			if (m_Height != 0)
			{
				float relation = (float)m_Width / (float)m_Height;
				sizeHeightTexture = 1.0f / relation;
			}			
			
		}		
	}

	void ASGStreamer::SetDataImage(unsigned char* data, int size, int channel)
	{
		if (indexChannel == channel) {
			
			ASS_PROFILE_FUNCTION();
			//clock_t timeStart1 = clock();
						
			std::vector<unsigned char> dataRawImage(data, data + size);
			
			//Temp
			
			/*tbb::parallel_invoke(
				[this, flow]() {BranchTracking(flow); },
				[this, data, size]() {BranchVideo(data, size); }
			);*/

			if (isInitTracking && !trackingInno->IsTrackFinish())
			{
				std::thread tt(&ASGStreamer::BranchTracking, this, dataRawImage);
				tt.detach();

				/*std::vector<unsigned char> dataDetected = trackingOpenCV->GetFlowData();
				BranchVideo(dataDetected);*/
			}
			
			BranchVideo(data, size);
			
			/*clock_t duration1 = clock() - timeStart1;
			int durationMs1 = int(1000 * ((float)duration1) / CLOCKS_PER_SEC);

			ASS_INFO("Time Load Image {0}", durationMs1);*/
		}
		
	}
	void ASGStreamer::BranchTracking(std::vector<unsigned char> data)
	{
		trackingInno->Detect(data);										
	}

	void ASGStreamer::BranchVideo(std::vector<unsigned char> data) {
		int width, height;

		unsigned char* dataTemp = SOIL_load_image_from_memory(&data[0],
			(int)data.size(), &width, &height, &m_Channels, SOIL_LOAD_RGB);

		m_SizeImage = width * height * m_Channels;

		mtxTexture.lock();
		dataImage.clear();
		dataImage.assign(dataTemp, dataTemp + m_SizeImage);
		mtxTexture.unlock();
		SOIL_free_image_data(dataTemp);
	}
	void ASGStreamer::ClearCoordinatesImage(int indexTracked)
	{
		int index = indexTracked * NUM_COORDINATES_X_IMAGE;
		imageCoordinatesFollowed[index] = 0.0f;
		imageCoordinatesFollowed[index + 1] = 0.0f;
		imageCoordinatesFollowed[index + 2] = 0.0f;
		imageCoordinatesFollowed[index + 3] = 0.0f;
	}
	void ASGStreamer::ClearAllCoordinatesImage()
	{
		for (int i = 0; i < NUM_TRACKED_OBJECTS; i++) {
			ClearCoordinatesImage(i);
		}
	}
	void ASGStreamer::ObserverTracking()
	{
		auto trackObservable = trackingInno->Get()
			.map([](DetectSpecification specDetect) {
			return specDetect;
		});

		auto subscriptionTrack = trackObservable
			.subscribe([this](DetectSpecification specDetect) {			
			innoTask->DoTask(specDetect);
		});

		subscriptionTrack.clear();		
	}

	void ASGStreamer::ObserverIdentify()
	{
		auto taskObservable = innoTask->Get()
			.map([](IdentitySpecification specIdentify) {
			return specIdentify;
		});

		auto subscriptionTask = taskObservable
			.subscribe([this](IdentitySpecification specIdentify) {

			if (!specIdentify.cropData.empty())
			{
				shootSpecIdentity.on_next(specIdentify);

			}

		});
		subscriptionTask.clear();

	}

	void ASGStreamer::BranchVideo(unsigned char* data, int size)
	{
		
		int width = 0, height = 0, lenght = 0;

		unsigned char* copyFlow = new unsigned char[size];
		memcpy(copyFlow, data, size);

		unsigned char* dataTemp = SOIL_load_image_from_memory(copyFlow,
			size, &width, &height, &m_Channels, SOIL_LOAD_AUTO);

		if (m_SizeImage == 0)
		{
			m_SizeImage = width * height * m_Channels;
		}

		lenght = width * height * m_Channels;
		if (lenght == m_SizeImage)
		{
			mtxTexture.lock();
			dataImage.clear();			
			dataImage.assign(dataTemp, dataTemp + m_SizeImage);
			mtxTexture.unlock();
		}
						
		SOIL_free_image_data(dataTemp);
		delete[] copyFlow;

	}
}