
// smsdemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CsmsdemoApp:
// �йش����ʵ�֣������ smsdemo.cpp
//

class CsmsdemoApp : public CWinApp
{
public:
	CsmsdemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CsmsdemoApp theApp;