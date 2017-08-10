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

	//cSection:[��ȡ��ʶ]  cKeyName:��ȡ�Ĺؼ���  cKeyValue:�ؼ��ֵ�ֵ�����ı���  nLen:��ȡ�ĳ���  cFileName:�����ļ�·��
	bool GetConfigStr(const char* cSection, const char* cKeyName, string& sKeyValue, unsigned int nLen, const char* cFileName);

private:
	CMyLogMessages cls_LogMessage;
	bool ReadLine(char* cLine, FILE* cStream); 
	int IsRemark(const char* cLine);//#����1
	bool GetSection(const char* cLine, char* cSection) ; 
	bool GetKey(const char* cLine, char* cKeyName, char* cKeyValue);
	int GetConfig(const char* cSection, const char* cKeyName, char* cKeyValue, unsigned int nLen, const char* cFileName);
};


#endif

