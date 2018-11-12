#pragma once
#include<vector>
#include<string>
#include<cassert>
using std::string;
using std::vector;

typedef vector<unsigned long> Vtor;//���ⲿʹ��



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
	data_t datavector;      //ȫ������
	bool _isnegative{ false };//ȫ�ֱ���


	
public://����汾�Ĺ��캯��
	
	//constructor,�����ǰѸ����ַ����루ʮ�������ڵ���ĸ�����֣����ͳһ��ʽ�����洢��ȫ��������
	BigInt() :_isnegative(false)
	{ 
		datavector.push_back(0); 
	}
	//���������string�ͣ������Ϊ�������ú����淶���ݴ洢Ϊͳһ��ʽ
	BigInt(const string& num) :datavector(0), _isnegative(false)
	{ 
		copyFromHexString(num); 
	//	trim();
	}
	//���������long�ͣ������Ϊ�������ú����淶���ݴ洢Ϊͳһ��ʽ
	BigInt(const long n) :_isnegative(false)
	{ 
		copyFromLong(n); 
	}
	//���������������������ֱ�Ӹ�ֵ������Ϊ��
	BigInt(Cdata_t data) :datavector(data), _isnegative(false)
	{ 
	//	trim(); 
	}
	// ���������BigInt���ͣ�������ֱ�Ӹ�ֵ����ֵ����
	BigInt(const BigInt& a, bool isnegative) :datavector(a.datavector), _isnegative(isnegative)
	{}

	BigInt(const BigInt& a) :datavector(a.datavector), _isnegative(a._isnegative)
	{}
public:
	 const data_t& getVector(){ return datavector; };
	 const bool& getsymbol(){ return _isnegative; };
private:
	char hexto4bit(char ch);//��hex�ַ���4bit��ʾ
	void copyFromHexString(const string& s);//��STRING��װ��vector
	void copyFromLong(const long n);	//��long��װ��vector
	void trim(); //�õ�vectorβ�˵�0 ��4byteΪ��λ���������ַ�λ��λ
	void tonagetive(data_t& a);//��vector��ɷ���
	void filltoEqure(BigInt& guest);//�Ƚ�������ͷ�vector���ȣ��̵Ĳ�0��һ����
public:
	bool atbit(const UINT i)const;//�ж�vector��ָ��λ��n�Ƿ�Ϊ1
	UINT atsize(void)const;//�õ�vector�����ݵĳ���	//����������vector����cpp�в���ʶ����Ϊ������
	

public://��������������
	//�������������㣬�����Ⱥ�˳������Ҫ�ж��������Ĵ�С���Ծ����㷨
	bool smallThan(const BigInt& a)const;//�жϾ���ֵ�Ƿ�С��
	bool smallOrEquals(const BigInt& a)const;//�жϾ���ֵ�Ƿ�С�����
	bool equals(const BigInt& a)const;//�жϾ���ֵ�Ƿ����

	BigInt& add(const BigInt& b);	//�������������ã������þ��Ǳ���������ı���
	BigInt& sub(const BigInt& b);
	BigInt& multi(const BigInt& b);
	BigInt& div(const BigInt& b,BigInt& remind);

	BigInt& leftShift(const unsigned int n);
	BigInt& rightShift(const unsigned int n);
//	static void div(const BigInt& a, const BigInt& b, BigInt& result, BigInt& ca);
public:
	//���������
	
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

	friend BigInt operator - (BigInt& a){ a._isnegative = (a._isnegative == false) ? true : false; return a; };//����
	
	
	//��չŷ���������Ԫ
	BigInt exgcd(const BigInt& m);
	//������ģ����	
	BigInt moden(const BigInt& exp, const BigInt& p)const;
	
	//ŷ������㷨�����Լ����շת�������
	BigInt gcd(const BigInt a,const BigInt b)const;

	

};
