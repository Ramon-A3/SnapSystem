//=============================================================================
// Module name  : CDSalesOrderAddOn.h
// Description  : Client Document for Sales Order margin/commission calculation
//=============================================================================
#pragma once

#include <SaleOrders\Documents\DSalesOrder.h>
#include <SaleOrders\Documents\UISalesOrder.hjson>

#include "beginh.dex"

//////////////////////////////////////////////////////////////////////////////
//             CDSalesOrderAddOn class declaration 
//             Extends Sales Order with automatic margin/commission calculation
//
//             Calculates:
//               - LastCost: from MA_ItemsBalances.f_LastCost
//               - Margin: (UnitValue - LastCost) × Quantity
//               - Commission: Margin × (BaseCommission / 100)
//////////////////////////////////////////////////////////////////////////////
//                                                                          
//=============================================================================
class CDSalesOrderAddOn : public CClientDoc
{
	DECLARE_DYNCREATE(CDSalesOrderAddOn)

public:	
	CDSalesOrderAddOn();

public:
	DSalesOrder* GetServerDoc() const
	{
		return (DSalesOrder*)m_pServerDocument;
	}

protected:
	virtual BOOL OnAttachData();
	virtual BOOL OnInitAuxData();
	virtual BOOL OnPrepareAuxData();

protected:
	// Helper methods for margin/commission calculation
	void CalculateMarginAndCommission(TSaleOrdDetails* pDetail);
	double GetCostByPolicy(const DataStr& sItem, const DataStr& sPriceList, const DataStr& sPolicy, double dQty);
	double GetBaseCommission(const DataStr& sSalesperson);
	double GetLastCostFromBalances(const DataStr& sItem);
	double GetPriceFromPriceList(const DataStr& sPriceList, const DataStr& sItem, double dQty);

protected:
	//{{AFX_MSG(CDSalesOrderAddOn)
	afx_msg void OnItemChanged();
	afx_msg void OnQuantityChanged();
	afx_msg void OnUnitValueChanged();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

#include "endh.dex"
