#pragma once
#include<vector>
#include<string>
#include<cassert>
using std::string;
using std::vector;

typedef vector<unsigned long> Vtor;//供外部使用



class BigInt
{
private:
	const static int base_char{8};
	const static int base{0xffffffff};
	const static int basebitnum{32};
	const static int basebitchar{0x1f};
	const static int basebit{5};
		

public:
	typedef unsigned long basedata;
	typedef vector<basedata> data_t;
	typedef const vector<basedata> Cdata_t;

private:
	data_t datavector;      //全局向量
	bool _isnegative{ false };//全局变量


	
public://多个版本的构造函数
	
	//constructor,作用是把各种字符输入（十六进制内的字母、数字）变成统一格式，并存储在全局向量中
	BigInt() :_isnegative(false)
	{ 
		datavector.push_back(0); 
	}
	//如果参数是string型，则符号为正，调用函数规范数据存储为统一格式
	BigInt(const string& num) :datavector(0), _isnegative(false)
	{ 
		copyFromHexString(num); 
	//	trim();
	}
	//如果参数是long型，则符号为正，调用函数规范数据存储为统一格式
	BigInt(const long n) :_isnegative(false)
	{ 
		copyFromLong(n); 
	}
	//如果参数是向量，则向量直接赋值、符号为正
	BigInt(Cdata_t data) :datavector(data), _isnegative(false)
	{ 
	//	trim(); 
	}
	// 如果参数是BigInt类型，则向量直接赋值、赋值符号
	BigInt(const BigInt& a, bool isnegative) :datavector(a.datavector), _isnegative(isnegative)
	{}

	BigInt(const BigInt& a) :datavector(a.datavector), _isnegative(a._isnegative)
	{}
public:
	 const data_t& getVector(){ return datavector; };
	 const bool& getsymbol(){ return _isnegative; };
private:
	char hexto4bit(char ch);//把hex字符用4bit表示
	void copyFromHexString(const string& s);//把STRING型装入vector
	void copyFromLong(const long n);	//把long型装入vector
	void trim(); //裁掉vector尾端的0 以4byte为单位而不是以字符位单位
	void tonagetive(data_t& a);//把vector变成反码
	void filltoEqure(BigInt& guest);//比较主方与客方vector长度，短的补0成一样长
public:
	bool atbit(const UINT i)const;//判断vector中指定位置n是否为1
	UINT atsize(void)const;//得到vector中数据的长度	//参数类型是vector，在cpp中不被识别，因为在类外
	

public://大数的四则运算
	//二进制四则运算，讲究先后顺序，所以要判断两个数的大小，以决定算法
	bool smallThan(const BigInt& a)const;//判断绝对值是否小于
	bool smallOrEquals(const BigInt& a)const;//判断绝对值是否小于相等
	bool equals(const BigInt& a)const;//判断绝对值是否相等

	BigInt& add(const BigInt& b);	//返回类型是引用，而引用就是本对象自身的别名
	BigInt& sub(const BigInt& b);
	BigInt& multi(const BigInt& b);
	BigInt& div(const BigInt& b,BigInt& remind);

	BigInt& leftShift(const unsigned int n);
	BigInt& rightShift(const unsigned int n);
//	static void div(const BigInt& a, const BigInt& b, BigInt& result, BigInt& ca);
public:
	//运算符重载
	
	friend BigInt operator + (const BigInt& a, const BigInt& b);
	friend BigInt operator - (const BigInt& a, const BigInt& b);
	friend BigInt operator * (const BigInt& a, const BigInt& b);
	friend BigInt operator / (const BigInt& a, const BigInt& b);
	friend BigInt operator % (const BigInt& a, const BigInt& b);
	friend bool operator < (const BigInt& a, const BigInt& b);
	friend bool operator <= (const BigInt& a, const BigInt& b);
	friend bool operator == (const BigInt& a, const BigInt& b);
//	friend bool operator == (const BigInt& a, const long& b); 
	friend bool operator != (const BigInt& a, const BigInt& b);

	friend BigInt operator - (BigInt& a){ a._isnegative = (a._isnegative == false) ? true : false; return a; };//负号
	
	
	//扩展欧几里得求逆元
	BigInt exgcd(const BigInt& m);
	//大数幂模运算	
	BigInt moden(const BigInt& exp, const BigInt& p)const;
	
	//欧几里得算法求最大公约数（辗转相除法）
	BigInt gcd(const BigInt a,const BigInt b)const;

	

};
