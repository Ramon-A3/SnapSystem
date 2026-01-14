#include "stdafx.h"

// ERP includes
#include <SaleOrders\Documents\DBTSalesOrder.h>
#include <SaleOrders\Documents\UISalesOrder.hjson>
#include <SaleOrders\Dbl\TSalesOrder.h>
#include <SaleOrders\ModuleObjects\SaleOrd\JsonForms\IDD_TD_SALESORDER_MAIN_DATA.hjson>
#include <Inventory\Dbl\TItemsBalances.h>
#include <SalesPeople\Dbl\TSalesPeople.h>
#include <PricePolicies\Dbl\TItemPricesPolicies.h>
#include <SalesPeople\JsonForms\UISalesPeopleDialog\IDD_TD_SALESPEOPLE.hjson>
#include <SalesPeople\Components\SalesPeople.h>

// Local includes
#include "CDSalesOrderAddOn.h"
#include <SnapSystem\SnapSystemDbl\TSalesOrdDetailsAddOn.h>
#include <SnapSystem\SnapSystemDbl\TItemsPriceListsAddOn.h>
#include <SnapSystem\SnapSystemDbl\TSalesPeopleAddOn.h>
#include <SnapSystem\ModuleObjects\CDSalesOrderAddOn\JsonForms\IDD_TD_SALE_ORD_ADDON.hjson>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Parameter names for queries
static TCHAR szP1[] = _T("P1");
static TCHAR szP2[] = _T("P2");
static TCHAR szP3[] = _T("P3");

// Hardcoded PriceList filter value
static TCHAR szPriceList10[] = _T("10");

// Policy constants
static TCHAR szPolicy01[] = _T("01");  // LastCost from MA_ItemsBalances
static TCHAR szPolicy02[] = _T("02");  // Price from MA_ItemsPriceLists

//////////////////////////////////////////////////////////////////////////////
//             TRItemsBalancesLocal - TableReader for MA_ItemsBalances
//             Fetches LastCost for an item
//////////////////////////////////////////////////////////////////////////////
class TRItemsBalancesLocal : public TableReader
{
	DECLARE_DYNAMIC(TRItemsBalancesLocal)

public:
	DataStr m_sItem;

	TRItemsBalancesLocal(CAbstractFormDoc* pDocument = NULL)
		: TableReader(RUNTIME_CLASS(TItemsBalances), pDocument)
	{
	}

	TItemsBalances* GetRecord() const
	{
		return (TItemsBalances*)m_pRecord;
	}

	TableReader::FindResult FindRecord(const DataStr& sItem)
	{
		m_sItem = sItem;
		return TableReader::FindRecord();
	}

protected:
	virtual void OnDefineQuery()
	{
		m_pTable->SelectAll();
		// Filter by Item (primary filter)
		m_pTable->AddFilterColumn(GetRecord()->f_Item);
		m_pTable->AddParam(szP1, GetRecord()->f_Item);
	}

	virtual void OnPrepareQuery()
	{
		m_pTable->SetParamValue(szP1, m_sItem);
	}

	virtual BOOL IsEmptyQuery()
	{
		return m_sItem.IsEmpty();
	}
};

IMPLEMENT_DYNAMIC(TRItemsBalancesLocal, TableReader)

//////////////////////////////////////////////////////////////////////////////
//             TRSalesPeopleLocal - TableReader for MA_SalesPeople
//             Fetches BaseCommission for a salesperson
//////////////////////////////////////////////////////////////////////////////
class TRSalesPeopleLocal : public TableReader
{
	DECLARE_DYNAMIC(TRSalesPeopleLocal)

public:
	DataStr m_sSalesperson;

	TRSalesPeopleLocal(CAbstractFormDoc* pDocument = NULL)
		: TableReader(RUNTIME_CLASS(TSalesPeople), pDocument)
	{
	}

	TSalesPeople* GetRecord() const
	{
		return (TSalesPeople*)m_pRecord;
	}

	TableReader::FindResult FindRecord(const DataStr& sSalesperson)
	{
		m_sSalesperson = sSalesperson;
		return TableReader::FindRecord();
	}

protected:
	virtual void OnDefineQuery()
	{
		m_pTable->SelectAll();
		m_pTable->AddFilterColumn(GetRecord()->f_Salesperson);
		m_pTable->AddParam(szP1, GetRecord()->f_Salesperson);
	}

