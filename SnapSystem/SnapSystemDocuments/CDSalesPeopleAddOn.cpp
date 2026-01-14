//=============================================================================
// Module name  : CDSalesPeopleAddOn.cpp
// Description  : Client Document for SalesPeople
//=============================================================================

#include "stdafx.h"

#include <SalesPeople\Dbl\TSalesPeople.h>

#include "CDSalesPeopleAddOn.h"
#include <SnapSystem\SnapSystemDbl\TSalesPeopleAddOn.h>
#include <SnapSystem\ModuleObjects\CDSalesPeopleAddOn\JsonForms\IDD_TD_SALESPEOPLE_ADDON.hjson>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//             CDSalesPeopleAddOn Implementation
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDSalesPeopleAddOn, CClientDoc)

//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDSalesPeopleAddOn, CClientDoc)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDSalesPeopleAddOn::CDSalesPeopleAddOn()
    : CClientDoc()
{
}

//-----------------------------------------------------------------------------
BOOL CDSalesPeopleAddOn::OnAttachData()
{
    return TRUE;
}
