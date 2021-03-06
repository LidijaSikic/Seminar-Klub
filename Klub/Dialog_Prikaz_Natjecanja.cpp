// Dialog_Prikaz_Natjecanja.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "Klub.h"
#include "DialogClanovi.h"
#include "DialogClanarine.h"
#include "Dialog_Natjecanja.h"
#include "Dialog_Prikaz_Natjecanja.h"



// Dialog_Prikaz_Natjecanja dialog

IMPLEMENT_DYNAMIC(Dialog_Prikaz_Natjecanja, CDialog)

Dialog_Prikaz_Natjecanja::Dialog_Prikaz_Natjecanja(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PRIKAZ_NATJECANJA, pParent)
	, m_rb_stanje(0)
{	
	COLORREF clrBlack = GetSysColor(COLOR_WINDOWFRAME);
	m_penBlack.CreatePen(PS_SOLID, 0, clrBlack);
	m_pFntData = 0;
	m_pFntHeader1 = 0;
	m_pFntHeader2 = 0;
}

Dialog_Prikaz_Natjecanja::~Dialog_Prikaz_Natjecanja()
{
	delete m_pFntData;
	delete m_pFntHeader1;
	delete m_pFntHeader2;
}

void Dialog_Prikaz_Natjecanja::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RD_NATJECATELJ, m_rd_natjecatelj);
	DDX_Control(pDX, IDC_RD_KLUB, m_rd_klub);
	DDX_Control(pDX, IDC_RD_NATJECANJE, m_rd_natjecanje);
	DDX_Radio(pDX, IDC_RD_NATJECATELJ, m_rb_stanje);
	DDX_Control(pDX, IDC_COMBO_NATJECATELJ, m_combo_natjecatelj);
	DDX_Control(pDX, IDC_DATE_OD, m_date_od);
	DDX_Control(pDX, IDC_DATE_DO, m_date_do);
	DDX_Control(pDX, IDC_COMBO_NATJECANJE, m_combo_natjecanje);
	DDX_Control(pDX, IDC_BTN_PRIKAZI, m_btn_prikazi);
	DDX_Control(pDX, IDC_LISTA, m_Lista);
	DDX_Control(pDX, IDC_EDIT_POM, m_edit_pom);
	DDX_Control(pDX, IDC_STATIC_POM, m_static_pom);
	DDX_Control(pDX, IDC_COMBO_POM, m_combo_pom);	
}


BEGIN_MESSAGE_MAP(Dialog_Prikaz_Natjecanja, CDialog)
	ON_BN_CLICKED(IDC_RD_NATJECANJE, &Dialog_Prikaz_Natjecanja::OnBnClickedRdNatjecanje)
	ON_BN_CLICKED(IDC_BTN_PRIKAZI, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnPrikazi)	
	ON_BN_CLICKED(IDC_BTN_POCETNA5, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnPocetna5)
	ON_BN_CLICKED(IDC_BTN_ZATVORI5, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnZatvori5)
	ON_BN_CLICKED(IDC_BTN_CLANOVI5, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnClanovi5)
	ON_BN_CLICKED(IDC_BTN_CLANARINE5, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnClanarine5)
	ON_BN_CLICKED(IDC_RD_NATJECATELJ, &Dialog_Prikaz_Natjecanja::OnBnClickedRdNatjecatelj)
	ON_BN_CLICKED(IDC_RD_KLUB, &Dialog_Prikaz_Natjecanja::OnBnClickedRdKlub)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTA, &Dialog_Prikaz_Natjecanja::OnLvnItemchangedLista)
	ON_BN_CLICKED(IDC_BTN_ISPISI_PRIKAZ, &Dialog_Prikaz_Natjecanja::OnBnClickedBtnIspisiPrikaz)
END_MESSAGE_MAP()


// Dialog_Prikaz_Natjecanja message handlers