	virtual void OnPrepareQuery()
	{
		m_pTable->SetParamValue(szP1, m_sSalesperson);
	}

	virtual BOOL IsEmptyQuery()
	{
		return m_sSalesperson.IsEmpty();
	}
};

IMPLEMENT_DYNAMIC(TRSalesPeopleLocal, TableReader)

//////////////////////////////////////////////////////////////////////////////
//             RRItemsPriceListsByItem - RowsetReader for MA_ItemsPriceLists
//             Selects most recent valid price based on:
//               - ValidityStart <= PriceListValidityDate (ordered DESC for most recent)
//               - ValidityEnd >= PriceListValidityDate OR ValidityEnd is empty/null
//               - Quantity >= OrderQuantity
//////////////////////////////////////////////////////////////////////////////
class RRItemsPriceListsByItem : public RowsetReader
{
	DECLARE_DYNAMIC(RRItemsPriceListsByItem)

private:
	DataStr		m_Item;
	DataDate	m_Date;
	DataQty		m_Qty;

public:
	RRItemsPriceListsByItem(CAbstractFormDoc* pDocument = NULL)
		: RowsetReader(RUNTIME_CLASS(TItemsPriceLists), pDocument)
	{
	}

	TItemsPriceLists* GetRecord() const
	{
		return (TItemsPriceLists*)m_pRecord;
	}

	RowsetReader::FindResult FindRecord(const DataStr& sItem, const DataDate& aDate, const DataQty& aQty)
	{
		m_Item = sItem;
		m_Date = aDate;
		m_Qty = aQty;

		SetForceQuery();
		RowsetReader::FindResult aResult = RowsetReader::FindRecord();
		SetForceQuery(FALSE);

		if (IsEmptyQuery())
		{
			if (m_pTable)
				m_pTable->Disconnect();
			return aResult;
		}
		
		// Query is ordered by StartingValidityDate DESC, Quantity ASC
		while (!m_pTable->IsEOF())
		{
			// Check validity period: EndDate empty OR EndDate >= PriceListValidityDate
			if (GetRecord()->f_ValidityEndingDate.IsEmpty() ||
				m_Date <= GetRecord()->f_ValidityEndingDate)
			{
				// Quantity loop: find first record where PriceList.Qty >= Order.Qty
				while (!m_pTable->IsEOF())
				{
					if (GetRecord()->f_Quantity < m_Qty)
					{
						m_pTable->MoveNext();
						if (m_pTable->IsEOF())
							return RowsetReader::NOT_FOUND;
						continue;
					}
					else
					{
						if (m_pTable)
							m_pTable->Disconnect();
						return RowsetReader::FOUND;
					}
				}
			}
			else
			{
				m_pTable->MoveNext();
			}
		}

		return RowsetReader::NOT_FOUND;
	}

protected:
	virtual void OnDefineQuery()
	{
		m_pTable->SelectAll();

		// Filter by Item
		m_pTable->AddFilterColumn(GetRecord()->f_Item);
		m_pTable->AddParam(szP1, GetRecord()->f_Item);

		// Filter by date range: StartingValidityDate <= PriceListValidityDate
		DataDate ToDateEmpty(DataDate::NULLDATE);
		m_pTable->AddFilterColumn(GetRecord()->f_StartingValidityDate, _T("<="));
		m_pTable->AddParam(szP2, GetRecord()->f_StartingValidityDate);

		// Filter by PriceList ("10")
		m_pTable->AddFilterColumn(GetRecord()->f_PriceList);
		m_pTable->AddParam(szP3, GetRecord()->f_PriceList);

		// ValidityEndingDate filter handled in FindRecord loop

		// Sort by StartingValidityDate DESC (most recent first), then Quantity ASC
		m_pTable->AddSortColumn(GetRecord()->f_StartingValidityDate, TRUE); // descending
		m_pTable->AddSortColumn(GetRecord()->f_Quantity); // ascending
	}

