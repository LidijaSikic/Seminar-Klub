// RezultatiSet.h : Implementation of the RezultatiSet class



// RezultatiSet implementation

// code generated on 7. veljaèe 2019., 15:09

#include "stdafx.h"
#include "RezultatiSet.h"
IMPLEMENT_DYNAMIC(RezultatiSet, CRecordset)

RezultatiSet::RezultatiSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_IDrezultata = 0;
	m_IDclana = 0;
	m_IDnatjecanja = 0;
	m_Kategorija = L"";
	m_Rezultat = L"";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}

CString RezultatiSet::GetDefaultConnect()
{
	return _T("DSN=Baza;");
}

CString RezultatiSet::GetDefaultSQL()
{
	return _T("[Rezultati]");
}

void RezultatiSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[IDrezultata]"), m_IDrezultata);
	RFX_Long(pFX, _T("[IDclana]"), m_IDclana);
	RFX_Long(pFX, _T("[IDnatjecanja]"), m_IDnatjecanja);
	RFX_Text(pFX, _T("[Kategorija]"), m_Kategorija);
	RFX_Text(pFX, _T("[Rezultat]"), m_Rezultat);

}
/////////////////////////////////////////////////////////////////////////////
// RezultatiSet diagnostics

#ifdef _DEBUG
void RezultatiSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void RezultatiSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG




long RezultatiSet::MaxID()
{
	MoveLast();
	return m_IDrezultata;
}
