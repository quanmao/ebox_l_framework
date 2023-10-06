/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

// 浮点到字符串，Val 要转换的浮点数，width 输出宽度，prec 小数位数, sout 返回字符串，和该函数的返回值一致。
char *dtostrf (double val, signed char width, unsigned char prec, char *sout);

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

class __FlashStringHelper; // 用来处理flash指针
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
// #define F(string_literal) (const char*)string_literal
// PSTR(string_literal)将字符串放在Flash中，宏的结果是一个指向Flash中字符串string_literal的指针
// 然后使用C++关键字reinterpret_case将字符串指针强制转换成const __FlashStringHelper类型的指针。
// 这种类型的指针即第三个构造函数的参数。 eg.  String *s = new String(F("abc"))

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

// The string class
class String
{
    // use a function pointer to allow for "if (s)" without the
    // complications of an operator bool(). for more information, see:
    // http://www.artima.com/cppsource/safebool.html
    typedef void (String::*StringIfHelperType)() const;
    void StringIfHelper() const {}

public:
    // constructors 构造函数，创建一个初始值的拷贝,null,无效，内存分配失败，将被标记为false
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String(const char *cstr = "");
    String(const String &str);
    String(const __FlashStringHelper *str); // 用flash中的字符串指针构造
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String(String &&rval);
    String(StringSumHelper &&rval);
#endif
    // 用数字构造String,需要设定数字进制。 explicit 关键字限定函数只能被显式调用
    explicit String(char c);
    explicit String(unsigned char, unsigned char base = 10);
    explicit String(int, unsigned char base = 10);
    explicit String(unsigned int, unsigned char base = 10);
    explicit String(long, unsigned char base = 10);
    explicit String(unsigned long, unsigned char base = 10);
    explicit String(float, unsigned char decimalPlaces = 2);
    explicit String(double, unsigned char decimalPlaces = 2);
    ~String(void);

    // memory management
    // return true on success, false on failure (in which case, the string
    // is left unchanged).  reserve(0), if successful, will validate an
    // invalid string (i.e., "if (s)" will be true afterwards)
    unsigned char reserve(unsigned int size);  // 判断当前Streng对象能否容纳下长度为size的字符串，能返回1，不能（会尝试申请内存）返回0
    inline unsigned int length(void) const
    {
        return len;
    }