BOOL Dialog_Prikaz_Natjecanja::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, FALSE);

	RClanovi = new ClanoviSet;
	RNatjecanja = new NatjecanjaSet;
	RRezultati = new RezultatiSet;

	m_date_od.SetFormat(_T("dd.MM.yyyy"));
	m_date_do.SetFormat(_T("dd.MM.yyyy"));

	m_date_od.EnableWindow(FALSE);
	m_date_do.EnableWindow(FALSE);
	m_combo_natjecanje.EnableWindow(FALSE);

	CString Ime;
	
	if (!RClanovi->IsOpen())
		RClanovi->Open();
	while (!RClanovi->IsEOF()) {
		Ime = RClanovi->m_ImePrezime;
		m_combo_natjecatelj.AddString(Ime);
		RClanovi->MoveNext();
	}
	RClanovi->Close();
	m_combo_natjecatelj.SetCurSel(0);

	CString Naziv;
	CString s;

	if (!RNatjecanja->IsOpen())
		RNatjecanja->Open();
	while (!RNatjecanja->IsEOF()) {
		Naziv = RNatjecanja->m_NazivNatjecanja;
		CTime datum = RNatjecanja->m_DatumNatjecanja;
		CString strDatum = datum.Format("%d.%m.%Y");
		s.LoadString(IDS_STRING_ZAREZ_RAZMAK_1);
		m_combo_natjecanje.AddString(Naziv + s + strDatum);
		RNatjecanja->MoveNext();
	}
	RNatjecanja->Close();
	m_combo_natjecanje.SetCurSel(-1);

	m_Lista.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnPrikazi()
{
	m_Lista.DeleteAllItems();
	while (m_Lista.DeleteColumn(0));
	m_combo_pom.ResetContent();
	m_edit_pom.SetWindowTextW(_T(""));
	
	if (m_rd_natjecatelj.GetCheck() == 1)
	{
		Prikazi_rezultate_za_natjecatelja();
	}
	else if (m_rd_klub.GetCheck() == 1)
	{
		Prikazi_rezultate_za_klub();
	}
	else if (m_rd_natjecanje.GetCheck() == 1)
	{
		Prikaz_rezultata_za_natjecanje();
	}
	RRezultati->m_strFilter = _T("");
	RRezultati->Close();
}


void Dialog_Prikaz_Natjecanja::NadiIme(long IDclana)
{
	CString strIDclana;
	strIDclana.Format(_T("%ld"), IDclana);
	RClanovi->m_strFilter = (_T("IDclana = ") + strIDclana + _T(""));
	if(!RClanovi->IsOpen())
	RClanovi->Open();

	CString ime = RClanovi->m_ImePrezime;
	RClanovi->m_strFilter = _T("");
	RClanovi->Close();

	m_static_pom.SetWindowTextW(ime);
}

void Dialog_Prikaz_Natjecanja::NadiNatjecanje(long IDnatjecanja)
{
	long natjecanjeID = IDnatjecanja;
	CString strIDnatjecanja;
	CString s;
	strIDnatjecanja.Format(_T("%ld"), natjecanjeID);
	RNatjecanja->m_strFilter = (_T("IDnatjecanja = ") + strIDnatjecanja + _T(""));
	if(!RNatjecanja->IsOpen())
	RNatjecanja->Open();

	CString naziv = RNatjecanja->m_NazivNatjecanja;
	CTime datum = RNatjecanja->m_DatumNatjecanja;

	RNatjecanja->m_strFilter = _T("");
	RNatjecanja->Close();
	s.LoadString(IDS_STRING_ZAREZ_2);
	CString natjecanje = naziv + s + datum.Format(_T("%d.%m.%Y"));

	m_edit_pom.SetWindowTextW(natjecanje);
}

