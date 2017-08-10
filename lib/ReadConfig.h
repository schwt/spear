#ifndef READCONFIG_H
#define READCONFIG_H

//#pragma warning(disable:4996)
//#pragma warning(disable:4267)
//#pragma warning(disable:4018)

#include <string.h>
#include <stdio.h>   
#include <stdlib.h>   
#include <ctype.h>   
#include <string>
#include "MyLogMessage.h"

#define CFG_LINE 1024   
#define CFG_NOERROR 0 //read configuration file successfully   
#define CFG_NOFILE -1 //not find or open configuration file   
#define CFG_NOFIND -2 //not find section or key name in configuration file

typedef long long __int64;

using namespace std;

class CReadConfig 
{ 
public: 
	CReadConfig(const string& sLogFileName); 
	virtual ~CReadConfig(); 

	//cSection:[读取标识]  cKeyName:读取的关键字  cKeyValue:关键字的值存贮的变量  nLen:读取的长度  cFileName:配置文件路径
	bool GetConfigStr(const char* cSection, const char* cKeyName, string& sKeyValue, unsigned int nLen, const char* cFileName);

private:
	CMyLogMessages cls_LogMessage;
	bool ReadLine(char* cLine, FILE* cStream); 
	int IsRemark(const char* cLine);//#返回1
	bool GetSection(const char* cLine, char* cSection) ; 
	bool GetKey(const char* cLine, char* cKeyName, char* cKeyValue);
	int GetConfig(const char* cSection, const char* cKeyName, char* cKeyValue, unsigned int nLen, const char* cFileName);
};


#endif

