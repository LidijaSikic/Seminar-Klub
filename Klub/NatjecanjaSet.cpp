// NatjecanjaSet.h : Implementation of the NatjecanjaSet class



// NatjecanjaSet implementation

// code generated on 7. veljaèe 2019., 15:06

#include "stdafx.h"
#include "NatjecanjaSet.h"
IMPLEMENT_DYNAMIC(NatjecanjaSet, CRecordset)

NatjecanjaSet::NatjecanjaSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_IDnatjecanja = 0;
	m_NazivNatjecanja = L"";
	m_DatumNatjecanja;
	m_Koeficijent = 0.0;
	m_nFields = 4;
	m_nDefaultType = dynaset;
}

CString NatjecanjaSet::GetDefaultConnect()
{
	return _T("DSN=Baza;");
}

CString NatjecanjaSet::GetDefaultSQL()
{
	return _T("[Natjecanja]");
}

void NatjecanjaSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// Macros such as RFX_Text() and RFX_Int() are dependent on the
// type of the member variable, not the type of the field in the database.
// ODBC will try to automatically convert the column value to the requested type
	RFX_Long(pFX, _T("[IDnatjecanja]"), m_IDnatjecanja);
	RFX_Text(pFX, _T("[NazivNatjecanja]"), m_NazivNatjecanja);
	RFX_Date(pFX, _T("[DatumNatjecanja]"), m_DatumNatjecanja);
	RFX_Double(pFX, _T("[Koeficijent]"), m_Koeficijent);

}
/////////////////////////////////////////////////////////////////////////////
// NatjecanjaSet diagnostics

#ifdef _DEBUG
void NatjecanjaSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void NatjecanjaSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

long NatjecanjaSet::MaxID()
{
	MoveLast();
	return m_IDnatjecanja;
}
