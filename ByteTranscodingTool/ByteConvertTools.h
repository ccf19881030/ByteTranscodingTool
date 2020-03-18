#pragma once
#include "type.h"
#include <algorithm>
#include <array>
#include <regex>	// for std::regex_match

namespace ByteConvertTools
{
	// 输入的byte数组中获取指定类型的数据
	// 支持int16,int32,int64,float,double
	template<typename T>
	bool get_data(T& _return, const uint8* buffer, size_t buffersize,
		uint16 offset_bytes, bool isLittle, bool isSwapByte)
	{
		uint32 totalByteNum = buffersize;
		uint32 byteNum = sizeof(T);
		uint32 regNum = byteNum / 2;
		uint32 startPos = offset_bytes;
		uint32 endPos = startPos + byteNum;
		if ((regNum == 0 || byteNum % 2 != 0) || (startPos > totalByteNum || endPos > totalByteNum)) {
			return false;
		}
		// 获取模板参数T的具体类型(int16,int32,int64,float,double)
		auto& type = typeid(T);
		if ((type == typeid(double) || type == typeid(int64) || type == typeid(uint64)) ||
			(type == typeid(float) || type == typeid(uint32) || type == typeid(int32)) ||
			(type == typeid(int16) || type == typeid(uint16))) {
			Uint8Array tmp8; Uint16Array tmp16(regNum);
			/*
				不同的计算机体系结构使用不同的字节顺序存储数据。
				“大端”表示最高有效字节在单词的左端。即最高位字节存放在字节数组的低位
				“小端”表示最高有效字节在单词的右端。即最高位字节存放在字节数组的高位
			*/
			if (isLittle) {
				// 小端字节序 dcba
				std::copy(buffer + startPos, buffer + endPos, std::back_inserter(tmp8));
			}
			else {
				// 大端字节序，则将字节数组进行反转 abcd
				std::reverse_copy(buffer + startPos, buffer + endPos, std::back_inserter(tmp8));
			}
			memcpy(tmp16.data(), tmp8.data(), byteNum);
			if (isSwapByte)
			{
				std::reverse(tmp16.begin(), tmp16.end());
				Uint8Array tmp1(byteNum);
				memcpy(tmp1.data(), tmp16.data(), byteNum);
				std::reverse(tmp1.begin(), tmp1.end());
				memcpy(tmp16.data(), tmp1.data(), byteNum);
			}
			memcpy(&_return, tmp16.data(), byteNum);
			return true;
		}
		return false;
	}

	template<typename T>
	bool get_data(T& _return, const Uint8Array& buffer,
		uint16 offset_bytes, bool isLittle, bool isSwapByte)
	{
		return get_data(_return, buffer.data(), buffer.size(), offset_bytes, isLittle, isSwapByte);
	}

	// 判断本台机器是大端字节序还是小端字节序
	bool isLittleEndian()
	{
		int iData = 1;
		char *p = (char*)&iData;
		if (*p == 1)
		{
			return true;
		}
		else {
			return false;
		}
	}

	template <typename T>
	void swapBytes(T& var)
	{
		static_assert(std::is_pod<T>::value, "Type must be POD type for safety");
		std::array<char, sizeof(T)> varArray;
		std::memcpy(varArray.data(), &var, sizeof(T));
		for (int i = 0; i < static_cast<int>(sizeof(var) / 2); i++)
			std::swap(varArray[sizeof(var) - 1 - i], varArray[i]);
		std::memcpy(&var, varArray.data(), sizeof(T));
	}

	// 将int16,int32,int64,float,double等转换成16进制字节数组
	template<typename T>
	bool convertToBytesArray(Uint8Array& _return, const T value, bool isLittle, bool isSwapByte)
	{
		uint32 byteNum = sizeof(T);

		// 将T类型(int16,int32,int64,float,double等)的内容拷贝至tmp8中
		Uint8Array tmp8(byteNum);
		Uint16Array tmp16(byteNum / 2);

		bool isLocalMachineLittleEndian = isLittleEndian();

		if (isLittle == isLocalMachineLittleEndian)	// 如果是小端
		{
			memcpy(tmp8.data(), &value, byteNum);
		} 
		else {
			memcpy(tmp8.data(), &value, byteNum);
			// 将小端字节序转换成大端字节序或者将大端字节序转换成小端字节序
			std::reverse(tmp8.begin(), tmp8.end());
		}
		
		// 交换相邻的两个字节
		memcpy(tmp16.data(), tmp8.data(), byteNum);
		if (isSwapByte)
		{
			std::reverse(tmp16.begin(), tmp16.end());
			Uint8Array tmp1(byteNum);
			memcpy(tmp1.data(), tmp16.data(), byteNum);
			std::reverse(tmp1.begin(), tmp1.end());
			memcpy(tmp16.data(), tmp1.data(), byteNum);
		}
		memcpy(tmp8.data(), tmp16.data(), byteNum);

		_return = tmp8;

		return true;
	}

	// c++用正则表达式判断匹配字符串中的数字数值（包括负数，小数，整数）MFC编辑框判断数值
	static bool isStrNumber(String strInput)
	{
		//正则匹配数值包括负数 小数 整数
		std::string strPattern{ "-[0-9]+(.[0-9]+)?|[0-9]+(.[0-9]+)?" };
		std::regex re(strPattern);
		bool retMatchStr = false;

		retMatchStr = std::regex_match(strInput, re);

		return retMatchStr;
	}
};

