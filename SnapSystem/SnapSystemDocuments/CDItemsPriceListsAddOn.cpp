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
    ON_EN_VALUE_CHANGED(IDC_ITEM_PRICELIST_PRICE, OnPriceChanged)
    ON_EN_VALUE_CHANGED(IDC_ITEM_PRICELIST_DISC1, OnDiscount1Changed)
    ON_EN_VALUE_CHANGED(IDC_ITEM_PRICELIST_DISC2, OnDiscount2Changed)
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

    // Get DBT for price lists
    DBTSlaveBuffered* pDBTPriceLists = (DBTSlaveBuffered*)pServerDoc->GetDBTItemsPriceLists();
    if (!pDBTPriceLists)
        return;

    // Iterate through all rows and recalculate
    for (int i = 0; i < pDBTPriceLists->GetSize(); i++)
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

    DBTSlaveBuffered* pDBTPriceLists = (DBTSlaveBuffered*)pServerDoc->GetDBTItemsPriceLists();
    if (!pDBTPriceLists)
        return;

    TItemsPriceLists* pRecord = (TItemsPriceLists*)pDBTPriceLists->GetRow(pDBTPriceLists->GetCurrentRow());
    if (pRecord)
        CalculateMinimumCost(pRecord);
}

//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::OnDiscount1Changed()
{
    OnPriceChanged();  // Same logic
}

//-----------------------------------------------------------------------------
void CDItemsPriceListsAddOn::OnDiscount2Changed()
{
    OnPriceChanged();  // Same logic
}
