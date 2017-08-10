#include "MyLogMessage.h"

CMyLogMessages::CMyLogMessages()
{} 
CMyLogMessages::~CMyLogMessages() 
{
	LogMessages("********************************************************\n\n");	
}


bool CMyLogMessages::SetLogFilePath(string sLogFileName, string sProjectName)
{
	cls_sLogFileName=sLogFileName;

	char cBuffer[1024]={0};
	
	time_t t_Time;
	struct tm *tm_time;
	t_Time=time(NULL);
	tm_time=localtime(&t_Time);		

	FILE *fpLog;
	if((fpLog=fopen(cls_sLogFileName.c_str(),"at"))==NULL)
	{	
		sprintf(cBuffer,"%02d-%02d-%02d_%02d:%02d:%02d [ ERROR ] [Msg:Open Log.txt File] [Line:%d]\n",
		        tm_time->tm_year+1900, tm_time->tm_mon+1, tm_time->tm_mday, 
				tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec, __LINE__);
				
		printf("%s", cBuffer);
		fprintf(fpLog, "%s", cBuffer);
		return false;
	}

	if (sProjectName!="")
	{
		fprintf(fpLog,"\n\n*************************************************************\n");
		fprintf(fpLog,"**************************** %s ****************************\n", sProjectName.c_str());
	}

	fclose(fpLog);
	fpLog=NULL;

	return true;
}

//日志信息
bool CMyLogMessages::LogMessages(string sMessages, int nLine, bool bReturn)
{
	char cBuffer[1024]={0};
	
	time_t t_Time;
	struct tm *tm_time;
	t_Time=time(NULL);
	tm_time=localtime(&t_Time);	

	FILE *fpLog;
	if((fpLog=fopen(cls_sLogFileName.c_str(),"at"))==NULL)
	{	
		sprintf(cBuffer, "%02d-%02d-%02d_%02d:%02d:%02d [ ERROR ] [Msg:Open Log.txt File] [Line:%d]\n", 
		        tm_time->tm_year+1900, tm_time->tm_mon+1, tm_time->tm_mday, 
				tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec, nLine);
				
		printf("%s", cBuffer);
		fprintf(fpLog, "%s", cBuffer);
		return false;
	}

	if (bReturn)
	{
		sprintf(cBuffer,"%02d-%02d-%02d_%02d:%02d:%02d [ OK ] [Msg:%s] [Line:%d]\n", 
		        tm_time->tm_year+1900, tm_time->tm_mon+1, tm_time->tm_mday, 
				tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec, 
				sMessages.c_str(), nLine);
				
		printf("%s", cBuffer);
		fprintf(fpLog, "%s", cBuffer);
	}
	else
	{
		sprintf(cBuffer,"%02d-%02d-%02d_%02d:%02d:%02d [ ERROR ] [Msg:%s] [Line:%d]\n", 
				tm_time->tm_year+1900, tm_time->tm_mon+1, tm_time->tm_mday, 
				tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec,
				sMessages.c_str(), nLine);
				
		printf("%s", cBuffer);
		fprintf(fpLog, "%s", cBuffer);
	}

	fclose(fpLog);
	fpLog=NULL;

	return bReturn;
}

//分隔符
bool CMyLogMessages::LogMessages(string sMessages)
{
	char cBuffer[1024]={0};
	
	time_t t_Time;
	struct tm *tm_time;
	t_Time=time(NULL);
	tm_time=localtime(&t_Time);	

	FILE *fpLog;
	if((fpLog=fopen(cls_sLogFileName.c_str(),"at"))==NULL)
	{	
		sprintf(cBuffer,"%02d-%02d-%02d_%02d:%02d:%02d [ ERROR ] [Msg:Open Log.txt File] [Line:%d]\n",
		        tm_time->tm_year+1900, tm_time->tm_mon+1, tm_time->tm_mday, 
				tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec, __LINE__);
				
		printf("%s", cBuffer);
		fprintf(fpLog, "%s", cBuffer);
		return false;
	}

	printf("%s", sMessages.c_str());
	fprintf(fpLog, "%s", sMessages.c_str());

	fclose(fpLog);
	fpLog=NULL;

	return true;
}
