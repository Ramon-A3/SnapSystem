//=============================================================================
// Module name  : CDItemsPriceListsAddOn.cpp
// Description  : Client Document for Items Price Lists
//=============================================================================

#include "stdafx.h"

#include <PricePolicies\Dbl\TItemPricesPolicies.h>
#include <PricePolicies\AddOnsItems\CDItemsPricesPolicies.h>
#include <Items\Documents\DBTItems.h>

#include "CDItemsPriceListsAddOn.h"
#include <SnapSystem\SnapSystemDbl\TItemsPriceListsAddOn.h>
#include <SnapSystem\ModuleObjects\CDItemsPriceListsAddOn\JsonForms\IDD_TD_PRICELISTS_ADDON.hjson>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//             CDItemsPriceListsAddOn Implementation
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDItemsPriceListsAddOn, CClientDoc)

//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDItemsPriceListsAddOn, CClientDoc)
    ON_EN_VALUE_CHANGED(IDC_PRICELISTS_ITM_PRICE, OnPriceChanged)
    ON_EN_VALUE_CHANGED(IDC_PRICELISTS_ITM_DISCOUNT, OnDiscount1Changed)
    // Note: There is no separate IDC for Discount2 in the ERP UI.
    // MinimumCost will be recalculated when Price or Discount1 changes.
    // If Discount2 is edited via direct DB access, recalculate on form load.
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDItemsPriceListsAddOn::CDItemsPriceListsAddOn()
    : CClientDoc()
{
    SetMsgRoutingMode(CD_MSG_AFTER);
}

//-----------------------------------------------------------------------------
BOOL CDItemsPriceListsAddOn::OnAttachData()
{
    return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CDItemsPriceListsAddOn::OnInitAuxData()
{
    return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CDItemsPriceListsAddOn::OnPrepareAuxData()
{
    // This is called during NEW/EDIT/BROWSE mode transitions
    // Recalculate MinimumCost for all price list rows
    RecalculateAllPriceLists();

    return TRUE;
}

//-----------------------------------------------------------------------------
// CalculateMinimumCost - Core calculation
// MinimumCost = Price - (Price * Discount1/100)
// If Discount2 > 0: MinimumCost = Step1 - (Step1 * Discount2/100)
//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::CalculateMinimumCost(TItemsPriceLists* pRecord)
{
    if (!pRecord)
        return;

    // Get AddOn fields
    TItemsPriceListsAddOn* pAddOn = (TItemsPriceListsAddOn*)pRecord->GetAddOnFields(
        RUNTIME_CLASS(TItemsPriceListsAddOn));
    if (!pAddOn)
        return;

    // Get values from record
    double dPrice = (double)pRecord->f_Price;
    double dDiscount1 = (double)pRecord->f_Discount1;
    double dDiscount2 = (double)pRecord->f_Discount2;

    // Validate inputs - ensure non-negative price and reasonable discounts
    if (dPrice < 0.0 || dDiscount1 < 0.0 || dDiscount2 < 0.0 ||
        dDiscount1 > 100.0 || dDiscount2 > 100.0)
    {
        pAddOn->f_MinimumCost = 0.0;
        return;
    }

    // Step 1: Apply Discount1
    double dStep1 = dPrice - (dPrice * dDiscount1 / 100.0);

    // Step 2: Apply Discount2 only if > 0
    double dMinimumCost = dStep1;
    if (dDiscount2 > 0.0)
    {
        dMinimumCost = dStep1 - (dStep1 * dDiscount2 / 100.0);
    }

    // Ensure MinimumCost is non-negative
    if (dMinimumCost < 0.0)
        dMinimumCost = 0.0;

    // Update AddOn field
    pAddOn->f_MinimumCost = dMinimumCost;
}

//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::RecalculateAllPriceLists()
{
    DItems* pServerDoc = GetServerDoc();
    if (!pServerDoc)
        return;

    // Get the CDItemsPricesPolicies CClientDoc which manages the PriceLists DBT
    CDItemsPricesPolicies* pPricesPolicies = (CDItemsPricesPolicies*)pServerDoc->GetClientDoc(
        RUNTIME_CLASS(CDItemsPricesPolicies));
    if (!pPricesPolicies || !pPricesPolicies->m_pDBTItemsPriceLists)
        return;

    DBTItemsPriceLists* pDBTPriceLists = pPricesPolicies->m_pDBTItemsPriceLists;

    // Iterate through all rows and recalculate (use GetUpperBound like ERP does)
    for (int i = 0; i <= pDBTPriceLists->GetUpperBound(); i++)
    {
        TItemsPriceLists* pRecord = (TItemsPriceLists*)pDBTPriceLists->GetRow(i);
        if (pRecord)
        {
            CalculateMinimumCost(pRecord);
        }
    }
}

//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::OnPriceChanged()
{
    DItems* pServerDoc = GetServerDoc();
    if (!pServerDoc)
        return;

    // Get the CDItemsPricesPolicies CClientDoc which manages the PriceLists DBT
    CDItemsPricesPolicies* pPricesPolicies = (CDItemsPricesPolicies*)pServerDoc->GetClientDoc(
        RUNTIME_CLASS(CDItemsPricesPolicies));
    if (!pPricesPolicies || !pPricesPolicies->m_pDBTItemsPriceLists)
        return;

    DBTItemsPriceLists* pDBTPriceLists = pPricesPolicies->m_pDBTItemsPriceLists;

    // GetCurrentRow() returns the current SqlRecord* directly
    TItemsPriceLists* pRecord = (TItemsPriceLists*)pDBTPriceLists->GetCurrentRow();
    if (pRecord)
        CalculateMinimumCost(pRecord);
}

//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::OnDiscount1Changed()
{
    OnPriceChanged();  // Same logic
}