long Dialog_Prikaz_Natjecanja::NadiIDnatjecanja()
{
	CString naziv;
	long IDnatjecanja;

	int nSel = m_combo_natjecanje.GetCurSel();
	if (nSel != LB_ERR)
	{
		m_combo_natjecanje.GetLBText(nSel, naziv);
	}
	naziv.Delete(naziv.Find(_T(",")), 16);

	RNatjecanja->m_strFilter = _T("NazivNatjecanja = '") + naziv + _T("'");
	if(!RNatjecanja->IsOpen())
	RNatjecanja->Open();

	IDnatjecanja = RNatjecanja->m_IDnatjecanja;
	RNatjecanja->m_strFilter = _T("");
	RNatjecanja->Close();

	return IDnatjecanja;
}


long Dialog_Prikaz_Natjecanja::NadiIDnatjecatelja()
{
	CString ime;
	long IDclana;

	m_combo_natjecatelj.GetWindowTextW(ime);
	RClanovi->m_strFilter = _T("ImePrezime = '") + ime + _T("'");
	if (!RClanovi->IsOpen())
	RClanovi->Open();
	IDclana = RClanovi->m_IDclana;
	
	RClanovi->m_strFilter = _T("");
	RClanovi->Close();
	
	return IDclana;
}

void Dialog_Prikaz_Natjecanja::NadiRezultate(long IDnatjecanja)
{
	CString ime, kategorija, rezultat;
	CString strIDnatjecanja;
	CString s;
	int brojac = 0;
	long natjecanjeID= IDnatjecanja;
	strIDnatjecanja.Format(_T("%ld"), natjecanjeID);

	RRezultati->m_strFilter = _T("IDnatjecanja = ") + strIDnatjecanja + _T("");
	RRezultati->m_strSort = _T("IDrezultata ASC");
	if (!RRezultati->IsOpen())
		RRezultati->Open();
	
		while (!RRezultati->IsEOF())
		{		
			brojac += 1;

			NadiIme(RRezultati->m_IDclana);
			m_static_pom.GetWindowTextW(ime);

			kategorija = RRezultati->m_Kategorija;
			rezultat = RRezultati->m_Rezultat;
			
			s.LoadString(IDS_STRING_ZAREZ_RAZMAK_2);
			m_combo_pom.AddString(ime + s + kategorija + s + rezultat);
			RRezultati->MoveNext();
		}
	RRezultati->m_strFilter = _T("");
	RRezultati->Close();
}

