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

	CString s, s1;

	if (RClanovi->IsBOF() && RClanovi->IsEOF())
	{
		s.LoadString(IDS_STRING_BAZA_BEZ_PODATAKA);
		s1.LoadString(IDS_STRING_GRESKA);
		MessageBox(s, s1, MB_ICONEXCLAMATION | MB_OK);		
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
	CString s;
	s.LoadString(IDS_STRING_IME);
	m_Lista.InsertColumn(0, s, LVCFMT_LEFT, 180); 
	s.LoadString(IDS_STRING_DAT_UPLATE);
	m_Lista.InsertColumn(1, s, LVCFMT_CENTER, 120);
	s.LoadString(IDS_STRING_IZNOS);
	m_Lista.InsertColumn(2, s, LVCFMT_CENTER, 80);
	s.LoadString(IDS_STRING_SVRHA);
	m_Lista.InsertColumn(3, s, LVCFMT_LEFT, 200);
}


void Dialog_prikaz_clanarina::OnBnClickedBtnPrikazi()
{
	CString s, s1, s2;
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

		CString s;
		m_Lista.InsertColumn(0, ime, LVCFMT_LEFT, 180);
		s.LoadString(IDS_STRING_DAT_UPLATE);
		m_Lista.InsertColumn(1, s, LVCFMT_CENTER, 120);
		s.LoadString(IDS_STRING_IZNOS);
		m_Lista.InsertColumn(2, s, LVCFMT_CENTER, 80);
		s.LoadString(IDS_STRING_SVRHA);
		m_Lista.InsertColumn(3, s, LVCFMT_LEFT, 200);

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
			s.LoadString(IDS_STRING_CLAN);
			s1.LoadString(IDS_STRING_NEMA_UPLATE);
			s2.LoadString(IDS_STRING_OBAVIJEST);
			MessageBox(s + ime + s1, s2, MB_ICONINFORMATION | MB_OK);
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
		s.LoadString(IDS_STRING_IME);
		m_Lista.InsertColumn(0, s, LVCFMT_LEFT, 180);
		s.LoadString(IDS_STRING_DAT_UPLATE);
		m_Lista.InsertColumn(1, s, LVCFMT_CENTER, 120);
		s.LoadString(IDS_STRING_IZNOS);
		m_Lista.InsertColumn(2, s, LVCFMT_CENTER, 80);
		s.LoadString(IDS_STRING_SVRHA);
		m_Lista.InsertColumn(3, s, LVCFMT_LEFT, 200);

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
			s.LoadString(IDS_STRING_RAZDOBLJE_BEZ_UPLATA);
			s1.LoadString(IDS_STRING_OBAVIJEST);
			MessageBox(s, s1, MB_ICONINFORMATION | MB_OK);
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
	CString s, s1;
	int z = m_Lista.GetItemCount();
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

	// Create the bold font used for the fields. 	
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
	int brIt = m_Lista.GetItemCount();
	int visinaReda = VisinaReda(pDC);

	int brStr;
	if (brIt % (visina / visinaReda) == 0)
		brStr = brIt / (visina / visinaReda);
	else
		brStr = brIt / (visina / visinaReda) + 1;

	pInfo->SetMaxPage(brStr);	
}

