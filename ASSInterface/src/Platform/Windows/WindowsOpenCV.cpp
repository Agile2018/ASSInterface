#include "hzpch.h"
#include "WindowsOpenCV.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace ASSInterface {
	WindowsOpenCV::WindowsOpenCV()
	{
	}
	WindowsOpenCV::~WindowsOpenCV()
	{
	}
	std::vector<unsigned char> WindowsOpenCV::ImenCode(std::vector<unsigned char> flowBytes, 
		int rows, int cols) {
		std::vector<uchar> bufferImage;		

		try
		{
			/*unsigned char* copyFlow = new unsigned char[flowBytes.size()];
			memcpy(copyFlow, &flowBytes[0], flowBytes.size());*/

			cv::Mat img = cv::Mat(rows, cols, CV_8UC3);
			img.data = &flowBytes[0]; //

			//int cnn = (int)flowBytes.size() / (rows * cols);
			
			//cv::Mat img = cv::imdecode(flowBytes, cv::IMREAD_COLOR);

			//if (cnn == 3)
			//{
				int params[3] = { 0 };
				params[0] = cv::IMWRITE_JPEG_QUALITY;
				params[1] = 100;

				/*std::vector<int> compression_params;
				compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
				compression_params.push_back(9);*/

				if (!img.empty()) {
					bool code = cv::imencode(".jpg", img,
						bufferImage, std::vector<int>(params, params + 2));
					if (!code)
					{
						bufferImage.clear();
					}
				}
			//}						
		}
		catch (cv::Exception& ex)
		{
			ASS_ERROR("ERROR IMENCODE: {0}", ex.what());
		}
		
		return bufferImage;
	}

	std::vector<unsigned char> WindowsOpenCV::ImDecode(std::vector<unsigned char> flowBytes)
	{
		std::vector<uchar> bufferImage;

		cv::Mat srcMat = cv::imdecode(flowBytes, cv::IMREAD_COLOR);

		int params[3] = { 0 };
		params[0] = cv::IMWRITE_JPEG_QUALITY; //cv::IMWRITE_PNG_COMPRESSION;
		params[1] = 100;

		bool code = cv::imencode(".jpg", srcMat,
			bufferImage, std::vector<int>(params, params + 2));

		return bufferImage;
	}

	std::vector<unsigned char> WindowsOpenCV::Resize(std::vector<unsigned char> flowBytes, int rows, int cols) {
		std::vector<uchar> bufferImage;				

		int params[3] = { 0 };
		params[0] = cv::IMWRITE_JPEG_QUALITY;
		params[1] = 100;

		float relationSize = (float)cols / (float)rows;
		int newWidth = MAX_SIZE_IMAGE;
		int newHeight = (int)((float)newWidth / relationSize);

		cv::Mat srcMat = cv::imdecode(flowBytes, cv::IMREAD_COLOR);	
				
		cv::Mat newImg;

		cv::resize(srcMat, newImg, cv::Size(newWidth, newHeight));

		bool code = cv::imencode(".jpg", srcMat,
			bufferImage, std::vector<int>(params, params + 2));
		
		return bufferImage;
	}

	std::vector<unsigned char> WindowsOpenCV::Resize(const char* fileName, int height, int width) {
		std::vector<uchar> bufferImage;
		/*int params[3] = { 0 };
		params[0] = cv::IMWRITE_JPEG_QUALITY;
		params[1] = 100;*/

		std::vector<int> compression_params;
		compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
		compression_params.push_back(100);		

		float relationSize = (float)width / (float)height;
		int newWidth = MAX_SIZE_IMAGE;
		int newHeight = (int)((float)newWidth / relationSize);


		cv::Mat img = cv::imread(fileName);
		cv::Mat newImg;
		cv::resize(img, newImg, cv::Size(newWidth, newHeight));

		//int m_width, m_height, m_channels;

		//int sizeImg = newWidth * newHeight * newImg.channels();
		//unsigned char* dataTemp = SOIL_load_image_from_memory(newImg.data,
		//	sizeImg, &m_width, &m_height, &m_channels, SOIL_LOAD_RGB);

		//int m_SizeImage = m_width * m_height * m_channels;
		//bufferImage.assign(dataTemp, dataTemp + m_SizeImage);

		////cv::imwrite("opencv.jpg", newImg);
		//
		//

		//return bufferImage;
		try
		{
			bool code = cv::imencode(".jpg", newImg, bufferImage, compression_params);
			if (code)
			{
				cv::imwrite("opencv.jpg", newImg);
				ASS_INFO("Good");
			}
		}
		catch (cv::Exception& ex)
		{
			ASS_ERROR("WindowsOpenCV::WriteImage {0} ", ex.what());
		}
		 //std::vector<int>(params, params + 2)
		/*cv::Mat img1 = cv::Mat(newHeight, newWidth, CV_8UC3);
		img.data = &bufferImage[0];*/
		

		return bufferImage;
	}

	unsigned char* WindowsOpenCV::Resize(unsigned char* dataImage, 
		int* width, int* height, int* lenght)
	{

		avir::CImageResizer<> ImageResizer(8);

		float relationSize = (float)*width / (float)*height;
		int newWidth = MAX_SIZE_IMAGE;
		int newHeight = (int)((float)newWidth / relationSize);

		*lenght = newWidth * newHeight * 3;
		unsigned char* flowNewImage = new unsigned char[*lenght];

		ImageResizer.resizeImage(dataImage, *width, *height, 0, 
			flowNewImage, newWidth, newHeight, 3, 0);
		*width = newWidth;
		*height = newHeight;

		return flowNewImage;
	}

	bool WindowsOpenCV::GetSize(const char* fileName, int* x, int* y) {
		FILE* f = fopen(fileName, "rb");

		if (f == 0) return false;

		fseek(f, 0, SEEK_END);
		long len = ftell(f);
		fseek(f, 0, SEEK_SET);

		if (len < 24) {
			fclose(f);
			return false;
		}

		// Strategy:
		// reading GIF dimensions requires the first 10 bytes of the file
		// reading PNG dimensions requires the first 24 bytes of the file
		// reading JPEG dimensions requires scanning through jpeg chunks
		// In all formats, the file is at least 24 bytes big, so we'll read that always
		unsigned char buf[24]; fread(buf, 1, 24, f);


		// For JPEGs, we need to read the first 12 bytes of each chunk.
		// We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
		if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 && buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F')
		{
			long pos = 2;
			while (buf[2] == 0xFF)
			{
				if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 || buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA || buf[3] == 0xCB)
					break;

				pos += 2 + (buf[4] << 8) + buf[5];
				if (pos + 12 > len) break;
				fseek(f, pos, SEEK_SET);
				fread(buf + 2, 1, 12, f);
			}
		}


		fclose(f);

		// JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
		if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF)
		{
			*y = (buf[7] << 8) + buf[8];
			*x = (buf[9] << 8) + buf[10];

			return true;
		}

		// GIF: first three bytes say "GIF", next three give version number. Then dimensions
		if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F')
		{
			*x = buf[6] + (buf[7] << 8);
			*y = buf[8] + (buf[9] << 8);
			return true;
		}

		// PNG: the first frame is by definition an IHDR frame, which gives dimensions
		if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A && buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R')
		{
			*x = (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
			*y = (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);

			return true;
		}

		return false;
	}
	
	std::vector<unsigned char> WindowsOpenCV::WriteImage(std::vector<unsigned char> dataImage, 
		int rows, int cols)
	{		
		unsigned char* buffer = nullptr;
		std::vector<uchar> bufferImage;
		std::vector<uchar> bufferResult;
		try
		{			
			cv::Mat img = cv::Mat(rows, cols, CV_8UC3);
			img.data = &dataImage[0];			

			int cnn = (int)dataImage.size() / (rows * cols);
			
			if (cnn == 3 && !img.empty()) {

				/*std::vector<int> compression_params;
				compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
				compression_params.push_back(9);*/

				std::vector<int> compression_params;
				compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
				compression_params.push_back(100);

				/*int params[3] = { 0 };
				params[0] = cv::IMWRITE_JPEG_QUALITY;
				params[1] = 100;*/

				bool code = cv::imencode(".jpg", img,
					bufferImage, compression_params); // std::vector<int>(params, params + 2)
				if (code)
				{
					cv::imwrite("opencv.jpg", img);
					bufferResult.assign(img.data, img.data + (img.rows * img.cols * 3));
					
				}
				
			}
					
		}
		catch (cv::Exception& ex)
		{
			ASS_ERROR("WindowsOpenCV::WriteImage {0} ", ex.what());
		}
		
		return bufferResult;
	}

	void WindowsOpenCV::VerifyImage(unsigned char* data, int size, int height, int width)
	{
		std::vector<unsigned char> imgData(data, data + size);
		cv::Mat srcMat = cv::imdecode(imgData, cv::IMREAD_COLOR);
		cv::imwrite("source.png", srcMat);
		
		//std::vector<unsigned char> imgJPG = ImenCode(imgData, height, width);
		//WriteImage(imgData, height, width);
	}
	
	void WindowsOpenCV::CaptureDocument(unsigned char* dataImage, int* cols, 
		int* rows, std::vector<unsigned char>& bufferImg) //std::string& fileName
	{
		cv::Mat src_gray;
		int thresh = 100;		
		int width = 0, height = 0, index = 0;

		try
		{
			
			cv::Mat src = cv::Mat(*rows, *cols, CV_8UC3);
			src.data = dataImage;

			cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
			cv::blur(src_gray, src_gray, cv::Size(3, 3));

			cv::Mat canny_output;
			cv::Canny(src_gray, canny_output, thresh, thresh * 2);

			std::vector<std::vector<cv::Point>> contours;
			std::vector<cv::Vec4i> hierarchy;
			cv::findContours(canny_output, contours, hierarchy, cv::RetrievalModes::RETR_EXTERNAL,
				cv::ContourApproximationModes::CHAIN_APPROX_TC89_KCOS);
			//cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
			//cv::Scalar color = cv::Scalar(255, 255, 255);
			concurrent_vector<std::vector<cv::Point>> concurrentContours;
			std::copy(&contours[0], &contours[0] + contours.size(), concurrentContours.grow_by(contours.size()));

			parallel_for(blocked_range<size_t>(0, concurrentContours.size()),
				[&width, &height, &index, concurrentContours](const blocked_range<size_t>& r) {
					for (size_t i = r.begin(); i != r.end(); ++i) {
						cv::Rect rect = cv::boundingRect(concurrentContours[i]);
						if (rect.width > width && rect.height > height)
						{
							width = rect.width;
							height = rect.height;
							index = (int)i;

						}
					}
				}
			);
			
			*cols = width;
			*rows = height;

			cv::Rect rectMax = cv::boundingRect(contours[index]);
			cv::Mat cropImg = src(rectMax);

			std::vector<int> compression_params;
			compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
			compression_params.push_back(100);

			cv::cvtColor(cropImg, cropImg, cv::COLOR_BGR2RGB);
			bool result = cv::imencode(".jpg", cropImg,
				bufferImg, compression_params); 
			if (!result)
			{
				ASS_ERROR_PROFILE_SCOPE("WindowsOpenCV::CaptureDocument", "cv::imencode");				
			}
			
			/*
			
			cv::imwrite(fileName, cropImg);*/
			
			/*std::vector<uchar> buffer;
			#define MB 1024*1024
			buffer.resize(200 * MB);
			cv::imencode(".png", image, buffer);*/

			/*src.release();
			src_gray.release();
			canny_output.release();
			cropImg.release();*/
		}
		catch (cv::Exception& ex) 
		{
			ASS_ERROR_PROFILE_SCOPE("WindowsOpenCV::CaptureDocument", ex.what());
			
		}
				
	}
	
}

