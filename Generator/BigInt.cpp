#include"stdafx.h"
#include"BigInt.h"


char BigInt::hexto4bit(char ch)
{
	//只能转换数字0~9，字母A~F     超出范围的字符不能正确转换
	static char table[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	if (isdigit(ch))//isdigit函数，若ch是数字，返回true,否则返回false
		ch -= '0';
	else if (islower(ch))//islower函数，检查参数ch是否为小写英文字母。
		ch -= 'a' - 10;
	else if (isupper(ch))//isupper函数，检查参数ch是否为大写英文字母。
		ch -= 'A' - 10;

	return table[ch];
}
//把string转为BigInt
void BigInt::copyFromHexString(const string& s)
{
	string str{s};
	//负号时，把负号去掉，负号由_isnegative表示
	if (str.length() && str.at(0) == '-')
	{
		if (str.length()>1)
			_isnegative = true;
		str = str.substr(1);
	}

	//一个unigned long可以存8个字符。但如果有9个字符呢？需要再造出7字符（全0）,接在一起后用2个unsigned long存。
	//字符串temp用于把字符串str凑整成8的倍数。全是0，数量0~7。
	int count = (8 - (str.length() % 8)) % 8;//count是凑整所需字符位数，0~7
	string temp{""};
	for (int i = 0; i<count; ++i)
		temp.push_back(0);
	
	str = temp + str;//把temp加到字符串str的高位，使str字符串正好能装满vector每个bit而没有空缺。




	//顺序定位每个字符并将其转换为用4bit表示，并存储
	for (basedata i = 0; i<str.length(); i += 8)//每8个字符位就是一个unsigned long（4字节）
	{
		basedata sum = 0;
		for (int j = 0; j<8; ++j)//顺序取出这8个字符
		{
			char ch = str[i + j];//ch是表示HEX的字符1，2，3...f但是用ASCII码表示的

			ch = hexto4bit(ch); //转换后仍然是同样的字符1，2，3...f但是是用4个bit表示
			sum = ((sum << 4) | (ch));//一次4个bit，向vector单元sum中存入
		}
		//把sum往全局向量中存入
		datavector.push_back(sum);//此时，头部datavector[0]：数值的高位   容器尾部datavector[N]：数值的低位， 
	}
	
	reverse(datavector.begin(), datavector.end());//翻转过来，容器尾部datavector[N]：数值的高位  可以随意在尾部插入0（即高位补0）

}

//函数功能：把vector尾部的0去掉,以4byte为单位
 void BigInt::trim()
{
	int count = 0;
	int cou1=0;
	//检查为0的元素的数量		
	for (data_t::reverse_iterator it = datavector.rbegin(); it != datavector.rend(); ++it)
	{
		if ((*it) == 0)
			++count;
		else
			break;//如果不为0则跳出for循环
	}

		if (count == datavector.size())//只有零的情况保留			
			--count;
		for (int i = 0; i < count; ++i)
			datavector.pop_back();
	
//	return *this;
	return;
}

 //把long型转为BigInt
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
		 a = a >> (BigInt::basebitnum/2);  //位操作=位操作%32,因此位操作范围是:0~31  右移32位，其实不移位
		
	 } while (a);
 }


 //11月1日：今天理解了以上函数，下一步搞定四则运算

 //函数功能：将大数整体左移n位  原理：对vector的数据进行移动，从低位插入一个单元，等于左移32bit!! 
 //因此，最高效的左移方法是：1、先单元移动：由移动位数计算单元移动数（puck_back）ElemSftnum=n/32，若有余数，则实际左移ElemSftnum+1（后面再通过位移动 右移回来） 
 //2、再进行位移动：往右移回，整个vector每个单元都要重排
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
		 //左移
		 basedata ch = 0;
		 for (std::size_t i = 0; i<datavector.size(); ++i)
		 {
			 basedata t = datavector[i];
			 datavector[i] = (t << off) | ch;
			 ch = (t&T) >> (BigInt::basebitnum - off);//32,最高位
		 }
	 }
	 trim();
	 */
	 
	 UINT ElemLSftnum = n >> (BigInt::basebit);//5  相当于n/32取整  单元移动数
	 UINT BitLShfnum = n&(BigInt::basebitchar);//0x1F              位移动数
	 UINT ActElemSftnum = (BitLShfnum == 0) ? ElemLSftnum : 1 + ElemLSftnum; //实际单元移动数：有余数，单元移动数+1
	 	 	 
	 //1、先做单元移动  注意尾部是高位，头部是低位，要往头部加0单元！
	 const basedata none{ 0 };
	 for (UINT i = 0; i<ActElemSftnum; ++i)
		 datavector.insert(datavector.begin(), none);	 	 

	 //2、若有位移动，按照低位到高位的顺序，逐个vector单元左移32-BitSftnum位
	 //注意每个单元都被一分为二，下一单元的低位部分接到本单元高位部分之前。
	 if (BitLShfnum)
	 {		
		 UINT BitRShfnum{ BigInt::basebitnum - BitLShfnum };//BitRShfnum是要右移的位数

		 //前面插入了ElemLSftnum个单元，这些单元全是0，位位移不用考虑移动这些.直接从第ElemLSftnum个单元开始，直到到最后一个单元		
		 UINT N = datavector.size();
		 basedata ch = 0;

		 //特别注意原数据第一位和最后一位容易丢失！！需要单独处理
		 //如果有插入单元，原数据的第一位低位部分取出并左移BitLShfnum到高位后放入插入的0单元中
		 if (ActElemSftnum)
		 {
		     ch = datavector[ActElemSftnum];//这是原数据的第一位
		     ch = (ch << BitLShfnum);
			 datavector[ActElemSftnum - 1] = ch;
		 }

		 //从原数据第2位开始可以这样操作
		 for (UINT i = ActElemSftnum; i<N - 1; ++i)
		 {			 
			 //先把下一单元的低位部分取出并左移BitLShfnum到高位
			 ch = datavector[i + 1];
			 ch = (ch << BitLShfnum);
			 //再把本单元右移RShfnum，同时与上述移到高位处的低位结合
			 datavector[i] = ((datavector[i] >> BitRShfnum) | ch);
		 }
		     //最后一个单元的位移
		 datavector[N-1] = datavector[N-1] >> BitRShfnum;
	 }
	 trim();//修剪高位前的0，以单元为单位，不是以bit
	 
	 return *this;
 }

 
 //右移会删除容器尾部数据！！！！！  右移比左移情况简单  右移的位数不能超过容器现有总位数
 BigInt& BigInt::rightShift(const unsigned int n)
 {
//	 if (n > datavector.size()*BigInt::basebitnum)
//		 return *this;  //若右移超过位数，则直接返回，不执行右移操作

	 UINT ElemRSftnum = n >> (BigInt::basebit);//5  相当于n/32 单元移动数：总移动数n/32的整数部分
	 UINT BitRShfnum = n&(BigInt::basebitchar);//0x1F  位移动数
	 UINT ActElemSftnum = ElemRSftnum; //实际单元移动数：右移要删除低位数据，所以不用预留空间

	 //1、先做单元移动  注意尾部是高位，头部才是低位	
	 for (UINT i = 0; i<ActElemSftnum; ++i)
		 datavector.erase(datavector.begin());

	 //2、若有位移动，按照低位到高位的顺序，逐个vector单元右移BitRShfnum位
	 //注意每个单元都被一分为二，下一单元的低位部分接到本单元高位部分之前。
	 if (BitRShfnum)
	 {
		 UINT BitLShfnum{ BigInt::basebitnum - BitRShfnum };//32 要右移的位数,下一字节要补过来

		 		
		 UINT N = datavector.size();
		 basedata ch = 0;

		 //特别注意原数据第一位和最后一位容易丢失！！
		 		

		 //从原数据第2位开始可以这样操作
		 for (UINT i = 0; i<N - 1; ++i)
		 {
			 //先把下一单元的低位部分取出并左移BitLShfnum到高位
			 ch = datavector[i + 1];
			 ch = (ch << BitLShfnum);
			 //再把本单元右移RShfnum，同时与上述移到高位处的低位结合
			 datavector[i] = ((datavector[i] >> BitRShfnum) | ch);
		 }
		 //最后一个单元的位移
		 datavector[N - 1] = datavector[N - 1] >> BitRShfnum;
	 }
	 trim();//修剪高位前的0，以单元为单位，不是以bit
	 return *this;
 }

 void BigInt::filltoEqure(BigInt& guest)
 {
	 //比较两个vector的长度，把短的一个高位补0变成一样长

	 int len{0};
	 if (datavector.size() == guest.datavector.size()){} //1、两者都不需要补位
	 else if (datavector.size() < guest.datavector.size())//2、主方短，主方补位
	 {
		 len = guest.datavector.size() - datavector.size();
		 while ((len--) > 0)

			 datavector.push_back(0);//主方高位补0,尾部是高位
	 }
	 else      //3、客方短，客方补位
	 {
		 len = datavector.size() - guest.datavector.size();
		 while ((len--) > 0)
			 guest.datavector.push_back(0);//guest 需要高位补0,尾部是高位
	 }
 }


 //四则运算：根据BigInt类的定义，数值与正负号分开。数值由vector存储，正负号由_isnagative存储。在四则运算的函数层面只对vector进行处理，
 //因此add sub multi div中任何变量都没有正负号，相当于是绝对值运算。 只有在运算符重载函数中才考虑正负号。


 //加法原理：以一个unsigned long（4字节）为单位，把客方加到主方中，溢出则进位
 BigInt& BigInt::add(const BigInt& guest)
 {
	 BigInt calguest{guest};  //被加数不可以更改，因此要将其赋给一个中间变量calguest
	 basedata previous{ 0 };

	 //相加之前，需要让两个vector的长度相同，不同则短的高位补0 这里补0需要注意
	 filltoEqure(calguest);
	
	
	 //同号时,vector直接相加
//	 if (this->_isnegative == calguest._isnegative)
//	 { 	
		 UINT carrybit{ 0 };
		 //头部datavector[0]数值的低位；    尾部datavector[N]：数值的高位  应从数值低位开始加 		 
		 for (UINT i = 0; i<datavector.size(); i++)
		 {
			 previous = datavector[i];
			 datavector[i] = datavector[i] + calguest.datavector[i] + carrybit;
			 		
			 //进位判断，一定要是充分必要条件。若不进位，则相加之后的值必定大于或等于任何一个加数 反之，则一定进位
			 (datavector[i]>=previous && datavector[i] >=calguest.datavector[i]) ? carrybit = 0 : carrybit = 1;		 
			
		 }
		 //这里就产生了一个问题：若加到最后一个datavector[size]时仍产生进位，即datavector发生溢出，进位将没有空间保存。
		 //解决办法是：识别这种情况，出现这情况时在高位再增加一个单元存储进位值

		 if (carrybit)//datavector加完所有单元后，没有溢出的则carrybit应该为0；为1就是上述的情况
		 {			
			 datavector.push_back(1);
		 }


		 /*
		 for (data_t::reverse_iterator p = datavector.rbegin(), p_guest = guest.datavector.rbegin(); p != datavector.rend(); p++, p_guest++)
	     {
		    temp = *p;
		    *p = *p+*p_guest+carrybit;

		  //判断进位。对于无符号类型，若进位，则相加后的值必定小于两个加数之一；否则就是不进位。
			(*p<temp || *p < *p_guest) ? carrybit = 1 : carrybit = 0;
		   //这里就产生了一个问题：若两个加数向量满了，相加产生进位，进位将没有空间保存。解决办法是构建向量时，size加1.（4个字节）
	     }
		 */

	
	 return *this;

 }
 
 //减法原理：补码原理只适用于有符号类型。本类型不适用 
 BigInt& BigInt::sub(const BigInt& guest)
 { 
	 const BigInt& bigone{ this->smallThan(guest) ? guest : *this};
	 const BigInt& smallone{ this->smallThan(guest) ? *this : guest};

	 BigInt calbigone{ bigone };
	 BigInt calsmallone{ smallone };
	 BigInt& result{ *this };

	 data_t& res = calbigone.datavector;

		 UINT cn = 0;//借位
		 //大数减小数
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
 //乘法原理：绝对值小的作为乘数 把乘数的每个bit与被乘数的乘积，然后相加。bit与被乘数相乘可化为左移操作。
 BigInt& BigInt::multi(const BigInt& guest)
 {	 

	 //若其中之一为0，值为0 
	 if (*this == 0 || guest == 0)
		 return *this=0;
	
	 
	 //把主方、客方的数值赋给bigone,smallone  为防止出错，定义禁止改变small\big变量的值，如需改变另设中间变量
	 const BigInt& bigone{ this->smallThan(guest) ? guest : *this ,false};
	 const BigInt& smallone{ this->smallThan(guest) ? *this : guest ,false};  	 
	
	 //把主方vector作为容器来存储结果，注意要先将其清零并把负号暂时去掉，以免累加混乱
	 BigInt& result=*this;
	 result=0;		

//	 UINT N = smallone.datavector.size() * 32-1;
	 for (UINT i = 0; i <smallone.datavector.size() * 32; i++)
	 {		 
//		 j = N - i;
		 BigInt tempbig{ bigone ,false};		 
		 if (smallone.atbit(i))//判读第i位是不是1
		 {			
			result.add(tempbig.leftShift(i));//是1，则左移i位
		 }
	 }		

	 return result;
 }

 //除法原理：计算除数与被除数位差i，除数左移i位(或i-1)，得到1位商，被除数减除数；再次计算除数与被除数位差i，除数左移i位(或i-1)，得到1位商，被除数减除数；...
 //直到被除数小于除数为止！其中关键在于计算出左移的位数。
 BigInt& BigInt::div(const BigInt& guest,BigInt& re)//guest是除数，调用函数者是被除数。
 {		 
	 
	 assert(guest != 0); //除数若为0，则属于异常，输出断言错误

	 BigInt a{ *this};//a是被除数  b是除数  *this存储商  re存储余数
	
	 BigInt& consult = *this;//商
	 consult = 0;//初始化
	 
		
		 if (a.smallThan(guest))//被除数小于除数，商为0，余数为被除数
		 {
			 re = a;
			 return consult = 0;
		 } 
		 else if (a.equals(guest))//除数等于被除数，商为1，余数为0
		 {
			 re = 0;
			 return consult = 1;
		 }
		 else//除数小于被除数，做运算
		 {
			 while (guest.smallOrEquals(a) == true)
			 { 
				BigInt b{ guest, false };
			    UINT len = a.atsize() - b.atsize();//求出a和b的有效长度，相减得到b需要左移的位数 //bug:a比b小，len无穷大时
			 
			   
				b = b.leftShift(len);//一位一位移动效率太低。 求出有效长度后相减效率高 把vector左移是一个非常耗CPU的操作，越少越好
				//b左移len位后可能有:b<a,b=a,b>a三种情况   其中 b>a是不允许的，下面用while循环来排除这种情况 
				while (1)
			   {								
				 if (b.smallOrEquals(a))//
					 break;
				 b = b.rightShift(1);//左移后太大，需要右移一位
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







 //输入vector任意一个bit的编号，输出vector中该位是否为1
 bool BigInt::atbit(const UINT i)const
 {
	 UINT ElemIndex = i >> (BigInt::basebit);//定位到单元
	 UINT BitIndex = i&(BigInt::basebitchar);//定位到bit
	 basedata t = datavector[ElemIndex]; //把单元的值赋给临时变量
	 return (t&(1 << BitIndex));//把1左移BitIndex位，再与datavector[ElemIndex]进行与操作，得到判定结果
 }
 
 //得到vector中数据的有效长度
 UINT BigInt::atsize()const	
 {
	 //思路：从最高位往低位数，碰到第一个1就认为是BigInt的第一位
	 
	 UINT size = this->datavector.size() * BigInt::basebitnum;
	 while (this->atbit(size-1) == false)
	 {
		 --size;
		 if (size == 0)
			 break;
	 }
	 return size;
 }



 //把传进来的vector对象变成补码  只对有符号类型起作用
 void BigInt::tonagetive(data_t& a)
 {  	 	
	 //应特别注意vector中数据的高低位如何定义 头部datavector[0]数值的高位；   尾部datavector[N]：数值的低位  
	 //把参数变成反码 
	 for (UINT i = 0; i < a.size(); i++)
	 {
		 a[i] = 0xFFFFFFFF - a[i];
	 }	

	 //再加1变成补码，从低位开始加，注意进位
	 bool carrybit = 0;
	 basedata previous;
	 for (UINT i = 0; i < a.size(); i++)
	 {
		 UINT j = a.size() - i-1;

		 previous = a[j];

		 if (i == 0){ a[j] += 1; }

		 a[j] = a[j] + carrybit;

		 //进位判断，一定要是充分必要条件。若不进位，则相加之后的值必定大于或等于任何一个加数 反之，则一定进位
		 (a[j] >= previous ) ? carrybit = 0 : carrybit = 1;

	 }

	 	
 }

 //比较绝对值，方法是先使两个vector对象长度一致，以vector的存储单元为比较单位，从高往低逐个比，相同继续，不同则返回比较结果
 bool BigInt::smallThan(const BigInt& guest) const 
 {
	 BigInt calguest{ guest };	 
	 BigInt calmaster{ *this };
	 calmaster.filltoEqure(calguest);//若两者长度不同，则把短的高位补0变成一样长
	 
		 //头部datavector[0]数值的低位；    尾部datavector[N]：数值的高位   应从数值最高位开始比较
	 for (UINT i = 0; i<calmaster.datavector.size(); i++)
		 {
			 UINT j = calmaster.datavector.size() - 1 - i;

			 if (calmaster.datavector[j] == calguest.datavector[j])
			 {
				 continue;//相等则跳过此循环往下继续判断
			 }
			 else if (calmaster.datavector[j] < calguest.datavector[j])
			 {
				 return true;//小于
			 }
			 else
				 return false;//大于
		 }
	 return false;//仍未退出函数，说明两者相等
		

 }

 //11月2日：完成加法、减法运算。问题有：最高位进位问题、减法的正确性验证
 //11月3日：加、减法正确性通过验证。进位问题已解决，是由于构建向量时，trim()函数删掉了尾端的0。




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
		 calmaster.filltoEqure(calguest);//若两者长度不同，则把短的高位补0变成一样长

		 //头部datavector[0]数值的低位；    尾部datavector[N]：数值的高位   应从数值最高位开始比较
		 for (UINT i = 0; i<calmaster.datavector.size(); i++)
		 {
			 UINT j = calmaster.datavector.size() - 1 - i;

			 if (calmaster.datavector[j] == calguest.datavector[j])
			 {
				 continue; //等于
			 }//相等则往下继续
			 else if (calmaster.datavector[j] < calguest.datavector[j])
			 {
				 return true;//小于
			 }
			 else
				 return false;//大于
		 }
		 return true;//以上仍未判断出，说明两者相等

	 } 
	 
	 
 }



 bool BigInt::equals(const BigInt& guest)const
 {
	 return datavector == guest.datavector;
 }

 //运算符重载
 //add() sub() multi() div() smallthan 。。。等函数均是绝对值运算
 //运算符重载均是带符号运算
 BigInt operator + (const BigInt& a, const BigInt& b)
 {
	
	 BigInt result{ a };
	 if (a._isnegative==b._isnegative)//同号
	 {		 
		 result.add(b);
		 result._isnegative = a._isnegative;
	 }
	 else//异号
	 {
		 result.sub(b);//绝对值
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

	
	 if (a._isnegative == b._isnegative)//同号
	 {		 
		 result.sub(b);//绝对值
		 bool aISmall = a.smallOrEquals(b);

		 if (a._isnegative == true)//负号
			 result._isnegative = aISmall ? false : true;
		 else//正号
			 result._isnegative = aISmall ? true : false;
		
	 }
	 else//异号
	 {
		 result.add(b);
		 result._isnegative = a._isnegative;//a-b异号，符号总是与a一致
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
	 //调用div，只取商
	 BigInt result{ a };
	 BigInt reminder{};//存储余数

	 result.div(b, reminder);	
	 result._isnegative = a._isnegative == b._isnegative ? false : true;

	 return result;//只返回商
 }
 BigInt operator % (const BigInt& a, const BigInt& b)
 {
	 //调用div，只取余数
		 BigInt result{ a };
		 BigInt reminder{};//存储余数
		 result.div(b, reminder);
		 return reminder;//返回余数	 
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
 {//模幂运算


	 BigInt d(1);
	 for (int i = exp.atsize() - 1; i >= 0; --i)
	 {
		 d = (d*d) % p;
		 if (exp.atbit(i))
			 d = (d*(*this)) % p;
	 }
	 return d;
 }

 //扩展欧几里得算法:求逆元
 BigInt BigInt::exgcd(const BigInt& m)
 {//扩展欧几里得算法求乘法逆元
	 assert(m._isnegative == false);//m为正数
	 BigInt a[2], b[2], t[2];

	 a[0] = 0; a[1] = m; 
	 b[0] = 1; b[1] = *this; 

	
	 

	 if (b[1] == 0 || b[1] == 1)
		 return b[1];

	 while (true)
	 {
		 if (b[1] == 1)
		 {
			 if (b[0]._isnegative == true)//负数
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

 //普通欧几里得算法:求两个数的最大公约数 依据：辗转相除法gcd(a,b)=gcd(b,a%b)  
 BigInt BigInt::gcd(const BigInt a,const BigInt b)const
 {
	 bool flag = a.smallThan(b);

	 BigInt dividend{ flag==true ? b : a };//被除数
	 BigInt divisor{ flag==true ? a : b };//除数
	 	 
	 BigInt reminder{};//余数
	 
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
 
 

 //快速幂运算

