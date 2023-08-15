 //   HttpClient.cpp:   implementation   of   the   CHttpClient   class.  
  //  
  //////////////////////////////////////////////////////////////////////  
   
  #include   "stdafx.h"  
  //#include   "emailsenderv2.h"  
  #include   "HttpClient.h"  
   
  #ifdef   _DEBUG  
  #undef   THIS_FILE  
  static   char   THIS_FILE[]=__FILE__;  
  #define   new   DEBUG_NEW  
  #endif  
   
  //////////////////////////////////////////////////////////////////////  
  //   Construction/Destruction  
  //////////////////////////////////////////////////////////////////////  
   
  CHttpClient::CHttpClient()  
  {  
   
  }  
   
  CHttpClient::~CHttpClient()  
  {  
   
  }  
   
  CString   CHttpClient::doGet(CString   href)  
  {  
  CString   httpsource="";  
  CInternetSession   session1(NULL,0);  
  CHttpFile*   pHTTPFile=NULL;  
  try{  
  pHTTPFile=(CHttpFile*)session1.OpenURL(href);  
  //session1.  
  }catch(CInternetException){  
  pHTTPFile=NULL;  
  }  
  if(pHTTPFile)  
  {  
  CString   text;  
  for(int   i=0;pHTTPFile->ReadString(text);i++)  
  {  
      httpsource = httpsource + text;  
  }  
  pHTTPFile->Close();  
  delete   pHTTPFile; 
  session1.Close();
  int wideLen = MultiByteToWideChar(CP_UTF8, 0, httpsource, -1, NULL, 0); //�õ� ���ֽ��ַ��� ת���ַ��ַ��� ���賤��
			wchar_t* pWideStr = new wchar_t[wideLen];	//������ַ����ڴ��С
			if (!pWideStr)								//ʧ���˳�
			{
				//MessageBox(_T("���ֽ�ת���ַ�ʱ�������ڴ�ʧ��"));
				return _T("���ֽ�ת���ַ�ʱ�������ڴ�ʧ��");
			}
			MultiByteToWideChar(CP_UTF8, 0, httpsource, -1, pWideStr, wideLen);	//��ʼת��
			//::SetDlgItemTextW(this->m_hWnd, IDC_EDIT_CONTENT, pWideStr);
			httpsource = CString(pWideStr);
			delete[] pWideStr;	//�����ͷ��ڴ�

  }else  
  {  
   
  }  
  return   httpsource;  
  }  
   
  CString   CHttpClient::doPost(CString   href)  
  {  
  CString   httpsource="";  
  CInternetSession   session1;  
  CHttpConnection*   conn1=NULL;  
          CHttpFile*   pFile   =   NULL;      
  CString   strServerName;  
          CString   strObject;  
          INTERNET_PORT   nPort;  
          DWORD   dwServiceType;  
  AfxParseURL((LPCTSTR)href,dwServiceType,   strServerName,   strObject,   nPort);  
          DWORD   retcode;              
          char*   outBuff   =   CONTENT.GetBuffer(1000);  
          try      
          {                  
                  conn1   =   session1.GetHttpConnection(strServerName,nPort);          
                  pFile   =   conn1->OpenRequest(0,strObject,NULL,1,NULL,"HTTP/1.1",INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT);         
                  pFile   ->   AddRequestHeaders("Content-Type:application/x-www-form-unlencoded;charset=UTF-8");          
                  pFile   ->   AddRequestHeaders("Accept:   */*");                          
                  pFile   ->   SendRequest(NULL,0,outBuff,strlen(outBuff)+1);                  
                  pFile   ->   QueryInfoStatusCode(retcode);                  
          }                            
          catch   (CInternetException   *   e){};                  
  if(pFile)  
  {  
  CString   text;  
  for(int   i=0;pFile->ReadString(text);i++)  
  {  
  httpsource=httpsource+text+"/r/n";  
  }  
  pFile->Close();  
  }else  
  {  
   
  }  
  return   httpsource;  
          delete   pFile;          
          delete   conn1;          
          session1.Close();  
  }  
   
  void   CHttpClient::addParam(CString   name,   CString   value)  
  {  
  names.AddTail((LPCTSTR)name);  
  values.AddTail((LPCTSTR)value);  
  CString   eq="=";  
  CString   an="&";  
  CONTENT=CONTENT+name+eq+value+an;  
  CL=CONTENT.GetLength();  
  } 

  CString CHttpClient::ExchangeMsg(CString msg)
{
   CString retstr = "";
   CString tempstr = "";
  
   char aa[500]="";
   int len=msg.GetLength();
   BSTR x=msg. AllocSysString();
   long num=*((long*)x-1);  
   for(int i=0;i<num;i+=1)
   {
      if(i>=len)
        break;
      char cc[10]="";
	  if((*x>>8)==0)
	  {
		  tempstr.Format("00%x",*x);
		  retstr = retstr + "\\u" + tempstr;
	      msg=msg.Right (msg.GetLength ()-1);   
	  }
	  else
	  {
	   tempstr.Format("%x", *x);
	   retstr = retstr + "\\u" + tempstr;
	   msg=msg.Right (msg.GetLength ()-2);
	   i=i+1;
	  }
	  tempstr = "";
     SysFreeString(x); 
     x=msg. AllocSysString();
   }
  SysFreeString(x);
 
   retstr.MakeLower ();
   int msglth=retstr.GetLength(); 
   return retstr;
}

