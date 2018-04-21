
// FDTDDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "FDTD.h"
#include "FDTDDlg.h"
#include "afxdialogex.h"

#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CFDTDDlg



CFDTDDlg::CFDTDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FDTD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFDTDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFDTDDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CFDTDDlg::OnBnClickedOk)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// Обработчики сообщений CFDTDDlg

BOOL CFDTDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	CDC* dc = GetDlgItem(IDC_GRAPHICS)->GetDC();
	GetDlgItem(IDC_GRAPHICS)->GetClientRect(&rect);
	InitiateOPGL(rect, dc);

	angleX = 0;
	angleY = 0;
	drawer.angleX = &angleX;
	drawer.angleY = &angleY;
	oldPoint.x = 0;
	oldPoint.y = 0;
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CFDTDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{

		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CFDTDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFDTDDlg::InitiateOPGL(CRect & rt, CDC* pdc)
{
	rect = rt;
	pDC = pdc;
	HGLRC hrc; //rendering context for OpGL(просчитывающий контекст для OpGl)
	if (!bSetupPixelFormat())
	{
		return;
	}

	hrc = wglCreateContext(pDC->GetSafeHdc()); //инициализирую его необходимым cdc
	ASSERT(hrc != NULL); //если контекст проинициализирован, то продолжаем программу, иначе немедленное завершение

	wglMakeCurrent(pDC->GetSafeHdc(), hrc); //выставляю созданный контекст рисования

	glViewport(0, 0, rect.right, rect.bottom);

	glCullFace(GL_FRONT);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0, 0.0, 0.0, 1.0);


}

BOOL CFDTDDlg::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |          // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};
	int pixelformat;
	if ((pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd)) == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}


void CFDTDDlg::OnBnClickedOk()
{
	drawer.Draw();
}


void CFDTDDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int dX = (point.x - oldPoint.x);
	int dY = (point.y - oldPoint.y);
	bool IsInRect = (point.x > rect.left) && (point.x < rect.right) && (point.y > rect.top) && (point.y < rect.bottom);
	
	oldPoint.x = point.x;
	oldPoint.y = point.y;
	if (IsInRect)
	{
		if (nFlags & MK_LBUTTON)
		{			
				angleX += 0.5*dY;
			angleY += 0.5*dX;
		}
	}
	drawer.Draw();
	CDialog::OnMouseMove(nFlags, point);
}
