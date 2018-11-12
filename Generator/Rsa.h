#pragma once
#include"BigInt.h"
#include<vector>
#include<string>
#include<cassert>
using std::string;
using std::vector;

class Rsa
{
public:
	Rsa();
	~Rsa();
	void init(unsigned int n);//初始化，输入密匙长度来随机产生公私钥对
	void init(const BigInt& a,const BigInt& b);//初始化重载，输入质数p、q来产生公私钥对
	
	friend void test();
	
	bool testprime(const BigInt& input);
public:
	BigInt encryptByPu(const BigInt& m);//私钥加密
	BigInt decodeByPr(const BigInt& c);//公钥解密

	BigInt encryptByPr(const BigInt& m);//公钥加密
	BigInt decodeByPu(const BigInt& m);//私钥解密
private:
	BigInt createOddNum(unsigned int n);//生成长度为n的奇数
	bool isPrime(const BigInt& a,const unsigned int k);//判断素数
	BigInt createPrime(unsigned int n,int it_cout);//生成长度为n的素数
	void createExp(const BigInt& ou);//从一个欧拉数中生成公钥、私钥指数
	BigInt createRandomSmallThan(const BigInt& a);//创建小数
	
public:
	BigInt e,N;//公钥
//private:
	BigInt _d;//私钥
	BigInt _p,_q;//
	BigInt _ol;//欧拉数
//	friend class CGeneratorDlg;//使CGeneratorDlg能访问Rsa类的私有成员 耦合
};