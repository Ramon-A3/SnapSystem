//=============================================================================
// Module name  : TItemsPriceListsAddOn.cpp
// Description  : AddOn columns for MA_ItemsPriceLists table
//=============================================================================

#include "stdafx.h"
#include "TItemsPriceListsAddOn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//             TItemsPriceListsAddOn Implementation
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(TItemsPriceListsAddOn, SqlAddOnFieldsColumn)

//-----------------------------------------------------------------------------
TItemsPriceListsAddOn::TItemsPriceListsAddOn()
{
}

//-----------------------------------------------------------------------------
void TItemsPriceListsAddOn::InitAddOnFields()
{
    // Initialize default values if needed
}

//-----------------------------------------------------------------------------
int TItemsPriceListsAddOn::BindAddOnFields(int nStartPos /*=0*/)
{
    BEGIN_BIND_ADDON_FIELDS(nStartPos);
        BIND_ADDON_DATA(_NS_FLD("MinimumCost"), f_MinimumCost);
    END_BIND_ADDON_FIELDS();
}
