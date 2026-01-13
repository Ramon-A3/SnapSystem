//=============================================================================
// Module name  : CDSalesPeopleAddOn.h
// Description  : Client Document for SalesPeople
//                Provides UI for MaxDeviationPerc field
//=============================================================================
#pragma once

#include <SalesPeople\Documents\DSalesPeople.h>
#include <SalesPeople\Documents\UISalesPeople.hjson>

#include "beginh.dex"

///////////////////////////////////////////////////////////////////////////////
//             CDSalesPeopleAddOn class declaration
///////////////////////////////////////////////////////////////////////////////
class CDSalesPeopleAddOn : public CClientDoc
{
    DECLARE_DYNCREATE(CDSalesPeopleAddOn)

public:
    CDSalesPeopleAddOn();

public:
    DSalesPeople* GetServerDoc() const
    {
        return (DSalesPeople*)m_pServerDocument;
    }

protected:
    virtual BOOL OnAttachData();

    DECLARE_MESSAGE_MAP()
};

#include "endh.dex"
