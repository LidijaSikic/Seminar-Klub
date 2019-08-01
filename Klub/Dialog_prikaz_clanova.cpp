// Dialog_prikaz_clanova.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Klub.h"
#include "DialogClanovi.h"
#include "Dialog_prikaz_clanova.h"
#include "DialogClanarine.h"
#include "Dialog_Natjecanja.h"


// Dialog_prikaz_clanova dialog

IMPLEMENT_DYNAMIC(Dialog_prikaz_clanova, CDialog)

Dialog_prikaz_clanova::Dialog_prikaz_clanova(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PRIKAZ_CLANOVA, pParent)
	, m_rb_stanje(0)
{	
	COLORREF clrBlack = GetSysColor(COLOR_WINDOWFRAME);
	m_penBlack.CreatePen(PS_SOLID, 0, clrBlack);
	m_pFntData = 0;
	m_pFntHeader1 = 0;
	m_pFntHeader2 = 0;
}


Dialog_prikaz_clanova::~Dialog_prikaz_clanova()
{
	delete m_pFntData;
	delete m_pFntHeader1;
	delete m_pFntHeader2;
}

void Dialog_prikaz_clanova::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLANOVI, m_Lista_Clanovi);
	DDX_Control(pDX, IDC_RD_AKTIVNI, m_rd_aktivni);
	DDX_Control(pDX, IDC_RD_NEAKTIVNI, m_rd_neaktivni);
	DDX_Control(pDX, IDC_RD_SVI, m_rd_svi);
	DDX_Radio(pDX, IDC_RD_AKTIVNI, m_rb_stanje);
	DDX_Control(pDX, IDC_BTN_PRIKAZI, m_btn_prikazi);
}


BEGIN_MESSAGE_MAP(Dialog_prikaz_clanova, CDialog)
	ON_BN_CLICKED(IDC_BTN_PRIKAZI, &Dialog_prikaz_clanova::OnBnClickedBtnPrikazi)	
	ON_BN_CLICKED(IDC_BTN_ISPISI_PRIKAZ, &Dialog_prikaz_clanova::OnBnClickedBtnIspisiPrikaz)
	ON_BN_CLICKED(IDC_BTN_ZATVORI_APLIKACIJU, &Dialog_prikaz_clanova::OnBnClickedBtnZatvoriAplikaciju)
	ON_BN_CLICKED(IDC_BTN_POCETNA4, &Dialog_prikaz_clanova::OnBnClickedBtnPocetna4)
	ON_BN_CLICKED(IDC_BTN_CLANARINE4, &Dialog_prikaz_clanova::OnBnClickedBtnClanarine4)
	ON_BN_CLICKED(IDC_BTN_NATJECANJA4, &Dialog_prikaz_clanova::OnBnClickedBtnNatjecanja4)
END_MESSAGE_MAP()


// Dialog_prikaz_clanova message handlers


BOOL Dialog_prikaz_clanova::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, FALSE);

	RClanovi = new ClanoviSet;
	popuni_naslove();
	m_Lista_Clanovi.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void Dialog_prikaz_clanova::popuni_naslove()
{
	CString s;
	s.LoadString(IDS_STRING_IME);
	m_Lista_Clanovi.InsertColumn(0, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_SPOL);
	m_Lista_Clanovi.InsertColumn(1, s, LVCFMT_LEFT, 50);
	s.LoadString(IDS_STRING_DATUM_RODENJA);
	m_Lista_Clanovi.InsertColumn(2, s, LVCFMT_CENTER, 120);
	s.LoadString(IDS_STRING_TELEFON);
	m_Lista_Clanovi.InsertColumn(3, s, LVCFMT_CENTER, 120);
	s.LoadString(IDS_STRING_ADRESA);
	m_Lista_Clanovi.InsertColumn(4, s, LVCFMT_LEFT, 180);
	s.LoadString(IDS_STRING_MAIL);
	m_Lista_Clanovi.InsertColumn(5, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_AKTIVNOST);
	m_Lista_Clanovi.InsertColumn(6, s, LVCFMT_LEFT, 80);
}

