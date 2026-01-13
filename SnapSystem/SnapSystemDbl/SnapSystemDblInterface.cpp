#include "stdafx.h"
#include "TSalesOrdDetailsAddOn.h"
#include "TItemsPriceListsAddOn.h"
#include "TSalesPeopleAddOn.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _AddOn_Interface_Of snapsystemdbl

/////////////////////////////////////////////////////////////////////////////
//					Add-On Interface Definition
/////////////////////////////////////////////////////////////////////////////
//

//-----------------------------------------------------------------------------
BEGIN_ADDON_INTERFACE()
	DATABASE_RELEASE(3)

	//-----------------------------------------------------------------------------
	BEGIN_TABLES()
		BEGIN_REGISTER_TABLES()
		END_REGISTER_TABLES()
	END_TABLES()

	//-----------------------------------------------------------------------------
	BEGIN_ADDON_NEW_COLUMNS()

		WHEN_TABLE(TSaleOrdDetails)
			ADDON_COLUMNS_CLASS(TSalesOrdDetailsAddOn)
		END_TABLE

		WHEN_TABLE(TItemsPriceLists)
			ADDON_COLUMNS_CLASS(TItemsPriceListsAddOn)
		END_TABLE

		WHEN_TABLE(TSalesPeople)
			ADDON_COLUMNS_CLASS(TSalesPeopleAddOn)
		END_TABLE

	END_ADDON_NEW_COLUMNS()

	//-----------------------------------------------------------------------------
	BEGIN_FUNCTIONS()
	END_FUNCTIONS()

	//-----------------------------------------------------------------------------
	BEGIN_HOTLINK()
	END_HOTLINK()

	//-----------------------------------------------------------------------------
	BEGIN_TEMPLATE()
	END_TEMPLATE()

	//-----------------------------------------------------------------------------
	BEGIN_CLIENT_DOC()
	END_CLIENT_DOC()

END_ADDON_INTERFACE()

#undef _AddOn_Interface_Of

