#ifndef MYLOGMESSAGE_H
#define MYLOGMESSAGE_H

#include <stdio.h>   
#include <stdlib.h>   
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

class CMyLogMessages
{ 
public: 
	CMyLogMessages(); 
	virtual ~CMyLogMessages();

public:
	bool SetLogFilePath(string sLogFileName, string sProjectName);
	bool LogMessages(string sMessages, int nLine, bool bReturn);//输出日志内容
	bool LogMessages(string sMessages);//输出分隔符

private:
	string cls_sLogFileName;
};
#endif
