#pragma once
#include "type.h"
#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

namespace Math
{
	class Byte
	{
	public:
		// bytes转换成BCD码
		static Uint8Array bytes2bcd(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 20 18 转换成 0x20 0x18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 10 * 16 + d % 10);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const Uint8Array& data)
		{
			Uint8Array out;
			// 比如 0x20 0x18 转换成 20 18
			for (size_t i = 0; i < data.size(); i++)
			{
				const uint8 d = data[i];
				out.push_back(d / 16 * 10 + d % 16);
			}
			return out;
		}

		// BCD码转换成bytes
		static Uint8Array bcd2bytes(const uint8* buff, uint32 buffsize)
		{
			Uint8Array out(buff, buff + buffsize);
			return bcd2bytes(out);
		}

		// 计算16位校验和
		static uint16 getsum(const uint8* buff, uint32 buffsize)
		{
			uint32 sum = 0;
			for (uint32 i = 0; i < buffsize; i++)
			{
				sum += (uint8)buff[i];
			}
			return sum;
		}

		// 计算32位校验和
		static uint32 getsum32(const uint8* buff, uint32 buffsize)
		{
			uint32 sum = 0;
			for (uint32 i = 0; i < buffsize; i++)
			{
				sum += (uint8)buff[i];
			}
			return sum;
		}

		// 获取CRC16校验
		static uint16 get_crc16(uint8* puchMsg, uint32 usDataLen)
		{
			uint32 i, j, crc_reg, check;
			crc_reg = 0xFFFF;
			for (i = 0; i < usDataLen; i++)
			{
				crc_reg = (crc_reg >> 8) ^ puchMsg[i];
				for (j = 0; j < 8; j++)
				{
					check = crc_reg & 0x0001;
					crc_reg >>= 1;
					if (check == 0x0001)
					{
						crc_reg ^= 0xA001;
					}
				}
			}
			return crc_reg;
		}
	};

	class Tools
	{
	public:
		// 替换字符
		static inline String replace(const String& str, const String& search, const String& dest)
		{
			String result;
			size_t start = 0, end = 0;
			while (start < str.size())
			{
				end = str.find_first_of(search, start);
				result += str.substr(start, end - start);
				start = end + search.size();
				if (end == String::npos) {
					break;
				}
				result += dest;
			}
			return result;
		}

		// 字符串分割函数
		static inline StringArray split(const std::string& s, const std::string& sep, const bool compress = false)
		{
			StringArray out;
			int pos = 0;
			int index = -1;
			while ((index = s.find(sep, pos)) != s.npos)
			{
				if (index - pos == 0)
				{

				}
				// s.substr(pos, index - pos == 0 ? 1 : index - pos);
				std::string it = index - pos == 0 ? "" : s.substr(pos, index - pos);
				if (compress && it == "") // 压缩 index - pos == sep.size() && 
				{

				}
				else // 不用压缩
				{
					out.push_back(it);
				}

				pos = index + sep.size();
			}
			if (pos < s.size())
			{
				out.push_back(s.substr(pos));
			}
			else if (pos == s.size())
			{
				out.push_back("");
			}
			return out;
		}

		// 数据组合字符串
		static inline String join(const StringArray& arr, const std::string& sep) {
			String result;
			for (auto& i : arr) {
				if (result.empty()) {
					result = i;
					continue;
				}
				result += sep + i;
			}
			return result;
		}

		// 转换hex到字符串显示
		static inline String hex2str(const char* buff, const size_t buffsize, const char* sep = "", bool is_case = false) {
			String out;
			char ch[4];
			const char* fmt = is_case ? "%02x" : "%02X";
			for (size_t i = 0; i < buffsize; i++) {
				sprintf(ch, fmt, buff[i] & 0xFF);
				if (out.empty()) {
					out = ch;
				}
				else {
					out += sep;
					out += ch;
				}
			}
			return out;
		}

		// 转换字符串显示到hex数组
		static inline String str2hex(const String& buff, const String& sep = "") {
			String out;
			size_t buffsize = buff.size();
			StringArray items;
			if (sep.empty() && buffsize % 2 == 0) {
				for (size_t i = 0; i < buffsize / 2; i++) {
					items.emplace_back(buff.substr(i * 2, 2));
				}
			}
			else if (sep.size()) {
				items = split(buff, sep, true);
			}
			for (auto& i : items) {
				int ch;
				if (!to_int(ch, i, 16)) {
					return out;
				}
				out.push_back(ch & 0xff);
			}
			return out;
		}

		// 获取字符串中数字 转换成int int32
		static inline bool to_int(int& _return, const std::string& buff, int base = 10, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stoi(buff.substr(offset, count), 0, base);
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 获取字符串中数字 转换成 unsigned int
		static inline bool to_uint32(unsigned int& _return, const std::string& buff, int base = 10, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stoul(buff.substr(offset, count), 0, base);
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 获取字符串中数字 转换成 int64
		static inline bool to_int64(int64& _return, const std::string& buff, int base = 10, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stoll(buff.substr(offset, count), 0, base);
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 获取字符串中数字 转换成 uint64
		static inline bool to_uint64(uint64& _return, const std::string& buff, int base = 10, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stoull(buff.substr(offset, count), 0, base);
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 截取字符串buff中从offset开始的count个字符转换成float
		static inline bool to_float(double& _return, const std::string& buff, size_t offset = 0, size_t count = String::npos)
		{
			_return = 0;
			if (buff.empty())
				return false;
			try {
				_return = std::stod(buff.substr(offset, count));
			}
			catch (...) {
				return false;
			}
			return true;
		}

		// 去掉尾部的特定字符
		static inline String trim(const String& buffer, const String& sep = " ", bool is_tail = true) {
			size_t start = 0;
			size_t end = String::npos;
			for (int i = 0; i < (int)buffer.size() - (int)sep.size(); i++)
			{
				size_t pos = is_tail ? (buffer.size() - sep.size() * (i + 1)) : (sep.size() * i);
				if (buffer.substr(pos, sep.size()) != sep)
				{
					if (is_tail){
						end = pos;
					}
					else {
						start = pos;
					}
					break;
				}
			}
			return buffer.substr(start, end - start + 1);
		}
	};
}
