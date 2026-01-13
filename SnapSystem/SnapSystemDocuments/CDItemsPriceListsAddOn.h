//=============================================================================
// Module name  : CDItemsPriceListsAddOn.h
// Description  : Client Document for Items Price Lists
//                Calculates MinimumCost when Price/Discounts change
//=============================================================================
#pragma once

#include <Items\Documents\DItems.h>
#include <Items\Documents\UIItems.hjson>

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

public:
    void CalculateMinimumCost(TItemsPriceLists* pRecord);
    void RecalculateAllPriceLists();

protected:
    afx_msg void OnPriceChanged();
    afx_msg void OnDiscount1Changed();
    afx_msg void OnDiscount2Changed();

    DECLARE_MESSAGE_MAP()
};

#include "endh.dex"