CString CHttpClient::MD5(CString &inData)
{
	CString outData;
	//CSPָ�� 
	HCRYPTPROV hProv = 0;
	//MD5 HASH 
	HCRYPTHASH hHash = 0;

	BYTE bHash[32]; 
	DWORD dwHashLen = 16; // The MD5 algorithm always returns 16 bytes. 
	DWORD cbContent = inData.GetLength(); 
	BYTE* pbContent = (BYTE*)inData.GetBuffer(cbContent);

	if(!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return "";
	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		return "";
	if(!CryptHashData(hHash, pbContent, cbContent, 0))
		return "";
	if(!CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0))
		return "";

	CString tmp; 
	for (unsigned int i = 0; i < dwHashLen; i += 4) 
	{ 
		tmp.Format("%02x%02x%02x%02x", bHash[i], bHash[i+1],bHash[i+2],bHash[i+3]); 
		outData += tmp; 
	}

	if(hHash)
		CryptDestroyHash(hHash); 
	if(hProv)
		CryptReleaseContext(hProv, 0);

	outData.MakeUpper();
	return outData;
}

CString SHA1(CString &inData)
{
	CString outData;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	BYTE bHash[32]; 
	DWORD dwHashLen = 20; // The SHA1 algorithm always returns 20 bytes. 
	DWORD cbContent = inData.GetLength(); 
	BYTE* pbContent = (BYTE*)inData.GetBuffer(cbContent);

	if(!CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		return "";
	if(!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
		return "";
	if(!CryptHashData(hHash, pbContent, cbContent, 0))
		return "";
	if(!CryptGetHashParam(hHash, HP_HASHVAL, bHash, &dwHashLen, 0))
		return "";

	CString tmp; 
	for (unsigned int i = 0; i < dwHashLen; i += 4) 
	{ 
		tmp.Format("%02x%02x%02x%02x", bHash[i], bHash[i+1],bHash[i+2],bHash[i+3]); 
		outData += tmp; 
	}

	if(hHash)
		CryptDestroyHash(hHash); 
	if(hProv)
		CryptReleaseContext(hProv, 0);

	return outData;
}

inline BYTE toHex(const BYTE &x)
{
       return x > 9 ? x + 55: x + 48;
}

CString CHttpClient::URLEncode(CString strToEncode)
{
	CString strNoEncode = _T("");
	// Ĭ�ϲ�ת�����ַ�
	if (strNoEncode.IsEmpty())
	{
		strNoEncode = _T("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:/.?_-=&;");
	}

	CString strEncoded;  // ת�����
	for (int i = 0; i < strToEncode.GetLength(); i++)
	{
		CString strCur = strToEncode.Mid(i, 1);  // ��ǰҪ��ת�����ַ�

		// �ж��Ƿ���Ҫת��
		BOOL bNoEncode = FALSE;
		for (int j = 0; j < strNoEncode.GetLength(); j++)
		{
			if (strCur == strNoEncode.Mid(j, 1))
			{
				bNoEncode = TRUE;
				break;
			}
		}

		// ����Ҫת�����ַ�ֱ��ƴ�ӵ�Ҫ������ַ�����
		if (bNoEncode)
		{
			strEncoded += strCur;
			continue;
		}

		// ��Ҫת�����ַ���ת��
		USES_CONVERSION;
		LPCWSTR unicode = T2CW(strCur);
		int len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, 0, 0, 0, 0);
		char *utf8 = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, unicode, len, utf8, len, 0, 0);

		// ת�����ַ���ʽ��ƴ�ӵ�������ַ�����
		for (int k = 0; k < len - 1; k++) // ���������һ���ַ�����ֹ����
		{
			strEncoded.AppendFormat(_T("%%%02X"), utf8[k] & 0xFF);
		}
	}
	return strEncoded;
}