void Dialog_Prikaz_Natjecanja::Prikazi_rezultate_za_natjecatelja()
{
	CString s,s1,s2;
	int nIndex;
	CString ime, podaci, naziv, datum, kategorija, rezultat;
	int nSel = m_combo_natjecatelj.GetCurSel();
	if (nSel != LB_ERR)
	{
		m_combo_natjecatelj.GetLBText(nSel, ime);
	}
	m_Lista.InsertColumn(0, ime, LVCFMT_LEFT, 160);
	s.LoadString(IDS_STRING_DATUM_NATJECANJA);
	m_Lista.InsertColumn(1, s, LVCFMT_CENTER, 140);
	s.LoadString(IDS_STRING_NAZIV_NATJECANJA);
	m_Lista.InsertColumn(2, s, LVCFMT_CENTER, 220);
	s.LoadString(IDS_STRING_KATEGORIJA);
	m_Lista.InsertColumn(3, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_REZULTAT);
	m_Lista.InsertColumn(4, s, LVCFMT_CENTER, 100);

	CString strIDclana;
	strIDclana.Format(_T("%ld"), NadiIDnatjecatelja());
	m_combo_pom.SetWindowTextW(strIDclana); //provjera
	RRezultati->m_strFilter = _T("IDclana = ") + strIDclana + _T("");

	if (!RRezultati->IsOpen())
		RRezultati->Open();

	if (RRezultati->IsBOF() && RRezultati->IsEOF())
	{
		s.LoadString(IDS_STRING_CLAN);
		s1.LoadString(IDS_STRING_CLAN_NEMA_REZULTATA);
		s2.LoadString(IDS_STRING_OBAVIJEST);
		MessageBox(s + ime + s1, s2, MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		while (!RRezultati->IsEOF())
		{

			long IDnatjecanja = RRezultati->m_IDnatjecanja;

			NadiNatjecanje(IDnatjecanja);
			m_edit_pom.GetWindowTextW(podaci);
			naziv = podaci.Left(podaci.Find(_T(',')));

			datum = podaci.Mid(podaci.ReverseFind(_T(',')) + 3);

			nIndex = m_Lista.InsertItem(0, _T(""));
			m_Lista.SetItemText(nIndex, 1, datum);
			m_Lista.SetItemText(nIndex, 2, naziv);
			m_Lista.SetItemText(nIndex, 3, RRezultati->m_Kategorija);
			m_Lista.SetItemText(nIndex, 4, RRezultati->m_Rezultat);

			RRezultati->MoveNext();
		}
	}
}


void Dialog_Prikaz_Natjecanja::Prikazi_rezultate_za_klub()
{
	int nIndex;
	CString podaci, ime, naziv, datum, kategorija, rezultat;
	CString s,s1;
	s.LoadString(IDS_STRING_DATUM_NATJECANJA);
	m_Lista.InsertColumn(0, s, LVCFMT_CENTER, 140);
	s.LoadString(IDS_STRING_NAZIV_NATJECANJA);
	m_Lista.InsertColumn(1, s, LVCFMT_LEFT, 220);
	s.LoadString(IDS_STRING_NATJECATELJ);
	m_Lista.InsertColumn(2, s, LVCFMT_CENTER, 150);
	s.LoadString(IDS_STRING_KATEGORIJA);
	m_Lista.InsertColumn(3, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_REZULTAT);
	m_Lista.InsertColumn(4, s, LVCFMT_CENTER, 100);

	CTime DateOd, DateDo;
	m_date_od.GetTime(DateOd);
	CString strDateOd = DateOd.Format(_T("%m/%d/%y"));

	m_date_do.GetTime(DateDo);
	CString strDateDo = DateDo.Format(_T("%m/%d/%y"));

	RNatjecanja->m_strFilter = _T("DatumNatjecanja between #") + strDateOd + _T("#") + _T(" and #") + strDateDo + _T("#");
	RNatjecanja->m_strSort = _T("DatumNatjecanja DESC");

	if (!RNatjecanja->IsOpen())
		RNatjecanja->Open();

	if (RNatjecanja->IsBOF() && RNatjecanja->IsEOF())
	{
		s.LoadString(IDS_STRING_RAZDOBLJE_BEZ_NATJECANJA);
		s1.LoadString(IDS_STRING_OBAVIJEST);
		MessageBox(s, s, MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		while (!RNatjecanja->IsEOF())
		{
			m_combo_pom.ResetContent();
			CTime dat = RNatjecanja->m_DatumNatjecanja;
			datum = dat.Format(_T("%d.%m.%Y"));

			naziv = RNatjecanja->m_NazivNatjecanja;

			m_edit_pom.SetWindowTextW(naziv);

			NadiRezultate(RNatjecanja->m_IDnatjecanja);

			if (m_combo_pom.GetCount() == 0)
			{
				nIndex = m_Lista.InsertItem(0, datum);
				m_Lista.SetItemText(nIndex, 1, naziv);
				m_Lista.SetItemText(nIndex, 2, _T(""));
				m_Lista.SetItemText(nIndex, 3, _T(""));
				m_Lista.SetItemText(nIndex, 4, _T(""));
			}
			else
			{
				for (int n = 0; n<m_combo_pom.GetCount(); n++)
				{
					if (n == (m_combo_pom.GetCount() - 1))
					{
						m_combo_pom.GetLBText(n, podaci);
						ime = podaci.Left(podaci.Find(_T(",")));
						kategorija = podaci.Mid(podaci.Find(_T(',')) + 1, (podaci.ReverseFind(_T(',')) - 1 - (podaci.Find(_T(',')) + 1)));
						rezultat = podaci.Mid(podaci.ReverseFind(_T(',')) + 1);

						nIndex = m_Lista.InsertItem(0, datum);
						m_Lista.SetItemText(nIndex, 1, naziv);
						m_Lista.SetItemText(nIndex, 2, ime);
						m_Lista.SetItemText(nIndex, 3, kategorija);
						m_Lista.SetItemText(nIndex, 4, rezultat);
					}
					else
					{
						m_combo_pom.GetLBText(n, podaci);
						ime = podaci.Left(podaci.Find(_T(",")));
						kategorija = podaci.Mid(podaci.Find(_T(',')) + 1, (podaci.ReverseFind(_T(',')) - 1 - (podaci.Find(_T(',')) + 1)));
						rezultat = podaci.Mid(podaci.ReverseFind(_T(',')) + 1);

						nIndex = m_Lista.InsertItem(0, _T(""));
						m_Lista.SetItemText(nIndex, 1, _T(""));
						m_Lista.SetItemText(nIndex, 2, ime);
						m_Lista.SetItemText(nIndex, 3, kategorija);
						m_Lista.SetItemText(nIndex, 4, rezultat);
					}
				}
			}
			RNatjecanja->MoveNext();
		}
	}
}


void Dialog_Prikaz_Natjecanja::Prikaz_rezultata_za_natjecanje()
{
	int nIndex;
	CString podaci, ime, naziv, datum, kategorija, rezultat;
	CString s,s1,s2;
	int nSel = m_combo_natjecanje.GetCurSel();
	if (nSel != LB_ERR)
	{
		m_combo_natjecanje.GetLBText(nSel, naziv);
	}
	m_Lista.InsertColumn(0, naziv, LVCFMT_LEFT, 320);
	s.LoadString(IDS_STRING_NATJECATELJ);
	m_Lista.InsertColumn(1, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_KATEGORIJA);
	m_Lista.InsertColumn(2, s, LVCFMT_LEFT, 150);
	s.LoadString(IDS_STRING_REZULTAT);
	m_Lista.InsertColumn(3, s, LVCFMT_CENTER, 100);

	naziv.Delete(naziv.Find(_T(",")), 16);
	CString strIDnatjecanja;
	strIDnatjecanja.Format(_T("%ld"), NadiIDnatjecanja());

	RRezultati->m_strFilter = _T("IDnatjecanja = ") + strIDnatjecanja + _T("");

	if (!RRezultati->IsOpen())
		RRezultati->Open();

	if (RRezultati->IsBOF() && RRezultati->IsEOF())
	{
		s.LoadString(IDS_STRING_ZA_NATJECANJE);
		s1.LoadString(IDS_STRING_NEMA_UPISANIH_REZULTATA);
		s2.LoadString(IDS_STRING_OBAVIJEST);
		MessageBox(s + naziv + s1, s2, MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		while (!RRezultati->IsEOF())
		{
			NadiIme(RRezultati->m_IDclana);
			m_static_pom.GetWindowTextW(ime);

			nIndex = m_Lista.InsertItem(0, _T(""));
			m_Lista.SetItemText(nIndex, 1, ime);
			m_Lista.SetItemText(nIndex, 2, RRezultati->m_Kategorija);
			m_Lista.SetItemText(nIndex, 3, RRezultati->m_Rezultat);

			RRezultati->MoveNext();
		}
	}
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnPocetna5()
{
	Dialog_Natjecanja *pPar = (Dialog_Natjecanja*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnZatvori5()
{
	PostQuitMessage(0);
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnClanovi5()
{
	DialogClanovi* DClanovi;
	DClanovi = new DialogClanovi;

	Dialog_Natjecanja *pPar = (Dialog_Natjecanja*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DClanovi->DoModal();
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnClanarine5()
{
	DialogClanarine* DClanarine;
	DClanarine = new DialogClanarine;

	Dialog_Natjecanja *pPar = (Dialog_Natjecanja*)GetParent();
	pPar->EndDialog(IDOK);
	OnCancel();

	DClanarine->DoModal();
}



void Dialog_Prikaz_Natjecanja::OnBnClickedRdNatjecatelj()
{
	m_combo_natjecatelj.SetCurSel(0);
	m_combo_natjecatelj.EnableWindow(TRUE);
	m_date_od.EnableWindow(FALSE);
	m_date_do.EnableWindow(FALSE);
	m_combo_natjecanje.EnableWindow(FALSE);
}


void Dialog_Prikaz_Natjecanja::OnBnClickedRdKlub()
{
	m_combo_natjecatelj.EnableWindow(FALSE);
	m_date_od.EnableWindow(TRUE);
	m_date_do.EnableWindow(TRUE);
	m_combo_natjecanje.EnableWindow(FALSE);
}

void Dialog_Prikaz_Natjecanja::OnBnClickedRdNatjecanje()
{
	m_combo_natjecanje.SetCurSel(0);
	m_combo_natjecatelj.EnableWindow(FALSE);
	m_date_od.EnableWindow(FALSE);
	m_date_do.EnableWindow(FALSE);
	m_combo_natjecanje.EnableWindow(TRUE);
}

void Dialog_Prikaz_Natjecanja::OnLvnItemchangedLista(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	CString text;
	int z = m_Lista.GetItemCount();
	text.Format(_T("%ld"), z); 
	m_edit_pom.SetWindowTextW(text);
	*pResult = 0;
}

/////////   P R I N T   ////////////////

void Dialog_Prikaz_Natjecanja::OnBeginPrinting(CDC *pDC, CPrintInfo* pInfo)
{
	ASSERT(m_pFntData == 0);
	ASSERT(m_pFntHeader1 == 0);
	ASSERT(m_pFntHeader2 == 0);
		
	m_pFntData = new CFont;
	ASSERT(m_pFntData);
	int fntDataSize = 12;
	m_pFntData->CreateFont(-fntDataSize*pDC->GetDeviceCaps(LOGPIXELSY)/72,
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

void Dialog_Prikaz_Natjecanja::OnPrint(CDC *pDC, CPrintInfo* pInfo)
{
	
	CPoint pt(0, 0);
	
	TEXTMETRIC tm;
	PrintHeader(pDC, pInfo);
	int visinaHeadera = VisinaHeadera(pDC);	
	CRect visinaStranice = pInfo->m_rectDraw;
	pDC->GetTextMetrics(&tm);

	int stranica = pInfo->m_nCurPage;
	int razmak = tm.tmHeight + tm.tmExternalLeading;	
	int visinaReda = VisinaReda(pDC);
	int visina = visinaStranice.Height() - visinaHeadera;
	int brRedova = visina / visinaReda;

	CString rb;
	
	pt.y = visinaHeadera + razmak;
	pt.x = 3.5 * tm.tmHeight;

	for (int red = brRedova * (stranica - 1); red < m_Lista.GetItemCount() && red <= (brRedova - 1) * stranica; red++)
	{
		if (m_rd_natjecatelj.GetState() == 1)
		{
			pDC->TextOut(pt.x, pt.y += 2*razmak, m_Lista.GetItemText(red, 1));
			pDC->TextOut(pt.x*3, pt.y, m_Lista.GetItemText(red, 2));
			pDC->TextOut(pt.x*6, pt.y, m_Lista.GetItemText(red, 3));
			pDC->TextOut(pt.x*8, pt.y, m_Lista.GetItemText(red, 4));
		}
		else if (m_rd_klub.GetState() == 1)
		{
			if (m_Lista.GetItemText(red, 0) != _T(""))
			{
				pDC->TextOut(pt.x, pt.y += 3.5 * razmak, m_Lista.GetItemText(red, 0));
			}
			else
			{
				pDC->TextOut(pt.x, pt.y += 2 * razmak, m_Lista.GetItemText(red, 0));
			}
			
			pDC->TextOut(pt.x*3, pt.y, m_Lista.GetItemText(red, 1));
			pDC->TextOut(pt.x*6, pt.y, m_Lista.GetItemText(red, 2));
			pDC->TextOut(pt.x*8.5, pt.y, m_Lista.GetItemText(red, 3));
			pDC->TextOut(pt.x*11, pt.y, m_Lista.GetItemText(red, 4));
		}
		else if (m_rd_natjecanje.GetState() == 1)
		{
			pDC->TextOut(pt.x, pt.y += 2 * razmak, m_Lista.GetItemText(red, 1));
			pDC->TextOut(pt.x*3.5, pt.y, m_Lista.GetItemText(red, 2));
			pDC->TextOut(pt.x*6, pt.y, m_Lista.GetItemText(red, 3));			
		}		
	}
}


void Dialog_Prikaz_Natjecanja::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
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

void Dialog_Prikaz_Natjecanja::Print()
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

void Dialog_Prikaz_Natjecanja::PrintHeader(CDC* pDC, CPrintInfo *pInfo)
{
	
	ASSERT(pDC);
	TEXTMETRIC tm;
	CPoint pt(0, 0);
	CString s,s1;
	
	CFont* pOldFont = pDC->SelectObject(m_pFntHeader1);
	
	ASSERT(pOldFont);
	pDC->GetTextMetrics(&tm);
	int cyText = tm.tmHeight*0.5 + tm.tmExternalLeading;
	pt.y += cyText;
	pt.x = tm.tmHeight;

	s.LoadString(IDS_STRING_KARATE_KLUB);
	pDC->TextOut(pt.x *6, pt.y, s);
	
	CPen* pOldPen = pDC->SelectObject(&m_penBlack);
	ASSERT(pOldPen);
	pt.x = 0;
	pt.y += cyText * 2;
	pDC->MoveTo(pt);
	pDC->LineTo(pDC->GetDeviceCaps(HORZRES), pt.y);
	
	VERIFY(pDC->SelectObject(m_pFntHeader2));
	
	pt.x = tm.tmHeight;
	pt.y += (cyText / 3);

	CTime dat1, dat2;
	CString datum1, datum2, ime;

	CString tekst;
	if (m_rd_natjecatelj.GetState() == 1)
	{		
		m_combo_natjecatelj.GetWindowTextW(ime);
		s.LoadString(IDS_STRING_REZULTATI);
		tekst = s + ime;
	}
	else if (m_rd_klub.GetState() == 1)
	{
		m_date_od.GetTime(dat1);
		datum1 = dat1.Format(_T("%d.%m.%Y"));

		m_date_do.GetTime(dat2);
		datum2 = dat2.Format(_T("%d.%m.%Y"));

		s.LoadString(IDS_STRING_REZULTATI_U_RAZDOBLJU);
		s1.LoadString(IDS_STRING_MINUS);
		tekst = s + datum1 + s1+ datum2;
	}
	else if (m_rd_natjecanje.GetState() == 1)
	{
		m_combo_natjecanje.GetWindowTextW(ime);
		s.LoadString(IDS_STRING_REZULTATI_NA_NATJECANJU);
		tekst =  s + ime  ;
	}

	UINT st = pInfo->m_nCurPage;
	UINT uk = pInfo->GetMaxPage();
	CString str, ukStr;
	str.Format(_T("%u"), st);
	ukStr.Format(_T("%u"), uk);

	pDC->TextOut(pt.x * 3, pt.y, tekst);
	s.LoadString(IDS_STRING_STR);
	s1.LoadString(IDS_STRING_KROZ);
	pDC->TextOut(pt.x * 16, pt.y, s + str + s1 + ukStr);
	// Restore GDI objects.
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


void Dialog_Prikaz_Natjecanja::OnBnClickedBtnIspisiPrikaz()
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

int Dialog_Prikaz_Natjecanja::VisinaHeadera(CDC* pDC)
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

int Dialog_Prikaz_Natjecanja::VisinaReda(CDC* pDC)
{
	CFont* pFont = pDC->SelectObject(m_pFntData);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int razmak = tm.tmHeight + tm.tmExternalLeading;
	int visinaReda = 2*razmak + tm.tmHeight;
	return visinaReda;
}