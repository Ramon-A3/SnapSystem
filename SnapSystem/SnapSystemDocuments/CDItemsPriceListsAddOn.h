//=============================================================================
// Module name  : CDItemsPriceListsAddOn.h
// Description  : Client Document for Items Price Lists
//                Calculates MinimumCost when Price/Discounts change
//=============================================================================
#pragma once

#include <Items\Documents\DItems.h>
#include <PricePolicies\AddOnsItems\UIItemsPricesPolicies.hjson>
#include <SnapSystem\ModuleObjects\CDItemsPriceListsAddOn\JsonForms\IDD_TD_PRICELISTS_ADDON.hjson>

#include "beginh.dex"

///////////////////////////////////////////////////////////////////////////////
//             CDItemsPriceListsAddOn class declaration
///////////////////////////////////////////////////////////////////////////////
class CDItemsPriceListsAddOn : public CClientDoc
{
    DECLARE_DYNCREATE(CDItemsPriceListsAddOn)

public:
    CDItemsPriceListsAddOn();

public:
    DItems* GetServerDoc() const
    {
        return (DItems*)m_pServerDocument;
    }

protected:
    virtual BOOL OnAttachData();
    virtual BOOL OnInitAuxData();
    virtual BOOL OnPrepareAuxData();

public:
    void CalculateMinimumCost(TItemsPriceLists* pRecord);
    void RecalculateAllPriceLists();

protected:
    afx_msg void OnPriceChanged();
    afx_msg void OnDiscount1Changed();
    // Note: OnDiscount2Changed removed - no UI control for Discount2 in Items PriceList grid

    DECLARE_MESSAGE_MAP()
};

#include "endh.dex"
