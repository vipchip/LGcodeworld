#include "stdafx.h"
#include "Rsa.h"





Rsa::Rsa()
{
}

Rsa::~Rsa()
{
}

void Rsa::init(unsigned int n)
{
	srand(time(NULL));
	//产生大素数p、q
	_p=createPrime(n,10);
	
	_q=createPrime(n,10);
	//计算N
	N=_p*_q;
	//计算出欧拉数
	_ol=(_p-1)*(_q-1);
	//加密指数e
	createExp(_ol);
	//d
}

void Rsa::init(const BigInt& a, const BigInt& b)
{
	_p = a;
	_q = b;
	//计算N
	N = _p*_q;
	//计算出欧拉数
	_ol = (_p - 1)*(_q - 1);
	//加密指数e
	createExp(_ol);
	//d
}

BigInt Rsa::createOddNum(unsigned int n)
{//生成长度为n的奇数
	
	string str;
	n = n / 4;//n是bit，换成用HEX来表示时，每个字符表示4bit 所以除以4
	static unsigned char hex_table[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	if (n)
	{

		for (unsigned char i = 0; i < n - 1; ++i)
			str += hex_table[rand() % 16];
		str += hex_table[1];

		return str;
	}
	else
		str = "0";
		BigInt OddNum{str};

		return OddNum;
}

bool Rsa::isPrime(const BigInt& n,const unsigned int k)//可能是四则运算没有考虑正负号造成，需要研究大素数生成的过程
{//判断素数
	
	assert(n!=0);
	

	if(n==2)
		return true;

	BigInt n_1=n-1;
//	BigInt::bit b(n_1);//二进制位

	if(n_1.atbit(0)==1)
		return false;

	for(std::size_t t=0;t<k;++t)//k是检测的次数，k越大检测越准确，但也越耗时间
	{
		BigInt a=createRandomSmallThan(n_1);//随机数
		BigInt d{n_1};
	
		for(int i=n_1.atsize()-1;i>=0;--i)
		{
			BigInt x=d;
			d=(d*d)%n;
			if(d==1 && x!=1 && x!=n_1)
				return false;

			if(n_1.atbit(i))
			{
			assert(d!=0);
				d=(a*d)%n;
			}
		}
		if(d!=1)//不符合费马小定理，必为合数
			return false;
			
	}
	return true;
}

BigInt Rsa::createRandomSmallThan(const BigInt& a)
{


	unsigned long t=0;
	do
	{
		t=rand();
	}while(t==0);

	BigInt mod(t);
	BigInt r=mod%a;
	if(r==0)
		r=a-1;	
	return r;
}

BigInt Rsa::createPrime(unsigned int n,int it_count)
{//生成长度为n的素数
//	assert(it_count>0);
	
	BigInt res=createOddNum(n);
	while(!isPrime(res,it_count))//位数大于32时，一直在isprime()中出不来,肯定是isprime有问题
	{
		res.add(2);

	}
	return res;
}


bool Rsa::testprime(const BigInt& input)
{
	if (isPrime(input, 10))
		return  true;
	return false;
}

void Rsa::createExp(const BigInt& ou)
{//从一个欧拉数中生成公钥、私钥指数
	//e=5;
	e=65537;
	_d=e.exgcd(ou);
}

BigInt Rsa::encryptByPu(const BigInt& m)
{//公钥加密
	return m.moden(e,N);
	
}

BigInt Rsa::decodeByPr(const BigInt& c)
{//私钥解密
	return c.moden(_d,N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{//私钥加密
	return decodeByPr(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
{//公钥解密
	return encryptByPu(c);
}