//=============================================================================
// Module name  : SnapSystemDocumentsInterface.cpp
//=============================================================================

#include "stdafx.h"
#include "CDSalesOrderAddOn.h"
#include "CDItemsPriceListsAddOn.h"
#include "CDSalesPeopleAddOn.h"

#include <Items\Documents\DItems.h>
#include <SalesPeople\Documents\DSalesPeople.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _AddOn_Interface_Of snapsystemdocuments

/////////////////////////////////////////////////////////////////////////////
//					Add-On Interface Definition
/////////////////////////////////////////////////////////////////////////////
//

//-----------------------------------------------------------------------------
BEGIN_ADDON_INTERFACE()
	DATABASE_RELEASE(3)

	//-----------------------------------------------------------------------------
	BEGIN_TABLES()
		BEGIN_REGISTER_TABLES	()
		END_REGISTER_TABLES		()
	END_TABLES()

	//-----------------------------------------------------------------------------
	BEGIN_FUNCTIONS()
	END_FUNCTIONS()

	//-----------------------------------------------------------------------------
	BEGIN_HOTLINK()
	END_HOTLINK ()

	//-----------------------------------------------------------------------------
	BEGIN_TEMPLATE()
	END_TEMPLATE()

	//-----------------------------------------------------------------------------
	BEGIN_CLIENT_DOC()
		WHEN_SERVER_DOC(DSalesOrder)
			ATTACH_CLIENT_DOC(CDSalesOrderAddOn, _NS_CD("SalesOrderAddOn"))
		END_SERVER_DOC()

		WHEN_SERVER_DOC(DSalesPeople)
			ATTACH_CLIENT_DOC(CDSalesPeopleAddOn, _NS_CD("SalesPeopleAddOn"))
		END_SERVER_DOC()
	END_CLIENT_DOC()

	//-----------------------------------------------------------------------------
	// Register CDItemsPriceListsAddOn for all DItems family documents
	// This enables MinimumCost calculation on the Items form Price Lists grid
	//-----------------------------------------------------------------------------
	BEGIN_FAMILY_CLIENT_DOC()
		WHEN_FAMILY_SERVER_DOC(DItems)
			ATTACH_FAMILY_CLIENT_DOC(CDItemsPriceListsAddOn, _NS_CD("ItemsPriceListsAddOn"))
		END_FAMILY_SERVER_DOC()
	END_FAMILY_CLIENT_DOC()

END_ADDON_INTERFACE()

#undef _AddOn_Interface_Of