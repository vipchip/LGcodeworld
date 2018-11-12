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
	void init(unsigned int n);//��ʼ���������ܳ׳��������������˽Կ��
	void init(const BigInt& a,const BigInt& b);//��ʼ�����أ���������p��q��������˽Կ��
	
	friend void test();
	
	bool testprime(const BigInt& input);
public:
	BigInt encryptByPu(const BigInt& m);//˽Կ����
	BigInt decodeByPr(const BigInt& c);//��Կ����

	BigInt encryptByPr(const BigInt& m);//��Կ����
	BigInt decodeByPu(const BigInt& m);//˽Կ����
private:
	BigInt createOddNum(unsigned int n);//���ɳ���Ϊn������
	bool isPrime(const BigInt& a,const unsigned int k);//�ж�����
	BigInt createPrime(unsigned int n,int it_cout);//���ɳ���Ϊn������
	void createExp(const BigInt& ou);//��һ��ŷ���������ɹ�Կ��˽Կָ��
	BigInt createRandomSmallThan(const BigInt& a);//����С��
	
public:
	BigInt e,N;//��Կ
//private:
	BigInt _d;//˽Կ
	BigInt _p,_q;//
	BigInt _ol;//ŷ����
//	friend class CGeneratorDlg;//ʹCGeneratorDlg�ܷ���Rsa���˽�г�Ա ���
};