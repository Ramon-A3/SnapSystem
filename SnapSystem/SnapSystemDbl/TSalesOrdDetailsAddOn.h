#pragma once

// Include for the original table class we're extending
#include <SaleOrders/Dbl/TSalesOrder.h>

#include "beginh.dex"

//////////////////////////////////////////////////////////////////////////////
//             class TSalesOrdDetailsAddOn
//             AddOn columns for MA_SaleOrdDetails table
//////////////////////////////////////////////////////////////////////////////
class TB_EXPORT TSalesOrdDetailsAddOn : public SqlAddOnFieldsColumn
{
    DECLARE_DYNCREATE(TSalesOrdDetailsAddOn)

public:
    // New persistent fields mapped to added columns
    DataMon f_LastCost;                // Last cost of the item
    DataMon f_Margin;                  // Calculated margin
    DataMon f_SalesPersonMarginComm;   // Sales person commission based on margin

public:
    TSalesOrdDetailsAddOn();

public:
    virtual void InitAddOnFields();
    virtual int BindAddOnFields(int nStartPos = 0);
};

#include "endh.dex"