    // creates a copy of the assigned value.  if the value is null or
    // invalid, or if the memory allocation fails, the string will be
    // marked as invalid ("if (s)" will be false).
    // 重载 = ，进行赋值操作
    String &operator = (const String &rhs);
    String &operator = (const char *cstr);
    String &operator = (const __FlashStringHelper *str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String &operator = (String &&rval);
    String &operator = (StringSumHelper &&rval);
#endif

    // concatenate (works w/ built-in types)

    // returns true on success, false on failure (in which case, the string
    // is left unchanged).  if the argument is null or invalid, the
    // concatenation is considered unsucessful.
    // 拼接， 拼接失败返回false，成功返回 true
    unsigned char concat(const String &str);
    unsigned char concat(const char *cstr);
    unsigned char concat(char c);
    unsigned char concat(unsigned char c);
    unsigned char concat(int num);
    unsigned char concat(unsigned int num);
    unsigned char concat(long num);
    unsigned char concat(unsigned long num);
    unsigned char concat(float num);
    unsigned char concat(double num);
    unsigned char concat(const __FlashStringHelper *str);

    // if there's not enough memory for the concatenated value, the string
    // will be left unchanged (but this isn't signalled in any way)
     // 重载+= ，该运算符可能需要扩大当前buffer容量，如果额外申请内存失败，不会改变当前对象，也不会返回错误，建议用concat
    String &operator += (const String &rhs)
    {
        concat(rhs);
        return (*this);
    }
    String &operator += (const char *cstr)
    {
        concat(cstr);
        return (*this);
    }
    String &operator += (char c)
    {
        concat(c);
        return (*this);
    }
    String &operator += (unsigned char num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (int num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (unsigned int num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (long num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (unsigned long num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (float num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (double num)
    {
        concat(num);
        return (*this);
    }
    String &operator += (const __FlashStringHelper *str)
    {
        concat(str);
        return (*this);
    }

    friend StringSumHelper &operator + (const StringSumHelper &lhs, const String &rhs);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, const char *cstr);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, char c);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned char num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, int num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned int num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, long num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, unsigned long num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, float num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, double num);
    friend StringSumHelper &operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs);

    // comparison (only works w/ Strings and "strings")
    operator StringIfHelperType() const
    {
        return buffer ? &String::StringIfHelper : 0;
    }
    int compareTo(const String &s) const;
    unsigned char equals(const String &s) const;
    unsigned char equals(const char *cstr) const;

    // 重载 == 比较类型
    unsigned char operator == (const String &rhs) const
    {
        return equals(rhs);
    }
    unsigned char operator == (const char *cstr) const
    {
        return equals(cstr);
    }
   
    unsigned char operator != (const String &rhs) const
    {
        return !equals(rhs);
    }
    unsigned char operator != (const char *cstr) const
    {
        return !equals(cstr);
    }
    unsigned char operator <  (const String &rhs) const;
    unsigned char operator >  (const String &rhs) const;
    unsigned char operator <= (const String &rhs) const;
    unsigned char operator >= (const String &rhs) const;
    unsigned char equalsIgnoreCase(const String &s) const;
    unsigned char startsWith( const String &prefix) const;
    unsigned char startsWith(const String &prefix, unsigned int offset) const;
    unsigned char endsWith(const String &suffix) const;

    // character acccess  获取和设置当前对象指定位置处的字符，也可以通过运算符[]来实现
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator [] (unsigned int index) const;
    char &operator [] (unsigned int index);
    // 获取当前存储的字符串，存入新的数组中
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const; //从当前对象的buffer中获取bufsize个字符转换成无符号字符型存入buf的index处。
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const        //从当前对象的buffer中获取bufsize个字符存入buf的index处。
    {
        getBytes((unsigned char *)buf, bufsize, index);
    }
    const char *c_str() const // 直接返回buffer
    {
        return buffer;
    }
    char *begin()
    {
        return buffer;
    }
    char *end()
    {
        return buffer + length();
    }
    const char *begin() const
    {
        return c_str();
    }
    const char *end() const
    {
        return c_str() + length();
    }

    // search
    int indexOf( char ch ) const; //在当前对象中查找字符ch，返回ch第一次出现的位置在字符串中的索引
    int indexOf( char ch, unsigned int fromIndex ) const;//从fromIndex处开始在当前对象中查找字符ch，返回ch第一次出现的位置在字符串中的索引
    int indexOf( const String &str ) const;//在当前对象中查找字符串str,返回str第一次出现的位置在字符串中的索引
    int indexOf( const String &str, unsigned int fromIndex ) const;//从fromIndex处开始在当前对象中查找字符str，返回str第一次出现的位置在字符串中的索引
    int lastIndexOf( char ch ) const;
    int lastIndexOf( char ch, unsigned int fromIndex ) const;
    int lastIndexOf( const String &str ) const;
    int lastIndexOf( const String &str, unsigned int fromIndex ) const;
    String substring( unsigned int beginIndex ) //获取从beginIndex到结尾的子字符串
    {
        return substring(beginIndex, len);
    };
    String substring( unsigned int beginIndex, unsigned int endIndex );//获取从beginIndex到endIndex之间的字符串。

    // modification
    void replace(char find, char replace); //将所有字符find用replace替换
    void replace(const String &find, const String &replace);//将所有字符串find用replace替换
    void remove(unsigned int index);//移除index以后所有字符
    void remove(unsigned int index, unsigned int count);//从index开始移除count个字符。
    void toLowerCase(void);// 小写
    void toUpperCase(void);// 大写
    void trim(void);//去掉字符串开头和结尾处的空格

    // parsing/conversion
    long toInt(void) const;// 字符串专为整型
    float toFloat(void) const;// 字符串专为浮点型
    double toDouble(void) const; 

protected:
    char *buffer;	        // the actual char array  指向字符数组
    unsigned int capacity;  // the array length minus one (for the '\0') 能容纳的最大字符数（）
    unsigned int len;       // the String length (not counting the '\0') 字符串长度（不包含结束符）
protected:
    void init(void);
    void invalidate(void);
    unsigned char changeBuffer(unsigned int maxStrLen);
    unsigned char concat(const char *cstr, unsigned int length);

    // copy and move
    String &copy(const char *cstr, unsigned int length);
    String &copy(const __FlashStringHelper *pstr, unsigned int length);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    void move(String &rhs);
#endif
};

class StringSumHelper : public String
{
public:
    StringSumHelper(const String &s) : String(s) {}
    StringSumHelper(const char *p) : String(p) {}
    StringSumHelper(char c) : String(c) {}
    StringSumHelper(unsigned char num) : String(num) {}
    StringSumHelper(int num) : String(num) {}
    StringSumHelper(unsigned int num) : String(num) {}
    StringSumHelper(long num) : String(num) {}
    StringSumHelper(unsigned long num) : String(num) {}
    StringSumHelper(float num) : String(num) {}
    StringSumHelper(double num) : String(num) {}
};

#endif  // __cplusplus
#endif  // String_class_h
