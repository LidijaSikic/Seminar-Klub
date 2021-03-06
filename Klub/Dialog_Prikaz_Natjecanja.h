#pragma once
#include "ClanoviSet.h"
#include "NatjecanjaSet.h"
#include "RezultatiSet.h"

// Dialog_Prikaz_Natjecanja dialog

class Dialog_Prikaz_Natjecanja : public CDialog
{
	DECLARE_DYNAMIC(Dialog_Prikaz_Natjecanja)

public:
	Dialog_Prikaz_Natjecanja(CWnd* pParent = nullptr);   // standard constructor
	ClanoviSet* RClanovi;
	NatjecanjaSet* RNatjecanja;
	RezultatiSet* RRezultati;

	virtual ~Dialog_Prikaz_Natjecanja();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PRIKAZ_NATJECANJA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CFont* m_pFntData;
	CFont* m_pFntHeader1;
	CFont* m_pFntHeader2;
	CPen m_penBlack;
	void PrintHeader(CDC* pDC, CPrintInfo *pInfo);
	int VisinaHeadera(CDC* pDC);
	int VisinaReda(CDC* pDC);

	DECLARE_MESSAGE_MAP()
public:
	
	CButton m_rd_natjecatelj;
	CButton m_rd_klub;
	CButton m_rd_natjecanje;
	int m_rb_stanje;
	CComboBox m_combo_natjecatelj;
	CDateTimeCtrl m_date_od;
	CDateTimeCtrl m_date_do;
	CComboBox m_combo_natjecanje;
	CButton m_btn_prikazi;
	CListCtrl m_Lista;
	CEdit m_edit_pom;
	CStatic m_static_pom;
	CComboBox m_combo_pom;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRdNatjecanje();
	afx_msg void OnBnClickedBtnPrikazi();
	void NadiIme(long IDclana);
	void NadiNatjecanje(long IDnatjecanja);
	void NadiRezultate(long IDnatjecanja);
	long NadiIDnatjecatelja();
	long NadiIDnatjecanja();	
	afx_msg void OnBnClickedBtnPocetna5();
	afx_msg void OnBnClickedBtnZatvori5();
	afx_msg void OnBnClickedBtnClanovi5();
	afx_msg void OnBnClickedBtnClanarine5();
	afx_msg void OnBnClickedRdNatjecatelj();
	afx_msg void OnBnClickedRdKlub();
	afx_msg void OnLvnItemchangedLista(NMHDR *pNMHDR, LRESULT *pResult);
	void Print();
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnBnClickedBtnIspisiPrikaz();
	void Prikazi_rezultate_za_natjecatelja();
	void Prikazi_rezultate_za_klub();
	void Prikaz_rezultata_za_natjecanje();
};
