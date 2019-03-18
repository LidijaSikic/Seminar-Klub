// ClanoviSet.h : Implementation of the ClanoviSet class



// ClanoviSet implementation

// code generated on 7. veljaèe 2019., 14:50

#include "stdafx.h"
#include "ClanoviSet.h"
IMPLEMENT_DYNAMIC(ClanoviSet, CRecordset)

ClanoviSet::ClanoviSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_IDclana = 0;
	m_ImePrezime = L"";
	m_Spol = L"";
	m_DatumRodenja;
	m_Tel = L"";
	m_Adresa = L"";
	m_Mail = L"";
	m_Aktivnost = L"";
	m_nFields = 8;
	m_nDefaultType = dynaset;
}

CString ClanoviSet::GetDefaultConnect()
{
	return _T("DSN=Baza;");
}

CString ClanoviSet::GetDefaultSQL()
{
	return _T("[Clanovi]");
}

void ClanoviSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[IDclana]"), m_IDclana);
	RFX_Text(pFX, _T("[ImePrezime]"), m_ImePrezime);
	RFX_Text(pFX, _T("[Spol]"), m_Spol);
	RFX_Date(pFX, _T("[DatumRodenja]"), m_DatumRodenja);
	RFX_Text(pFX, _T("[Tel]"), m_Tel);
	RFX_Text(pFX, _T("[Adresa]"), m_Adresa);
	RFX_Text(pFX, _T("[Mail]"), m_Mail);
	RFX_Text(pFX, _T("[Aktivnost]"), m_Aktivnost);

}
/////////////////////////////////////////////////////////////////////////////
// ClanoviSet diagnostics

#ifdef _DEBUG
void ClanoviSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void ClanoviSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG




long ClanoviSet::MaxID()
{
	MoveLast();
	return m_IDclana;
}