void Dialog_prikaz_clanarina::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{	
	CPoint pt(0, 0);
	
	TEXTMETRIC tm;
	PrintHeader(pDC, pInfo);
	int visinaHeadera = VisinaHeadera(pDC);
	CFont* pOldFont = pDC->SelectObject(m_pFntData);
	pDC->GetTextMetrics(&tm);

	CRect visinaStranice = pInfo->m_rectDraw;
	
	int stranica = pInfo->m_nCurPage;
	int razmak = tm.tmHeight + tm.tmExternalLeading;
	int visinaReda = 1.5*razmak + 2 * tm.tmHeight;
	int visina = visinaStranice.Height() - visinaHeadera;

	int brRedova = visina / visinaReda;

	CString rb;

	pt.y = visinaHeadera + razmak;
	pt.x = 2 * tm.tmHeight;

	
	for (int red = brRedova * (stranica - 1); red < m_Lista.GetItemCount() && red <= (brRedova - 1) * stranica; red++)
	{
		if (m_rd_clan.GetCheck() == 1)
		{
			rb.Format(_T("%ld"), red + 1);
			pDC->TextOut(pt.x, pt.y +=2*razmak , rb + _T("."));
			pDC->TextOut(pt.x *2, pt.y, m_Lista.GetItemText(red, 1));
			pDC->TextOut(pt.x *6, pt.y, m_Lista.GetItemText(red, 3));
			pDC->TextOut(pt.x *12, pt.y, m_Lista.GetItemText(red, 2));
		}
		else if (m_rd_klub.GetCheck() == 1)
		{
			rb.Format(_T("%ld"), red + 1);
			pDC->TextOut(pt.x, pt.y += 2*razmak, rb + _T("."));
			pDC->TextOut(pt.x *2, pt.y, m_Lista.GetItemText(red, 0));
			pDC->TextOut(pt.x *7, pt.y, m_Lista.GetItemText(red, 1));
			pDC->TextOut(pt.x *11, pt.y, m_Lista.GetItemText(red, 3));
			pDC->TextOut(pt.x *17, pt.y, m_Lista.GetItemText(red, 2));
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
	CString s, s1;
	ASSERT(pDC);
	TEXTMETRIC tm;
	CPoint pt(0, 0);
	
	CFont* pOldFont = pDC->SelectObject(m_pFntHeader1);
	ASSERT(pOldFont);
	pDC->GetTextMetrics(&tm);
	int cyText = tm.tmHeight*0.5 + tm.tmExternalLeading;
	pt.y += cyText;
	pt.x = tm.tmHeight;

	s.LoadString(IDS_STRING_KARATE_KLUB);
	pDC->TextOut(pt.x *6 , pt.y, s);
	
	CPen* pOldPen = pDC->SelectObject(&m_penBlack);
	ASSERT(pOldPen);
	pt.x = 0;
	pt.y += cyText * 2;
	pDC->MoveTo(pt);
	pDC->LineTo(pDC->GetDeviceCaps(HORZRES), pt.y);
	
	VERIFY(pDC->SelectObject(m_pFntHeader2));
	
	pt.x = tm.tmHeight;
	pt.y += (cyText / 3);

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
		s.LoadString(IDS_STRING_UPLATE_DO_M);
		tekst = ime +  s + datum;
		
	}		
	else if (m_rd_clan.GetState() == 1 && m_rd_razdoblje.GetState() == 1)
	{
		s.LoadString(IDS_STRING_UPLATE_U_RAZDOBLJU_M);
		s1.LoadString(IDS_STRING_MINUS);
		tekst = ime + s+ datum1 +s1 + datum2;
	}	
	else if (m_rd_klub.GetState() == 1 && m_rd_sve_uplate.GetState() == 1)
	{
		s.LoadString(IDS_STRING_UPLATE_DO_V);
		tekst = s+ datum;
	}
		
	else if (m_rd_klub.GetState() == 1 && m_rd_razdoblje.GetState() == 1)
	{
		s.LoadString(IDS_STRING_UPLATE_U_RAZDOBLJU_V);
		s1.LoadString(IDS_STRING_MINUS);

		tekst = s + datum1 + s1 + datum2;;
	}
		
	UINT st = pInfo->m_nCurPage;
	UINT uk = pInfo->GetMaxPage();
	CString str, ukStr;
	str.Format(_T("%u"), st);
	ukStr.Format(_T("%u"), uk);

	pDC->TextOut(pt.x *3, pt.y, tekst);
	s.LoadString(IDS_STRING_STR);
	s1.LoadString(IDS_STRING_KROZ);
	pDC->TextOut(pt.x *16, pt.y, s + str + s1 + ukStr);
	// Restore GDI objects.
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

int Dialog_prikaz_clanarina::VisinaHeadera(CDC* pDC)
{
	int y = 0;
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

int Dialog_prikaz_clanarina::VisinaReda(CDC* pDC)
{
	CFont* pFont = pDC->SelectObject(m_pFntData);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int razmak = tm.tmHeight + tm.tmExternalLeading;
	int visinaReda = 1.5*razmak + 2 * tm.tmHeight;
	return visinaReda;
}
