#ifndef BASE64_CONVERTER_H_
#define BASE64_CONVERTER_H_
#include <string>

namespace dev {
	/**
	 * @brief convert base64 string to cv::mat
	 *
	 * @param s
	 * @param decode_img
	 * @return true if decode right
	 * @return false
	 */
	bool is_base64(const unsigned char &c);

	std::string base64_encode(const char *bytes_to_encode, unsigned int in_len);

	std::string base64_decode(std::string const &encoded_string, unsigned int in_len);
}
#endif //BASE64_CONVERTER_H_