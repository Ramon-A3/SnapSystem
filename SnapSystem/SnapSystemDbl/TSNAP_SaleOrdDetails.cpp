
#include "stdafx.h"             

#include "TSNAP_SaleOrdDetails.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(TSNAP_SaleOrdDetails, SqlRecord) 

//-----------------------------------------------------------------------------
TSNAP_SaleOrdDetails::TSNAP_SaleOrdDetails(BOOL bCallInit)
	:
	SqlRecord	(GetStaticName())
{
	f_Code.SetUpperCase();

	BindRecord();	
	if (bCallInit) Init();
}

//-----------------------------------------------------------------------------
void TSNAP_SaleOrdDetails::BindRecord()
{
	BEGIN_BIND_DATA	();
			BIND_DATA	(_NS_FLD("Code"),			f_Code);
		BIND_DATA	(_NS_FLD("Description"),	f_Description);
			BIND_TB_GUID();
	END_BIND_DATA();    
}

//-----------------------------------------------------------------------------
LPCTSTR TSNAP_SaleOrdDetails::GetStaticName() { return _NS_TBL("SNAP_SaleOrdDetails"); }