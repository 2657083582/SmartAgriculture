
// smsdemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "smsdemo.h"
#include "smsdemoDlg.h"
#include "afxdialogex.h"
#include "string.h"
#include <wincrypt.h>
#include <Wininet.h>
#include "HttpClient.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsmsdemoDlg 对话框




CsmsdemoDlg::CsmsdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsmsdemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsmsdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editappid);
	DDX_Control(pDX, IDC_EDIT2, editsecretKey);
	DDX_Control(pDX, IDC_EDIT3, editmobile);
	DDX_Control(pDX, IDC_EDIT4, editsmssign);
	DDX_Control(pDX, IDC_EDIT5, editsmstemplateid);
	DDX_Control(pDX, IDC_EDIT6, editsmscode);
}

BEGIN_MESSAGE_MAP(CsmsdemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CsmsdemoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CsmsdemoDlg 消息处理程序

BOOL CsmsdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	editappid.SetWindowText(_T("开发者ID，在【设置】-【开发设置】中获取"));
	editsecretKey.SetWindowText(_T("开发者密钥，在【设置】-【开发设置】中获取"));
	editmobile.SetWindowText(_T("13800138000"));
	editsmssign.SetWindowText(_T("短信签名 这里填写短信签名，不需要填写签名id"));
	editsmstemplateid.SetWindowText(_T("ST_2019043000000001"));
	editsmscode.SetWindowText(_T("1958"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsmsdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsmsdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsmsdemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME tmSys;
    GetLocalTime(&tmSys);
    CTime tm3(tmSys);
    __int64 tmDst = __int64(tm3.GetTime())*1000 + tmSys.wMilliseconds;
    CString timestamp;
    _i64toa(tmDst,timestamp.GetBuffer(100),10);	
    timestamp.ReleaseBuffer();
	//AfxMessageBox(timestamp);

	CString mobile = "";	
	CString app_id = "";
	CString secretKey = "";
	CString smssign = "";
	CString smsCode = "";
	CString smstemplateid = "";
	editappid.GetWindowText(app_id);
	editsecretKey.GetWindowText(secretKey);
	editmobile.GetWindowText(mobile);
	editsmssign.GetWindowText(smssign);
	editsmstemplateid.GetWindowText(smstemplateid);
	editsmscode.GetWindowText(smsCode);	

	CString method = "sms.message.send";
    CString version = "1.0";
    CString sign_type = "md5";

	CHttpClient *client = new CHttpClient();

	smssign = client->ExchangeMsg(smssign);

	//type 验证码类型为0
	CString biz_content = "{\"mobile\":[\"" + mobile + "\"],\"sign\":\"" + smssign + "\",\"send_time\":\"\",\"type\":0,\"template_id\":\"" + smstemplateid + "\",\"params\":{\"code\":" + smsCode + "}}";
	
	CString txt = "app_id=" + app_id + "&biz_content=" + biz_content + "&method=" + method + "&sign_type=" + sign_type + "&timestamp=" + timestamp + "&version=" + version + "&key=" + secretKey;

	CString sign = client->MD5(txt);
	CString sPageUrl(_T("?app_id=" + client->URLEncode(app_id) + "&method=" + method + "&version=" + version + "&timestamp=" + timestamp + "&sign_type=" + sign_type + "&sign=" + sign + "&biz_content=" + client->URLEncode(biz_content)));
    CString sHomeUrl(_T("http://api.shansuma.com/gateway.do" + sPageUrl));    
    //CString sResult(_T(""));
	//HttpRequestGet(sHomeUrl, sPageUrl, sResult);

	//AfxMessageBox(sHomeUrl);

	
	CString sResult = client->doGet(sHomeUrl);
	delete client;

	AfxMessageBox(sResult);

	//AfxMessageBox(URLEncode(app_id));

	
   // CString sPageUrl(_T("Temporary/index.ashx?username=mrxyz098&password=123"));
    
 
    
  
	//CDialogEx::OnOK();
}
