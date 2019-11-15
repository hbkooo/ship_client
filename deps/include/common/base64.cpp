#include "base64.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

namespace dev {

    bool is_base64(const unsigned char &c)
    {
        return (isalnum(c) || c == '+' || c == '/');
    }

    string Encode(unsigned char const *Data, int DataByte)
    {
        //编码表
        const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        //返回值
        string strEncode;
        unsigned char Tmp[4] = {0};
        int LineLength = 0;
        for (int i = 0; i < (int) (DataByte / 3); i++) {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            Tmp[3] = *Data++;
            strEncode += EncodeTable[Tmp[1] >> 2];
            strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
            strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
            strEncode += EncodeTable[Tmp[3] & 0x3F];
            if (LineLength += 4, LineLength == 76) {
                strEncode += "\r\n";
                LineLength = 0;
            }
        }
        //对剩余数据进行编码
        int Mod = DataByte % 3;
        if (Mod == 1) {
            Tmp[1] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
            strEncode += "==";
        } else if (Mod == 2) {
            Tmp[1] = *Data++;
            Tmp[2] = *Data++;
            strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
            strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
            strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
            strEncode += "=";
        }

        return strEncode;
    }

    string readFileIntoString(char *filename)
    {
        ifstream ifile(filename);
        //将文件读入到ostringstream对象buf中
        ostringstream buf;
        char ch;
        while (buf && ifile.get(ch))
            buf.put(ch);
        //返回与流对象buf关联的字符串
        return buf.str();
    }

    std::string base64_encode(const char *bytes_to_encode, unsigned int in_len)
    {
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;
                for (i = 0; (i < 4); i++) {
                    ret += base64_chars[char_array_4[i]];
                }
                i = 0;
            }
        }
        if (i) {
            for (j = i; j < 3; j++) {
                char_array_3[j] = '\0';
            }

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++) {
                ret += base64_chars[char_array_4[j]];
            }

            while ((i++ < 3)) {
                ret += '=';
            }

        }
        return ret;
    }

    std::string base64_decode(std::string const &encoded_string, unsigned int in_len)
    {
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_];
            in_++;
            if (i == 4) {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }
        if (i) {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        return ret;
    }
}