void Dialog_prikaz_clanova::popuni_listu()
{
	CString s, s1;
	int nIndex;
	if (m_rd_aktivni.GetCheck() == 1)
	{	

		RClanovi->m_strFilter = _T("Aktivnost = 'aktivan'");
	}
	else if (m_rd_neaktivni.GetCheck() == 1)
	{		
		RClanovi->m_strFilter = _T("Aktivnost = 'neaktivan' or Aktivnost = 'ispisan'");
	}

	m_Lista_Clanovi.DeleteAllItems();

	if (!RClanovi->IsOpen())
		RClanovi->Open();

	if (RClanovi->IsBOF() && RClanovi->IsEOF())
	{
		s.LoadString(IDS_STRING_BAZA_BEZ_PODATAKA);
		s1.LoadString(IDS_STRING_GRESKA);
		MessageBox(s, s1, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	else
	{
		while (!RClanovi->IsEOF())
		{
			nIndex = m_Lista_Clanovi.InsertItem(0, RClanovi->m_ImePrezime);

			CTime dat = RClanovi->m_DatumRodenja;
			CString strDat = dat.Format(_T("%d.%m.%Y"));

			m_Lista_Clanovi.SetItemText(nIndex, 1, RClanovi->m_Spol);
			m_Lista_Clanovi.SetItemText(nIndex, 2, strDat);
			m_Lista_Clanovi.SetItemText(nIndex, 3, RClanovi->m_Tel);
			m_Lista_Clanovi.SetItemText(nIndex, 4, RClanovi->m_Adresa);
			m_Lista_Clanovi.SetItemText(nIndex, 5, RClanovi->m_Mail);
			m_Lista_Clanovi.SetItemText(nIndex, 6, RClanovi->m_Aktivnost);
			RClanovi->MoveNext();
		}		
		RClanovi->m_strFilter = _T("");
		RClanovi->Close();
	}
}


void Dialog_prikaz_clanova::OnBnClickedBtnPrikazi()
{
	popuni_listu();
}


void Dialog_prikaz_clanova::OnBnClickedBtnIspisiPrikaz()
{
	CString s, s1;
	int z = m_Lista_Clanovi.GetItemCount();
	if (z == 0)
	{
		s.LoadString(IDS_STRING_NEMA_PODATAKA_ZA_ISPIS);
		s1.LoadString(IDS_STRING_GRESKA);
		MessageBox(s, s1, MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	else
	{
		Print();
	}
}


void Dialog_prikaz_clanova::OnBnClickedBtnZatvoriAplikaciju()
{
	PostQuitMessage(0);
}


void Dialog_prikaz_clanova::OnBnClickedBtnPocetna4()
{
	DialogClanovi *pPar = (DialogClanovi*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();
}


void Dialog_prikaz_clanova::OnBnClickedBtnClanarine4()
{
	DialogClanarine* DClanarine;
	DClanarine = new DialogClanarine;

	DialogClanovi *pPar = (DialogClanovi*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DClanarine->DoModal();
}


void Dialog_prikaz_clanova::OnBnClickedBtnNatjecanja4()
{
	Dialog_Natjecanja* DNatjecanja;
	DNatjecanja = new Dialog_Natjecanja;

	DialogClanovi *pPar = (DialogClanovi*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DNatjecanja->DoModal();
}

void Dialog_prikaz_clanova::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFntData == 0);
	ASSERT(m_pFntHeader1 == 0);
	ASSERT(m_pFntHeader2 == 0);	
	
	m_pFntData = new CFont;
	ASSERT(m_pFntData);
	int fntDataSize = 12;
	m_pFntData->CreateFont(-fntDataSize * pDC->GetDeviceCaps(LOGPIXELSY) / 72,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		_T("Arial"));
	
	m_pFntHeader1 = new CFont;
	ASSERT(m_pFntHeader1);
	int fntHeader1Size = 25;
	m_pFntHeader1->CreateFont(-fntHeader1Size * pDC->GetDeviceCaps(LOGPIXELSY) / 72,
		0,
		0,
		0,
		FW_SEMIBOLD,
		TRUE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		_T("Times Roman"));
	
	m_pFntHeader2 = new CFont;
	ASSERT(m_pFntHeader2);
	int fntHeader2Size = 12;
	m_pFntHeader2->CreateFont(-fntHeader2Size * pDC->GetDeviceCaps(LOGPIXELSY) / 72,
		0,
		0,
		0,
		FW_BOLD,
		TRUE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_ROMAN,
		_T("Arial"));

	int visina = pInfo->m_rectDraw.Height() - VisinaHeadera(pDC);
	int brIt = m_Lista_Clanovi.GetItemCount();
	int visinaReda = VisinaReda(pDC);
	
	int brStr;
	if (brIt % (visina/visinaReda) == 0)
		brStr = brIt / (visina / visinaReda);
	else
		brStr = brIt / (visina / visinaReda) + 1;
	
	pInfo->SetMaxPage(brStr);
}

void Dialog_prikaz_clanova::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{	
	CString s;
	CPoint pt(0, 0);
	int sirina_p = pDC->GetDeviceCaps(HORZRES);
	int visina_p = pDC->GetDeviceCaps(VERTRES);
	
	TEXTMETRIC tm;
	PrintHeader(pDC, pInfo);
	
	int visinaHeadera = VisinaHeadera(pDC);

	CFont* pOldFont = pDC->SelectObject(m_pFntData);

	pDC->GetTextMetrics(&tm);

	CRect visinaStranice = pInfo->m_rectDraw;

	int stranica = pInfo->m_nCurPage;
	int razmak=  tm.tmHeight + tm.tmExternalLeading;
	
	int visinaReda = VisinaReda(pDC);
	int visina = visinaStranice.Height()-visinaHeadera;

	int brRedova = visina / visinaReda; 

	CString rb;

	pt.y = visinaHeadera + razmak;
	pt.x = 2 * tm.tmHeight;

	for (int red = brRedova * (stranica - 1); red < m_Lista_Clanovi.GetItemCount() && red <= (brRedova - 1) * stranica; red++)
		{
			rb.Format(_T("%ld"), red+1);
			s.LoadString(IDS_STRING_TOCKA);
			pDC->TextOut(pt.x, pt.y += 2.2*razmak, rb + s);
			pDC->TextOut(pt.x * 2, pt.y, m_Lista_Clanovi.GetItemText(red, 0));
			pDC->TextOut(pt.x * 7, pt.y, m_Lista_Clanovi.GetItemText(red, 1));
			pDC->TextOut(pt.x * 9, pt.y, m_Lista_Clanovi.GetItemText(red, 2));
			pDC->TextOut(pt.x * 13, pt.y, m_Lista_Clanovi.GetItemText(red, 3));
			pDC->TextOut(pt.x * 18, pt.y, m_Lista_Clanovi.GetItemText(red, 6));
			pDC->TextOut(pt.x * 4, pt.y += 1.1*razmak, m_Lista_Clanovi.GetItemText(red, 4));
			pDC->TextOut(pt.x * 11, pt.y, m_Lista_Clanovi.GetItemText(red, 5));			
		}
}

void Dialog_prikaz_clanova::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	delete m_pFntData;
	delete m_pFntHeader1;
	delete m_pFntHeader2;
	// Since the destructor also deletes these fonts, we have
	// to set pointers to 0 to avoid dangling pointers and exceptions
	// generated by invoking delete on a non-valid pointer.
	m_pFntData = 0;
	m_pFntHeader1 = 0;
	m_pFntHeader2 = 0;
}

void Dialog_prikaz_clanova::Print()
{
	CDC pDC;
	CPrintDialog printDlg(FALSE);

	if (printDlg.DoModal() == IDCANCEL)     // Get printer settings from user
		return;

	pDC.Attach(printDlg.GetPrinterDC());     // Get and attach a printer DC

	pDC.m_bPrinting = TRUE;

	CString strTitle;                       // Get the application title
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	DOCINFO di;                             // Initialize print document details

	::ZeroMemory(&di, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = strTitle;

	BOOL bPrintingOK = pDC.StartDoc(&di);    // Begin a new print job
											// Get the printing extents and store in the m_rectDraw field of a 
											// CPrintInfo object
	CPrintInfo pInfo;

	pInfo.m_rectDraw.SetRect(0, 0,
		pDC.GetDeviceCaps(HORZRES),
		pDC.GetDeviceCaps(VERTRES));

	OnBeginPrinting(&pDC, &pInfo);            // Call your "Init printing" function
	
	for (UINT page = pInfo.GetMinPage();
		page <= pInfo.GetMaxPage() && bPrintingOK;
		page++)
	{
		pDC.StartPage();                     // begin new page
		
		pInfo.m_nCurPage = page;

		OnPrint(&pDC, &pInfo);                // Call your "Print page" function

		bPrintingOK = (pDC.EndPage() > 0);   // end page
	}
	OnEndPrinting(&pDC, &pInfo);              // Call your "Clean up" function

	if (bPrintingOK)
		pDC.EndDoc();                        // end a print job

	else
		pDC.AbortDoc();                      // abort job.

	pDC.DeleteDC();                          // delete the printer DC
}



void Dialog_prikaz_clanova::PrintHeader(CDC* pDC, CPrintInfo *pInfo)
{
	CString s, s1;
	ASSERT(pDC);
	TEXTMETRIC tm;
	CPoint pt(0, 0);
	
	CFont* pOldFont = pDC->SelectObject(m_pFntHeader1);
	
	ASSERT(pOldFont);
	pDC->GetTextMetrics(&tm);
	int cyText =  tm.tmHeight*0.5 + tm.tmExternalLeading;
	pt.y += cyText;
	pt.x = tm.tmHeight;
	s.LoadString(IDS_STRING_KARATE_KLUB);
	pDC->TextOut(pt.x*6, pt.y, s);
	
	CPen* pOldPen = pDC->SelectObject(&m_penBlack);
	ASSERT(pOldPen);
	pt.x = 0;
	pt.y += cyText*2;
	pDC->MoveTo(pt);
	pDC->LineTo(pDC->GetDeviceCaps(HORZRES), pt.y);
	
	VERIFY(pDC->SelectObject(m_pFntHeader2));
	
	pt.x = tm.tmHeight;
	pt.y += (cyText / 3);

	CString clanovi;
	if (m_rd_aktivni.GetState() == 1)
	{
		s.LoadString(IDS_STRING_AKTIVNI_CLANOVI);
		clanovi = s;
	}
	else if (m_rd_neaktivni.GetState() == 1)
	{
		s.LoadString(IDS_STRING_NEAKTIVNI_CLANOVI);
		clanovi = s;
	}
	else if (m_rd_svi.GetState() == 1)
	{
		s.LoadString(IDS_STRING_POPIS_CLANOVA);
		clanovi = s;
	}
	CTime dat = CTime::GetCurrentTime();
	CString datum = dat.Format(_T("%d.%m.%Y"));

	UINT st = pInfo->m_nCurPage;
	UINT uk = pInfo->GetMaxPage();
	CString str, ukStr;
	str.Format(_T("%u"), st);
	ukStr.Format(_T("%u"), uk);

	s.LoadString(IDS_STRING_NA_DAN);
	pDC->TextOut(pt.x *3, pt.y, clanovi + s  + datum); 
	s.LoadString(IDS_STRING_STR);
	s1.LoadString(IDS_STRING_KROZ);
	pDC->TextOut(pt.x *15, pt.y, s + str + s1 + ukStr);
	// Restore GDI objects.
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);	
}

int Dialog_prikaz_clanova::VisinaHeadera(CDC* pDC)
{
	int y=0;
	ASSERT(pDC);
	TEXTMETRIC tm;	

	CFont* pFont = pDC->SelectObject(m_pFntHeader1);

	ASSERT(pFont);
	pDC->GetTextMetrics(&tm);
	int cyText = tm.tmHeight*0.5 + tm.tmExternalLeading;
		
	y = cyText * 3;
	
	VERIFY(pDC->SelectObject(m_pFntHeader2));
	
	y += (cyText / 3);

	return y;
}

int Dialog_prikaz_clanova::VisinaReda(CDC* pDC)
{
	CFont* pFont = pDC->SelectObject(m_pFntData);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int razmak = tm.tmHeight + tm.tmExternalLeading;
	int visinaReda = 1.5*razmak + 2 * tm.tmHeight;
	return visinaReda;

}
