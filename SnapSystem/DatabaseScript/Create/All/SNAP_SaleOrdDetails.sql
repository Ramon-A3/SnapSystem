if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[SNAP_SaleOrdDetails]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)
 BEGIN
CREATE TABLE [dbo].[SNAP_SaleOrdDetails] (
    [Code] [varchar] (10) NOT NULL,
    [Description] [varchar] (128) NULL CONSTRAINT DF_SNAP_SaleOrdDetails_Descriptio_00 DEFAULT ('')
   CONSTRAINT [PK_SNAP_SaleOrdDetails] PRIMARY KEY NONCLUSTERED 
    (
            [Code]
        ) ON [PRIMARY]
) ON [PRIMARY]

END
GO
