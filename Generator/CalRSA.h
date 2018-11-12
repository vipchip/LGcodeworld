#pragma once
#include "stdafx.h"

string createOddNum(unsigned int n)
{//生成长度为n的奇数
	string str;
	n = n / 4;//n是bit，换成用HEX来表示时，每个字符表示4bit 所以除以4
	static unsigned char hex_table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	if (n)
	{

		for (unsigned char i = 0; i<n - 1; ++i)
			str += hex_table[rand() % 16];
		str += hex_table[1];

		return str;
	}
	else
		return str;
}