	virtual void OnPrepareQuery()
	{
		m_pTable->SetParamValue(szP1, m_Item);
		m_pTable->SetParamValue(szP2, m_Date);
		m_pTable->SetParamValue(szP3, DataStr(szPriceList10));
	}

	virtual BOOL IsEmptyQuery()
	{
		return m_Item.IsEmpty();
	}
};

IMPLEMENT_DYNAMIC(RRItemsPriceListsByItem, RowsetReader)

//////////////////////////////////////////////////////////////////////////////
//             CSalesOrderAddOnEventMng Implementation
//             Intercepts events from SalesAgent
//////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSalesOrderAddOnEventMng, CEventManager)

//-----------------------------------------------------------------------------
BEGIN_TB_EVENT_MAP_EX(CSalesOrderAddOnEventMng, CEventManager)
	TB_EVENT(CSalesOrderAddOnEventMng, OnCommissionPolicySalespersonChanged)
END_TB_EVENT_MAP

//-----------------------------------------------------------------------------
// Called when SalesAgent fires OnCommissionPolicySalespersonChanged event
// At this point, the SalesAgent has already updated the header record with
// the new Policy value, so we can safely recalculate all details.
//-----------------------------------------------------------------------------
void CSalesOrderAddOnEventMng::OnCommissionPolicySalespersonChanged()
{
	if (m_pOwner)
	{
		m_pOwner->RecalculateAllDetails();
	}
}

//////////////////////////////////////////////////////////////////////////////
//             CDSalesOrderAddOn Implementation
//////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDSalesOrderAddOn, CClientDoc)

