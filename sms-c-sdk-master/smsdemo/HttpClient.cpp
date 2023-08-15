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
  int wideLen = MultiByteToWideChar(CP_UTF8, 0, httpsource, -1, NULL, 0); //得到 多字节字符串 转宽字符字符串 所需长度
			wchar_t* pWideStr = new wchar_t[wideLen];	//分配宽字符串内存大小
			if (!pWideStr)								//失败退出
			{
				//MessageBox(_T("多字节转宽字符时，分配内存失败"));
				return _T("多字节转宽字符时，分配内存失败");
			}
			MultiByteToWideChar(CP_UTF8, 0, httpsource, -1, pWideStr, wideLen);	//开始转换
			//::SetDlgItemTextW(this->m_hWnd, IDC_EDIT_CONTENT, pWideStr);
			httpsource = CString(pWideStr);
			delete[] pWideStr;	//完事释放内存

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
	//CSP指针 
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
	// 默认不转换的字符
	if (strNoEncode.IsEmpty())
	{
		strNoEncode = _T("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ:/.?_-=&;");
	}

	CString strEncoded;  // 转换结果
	for (int i = 0; i < strToEncode.GetLength(); i++)
	{
		CString strCur = strToEncode.Mid(i, 1);  // 当前要被转换的字符

		// 判断是否需要转换
		BOOL bNoEncode = FALSE;
		for (int j = 0; j < strNoEncode.GetLength(); j++)
		{
			if (strCur == strNoEncode.Mid(j, 1))
			{
				bNoEncode = TRUE;
				break;
			}
		}

		// 不需要转换的字符直接拼接到要输出的字符串中
		if (bNoEncode)
		{
			strEncoded += strCur;
			continue;
		}

		// 需要转换的字符先转码
		USES_CONVERSION;
		LPCWSTR unicode = T2CW(strCur);
		int len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, 0, 0, 0, 0);
		char *utf8 = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, unicode, len, utf8, len, 0, 0);

		// 转码后的字符格式化拼接到输出的字符串中
		for (int k = 0; k < len - 1; k++) // 不处理最后一个字符串终止符号
		{
			strEncoded.AppendFormat(_T("%%%02X"), utf8[k] & 0xFF);
		}
	}
	return strEncoded;
}