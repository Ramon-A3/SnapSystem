
#pragma once
#include "beginh.dex"

/////////////////////////////////////////////////////////////////////////////
class TB_EXPORT TSNAP_SaleOrdDetails : public SqlRecord
{
	DECLARE_DYNCREATE(TSNAP_SaleOrdDetails) 

public:
	DataStr		f_Code;
	DataStr		f_Description;
	
public:
	TSNAP_SaleOrdDetails(BOOL bCallInit = TRUE);

public:
    virtual void	BindRecord();
		
public:
	static  LPCTSTR  GetStaticName();
};


#include "endh.dex"