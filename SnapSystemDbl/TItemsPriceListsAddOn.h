//=============================================================================
// Module name  : TItemsPriceListsAddOn.h
// Description  : AddOn columns for MA_ItemsPriceLists table
//                Adds MinimumCost field (Price after discounts)
//=============================================================================
#pragma once

#include <PricePolicies/Dbl/TItemPricesPolicies.h>

#include "beginh.dex"

///////////////////////////////////////////////////////////////////////////////
//             class TItemsPriceListsAddOn
//             AddOn columns for MA_ItemsPriceLists table
///////////////////////////////////////////////////////////////////////////////
class TB_EXPORT TItemsPriceListsAddOn : public SqlAddOnFieldsColumn
{
    DECLARE_DYNCREATE(TItemsPriceListsAddOn)

public:
    // MinimumCost = Price - (Price * Discount1/100) - (Step1 * Discount2/100)
    DataMon f_MinimumCost;

public:
    TItemsPriceListsAddOn();

public:
    virtual void InitAddOnFields();
    virtual int BindAddOnFields(int nStartPos = 0);
};

#include "endh.dex"
