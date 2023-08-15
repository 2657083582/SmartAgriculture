
// smsdemoDlg.cpp : ʵ���ļ�
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


// CsmsdemoDlg �Ի���




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


// CsmsdemoDlg ��Ϣ�������

BOOL CsmsdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	editappid.SetWindowText(_T("������ID���ڡ����á�-���������á��л�ȡ"));
	editsecretKey.SetWindowText(_T("��������Կ���ڡ����á�-���������á��л�ȡ"));
	editmobile.SetWindowText(_T("13800138000"));
	editsmssign.SetWindowText(_T("����ǩ�� ������д����ǩ��������Ҫ��дǩ��id"));
	editsmstemplateid.SetWindowText(_T("ST_2019043000000001"));
	editsmscode.SetWindowText(_T("1958"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsmsdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsmsdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsmsdemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	//type ��֤������Ϊ0
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
