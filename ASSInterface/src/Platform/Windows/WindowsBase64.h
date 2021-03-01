#pragma once

#include "ASSInterface/Utils/Base64.h"

namespace ASSInterface {

	class WindowsBase64 : public Base64 {
	public:
		WindowsBase64();
		~WindowsBase64();
		virtual std::string base64_encode(unsigned char* bytes_to_encode, int in_len) override;
		virtual std::string base64_decode(std::string const& encoded_string) override;
	private:
		const std::string base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

		inline bool is_base64(unsigned char c) {
			return (isalnum(c) || (c == '+') || (c == '/'));
		}
	};
}