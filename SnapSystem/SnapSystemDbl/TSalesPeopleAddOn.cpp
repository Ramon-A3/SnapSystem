//=============================================================================
// Module name  : TSalesPeopleAddOn.cpp
// Description  : AddOn columns for MA_SalesPeople table
//=============================================================================

#include "stdafx.h"
#include "TSalesPeopleAddOn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//             TSalesPeopleAddOn Implementation
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(TSalesPeopleAddOn, SqlAddOnFieldsColumn)

//-----------------------------------------------------------------------------
TSalesPeopleAddOn::TSalesPeopleAddOn()
{
    f_MaxDeviationPerc = 40.0;  // Initialize to default 40%
}

//-----------------------------------------------------------------------------
void TSalesPeopleAddOn::InitAddOnFields()
{
    // Default value is set in SQL script (40%)
}

//-----------------------------------------------------------------------------
int TSalesPeopleAddOn::BindAddOnFields(int nStartPos /*=0*/)
{
    BEGIN_BIND_ADDON_FIELDS(nStartPos);
        BIND_ADDON_DATA(_NS_FLD("MaxDeviationPerc"), f_MaxDeviationPerc);
    END_BIND_ADDON_FIELDS();
}
