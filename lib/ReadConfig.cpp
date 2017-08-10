#include "ReadConfig.h"


CReadConfig::CReadConfig(const string& sLogFileName)//��ʼ��ʱ��������־�ļ�·��
{
	cls_LogMessage.SetLogFilePath(sLogFileName,"");
} 
CReadConfig::~CReadConfig() 
{}

bool CReadConfig::ReadLine(char* cLine, FILE* cStream) 
{ 
	int nFlag=1;   
	char cBuf[CFG_LINE];   
	int i, k=0;   

	if(fgets(cBuf,CFG_LINE,cStream)!=NULL) 
	{
		//cout<<"p1:"<<strlen(cBuf)<<" "<<cBuf<<endl;
		
		//Delete the last '\r' or '\n' or ' ' or '\t' character   
		for(i=strlen(cBuf)-1; i>=0; i--) 
		{   
			if(cBuf[i]=='\r' || cBuf[i]=='\n' || cBuf[i]==' ' || cBuf[i]=='\t') cBuf[i]='\0';
			else break;
		}   
		
		//cout<<"p2:"<<strlen(cBuf)<<" "<<cBuf<<endl;
		
		//Delete the front '\r' or '\n' or ' ' or '\t' character
		for(i=0; i<=(int)strlen(cBuf); i++) 
		{   
			if(nFlag && (cBuf[i]=='\r' || cBuf[i]=='\n' || cBuf[i]==' ' || cBuf[i]=='\t')) continue;   
			else 
			{   
				nFlag=0;   
				cLine[k++]=cBuf[i];   
			} 
		}

		return true; 
	} 

	return false;
}

int CReadConfig::IsRemark(const char* cLine) 
{ 
	unsigned int i;   

	for(i=0;i<strlen(cLine);i++) 
	{   
		if(isgraph(cLine[i]))//�ж��ַ��Ƿ�Ϊ���ո���Ŀɴ�ӡ�ַ�,Ϊ�淵�ط���ֵ
		{   
			if(cLine[i]=='#') return 1;   
			else return 0;   
		}   
	}

	return -1; 
}

bool CReadConfig::GetSection(const char* cLine, char* cSection)   
{   
	//cout<<"GetSection"<<endl;
	unsigned int nStart, nEnd;   

	//�ҵ�section��־��ʼ��"[",��ֵnStart
	for(nStart=0; nStart<strlen(cLine); nStart++) 
	{   
		if(cLine[nStart]!='[') return false; 
		else break;   
	}   

	//�ҵ�section��־��ֹ��"]",��ֵnEnd
	for(nEnd=strlen(cLine)-1; nEnd>1; nEnd--) 
	{   
		if(cLine[nEnd]!=']') return false;   
		else break;   
	}   

	//Section���ݳ�����֤
	if(nEnd-nStart<2) return false;   

	//Copy Section
	memset(cSection, 0, nEnd-nStart);
	strncpy(cSection, cLine+nStart+1, nEnd-nStart-1);   
	
	//cout<<"ok1"<<endl;
	return true;   
}  

bool CReadConfig::GetKey(const char* cLine, char* cKeyName, char* cKeyValue)   
{   
	//cout<<"GetKey"<<endl;
	unsigned int i, nStart;   
	
	//Find key name  
	for(nStart=0; nStart<strlen(cLine); nStart++) 
	{   
		//Find '='  
		if( cLine[nStart] == '=') break;   
	}
	//not Find '='
	if(nStart>=strlen(cLine)) return false;

	//Copy KeyName
	memcpy(cKeyName, cLine, nStart);   
	cKeyName[nStart] = '\0';

	//Delete the last '\t' or ' ' in KeyName
	for(i=strlen(cKeyName)-1; i>=0; i--) 
	{   
		if(cKeyName[i]==' ' || cKeyName[i]=='\t') cKeyName[i]='\0';
		else break;
	}

	//Find key value   
	for(nStart=nStart+1; nStart<strlen(cLine); nStart++) 
	{   
		if(cLine[nStart]!=' ' && cLine[nStart]!='\t') break;     
	}   

	//Copy KeyValue
	strcpy(cKeyValue,cLine+nStart);   

	//Delete the last '\t' or ' ' in KeyValue
	for(i=strlen(cKeyValue)-1; i>=0; i--) 
	{   
		if(cKeyValue[i]==' ' || cKeyValue[i]=='\t') cKeyValue[i]='\0';
		else break;   
	}   

	//cout<<"ok2"<<endl;
	
	return true;     
}  

