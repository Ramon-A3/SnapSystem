//=============================================================================
// Module name  : TSalesPeopleAddOn.h
// Description  : AddOn columns for MA_SalesPeople table
//                Adds MaxDeviationPerc field (Percentuale Massima Scostamento)
//=============================================================================
#pragma once

#include <SalesPeople/Dbl/TSalesPeople.h>

#include "beginh.dex"

///////////////////////////////////////////////////////////////////////////////
//             class TSalesPeopleAddOn
//             AddOn columns for MA_SalesPeople table
///////////////////////////////////////////////////////////////////////////////
class TB_EXPORT TSalesPeopleAddOn : public SqlAddOnFieldsColumn
{
    DECLARE_DYNCREATE(TSalesPeopleAddOn)

public:
    // MaxDeviationPerc = Percentuale Massima Scostamento (default 40%)
    // Used in Policy 02 commission calculation
    DataPerc f_MaxDeviationPerc;

public:
    TSalesPeopleAddOn();

public:
    virtual void InitAddOnFields();
    virtual int BindAddOnFields(int nStartPos = 0);
};

#include "endh.dex"