//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CDSalesOrderAddOn, CClientDoc)
	//{{AFX_MSG_MAP(CDSalesOrderAddOn)
	ON_EN_VALUE_CHANGED(IDC_SALESORDER_DETAIL_ITEM, OnItemChanged)
	ON_EN_VALUE_CHANGED(IDC_SALESORDER_DETAIL_QTY, OnQuantityChanged)
	ON_EN_VALUE_CHANGED(IDC_SALESORDER_DETAIL_VAL_UNIT, OnUnitValueChanged)
	ON_EN_VALUE_CHANGED(IDC_SALESORDER_ADT_DATE_VALIDITY_PRICELIST, OnPriceListValidityDateChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDSalesOrderAddOn::CDSalesOrderAddOn()
	:
	CClientDoc()
{
	SetMsgRoutingMode(CD_MSG_AFTER);
}

//-----------------------------------------------------------------------------
BOOL CDSalesOrderAddOn::OnAttachData()
{
	// Attach our event manager to intercept SalesAgent events
	
	Attach(new CSalesOrderAddOnEventMng(this));
	
	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CDSalesOrderAddOn::OnInitAuxData()
{
	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CDSalesOrderAddOn::OnPrepareAuxData()
{
	// This overload is called during NEW/EDIT/BROWSE mode transitions
	// Recalculate all details to ensure consistency
	RecalculateAllDetails();

	return TRUE;
}

//-----------------------------------------------------------------------------
// OnPrepareAuxData(UINT nID) - Called when a specific TileDialog is prepared
// 
// When the user changes Policy or Salesperson in the SalesPeople tile,
// 
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::OnPrepareAuxData(UINT nID)
{
	
	__super::OnPrepareAuxData(nID);

	
	if (nID == IDD_TD_SALESPEOPLE)
	{
		// Recalculate all detail lines with the new Policy/Salesperson values
		
		RecalculateAllDetails();
	}
}

//-----------------------------------------------------------------------------
// GetLastCostFromBalances - Fetches LastCost from MA_ItemsBalances
// Uses custom TableReader to query MA_ItemsBalances.f_LastCost
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetLastCostFromBalances(const DataStr& sItem)
{
	double dCost = 0.0;

	if (sItem.IsEmpty())
		return dCost;

	// Use custom TableReader to get LastCost from MA_ItemsBalances
	TRItemsBalancesLocal aTRItemsBalances(GetServerDoc());
	if (aTRItemsBalances.FindRecord(sItem) == TableReader::FOUND)
	{
		// Get LastCost from MA_ItemsBalances record (DataMon type)
		dCost = (double)aTRItemsBalances.GetRecord()->f_LastCost;
	}

	return dCost;
}

//-----------------------------------------------------------------------------
// GetPriceListValidityDate - Gets PriceListValidityDate from SalesOrder header
// Returns the date to use for price list validation
// If the field is empty, falls back to AfxGetApplicationDate()
//-----------------------------------------------------------------------------
DataDate CDSalesOrderAddOn::GetPriceListValidityDate()
{
	DSalesOrder* pServerDoc = GetServerDoc();
	if (pServerDoc)
	{
		TSalesOrder* pHeader = pServerDoc->GetHeader();
		if (pHeader && !pHeader->f_PriceListValidityDate.IsEmpty())
		{
			return pHeader->f_PriceListValidityDate;
		}
	}
	// Fallback to application date if header or field is not available
	return AfxGetApplicationDate();
}

//-----------------------------------------------------------------------------
// GetPriceFromPriceList - Fetches Price from MA_ItemsPriceLists
// Uses custom RRItemsPriceListsByItem to query price by Item only
// Automatically selects most recent valid price based on date and quantity
// Uses PriceListValidityDate from header for date validation
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetPriceFromPriceList(const DataStr& sPriceList, const DataStr& sItem, double dQty)
{
	double dPrice = 0.0;

	// Only Item is required - PriceList parameter is ignored in Policy 02
	if (sItem.IsEmpty())
		return dPrice;

	// Get validity date from header (PriceListValidityDate)
	DataDate aValidityDate = GetPriceListValidityDate();

	// Use our custom RowsetReader that searches by Item only (no PriceList filter)
	RRItemsPriceListsByItem aRRPriceLists(GetServerDoc());
	if (aRRPriceLists.FindRecord(sItem, aValidityDate, (DataQty)dQty) == RowsetReader::FOUND)
	{
		// Get Price from MA_ItemsPriceLists record (DataMon type)
		dPrice = (double)aRRPriceLists.GetRecord()->f_Price;
	}

	return dPrice;
}

//-----------------------------------------------------------------------------
// GetCostByPolicy - Fetches cost based on SalespersonPolicy
// Policy 01: LastCost from MA_ItemsBalances
// Policy 02: MinimumCost from MA_ItemsPriceLists
// Empty/Unknown: returns 0
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetCostByPolicy(const DataStr& sItem, const DataStr& sPriceList, const DataStr& sPolicy, double dQty)
{
	double dCost = 0.0;

	if (sItem.IsEmpty())
		return dCost;

	// Policy 01: Get LastCost from MA_ItemsBalances
	if (sPolicy == szPolicy01)
	{
		dCost = GetLastCostFromBalances(sItem);
	}
	// Policy 02: Get MinimumCost from MA_ItemsPriceLists 
	else if (sPolicy == szPolicy02)
	{
		dCost = GetMinimumCostFromPriceList(sItem, dQty);
	}
	// No policy or unknown policy: return 0 (dCost already initialized to 0)

	return dCost;
}

//-----------------------------------------------------------------------------
// GetBaseCommission - Fetches BaseCommission from MA_SalesPeople
// Uses custom TableReader to query MA_SalesPeople.f_BaseCommission
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetBaseCommission(const DataStr& sSalesperson)
{
	double dCommission = 0.0;

	if (sSalesperson.IsEmpty())
		return dCommission;

	// Use custom TableReader to get BaseCommission from MA_SalesPeople
	TRSalesPeopleLocal aTRSalesPeople(GetServerDoc());
	if (aTRSalesPeople.FindRecord(sSalesperson) == TableReader::FOUND)
	{
		// Get BaseCommission from MA_SalesPeople record (DataPerc type)
		dCommission = (double)aTRSalesPeople.GetRecord()->f_BaseCommission;
	}

	return dCommission;
}

//-----------------------------------------------------------------------------
// GetMinimumCostFromPriceList - Fetches MinimumCost from MA_ItemsPriceLists
// Uses PriceListValidityDate from header for date validation
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetMinimumCostFromPriceList(const DataStr& sItem, double dQty)
{
    double dMinimumCost = 0.0;

    if (sItem.IsEmpty())
        return dMinimumCost;

    // Get validity date from header (PriceListValidityDate)
    DataDate aValidityDate = GetPriceListValidityDate();

    // Use existing RRItemsPriceListsByItem to find the record
    RRItemsPriceListsByItem aRRPriceLists(GetServerDoc());
    if (aRRPriceLists.FindRecord(sItem, aValidityDate, (DataQty)dQty) == RowsetReader::FOUND)
    {
        // Get AddOn fields from the record
        TItemsPriceListsAddOn* pAddOn = (TItemsPriceListsAddOn*)aRRPriceLists.GetRecord()->GetAddOnFields(
            RUNTIME_CLASS(TItemsPriceListsAddOn));
        if (pAddOn)
        {
            dMinimumCost = (double)pAddOn->f_MinimumCost;
        }
    }

    return dMinimumCost;
}

//-----------------------------------------------------------------------------
// GetMaxDeviationPerc - Fetches MaxDeviationPerc from MA_SalesPeople
//-----------------------------------------------------------------------------
double CDSalesOrderAddOn::GetMaxDeviationPerc(const DataStr& sSalesperson)
{
    double dMaxDeviationPerc = 40.0;  // Default value

    if (sSalesperson.IsEmpty())
        return dMaxDeviationPerc;

    // Use existing TRSalesPeopleLocal to find the record
    TRSalesPeopleLocal aTRSalesPeople(GetServerDoc());
    if (aTRSalesPeople.FindRecord(sSalesperson) == TableReader::FOUND)
    {
        // Get AddOn fields from the record
        TSalesPeopleAddOn* pAddOn = (TSalesPeopleAddOn*)aTRSalesPeople.GetRecord()->GetAddOnFields(
            RUNTIME_CLASS(TSalesPeopleAddOn));
        if (pAddOn)
        {
            dMaxDeviationPerc = (double)pAddOn->f_MaxDeviationPerc;
        }
    }

    return dMaxDeviationPerc;
}

//-----------------------------------------------------------------------------
// CalculateMarginAndCommission - Core calculation logic
// Margin = (UnitValue - LastCost) × Quantity
// SalesPersonMarginComm = Margin × (BaseCommission / 100)
//
//
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::CalculateMarginAndCommission(TSaleOrdDetails* pDetail)
{
	if (!pDetail)
		return;

	// Get AddOn fields for this detail row (our custom fields)
	TSalesOrdDetailsAddOn* pAddOn = (TSalesOrdDetailsAddOn*)pDetail->GetAddOnFields(RUNTIME_CLASS(TSalesOrdDetailsAddOn));
	if (!pAddOn)
		return;

	// Get server document
	DSalesOrder* pServerDoc = GetServerDoc();
	if (!pServerDoc)
		return;

	// Get header record using GetHeader() method
	TSalesOrder* pHeader = pServerDoc->GetHeader();
	if (!pHeader)
	{
		// No header, zero everything
		pAddOn->f_LastCost = 0.0;
		pAddOn->f_Margin = 0.0;
		pAddOn->f_SalesPersonMarginComm = 0.0;
		return;
	}

	// Get values from detail (TSaleOrdDetails fields)
	DataStr sItem = pDetail->f_Item;
	double dQty = (double)pDetail->f_Quantity;
	double dUnitValue = (double)pDetail->f_UnitValue;

	// Get header values directly from the record

	DataStr sSalesperson = pHeader->f_Salesperson;
	DataStr sSalespersonPolicy = pHeader->f_SalespersonPolicy;
	DataStr sPriceList = pHeader->f_PriceList;

	// Validate required fields
	if (sItem.IsEmpty())
	{
		pAddOn->f_LastCost = 0.0;
		pAddOn->f_Margin = 0.0;
		pAddOn->f_SalesPersonMarginComm = 0.0;
		return;
	}

	// Get cost based on policy (from MA_ItemsBalances.f_LastCost or MA_ItemsPriceLists.f_Price)
	double dCost = GetCostByPolicy(sItem, sPriceList, sSalespersonPolicy, dQty);
	
	// Calculate Margin = (UnitValue - Cost) × Quantity
	double dMargin = (dUnitValue - dCost) * dQty;
	
	// Calculate Commission based on Policy
	double dCommission = 0.0;
	
	if (sSalespersonPolicy == szPolicy02)
	{
		// Policy 02: New formula based on PriceList
		// Commission = (PriceListPrice - MinimumCost) + (Difference × MaxDeviationPerc / 100)
		
		double dPriceListPrice = GetPriceFromPriceList(sPriceList, sItem, dQty);
		double dMinimumCost = GetMinimumCostFromPriceList(sItem, dQty);
		double dMaxDeviationPerc = GetMaxDeviationPerc(sSalesperson);
		
		double dDifference = dUnitValue - dPriceListPrice;
		// Policy 02 commission is calculated per unit, then scaled by quantity
		dCommission = ((dPriceListPrice - dMinimumCost) + (dDifference * dMaxDeviationPerc / 100.0)) * dQty;
	}
	else
	{
		// Policy 01 and others: Original formula
		// Commission = Margin × (BaseCommission / 100)
		
		double dBaseCommission = GetBaseCommission(sSalesperson);
		dCommission = dMargin * (dBaseCommission / 100.0);
	}

	// Update our AddOn fields
	pAddOn->f_LastCost = dCost;
	pAddOn->f_Margin = dMargin;
	pAddOn->f_SalesPersonMarginComm = dCommission;

	// Also populate ERP standard commission amount field
	pDetail->f_SalespersonCommAmt = dCommission;
	
	pDetail->f_SalespersonCommAuto = FALSE; 
	pDetail->f_SalespersonCommCtgAuto = FALSE;
}

//-----------------------------------------------------------------------------
// OnItemChanged - Triggered when Item field changes in detail grid
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::OnItemChanged()
{
	DSalesOrder* pServerDoc = GetServerDoc();
	if (!pServerDoc)
		return;

	// Use GetCurrentRow() to get current detail line
	TSaleOrdDetails* pDetail = pServerDoc->GetCurrentRow();
	if (!pDetail)
		return;

	CalculateMarginAndCommission(pDetail);
}

//-----------------------------------------------------------------------------
// OnQuantityChanged - Triggered when Quantity field changes in detail grid
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::OnQuantityChanged()
{
	DSalesOrder* pServerDoc = GetServerDoc();
	if (!pServerDoc)
		return;

	TSaleOrdDetails* pDetail = pServerDoc->GetCurrentRow();
	if (!pDetail)
		return;

	CalculateMarginAndCommission(pDetail);
}

//-----------------------------------------------------------------------------
// OnUnitValueChanged - Triggered when UnitValue field changes in detail grid
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::OnUnitValueChanged()
{
	DSalesOrder* pServerDoc = GetServerDoc();
	if (!pServerDoc)
		return;

	TSaleOrdDetails* pDetail = pServerDoc->GetCurrentRow();
	if (!pDetail)
		return;

	CalculateMarginAndCommission(pDetail);
}

//-----------------------------------------------------------------------------
// RecalculateAllDetails - Iterates all detail lines and recalculates margins
// Called when CSalesOrderAddOnEventMng intercepts OnCommissionPolicySalespersonChanged
// from SalesAgent. At this point, the header record already has the new Policy value.
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::RecalculateAllDetails()
{
	DSalesOrder* pServerDoc = GetServerDoc();
	if (!pServerDoc)
		return;

	// Get DBT for detail lines using GetDBTDetail()
	DBTSlaveBuffered* pDBTDetails = pServerDoc->GetDBTDetail();
	if (!pDBTDetails)
		return;

	// Iterate through all rows and recalculate each one
	for (int i = 0; i < pDBTDetails->GetSize(); i++)
	{
		TSaleOrdDetails* pDetail = (TSaleOrdDetails*)pDBTDetails->GetRow(i);
		if (pDetail)
		{
			CalculateMarginAndCommission(pDetail);
		}
	}
}

//-----------------------------------------------------------------------------
// OnPriceListValidityDateChanged - Triggered when PriceListValidityDate changes in header
// Recalculates all detail lines with the new validity date
//-----------------------------------------------------------------------------
void CDSalesOrderAddOn::OnPriceListValidityDateChanged()
{
	// Recalculate all details with the new PriceListValidityDate
	RecalculateAllDetails();
}


