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

// ���㵽�ַ�����Val Ҫת���ĸ�������width �����ȣ�prec С��λ��, sout �����ַ������͸ú����ķ���ֵһ�¡�
char *dtostrf (double val, signed char width, unsigned char prec, char *sout);

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

class __FlashStringHelper; // ��������flashָ��
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
// #define F(string_literal) (const char*)string_literal
// PSTR(string_literal)���ַ�������Flash�У���Ľ����һ��ָ��Flash���ַ���string_literal��ָ��
// Ȼ��ʹ��C++�ؼ���reinterpret_case���ַ���ָ��ǿ��ת����const __FlashStringHelper���͵�ָ�롣
// �������͵�ָ�뼴���������캯���Ĳ����� eg.  String *s = new String(F("abc"))

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
    // constructors ���캯��������һ����ʼֵ�Ŀ���,null,��Ч���ڴ����ʧ�ܣ��������Ϊfalse
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String(const char *cstr = "");
    String(const String &str);
    String(const __FlashStringHelper *str); // ��flash�е��ַ���ָ�빹��
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    String(String &&rval);
    String(StringSumHelper &&rval);
#endif
    // �����ֹ���String,��Ҫ�趨���ֽ��ơ� explicit �ؼ����޶�����ֻ�ܱ���ʽ����
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
    unsigned char reserve(unsigned int size);  // �жϵ�ǰStreng�����ܷ������³���Ϊsize���ַ������ܷ���1�����ܣ��᳢�������ڴ棩����0
    inline unsigned int length(void) const
    {
        return len;
    }

    // creates a copy of the assigned value.  if the value is null or
    // invalid, or if the memory allocation fails, the string will be
    // marked as invalid ("if (s)" will be false).
    // ���� = �����и�ֵ����
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
    // ƴ�ӣ� ƴ��ʧ�ܷ���false���ɹ����� true
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
     // ����+= ���������������Ҫ����ǰbuffer������������������ڴ�ʧ�ܣ�����ı䵱ǰ����Ҳ���᷵�ش��󣬽�����concat
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

    // ���� == �Ƚ�����
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

    // character acccess  ��ȡ�����õ�ǰ����ָ��λ�ô����ַ���Ҳ����ͨ�������[]��ʵ��
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator [] (unsigned int index) const;
    char &operator [] (unsigned int index);
    // ��ȡ��ǰ�洢���ַ����������µ�������
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const; //�ӵ�ǰ�����buffer�л�ȡbufsize���ַ�ת�����޷����ַ��ʹ���buf��index����
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const        //�ӵ�ǰ�����buffer�л�ȡbufsize���ַ�����buf��index����
    {
        getBytes((unsigned char *)buf, bufsize, index);
    }
    const char *c_str() const // ֱ�ӷ���buffer
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
    int indexOf( char ch ) const; //�ڵ�ǰ�����в����ַ�ch������ch��һ�γ��ֵ�λ�����ַ����е�����
    int indexOf( char ch, unsigned int fromIndex ) const;//��fromIndex����ʼ�ڵ�ǰ�����в����ַ�ch������ch��һ�γ��ֵ�λ�����ַ����е�����
    int indexOf( const String &str ) const;//�ڵ�ǰ�����в����ַ���str,����str��һ�γ��ֵ�λ�����ַ����е�����
    int indexOf( const String &str, unsigned int fromIndex ) const;//��fromIndex����ʼ�ڵ�ǰ�����в����ַ�str������str��һ�γ��ֵ�λ�����ַ����е�����
    int lastIndexOf( char ch ) const;
    int lastIndexOf( char ch, unsigned int fromIndex ) const;
    int lastIndexOf( const String &str ) const;
    int lastIndexOf( const String &str, unsigned int fromIndex ) const;
    String substring( unsigned int beginIndex ) //��ȡ��beginIndex����β�����ַ���
    {
        return substring(beginIndex, len);
    };
    String substring( unsigned int beginIndex, unsigned int endIndex );//��ȡ��beginIndex��endIndex֮����ַ�����

    // modification
    void replace(char find, char replace); //�������ַ�find��replace�滻
    void replace(const String &find, const String &replace);//�������ַ���find��replace�滻
    void remove(unsigned int index);//�Ƴ�index�Ժ������ַ�
    void remove(unsigned int index, unsigned int count);//��index��ʼ�Ƴ�count���ַ���
    void toLowerCase(void);// Сд
    void toUpperCase(void);// ��д
    void trim(void);//ȥ���ַ�����ͷ�ͽ�β���Ŀո�

    // parsing/conversion
    long toInt(void) const;// �ַ���רΪ����
    float toFloat(void) const;// �ַ���רΪ������
    double toDouble(void) const; 

protected:
    char *buffer;	        // the actual char array  ָ���ַ�����
    unsigned int capacity;  // the array length minus one (for the '\0') �����ɵ�����ַ�������
    unsigned int len;       // the String length (not counting the '\0') �ַ������ȣ���������������
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
