// Dialog_prikaz_clanarina.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Klub.h"
#include "DialogClanovi.h"
#include "DialogClanarine.h"
#include "Dialog_prikaz_clanarina.h"
#include "Dialog_Natjecanja.h"


// Dialog_prikaz_clanarina dialog

IMPLEMENT_DYNAMIC(Dialog_prikaz_clanarina, CDialog)

Dialog_prikaz_clanarina::Dialog_prikaz_clanarina(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_PRIKAZ_CLANARINA, pParent)
	, m_rb_stanje(0)
	, m_rb_stanje_datumi(0)
{	
	COLORREF clrBlack = GetSysColor(COLOR_WINDOWFRAME);
	m_penBlack.CreatePen(PS_SOLID, 0, clrBlack);
	m_pFntData = 0;
	m_pFntHeader1 = 0;
	m_pFntHeader2 = 0;
}

Dialog_prikaz_clanarina::~Dialog_prikaz_clanarina()
{
	delete m_pFntData;
	delete m_pFntHeader1;
	delete m_pFntHeader2;
}

void Dialog_prikaz_clanarina::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RD_CLAN, m_rd_clan);
	DDX_Control(pDX, IDC_RD_KLUB, m_rd_klub);
	DDX_Radio(pDX, IDC_RD_CLAN, m_rb_stanje);
	DDX_Control(pDX, IDC_DATE_OD, m_date_od);
	DDX_Control(pDX, IDC_DATE_DO, m_date_do);
	DDX_Control(pDX, IDC_BTN_PRIKAZI, m_btn_prikazi);
	DDX_Control(pDX, IDC_COMBO, m_combo);
	DDX_Control(pDX, IDC_RD_SVE_UPLATE, m_rd_sve_uplate);
	DDX_Control(pDX, IDC_RD_RAZDOBLJE, m_rd_razdoblje);
	DDX_Radio(pDX, IDC_RD_SVE_UPLATE, m_rb_stanje_datumi);
	DDX_Control(pDX, IDC_EDIT_POM, m_edit_pom);
	DDX_Control(pDX, IDC_LISTA, m_Lista);
}


BEGIN_MESSAGE_MAP(Dialog_prikaz_clanarina, CDialog)
	ON_BN_CLICKED(IDC_BTN_PRIKAZI, &Dialog_prikaz_clanarina::OnBnClickedBtnPrikazi)
	ON_BN_CLICKED(IDC_BTN_ISPISI_PRIKAZ, &Dialog_prikaz_clanarina::OnBnClickedBtnIspisiPrikaz)
	ON_BN_CLICKED(IDC_BTN_ZATVORI_APLIKACIJU1, &Dialog_prikaz_clanarina::OnBnClickedBtnZatvoriAplikaciju1)
	ON_BN_CLICKED(IDC_BTN_POCETNA, &Dialog_prikaz_clanarina::OnBnClickedBtnPocetna)
	ON_BN_CLICKED(IDC_BTN_CLANOVI1, &Dialog_prikaz_clanarina::OnBnClickedBtnClanovi1)
	ON_BN_CLICKED(IDC_BTN_NATJECANJA1, &Dialog_prikaz_clanarina::OnBnClickedBtnNatjecanja1)
END_MESSAGE_MAP()


// Dialog_prikaz_clanarina message handlers


BOOL Dialog_prikaz_clanarina::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, FALSE);

	m_date_od.SetFormat(_T("dd.MM.yyyy"));
	m_date_do.SetFormat(_T("dd.MM.yyyy"));

	RClanovi = new ClanoviSet;
	RClanarine = new ClanarineSet;

	if (!RClanovi->IsOpen())
		RClanovi->Open();

	if (RClanovi->IsBOF() && RClanovi->IsEOF())
	{
		MessageBox(_T("U bazi nema podataka"), _T("Greška"), MB_ICONEXCLAMATION | MB_OK);		
	}
	else
	{
		while (!RClanovi->IsEOF()) {
			m_combo.AddString(RClanovi->m_ImePrezime);
			RClanovi->MoveNext();
		}
	}
	RClanovi->Close();
	m_combo.SetCurSel(0);

	popuni_naslove();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE	
}


