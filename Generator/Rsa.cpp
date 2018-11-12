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
	//����������p��q
	_p=createPrime(n,10);
	
	_q=createPrime(n,10);
	//����N
	N=_p*_q;
	//�����ŷ����
	_ol=(_p-1)*(_q-1);
	//����ָ��e
	createExp(_ol);
	//d
}

void Rsa::init(const BigInt& a, const BigInt& b)
{
	_p = a;
	_q = b;
	//����N
	N = _p*_q;
	//�����ŷ����
	_ol = (_p - 1)*(_q - 1);
	//����ָ��e
	createExp(_ol);
	//d
}

BigInt Rsa::createOddNum(unsigned int n)
{//���ɳ���Ϊn������
	
	string str;
	n = n / 4;//n��bit��������HEX����ʾʱ��ÿ���ַ���ʾ4bit ���Գ���4
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

bool Rsa::isPrime(const BigInt& n,const unsigned int k)//��������������û�п�����������ɣ���Ҫ�о����������ɵĹ���
{//�ж�����
	
	assert(n!=0);
	

	if(n==2)
		return true;

	BigInt n_1=n-1;
//	BigInt::bit b(n_1);//������λ

	if(n_1.atbit(0)==1)
		return false;

	for(std::size_t t=0;t<k;++t)//k�Ǽ��Ĵ�����kԽ����Խ׼ȷ����ҲԽ��ʱ��
	{
		BigInt a=createRandomSmallThan(n_1);//�����
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
		if(d!=1)//�����Ϸ���С������Ϊ����
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
{//���ɳ���Ϊn������
//	assert(it_count>0);
	
	BigInt res=createOddNum(n);
	while(!isPrime(res,it_count))//λ������32ʱ��һֱ��isprime()�г�����,�϶���isprime������
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
{//��һ��ŷ���������ɹ�Կ��˽Կָ��
	//e=5;
	e=65537;
	_d=e.exgcd(ou);
}

BigInt Rsa::encryptByPu(const BigInt& m)
{//��Կ����
	return m.moden(e,N);
	
}

BigInt Rsa::decodeByPr(const BigInt& c)
{//˽Կ����
	return c.moden(_d,N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{//˽Կ����
	return decodeByPr(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
{//��Կ����
	return encryptByPu(c);
}