#pragma once
#include "stdafx.h"

string createOddNum(unsigned int n)
{//���ɳ���Ϊn������
	string str;
	n = n / 4;//n��bit��������HEX����ʾʱ��ÿ���ַ���ʾ4bit ���Գ���4
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



