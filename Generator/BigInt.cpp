#include"stdafx.h"
#include"BigInt.h"


char BigInt::hexto4bit(char ch)
{
	//ֻ��ת������0~9����ĸA~F     ������Χ���ַ�������ȷת��
	static char table[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	if (isdigit(ch))//isdigit��������ch�����֣�����true,���򷵻�false
		ch -= '0';
	else if (islower(ch))//islower������������ch�Ƿ�ΪСдӢ����ĸ��
		ch -= 'a' - 10;
	else if (isupper(ch))//isupper������������ch�Ƿ�Ϊ��дӢ����ĸ��
		ch -= 'A' - 10;

	return table[ch];
}
//��stringתΪBigInt
void BigInt::copyFromHexString(const string& s)
{
	string str{s};
	//����ʱ���Ѹ���ȥ����������_isnegative��ʾ
	if (str.length() && str.at(0) == '-')
	{
		if (str.length()>1)
			_isnegative = true;
		str = str.substr(1);
	}

	//һ��unigned long���Դ�8���ַ����������9���ַ��أ���Ҫ�����7�ַ���ȫ0��,����һ�����2��unsigned long�档
	//�ַ���temp���ڰ��ַ���str������8�ı�����ȫ��0������0~7��
	int count = (8 - (str.length() % 8)) % 8;//count�Ǵ��������ַ�λ����0~7
	string temp{""};
	for (int i = 0; i<count; ++i)
		temp.push_back(0);
	
	str = temp + str;//��temp�ӵ��ַ���str�ĸ�λ��ʹstr�ַ���������װ��vectorÿ��bit��û�п�ȱ��




	//˳��λÿ���ַ�������ת��Ϊ��4bit��ʾ�����洢
	for (basedata i = 0; i<str.length(); i += 8)//ÿ8���ַ�λ����һ��unsigned long��4�ֽڣ�
	{
		basedata sum = 0;
		for (int j = 0; j<8; ++j)//˳��ȡ����8���ַ�
		{
			char ch = str[i + j];//ch�Ǳ�ʾHEX���ַ�1��2��3...f������ASCII���ʾ��

			ch = hexto4bit(ch); //ת������Ȼ��ͬ�����ַ�1��2��3...f��������4��bit��ʾ
			sum = ((sum << 4) | (ch));//һ��4��bit����vector��Ԫsum�д���
		}
		//��sum��ȫ�������д���
		datavector.push_back(sum);//��ʱ��ͷ��datavector[0]����ֵ�ĸ�λ   ����β��datavector[N]����ֵ�ĵ�λ�� 
	}
	
	reverse(datavector.begin(), datavector.end());//��ת����������β��datavector[N]����ֵ�ĸ�λ  ����������β������0������λ��0��

}

//�������ܣ���vectorβ����0ȥ��,��4byteΪ��λ
 void BigInt::trim()
{
	int count = 0;
	int cou1=0;
	//���Ϊ0��Ԫ�ص�����		
	for (data_t::reverse_iterator it = datavector.rbegin(); it != datavector.rend(); ++it)
	{
		if ((*it) == 0)
			++count;
		else
			break;//�����Ϊ0������forѭ��
	}

		if (count == datavector.size())//ֻ������������			
			--count;
		for (int i = 0; i < count; ++i)
			datavector.pop_back();
	
//	return *this;
	return;
}

 //��long��תΪBigInt
 void BigInt::copyFromLong(const long n)
 {
	 long a = n;
	
	 BigInt::basedata ch;
	 if (a<0)
	 {
		 _isnegative = true;
		 a = -a;
	 }
	 do
	 {
		 ch = (a&(BigInt::base));
		 datavector.push_back(ch);
		 a = a >> (BigInt::basebitnum/2);
		 a = a >> (BigInt::basebitnum/2);  //λ����=λ����%32,���λ������Χ��:0~31  ����32λ����ʵ����λ
		
	 } while (a);
 }


 //11��1�գ�������������Ϻ�������һ���㶨��������

 //�������ܣ���������������nλ  ԭ����vector�����ݽ����ƶ����ӵ�λ����һ����Ԫ����������32bit!! 
 //��ˣ����Ч�����Ʒ����ǣ�1���ȵ�Ԫ�ƶ������ƶ�λ�����㵥Ԫ�ƶ�����puck_back��ElemSftnum=n/32��������������ʵ������ElemSftnum+1��������ͨ��λ�ƶ� ���ƻ����� 
 //2���ٽ���λ�ƶ��������ƻأ�����vectorÿ����Ԫ��Ҫ����
 BigInt& BigInt::leftShift(const unsigned int n)
 {	
	 /*
	 int k = n >> (BigInt::basebit);//5
	 int off = n&(BigInt::basebitchar);//0xFF

	 int inc = (off == 0) ? k : 1 + k;
	 for (int i = 0; i<inc; ++i)
		datavector.push_back(0);

	 if (k)
	 {
		 inc = (off == 0) ? 1 : 2;
		 for (int i = datavector.size() - inc; i >= k; --i)
			 datavector[i] = datavector[i - k];
		 for (int i = 0; i<k; ++i)
			 datavector[i] = 0;
	 }

	 if (off)
	 {
		 basedata T = BigInt::base;//0xffffffff
		 T = T << (BigInt::basebitnum - off);//32
		 //����
		 basedata ch = 0;
		 for (std::size_t i = 0; i<datavector.size(); ++i)
		 {
			 basedata t = datavector[i];
			 datavector[i] = (t << off) | ch;
			 ch = (t&T) >> (BigInt::basebitnum - off);//32,���λ
		 }
	 }
	 trim();
	 */
	 
	 UINT ElemLSftnum = n >> (BigInt::basebit);//5  �൱��n/32ȡ��  ��Ԫ�ƶ���
	 UINT BitLShfnum = n&(BigInt::basebitchar);//0x1F              λ�ƶ���
	 UINT ActElemSftnum = (BitLShfnum == 0) ? ElemLSftnum : 1 + ElemLSftnum; //ʵ�ʵ�Ԫ�ƶ���������������Ԫ�ƶ���+1
	 	 	 
	 //1��������Ԫ�ƶ�  ע��β���Ǹ�λ��ͷ���ǵ�λ��Ҫ��ͷ����0��Ԫ��
	 const basedata none{ 0 };
	 for (UINT i = 0; i<ActElemSftnum; ++i)
		 datavector.insert(datavector.begin(), none);	 	 

	 //2������λ�ƶ������յ�λ����λ��˳�����vector��Ԫ����32-BitSftnumλ
	 //ע��ÿ����Ԫ����һ��Ϊ������һ��Ԫ�ĵ�λ���ֽӵ�����Ԫ��λ����֮ǰ��
	 if (BitLShfnum)
	 {		
		 UINT BitRShfnum{ BigInt::basebitnum - BitLShfnum };//BitRShfnum��Ҫ���Ƶ�λ��

		 //ǰ�������ElemLSftnum����Ԫ����Щ��Ԫȫ��0��λλ�Ʋ��ÿ����ƶ���Щ.ֱ�Ӵӵ�ElemLSftnum����Ԫ��ʼ��ֱ�������һ����Ԫ		
		 UINT N = datavector.size();
		 basedata ch = 0;

		 //�ر�ע��ԭ���ݵ�һλ�����һλ���׶�ʧ������Ҫ��������
		 //����в��뵥Ԫ��ԭ���ݵĵ�һλ��λ����ȡ��������BitLShfnum����λ���������0��Ԫ��
		 if (ActElemSftnum)
		 {
		     ch = datavector[ActElemSftnum];//����ԭ���ݵĵ�һλ
		     ch = (ch << BitLShfnum);
			 datavector[ActElemSftnum - 1] = ch;
		 }

		 //��ԭ���ݵ�2λ��ʼ������������
		 for (UINT i = ActElemSftnum; i<N - 1; ++i)
		 {			 
			 //�Ȱ���һ��Ԫ�ĵ�λ����ȡ��������BitLShfnum����λ
			 ch = datavector[i + 1];
			 ch = (ch << BitLShfnum);
			 //�ٰѱ���Ԫ����RShfnum��ͬʱ�������Ƶ���λ���ĵ�λ���
			 datavector[i] = ((datavector[i] >> BitRShfnum) | ch);
		 }
		     //���һ����Ԫ��λ��
		 datavector[N-1] = datavector[N-1] >> BitRShfnum;
	 }
	 trim();//�޼���λǰ��0���Ե�ԪΪ��λ��������bit
	 
	 return *this;
 }

 
 //���ƻ�ɾ������β�����ݣ���������  ���Ʊ����������  ���Ƶ�λ�����ܳ�������������λ��
 BigInt& BigInt::rightShift(const unsigned int n)
 {
//	 if (n > datavector.size()*BigInt::basebitnum)
//		 return *this;  //�����Ƴ���λ������ֱ�ӷ��أ���ִ�����Ʋ���

	 UINT ElemRSftnum = n >> (BigInt::basebit);//5  �൱��n/32 ��Ԫ�ƶ��������ƶ���n/32����������
	 UINT BitRShfnum = n&(BigInt::basebitchar);//0x1F  λ�ƶ���
	 UINT ActElemSftnum = ElemRSftnum; //ʵ�ʵ�Ԫ�ƶ���������Ҫɾ����λ���ݣ����Բ���Ԥ���ռ�

	 //1��������Ԫ�ƶ�  ע��β���Ǹ�λ��ͷ�����ǵ�λ	
	 for (UINT i = 0; i<ActElemSftnum; ++i)
		 datavector.erase(datavector.begin());

	 //2������λ�ƶ������յ�λ����λ��˳�����vector��Ԫ����BitRShfnumλ
	 //ע��ÿ����Ԫ����һ��Ϊ������һ��Ԫ�ĵ�λ���ֽӵ�����Ԫ��λ����֮ǰ��
	 if (BitRShfnum)
	 {
		 UINT BitLShfnum{ BigInt::basebitnum - BitRShfnum };//32 Ҫ���Ƶ�λ��,��һ�ֽ�Ҫ������

		 		
		 UINT N = datavector.size();
		 basedata ch = 0;

		 //�ر�ע��ԭ���ݵ�һλ�����һλ���׶�ʧ����
		 		

		 //��ԭ���ݵ�2λ��ʼ������������
		 for (UINT i = 0; i<N - 1; ++i)
		 {
			 //�Ȱ���һ��Ԫ�ĵ�λ����ȡ��������BitLShfnum����λ
			 ch = datavector[i + 1];
			 ch = (ch << BitLShfnum);
			 //�ٰѱ���Ԫ����RShfnum��ͬʱ�������Ƶ���λ���ĵ�λ���
			 datavector[i] = ((datavector[i] >> BitRShfnum) | ch);
		 }
		 //���һ����Ԫ��λ��
		 datavector[N - 1] = datavector[N - 1] >> BitRShfnum;
	 }
	 trim();//�޼���λǰ��0���Ե�ԪΪ��λ��������bit
	 return *this;
 }

 void BigInt::filltoEqure(BigInt& guest)
 {
	 //�Ƚ�����vector�ĳ��ȣ��Ѷ̵�һ����λ��0���һ����

	 int len{0};
	 if (datavector.size() == guest.datavector.size()){} //1�����߶�����Ҫ��λ
	 else if (datavector.size() < guest.datavector.size())//2�������̣�������λ
	 {
		 len = guest.datavector.size() - datavector.size();
		 while ((len--) > 0)

			 datavector.push_back(0);//������λ��0,β���Ǹ�λ
	 }
	 else      //3���ͷ��̣��ͷ���λ
	 {
		 len = datavector.size() - guest.datavector.size();
		 while ((len--) > 0)
			 guest.datavector.push_back(0);//guest ��Ҫ��λ��0,β���Ǹ�λ
	 }
 }


 //�������㣺����BigInt��Ķ��壬��ֵ�������ŷֿ�����ֵ��vector�洢����������_isnagative�洢������������ĺ�������ֻ��vector���д���
 //���add sub multi div���κα�����û�������ţ��൱���Ǿ���ֵ���㡣 ֻ������������غ����вſ��������š�


 //�ӷ�ԭ����һ��unsigned long��4�ֽڣ�Ϊ��λ���ѿͷ��ӵ������У�������λ
 BigInt& BigInt::add(const BigInt& guest)
 {
	 BigInt calguest{guest};  //�����������Ը��ģ����Ҫ���丳��һ���м����calguest
	 basedata previous{ 0 };

	 //���֮ǰ����Ҫ������vector�ĳ�����ͬ����ͬ��̵ĸ�λ��0 ���ﲹ0��Ҫע��
	 filltoEqure(calguest);
	
	
	 //ͬ��ʱ,vectorֱ�����
//	 if (this->_isnegative == calguest._isnegative)
//	 { 	
		 UINT carrybit{ 0 };
		 //ͷ��datavector[0]��ֵ�ĵ�λ��    β��datavector[N]����ֵ�ĸ�λ  Ӧ����ֵ��λ��ʼ�� 		 
		 for (UINT i = 0; i<datavector.size(); i++)
		 {
			 previous = datavector[i];
			 datavector[i] = datavector[i] + calguest.datavector[i] + carrybit;
			 		
			 //��λ�жϣ�һ��Ҫ�ǳ�ֱ�Ҫ������������λ�������֮���ֵ�ض����ڻ�����κ�һ������ ��֮����һ����λ
			 (datavector[i]>=previous && datavector[i] >=calguest.datavector[i]) ? carrybit = 0 : carrybit = 1;		 
			
		 }
		 //����Ͳ�����һ�����⣺���ӵ����һ��datavector[size]ʱ�Բ�����λ����datavector�����������λ��û�пռ䱣�档
		 //����취�ǣ�ʶ��������������������ʱ�ڸ�λ������һ����Ԫ�洢��λֵ

		 if (carrybit)//datavector�������е�Ԫ��û���������carrybitӦ��Ϊ0��Ϊ1�������������
		 {			
			 datavector.push_back(1);
		 }


		 /*
		 for (data_t::reverse_iterator p = datavector.rbegin(), p_guest = guest.datavector.rbegin(); p != datavector.rend(); p++, p_guest++)
	     {
		    temp = *p;
		    *p = *p+*p_guest+carrybit;

		  //�жϽ�λ�������޷������ͣ�����λ������Ӻ��ֵ�ض�С����������֮һ��������ǲ���λ��
			(*p<temp || *p < *p_guest) ? carrybit = 1 : carrybit = 0;
		   //����Ͳ�����һ�����⣺�����������������ˣ���Ӳ�����λ����λ��û�пռ䱣�档����취�ǹ�������ʱ��size��1.��4���ֽڣ�
	     }
		 */

	
	 return *this;

 }
 
 //����ԭ������ԭ��ֻ�������з������͡������Ͳ����� 
 BigInt& BigInt::sub(const BigInt& guest)
 { 
	 const BigInt& bigone{ this->smallThan(guest) ? guest : *this};
	 const BigInt& smallone{ this->smallThan(guest) ? *this : guest};

	 BigInt calbigone{ bigone };
	 BigInt calsmallone{ smallone };
	 BigInt& result{ *this };

	 data_t& res = calbigone.datavector;

		 UINT cn = 0;//��λ
		 //������С��
		 for (UINT i = 0; i<smallone.datavector.size(); ++i)
		 {
			 basedata temp = res[i];
			 res[i] = (res[i] - smallone.datavector[i] - cn);
			 cn = temp<res[i] ? 1 : temp<smallone.datavector[i] ? 1 : 0;
		 }

		 for (UINT i = smallone.datavector.size(); i<calbigone.datavector.size() && cn != 0; ++i)
		 {
			 basedata temp = res[i];
			 res[i] = res[i] - cn;
			 cn = temp<cn;
		 }
		 calbigone.trim();

	 return result=calbigone;

 }
 //�˷�ԭ������ֵС����Ϊ���� �ѳ�����ÿ��bit�뱻�����ĳ˻���Ȼ����ӡ�bit�뱻������˿ɻ�Ϊ���Ʋ�����
 BigInt& BigInt::multi(const BigInt& guest)
 {	 

	 //������֮һΪ0��ֵΪ0 
	 if (*this == 0 || guest == 0)
		 return *this=0;
	
	 
	 //���������ͷ�����ֵ����bigone,smallone  Ϊ��ֹ���������ֹ�ı�small\big������ֵ������ı������м����
	 const BigInt& bigone{ this->smallThan(guest) ? guest : *this ,false};
	 const BigInt& smallone{ this->smallThan(guest) ? *this : guest ,false};  	 
	
	 //������vector��Ϊ�������洢�����ע��Ҫ�Ƚ������㲢�Ѹ�����ʱȥ���������ۼӻ���
	 BigInt& result=*this;
	 result=0;		

//	 UINT N = smallone.datavector.size() * 32-1;
	 for (UINT i = 0; i <smallone.datavector.size() * 32; i++)
	 {		 
//		 j = N - i;
		 BigInt tempbig{ bigone ,false};		 
		 if (smallone.atbit(i))//�ж���iλ�ǲ���1
		 {			
			result.add(tempbig.leftShift(i));//��1��������iλ
		 }
	 }		

	 return result;
 }

 //����ԭ����������뱻����λ��i����������iλ(��i-1)���õ�1λ�̣����������������ٴμ�������뱻����λ��i����������iλ(��i-1)���õ�1λ�̣���������������...
 //ֱ��������С�ڳ���Ϊֹ�����йؼ����ڼ�������Ƶ�λ����
 BigInt& BigInt::div(const BigInt& guest,BigInt& re)//guest�ǳ��������ú������Ǳ�������
 {		 
	 
	 assert(guest != 0); //������Ϊ0���������쳣��������Դ���

	 BigInt a{ *this};//a�Ǳ�����  b�ǳ���  *this�洢��  re�洢����
	
	 BigInt& consult = *this;//��
	 consult = 0;//��ʼ��
	 
		
		 if (a.smallThan(guest))//������С�ڳ�������Ϊ0������Ϊ������
		 {
			 re = a;
			 return consult = 0;
		 } 
		 else if (a.equals(guest))//�������ڱ���������Ϊ1������Ϊ0
		 {
			 re = 0;
			 return consult = 1;
		 }
		 else//����С�ڱ�������������
		 {
			 while (guest.smallOrEquals(a) == true)
			 { 
				BigInt b{ guest, false };
			    UINT len = a.atsize() - b.atsize();//���a��b����Ч���ȣ�����õ�b��Ҫ���Ƶ�λ�� //bug:a��bС��len�����ʱ
			 
			   
				b = b.leftShift(len);//һλһλ�ƶ�Ч��̫�͡� �����Ч���Ⱥ����Ч�ʸ� ��vector������һ���ǳ���CPU�Ĳ�����Խ��Խ��
				//b����lenλ�������:b<a,b=a,b>a�������   ���� b>a�ǲ�����ģ�������whileѭ�����ų�������� 
				while (1)
			   {								
				 if (b.smallOrEquals(a))//
					 break;
				 b = b.rightShift(1);//���ƺ�̫����Ҫ����һλ
				 --len;
			   }
			     a=a.sub(b);

				 BigInt kk(1);
				 if (len)
					 kk.leftShift(len);
				 consult.add(kk);
			 }
			 re = a;
			
		 }
		
	 return consult;
 }







 //����vector����һ��bit�ı�ţ����vector�и�λ�Ƿ�Ϊ1
 bool BigInt::atbit(const UINT i)const
 {
	 UINT ElemIndex = i >> (BigInt::basebit);//��λ����Ԫ
	 UINT BitIndex = i&(BigInt::basebitchar);//��λ��bit
	 basedata t = datavector[ElemIndex]; //�ѵ�Ԫ��ֵ������ʱ����
	 return (t&(1 << BitIndex));//��1����BitIndexλ������datavector[ElemIndex]������������õ��ж����
 }
 
 //�õ�vector�����ݵ���Ч����
 UINT BigInt::atsize()const	
 {
	 //˼·�������λ����λ����������һ��1����Ϊ��BigInt�ĵ�һλ
	 
	 UINT size = this->datavector.size() * BigInt::basebitnum;
	 while (this->atbit(size-1) == false)
	 {
		 --size;
		 if (size == 0)
			 break;
	 }
	 return size;
 }



 //�Ѵ�������vector�����ɲ���  ֻ���з�������������
 void BigInt::tonagetive(data_t& a)
 {  	 	
	 //Ӧ�ر�ע��vector�����ݵĸߵ�λ��ζ��� ͷ��datavector[0]��ֵ�ĸ�λ��   β��datavector[N]����ֵ�ĵ�λ  
	 //�Ѳ�����ɷ��� 
	 for (UINT i = 0; i < a.size(); i++)
	 {
		 a[i] = 0xFFFFFFFF - a[i];
	 }	

	 //�ټ�1��ɲ��룬�ӵ�λ��ʼ�ӣ�ע���λ
	 bool carrybit = 0;
	 basedata previous;
	 for (UINT i = 0; i < a.size(); i++)
	 {
		 UINT j = a.size() - i-1;

		 previous = a[j];

		 if (i == 0){ a[j] += 1; }

		 a[j] = a[j] + carrybit;

		 //��λ�жϣ�һ��Ҫ�ǳ�ֱ�Ҫ������������λ�������֮���ֵ�ض����ڻ�����κ�һ������ ��֮����һ����λ
		 (a[j] >= previous ) ? carrybit = 0 : carrybit = 1;

	 }

	 	
 }

 //�ȽϾ���ֵ����������ʹ����vector���󳤶�һ�£���vector�Ĵ洢��ԪΪ�Ƚϵ�λ���Ӹ���������ȣ���ͬ��������ͬ�򷵻رȽϽ��
 bool BigInt::smallThan(const BigInt& guest) const 
 {
	 BigInt calguest{ guest };	 
	 BigInt calmaster{ *this };
	 calmaster.filltoEqure(calguest);//�����߳��Ȳ�ͬ����Ѷ̵ĸ�λ��0���һ����
	 
		 //ͷ��datavector[0]��ֵ�ĵ�λ��    β��datavector[N]����ֵ�ĸ�λ   Ӧ����ֵ���λ��ʼ�Ƚ�
	 for (UINT i = 0; i<calmaster.datavector.size(); i++)
		 {
			 UINT j = calmaster.datavector.size() - 1 - i;

			 if (calmaster.datavector[j] == calguest.datavector[j])
			 {
				 continue;//�����������ѭ�����¼����ж�
			 }
			 else if (calmaster.datavector[j] < calguest.datavector[j])
			 {
				 return true;//С��
			 }
			 else
				 return false;//����
		 }
	 return false;//��δ�˳�������˵���������
		

 }

 //11��2�գ���ɼӷ����������㡣�����У����λ��λ���⡢��������ȷ����֤
 //11��3�գ��ӡ�������ȷ��ͨ����֤����λ�����ѽ���������ڹ�������ʱ��trim()����ɾ����β�˵�0��




 bool BigInt::smallOrEquals(const BigInt& guest)const
 {
	 BigInt calguest{ guest };
	 BigInt calmaster{ *this };

	 if (calmaster.atsize() < calguest.atsize())
		 return true;
	 else if (calmaster.atsize() > calguest.atsize())
		 return false;
	 else
	 {
		 calmaster.filltoEqure(calguest);//�����߳��Ȳ�ͬ����Ѷ̵ĸ�λ��0���һ����

		 //ͷ��datavector[0]��ֵ�ĵ�λ��    β��datavector[N]����ֵ�ĸ�λ   Ӧ����ֵ���λ��ʼ�Ƚ�
		 for (UINT i = 0; i<calmaster.datavector.size(); i++)
		 {
			 UINT j = calmaster.datavector.size() - 1 - i;

			 if (calmaster.datavector[j] == calguest.datavector[j])
			 {
				 continue; //����
			 }//��������¼���
			 else if (calmaster.datavector[j] < calguest.datavector[j])
			 {
				 return true;//С��
			 }
			 else
				 return false;//����
		 }
		 return true;//������δ�жϳ���˵���������

	 } 
	 
	 
 }



 bool BigInt::equals(const BigInt& guest)const
 {
	 return datavector == guest.datavector;
 }

 //���������
 //add() sub() multi() div() smallthan �������Ⱥ������Ǿ���ֵ����
 //��������ؾ��Ǵ���������
 BigInt operator + (const BigInt& a, const BigInt& b)
 {
	
	 BigInt result{ a };
	 if (a._isnegative==b._isnegative)//ͬ��
	 {		 
		 result.add(b);
		 result._isnegative = a._isnegative;
	 }
	 else//���
	 {
		 result.sub(b);//����ֵ
		 result._isnegative = a.smallOrEquals(b) ? b._isnegative : a._isnegative;
	 }

	 return result;
 }

 BigInt operator - (const BigInt& a, const BigInt& b)
 {
	 BigInt result{ a };
	 	
	 if (b == 0)
		 return a;
	 if (a == 0)
	 {
		 result = b;
		 return -result;
	 }

	
	 if (a._isnegative == b._isnegative)//ͬ��
	 {		 
		 result.sub(b);//����ֵ
		 bool aISmall = a.smallOrEquals(b);

		 if (a._isnegative == true)//����
			 result._isnegative = aISmall ? false : true;
		 else//����
			 result._isnegative = aISmall ? true : false;
		
	 }
	 else//���
	 {
		 result.add(b);
		 result._isnegative = a._isnegative;//a-b��ţ�����������aһ��
	 }
	 return result;
 }


 BigInt operator * (const BigInt& a, const BigInt& b)
 {
	 BigInt result{ a };

	 result.multi(b);
	 result._isnegative = a._isnegative == b._isnegative ? false : true;	 
	
	 return result;
 }
 BigInt operator / (const BigInt& a, const BigInt& b)
 {
	 //����div��ֻȡ��
	 BigInt result{ a };
	 BigInt reminder{};//�洢����

	 result.div(b, reminder);	
	 result._isnegative = a._isnegative == b._isnegative ? false : true;

	 return result;//ֻ������
 }
 BigInt operator % (const BigInt& a, const BigInt& b)
 {
	 //����div��ֻȡ����
		 BigInt result{ a };
		 BigInt reminder{};//�洢����
		 result.div(b, reminder);
		 return reminder;//��������	 
 }
 bool operator < (const BigInt& a, const BigInt& b)
 {
	 return a.smallThan(b);
 }
 bool operator <= (const BigInt& a, const BigInt& b)
 {
	 return a.smallOrEquals(b);
 }
 bool operator == (const BigInt& a, const BigInt& b)
 {
	 return a.equals(b);
 }
 /*
 bool operator == (const BigInt& a, const long& b)
 { 
	 BigInt bb{ b }; 
	 return a.equals(bb);
 }
 */

 bool operator != (const BigInt& a, const BigInt& b)
 { 
	 return !a.equals(b);
 }
 


 BigInt BigInt::moden(const BigInt& exp, const BigInt& p)const
 {//ģ������


	 BigInt d(1);
	 for (int i = exp.atsize() - 1; i >= 0; --i)
	 {
		 d = (d*d) % p;
		 if (exp.atbit(i))
			 d = (d*(*this)) % p;
	 }
	 return d;
 }

 //��չŷ������㷨:����Ԫ
 BigInt BigInt::exgcd(const BigInt& m)
 {//��չŷ������㷨��˷���Ԫ
	 assert(m._isnegative == false);//mΪ����
	 BigInt a[2], b[2], t[2];

	 a[0] = 0; a[1] = m; 
	 b[0] = 1; b[1] = *this; 

	
	 

	 if (b[1] == 0 || b[1] == 1)
		 return b[1];

	 while (true)
	 {
		 if (b[1] == 1)
		 {
			 if (b[0]._isnegative == true)//����
				 b[0] = (b[0] % m + m) % m;
			 return b[0];
		 }

		 BigInt q = a[1] / b[1];
		
		 t[0] = a[0] - q * b[0];
		
		 a[0] = b[0];
		 b[0] = t[0];

		 t[1] = a[1] - q * b[1];
		
		 a[1] = b[1];
		 b[1] = t[1];

		 /*
		 for (int i = 0; i<2; ++i)
		 {
		 t[i] = a[i] - q * b[i];
		 //			 TRACE("i=%d t=%x a=%x b=%x ",i,t[i],a[i],b[i]);
		 a[i] = b[i];
		 b[i] = t[i];
		 }
		 */


	 }
 }

 //��ͨŷ������㷨:�������������Լ�� ���ݣ�շת�����gcd(a,b)=gcd(b,a%b)  
 BigInt BigInt::gcd(const BigInt a,const BigInt b)const
 {
	 bool flag = a.smallThan(b);

	 BigInt dividend{ flag==true ? b : a };//������
	 BigInt divisor{ flag==true ? a : b };//����
	 	 
	 BigInt reminder{};//����
	 
	 while (1)
	 {
		 if (divisor == 0)
			 break;
			
		 reminder = dividend % divisor;
		 dividend = divisor;
		 divisor = reminder;	

		
	 }
	 return dividend;
 }
 
 

 //����������

