// ClanarineSet.h : Implementation of the ClanarineSet class



// ClanarineSet implementation

// code generated on 7. veljaèe 2019., 15:03

#include "stdafx.h"
#include "ClanarineSet.h"
IMPLEMENT_DYNAMIC(ClanarineSet, CRecordset)

ClanarineSet::ClanarineSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_IDuplate = 0;
	m_IDclana = 0;
	m_DatumUplate;
	m_IznosUplate = L"";
	m_SvrhaUplate = L"";
	m_nFields = 5;
	m_nDefaultType = dynaset;
}

CString ClanarineSet::GetDefaultConnect()
{
	return _T("DSN=Baza;");
}

CString ClanarineSet::GetDefaultSQL()
{
	return _T("[Clanarine]");
}

void ClanarineSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[IDuplate]"), m_IDuplate);
	RFX_Long(pFX, _T("[IDclana]"), m_IDclana);
	RFX_Date(pFX, _T("[DatumUplate]"), m_DatumUplate);
	RFX_Text(pFX, _T("[IznosUplate]"), m_IznosUplate);
	RFX_Text(pFX, _T("[SvrhaUplate]"), m_SvrhaUplate);

}
/////////////////////////////////////////////////////////////////////////////
// ClanarineSet diagnostics

#ifdef _DEBUG
void ClanarineSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void ClanarineSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG




long ClanarineSet::MaxID()
{
	MoveLast();
	return m_IDuplate;
}
