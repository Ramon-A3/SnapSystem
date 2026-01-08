IF NOT EXISTS (
    SELECT * FROM syscolumns 
    WHERE id = object_id(N'[dbo].[MA_SaleOrdDetails]') 
      AND name = 'LastCost'
)
BEGIN
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [LastCost] [numeric](19,6) NULL
    CONSTRAINT [DF_MA_SaleOrdDetails_LastCost_00] DEFAULT ((0))

    PRINT 'Column LastCost added successfully'
END
ELSE
    PRINT 'Column LastCost already exists'
GO

-- Margin
IF NOT EXISTS (
    SELECT * FROM syscolumns 
    WHERE id = object_id(N'[dbo].[MA_SaleOrdDetails]') 
      AND name = 'Margin'
)
BEGIN
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [Margin] [numeric](19,6) NULL
    CONSTRAINT [DF_MA_SaleOrdDetails_Margin_00] DEFAULT ((0))

    PRINT 'Column Margin added successfully'
END
ELSE
    PRINT 'Column Margin already exists'
GO

-- SalesPersonMarginComm
IF NOT EXISTS (
    SELECT * FROM syscolumns 
    WHERE id = object_id(N'[dbo].[MA_SaleOrdDetails]') 
      AND name = 'SalesPersonMarginComm'
)
BEGIN
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [SalesPersonMarginComm] [numeric](19,6) NULL
    CONSTRAINT [DF_MA_SaleOrdDetails_SalesPerson_00] DEFAULT ((0))

    PRINT 'Column SalesPersonMarginComm added successfully'
END
ELSE
    PRINT 'Column SalesPersonMarginComm already exists'
GO
