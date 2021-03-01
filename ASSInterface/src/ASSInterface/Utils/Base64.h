#pragma once
#include "hzpch.h"
#include <string>


namespace ASSInterface {
	class Base64 {
	public:
		virtual std::string base64_encode(unsigned char* bytes_to_encode, int in_len) = 0;
		virtual std::string base64_decode(std::string const& encoded_string) = 0;
		static Ref<Base64> Create();
			
	};
	
}

