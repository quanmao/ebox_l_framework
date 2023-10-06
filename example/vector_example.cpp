#include "ebox.h"
#include "bsp_ebox.h"
#include "vector.h"
#include "WString.h"

stl::Vector<int> v;
stl::Vector<String>s;

void setup()
{
	eboxInit();
	uart1.begin(115200);
	uart1.printf("Vector test\r\n");
}

void inttest()
{
	v.push_back(0);
	v.push_back(1);
		v.push_back(2);
	v.push_back(3);
		v.push_back(4);
	v.push_back(5);
	uart1.printf(" size %d\r\n",v.size());
	for(stl::Vector<int>::iterator p = v.begin(); p!= v.end();p++){
		uart1.printf("int: %d\r\n",*p);
	}
	
	for(uint8_t i = 0; i< v.size();i++){
		uart1.printf("int: %d\r\n",v[i]);
	}
}

void stringtest()
{
	s.push_back("1");
	s.push_back("22");
	s.push_back("333");
	s.push_back("4444");
	s.push_back("55555");
	s.push_back("666666");
	s.push_back("666667");
	s.push_back("666668");
	s.push_back("666669");
	s.push_back("666670");
	uart1.printf(" size %d\r\n",s.size());
	for(stl::Vector<String>::iterator p = s.begin(); p!= s.end();p++){
		uart1.printf("string: %d\r\n",p->length());
	}
	
	for(uint8_t i = 0; i< s.size();i++){
		uart1.printf("string: %s\r\n",s[i].c_str());
	}
}

void test(void)
{
	uart1.printf("v size %d ,v capacity %d \r\n",v.size(),v.capacity());
	uart1.printf("s size %d ,s capacity %d \r\n",s.size(),s.capacity());
	inttest();
	stringtest();  // 挪到 inttest()前面测试,就没有问题了
	while(1)
	{		

	}
}