int CReadConfig::GetConfig(const char* cSection, const char* cKeyName, char* cKeyValue, unsigned int nLen, const char* cFileName)   
{   
	int nStep=0;   
	FILE* fpStream;   
	char cSec[CFG_LINE]={0};   
	char cKen[CFG_LINE]={0};   
	char cKev[CFG_LINE]={0};   
	char cLine[CFG_LINE]={0};
	
	//�������ļ�
	if((fpStream=fopen(cFileName,"r"))==NULL)   
	{
		return CFG_NOFILE;
	}

	while(!feof(fpStream)) 
	{   
		//cout<<nStep<<endl;
		if(ReadLine(cLine,fpStream)==false)
		{   
			fclose(fpStream);   
			return CFG_NOFIND;   
		}
		if (strlen(cLine)<=0) continue;
		//cout<<"cLine: "<<cLine<<endl;
		
		if(nStep==1 && cLine[0]=='[')//���֡�[��,��ʾ������һ��section
		{
			//cout<<"h1"<<endl;
			break;   
		}
		else
		{
			//cout<<"h2"<<endl;
			if(nStep==0)//find section
			{      
				//cout<<"h3"<<endl;
				if(GetSection(cLine,cSec)==true)//��ȡ��section������
				{  
					//cout<<cSec<<endl;//test
					//�жϵ�ǰ��section�Ƿ���Ҫ��ȡ��section
					if(strcmp(cSec,cSection)==0) nStep=1;   
				}
				else
				{
					//cout<<"false"<<endl;
				}
			}     
			else//�Ѿ��ҵ�Ҫ��ȡ��section
			{   
				//cout<<"h4"<<endl;
				//find keyname, keyvalue 
				if(GetKey(cLine,cKen,cKev)==true) 
				{   
					//cout<<"h5"<<endl;
					//�жϵ�ǰ������key�Ƿ���Ҫ��ȡ��key
					if(strcmp(cKen,cKeyName)==0) 
					{   
						//cout<<"h6"<<endl;
						memset(cKeyValue, 0 , nLen);
						strncpy(cKeyValue, cKev, nLen-1);//Copy KeyValue  
						fclose(fpStream);   
						return CFG_NOERROR;   
					}     
				}   
			}   

		} //end isremark()->#   

	}//end while   

	fclose(fpStream);
	return CFG_NOFIND;
}

bool CReadConfig::GetConfigStr(const char* cSection, const char* cKeyName, string& sKeyValue, unsigned int nLen, const char* cFileName)
{
	int nRt;
	bool bRt=true;
	char temp[1024]={0};
	char cBuffer[1024]={0};
	
	//cout<<"here"<<endl;
	
	nRt=GetConfig(cSection,cKeyName,temp,nLen,cFileName);
	//cout<<"Here1"<<endl;
	//cout<<nRt<<endl;
	if (nRt==CFG_NOERROR)
	{
		bRt=true;
		sKeyValue=temp;
		sprintf(cBuffer,"Read [%s] %s: %s",cSection,cKeyName,sKeyValue.c_str());
		cls_LogMessage.LogMessages(cBuffer,__LINE__,bRt);	
	}
	else if (nRt==CFG_NOFILE)
	{
		bRt=false;
		sprintf(cBuffer,"Read [%s] %s: :not find or open configuration file",cSection,cKeyName);
		cls_LogMessage.LogMessages(cBuffer,__LINE__,bRt);		
	}
	else if (nRt==CFG_NOFIND)
	{
		bRt=false;
		sprintf(cBuffer,"Read [%s] %s: not find section or key name in configuration file",cSection,cKeyName);
		cls_LogMessage.LogMessages(cBuffer,__LINE__,bRt);		
	}

	return bRt;
}

