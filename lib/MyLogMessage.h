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
	bool LogMessages(string sMessages, int nLine, bool bReturn);//�����־����
	bool LogMessages(string sMessages);//����ָ���

private:
	string cls_sLogFileName;
};
#endif