void Dialog_prikaz_clanarina::popuni_naslove()
{
	m_Lista.InsertColumn(0, _T("Ime"), LVCFMT_LEFT, 180);         
	m_Lista.InsertColumn(1, _T("Datum uplate"), LVCFMT_CENTER, 120);
	m_Lista.InsertColumn(2, _T("Iznos"), LVCFMT_CENTER, 80);
	m_Lista.InsertColumn(3, _T("Svrha"), LVCFMT_LEFT, 200);
}


void Dialog_prikaz_clanarina::OnBnClickedBtnPrikazi()
{
	m_Lista.DeleteAllItems();
	while (m_Lista.DeleteColumn(0));
	
	int nIndex;
	CString ime;

	if (m_rd_clan.GetCheck() == 1)
	{			
		int nSel=m_combo.GetCurSel();
		if( nSel != LB_ERR)
		{
			m_combo.GetLBText(nSel, ime);
		}

		m_Lista.InsertColumn(0, ime, LVCFMT_LEFT, 180);
		m_Lista.InsertColumn(1, _T("Datum uplate"), LVCFMT_CENTER, 120);
		m_Lista.InsertColumn(2, _T("Iznos"), LVCFMT_CENTER, 80);
		m_Lista.InsertColumn(3, _T("Svrha"), LVCFMT_LEFT, 200);

		CString strIDclana;
		strIDclana.Format(_T("%ld"), NadiIDclana());
		if (m_rd_sve_uplate.GetCheck() == 1)
		{
			RClanarine->m_strFilter = _T("IDclana = ") + strIDclana + _T("");
			RClanarine->m_strSort = _T("DatumUplate DESC");
		}
		else if (m_rd_razdoblje.GetCheck() == 1)
		{
			CTime DateOd, DateDo;
			m_date_od.GetTime(DateOd);
			CString strDateOd = DateOd.Format(_T("%m/%d/%y"));

			m_date_do.GetTime(DateDo);
			CString strDateDo = DateDo.Format(_T("%m/%d/%y"));

			RClanarine->m_strFilter = _T("IDclana = ") + strIDclana + _T(" ") + _T(" and ") + _T("(DatumUplate between #") + strDateOd + _T("#") + _T(" and #") + strDateDo + _T("#)");
			RClanarine->m_strSort = _T("DatumUplate DESC");
		}
		if (!RClanarine->IsOpen())
			RClanarine->Open();

		if (RClanarine->IsBOF() && RClanarine->IsEOF())
		{
			MessageBox(_T("Èlan ") + ime + _T(" nema zabilježenih uplata"), _T("Obavijest"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			while (!RClanarine->IsEOF())
			{
				if (m_rd_klub.GetCheck() == 1)
				{
					NadiIme(RClanarine->m_IDclana);
					m_edit_pom.GetWindowTextW(ime);
				}
				CTime dat = RClanarine->m_DatumUplate;
				CString strDat = dat.Format(_T("%d.%m.%Y"));

				nIndex = m_Lista.InsertItem(0, _T(""));
				m_Lista.SetItemText(nIndex, 1, strDat);
				m_Lista.SetItemText(nIndex, 2, RClanarine->m_IznosUplate);
				m_Lista.SetItemText(nIndex, 3, RClanarine->m_SvrhaUplate);

				RClanarine->MoveNext();
			}
		}
	}
	else if (m_rd_klub.GetCheck() == 1)
	{
		m_Lista.InsertColumn(0, _T("Ime"), LVCFMT_LEFT, 180);
		m_Lista.InsertColumn(1, _T("Datum uplate"), LVCFMT_CENTER, 120);
		m_Lista.InsertColumn(2, _T("Iznos"), LVCFMT_CENTER, 80);
		m_Lista.InsertColumn(3, _T("Svrha"), LVCFMT_LEFT, 200);

		if (m_rd_sve_uplate.GetCheck() == 1)
		{
			RClanarine->m_strSort = _T("DatumUplate DESC");
		}
		else if (m_rd_razdoblje.GetCheck() == 1)
		{
			CTime DateOd, DateDo;
			m_date_od.GetTime(DateOd);
			CString strDateOd = DateOd.Format(_T("%m/%d/%y"));

			m_date_do.GetTime(DateDo);
			CString strDateDo = DateDo.Format(_T("%m/%d/%y"));

			RClanarine->m_strFilter = _T("(DatumUplate between #") + strDateOd + _T("#") + _T(" and #") + strDateDo + _T("#)");
			RClanarine->m_strSort = _T("DatumUplate DESC");
		}
		if (!RClanarine->IsOpen())
			RClanarine->Open();

		if (RClanarine->IsBOF() && RClanarine->IsEOF())
		{			
				MessageBox(_T("U izabranom razdoblju nema zabilježenih uplata"), _T("Obavijest"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			while (!RClanarine->IsEOF())
			{
				if (m_rd_klub.GetCheck() == 1)
				{
					NadiIme(RClanarine->m_IDclana);
					m_edit_pom.GetWindowTextW(ime);
				}
				CTime dat = RClanarine->m_DatumUplate;
				CString strDat = dat.Format(_T("%d.%m.%Y"));

				nIndex = m_Lista.InsertItem(0, ime);
				m_Lista.SetItemText(nIndex, 1, strDat);
				m_Lista.SetItemText(nIndex, 2, RClanarine->m_IznosUplate);
				m_Lista.SetItemText(nIndex, 3, RClanarine->m_SvrhaUplate);

				RClanarine->MoveNext();
			}
		}
	}
		
		RClanarine->m_strFilter = _T("");
		RClanarine->Close();	
}


long Dialog_prikaz_clanarina::NadiIDclana()
{
	CString ime;
	long IDclana;
	m_combo.GetWindowTextW(ime);
	RClanovi->m_strFilter = _T("ImePrezime = '") + ime + _T("'");
	RClanovi->Open();
	IDclana = RClanovi->m_IDclana;
	RClanovi->Close();

	return IDclana;
}

void Dialog_prikaz_clanarina::NadiIme(long IDclana)
{
	CString strIDclana;
	strIDclana.Format(_T("%ld"), IDclana);
	RClanovi->m_strFilter = (_T("[IDclana] = ") + strIDclana + _T(""));

	if(!RClanovi->IsOpen())
	RClanovi->Open();	
	CString ime = RClanovi->m_ImePrezime;
	RClanovi->Close();
	
	m_edit_pom.SetWindowTextW(ime);
}


void Dialog_prikaz_clanarina::OnBnClickedBtnIspisiPrikaz()
{
	int z = m_Lista.GetItemCount();
	if (z == 0)
	{
		MessageBox(_T("Nema podataka za ispis"), _T("Greška"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	else
	{
		Print();
	}
}



void Dialog_prikaz_clanarina::OnBnClickedBtnZatvoriAplikaciju1()
{
	PostQuitMessage(0);
}


void Dialog_prikaz_clanarina::OnBnClickedBtnPocetna()
{
	DialogClanarine *pPar = (DialogClanarine*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();
}


void Dialog_prikaz_clanarina::OnBnClickedBtnClanovi1()
{
	DialogClanovi* DClanovi;
	DClanovi = new DialogClanovi;

	DialogClanarine *pPar = (DialogClanarine*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DClanovi->DoModal();
}


void Dialog_prikaz_clanarina::OnBnClickedBtnNatjecanja1()
{
	Dialog_Natjecanja* DNatjecanja;
	DNatjecanja = new Dialog_Natjecanja;

	DialogClanarine *pPar = (DialogClanarine*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DNatjecanja->DoModal();
}

void Dialog_prikaz_clanarina::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFntData == 0);
	ASSERT(m_pFntHeader1 == 0);
	ASSERT(m_pFntHeader2 == 0);

	pDC->SetMapMode(MM_TWIPS);
	// Create the bold font used for the fields. 	
	m_pFntData = new CFont;
	ASSERT(m_pFntData);
	m_pFntData->CreateFont(280,
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
	// Create the big font used for the Headline banner.
	m_pFntHeader1 = new CFont;
	ASSERT(m_pFntHeader1);
	m_pFntHeader1->CreateFont(850,
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
	// Create the bold font used for the Headline highlight.
	// This is the text used under the headline banner
	m_pFntHeader2 = new CFont;
	ASSERT(m_pFntHeader2);
	m_pFntHeader2->CreateFont(280,
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

	INT brIt = m_Lista.GetItemCount();
	FLOAT brStr;
	if (brIt % 32 == 0)
		brStr = brIt / 32;
	else
		brStr = brIt / 32 + 1;

	pInfo->SetMaxPage(brStr);
}

void Dialog_prikaz_clanarina::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{
	CString s;
	int sirina_p = pDC->GetDeviceCaps(HORZRES);
	int visina_p = pDC->GetDeviceCaps(VERTRES);
	CSize font = pDC->GetTextExtent(_T("A"));
	int x1 = sirina_p / 6;
	int y1 = -(font.cy * 8);//4;

	CString text;

	TEXTMETRIC tm;
	PrintHeader(pDC, pInfo);

	CFont* pOldFont = pDC->SelectObject(m_pFntData);
	pDC->GetTextMetrics(&tm);

	int flag = pInfo->m_nCurPage;
	CString rb;
	for (int red = 32 * (flag - 1); red < m_Lista.GetItemCount() && red <= 31 * flag; red++)
	{
		if (m_rd_clan.GetCheck() == 1)
		{
			rb.Format(_T("%ld"), red + 1);
			pDC->TextOut(x1, y1 -= 550, rb + _T("."));
			pDC->TextOut(x1*1.5, y1, m_Lista.GetItemText(red, 1));
			pDC->TextOut(x1*4.7, y1, m_Lista.GetItemText(red, 3));
			pDC->TextOut(x1*8.4, y1, m_Lista.GetItemText(red, 2));
		}
		else if (m_rd_klub.GetCheck() == 1)
		{
			rb.Format(_T("%ld"), red + 1);
			pDC->TextOut(x1, y1 -= 550, rb + _T("."));
			pDC->TextOut(x1*1.5, y1, m_Lista.GetItemText(red, 0));
			pDC->TextOut(x1*5.1, y1, m_Lista.GetItemText(red, 1));
			pDC->TextOut(x1*7.4, y1, m_Lista.GetItemText(red, 3));
			pDC->TextOut(x1*10.9, y1, m_Lista.GetItemText(red, 2));
		}		
	}
}


void Dialog_prikaz_clanarina::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
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

void Dialog_prikaz_clanarina::Print()
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

void Dialog_prikaz_clanarina::PrintHeader(CDC* pDC, CPrintInfo *pInfo)
{
	ASSERT(pDC);
	TEXTMETRIC tm;
	CPoint pt(0, 0);
	// Select the banner font, and print the headline.
	CFont* pOldFont = pDC->SelectObject(m_pFntHeader1);
	ASSERT(pOldFont);
	pDC->GetTextMetrics(&tm);
	int cyText = tm.tmHeight*0.5 + tm.tmExternalLeading;
	pt.y -= cyText;
	pDC->TextOut(pt.x + 1070, pt.y, _T("KARATE  KLUB  \"RONIN\""));
	
	CPen* pOldPen = pDC->SelectObject(&m_penBlack);
	ASSERT(pOldPen);
	pt.y -= cyText * 2;
	pDC->MoveTo(pt);
	pDC->LineTo(12000, pt.y);
	
	VERIFY(pDC->SelectObject(m_pFntHeader2));
	
	pt.y -= (cyText / 4);

	CString tekst, ime;
	CTime dat = CTime::GetCurrentTime();
	CString datum = dat.Format(_T("%d.%m.%Y"));
	CTime dat1, dat2;
	m_date_od.GetTime(dat1);
	m_date_do.GetTime(dat2);
	CString datum1 = dat1.Format(_T("%d.%m.%Y"));
	CString datum2 = dat2.Format(_T("%d.%m.%Y"));

	m_combo.GetWindowTextW(ime);

	if (m_rd_clan.GetState() == 1 && m_rd_sve_uplate.GetState() == 1)
	{		
		tekst = ime +  _T("       uplate do dana:  ") + datum;
	}		
	else if (m_rd_clan.GetState() == 1 && m_rd_razdoblje.GetState() == 1)
	{
		tekst = ime + _T("       uplate u razdoblju:  ")+ datum1 +_T("  -  ") + datum2;
	}	
	else if (m_rd_klub.GetState() == 1 && m_rd_sve_uplate.GetState() == 1)
	{
		tekst = _T("Uplate do dana:  ")+ datum;
	}
		
	else if (m_rd_klub.GetState() == 1 && m_rd_razdoblje.GetState() == 1)
	{
		tekst = _T("Uplate u razdoblju:  ") + datum1 + _T("  -  ") + datum2;;
	}
		
	UINT st = pInfo->m_nCurPage;
	UINT uk = pInfo->GetMaxPage();
	CString str, ukStr;
	str.Format(_T("%u"), st);
	ukStr.Format(_T("%u"), uk);

	pDC->TextOut(pt.x += 1600, pt.y, tekst);
	pDC->TextOut(pt.x *5.5, pt.y, _T(" str.  ") + str + _T("/") + ukStr);
	// Restore GDI objects.
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


