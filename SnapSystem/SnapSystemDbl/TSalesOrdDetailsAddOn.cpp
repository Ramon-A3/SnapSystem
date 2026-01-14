//=============================================================================
// Module name  : TSalesOrdDetailsAddOn.cpp
// Description  : AddOn columns for MA_SaleOrdDetails table
//=============================================================================

#include "stdafx.h"
#include "TSalesOrdDetailsAddOn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////////
//             		TSalesOrdDetailsAddOn Implementation
//////////////////////////////////////////////////////////////////////////////
//
//-----------------------------------------------------------
IMPLEMENT_DYNCREATE(TSalesOrdDetailsAddOn, SqlAddOnFieldsColumn) 
//---------------------------------------------------------------------------
TSalesOrdDetailsAddOn::TSalesOrdDetailsAddOn()
{
}

//--------------------------------------------------------------------------
void TSalesOrdDetailsAddOn::InitAddOnFields()
{
}

//-----------------------------------------------------------------
int TSalesOrdDetailsAddOn::BindAddOnFields(int nStartPos /*=0*/)
{
    BEGIN_BIND_ADDON_FIELDS(nStartPos);
        BIND_ADDON_DATA(_NS_FLD("LastCost"), f_LastCost);
        BIND_ADDON_DATA(_NS_FLD("Margin"), f_Margin);
        BIND_ADDON_DATA(_NS_FLD("SalesPersonMarginComm"), f_SalesPersonMarginComm);
    END_BIND_ADDON_FIELDS();
}
