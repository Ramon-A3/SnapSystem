-- LastCost
if not exists (select dbo.syscolumns.name from dbo.syscolumns, dbo.sysobjects
where dbo.sysobjects.name = 'MA_SaleOrdDetails' and dbo.sysobjects.id = dbo.syscolumns.id and
dbo.syscolumns.name = 'LastCost')
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [LastCost] [numeric](19,6) NULL CONSTRAINT [DF_MA_SaleOrdDetails_LastCost_00] DEFAULT ((0))
GO

-- Margin
if not exists (select dbo.syscolumns.name from dbo.syscolumns, dbo.sysobjects
where dbo.sysobjects.name = 'MA_SaleOrdDetails' and dbo.sysobjects.id = dbo.syscolumns.id and
dbo.syscolumns.name = 'Margin')
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [Margin] [numeric](19,6) NULL CONSTRAINT [DF_MA_SaleOrdDetails_Margin_00] DEFAULT ((0))
GO

-- SalesPersonMarginComm
if not exists (select dbo.syscolumns.name from dbo.syscolumns, dbo.sysobjects
where dbo.sysobjects.name = 'MA_SaleOrdDetails' and dbo.sysobjects.id = dbo.syscolumns.id and
dbo.syscolumns.name = 'SalesPersonMarginComm')
ALTER TABLE [dbo].[MA_SaleOrdDetails]
    ADD [SalesPersonMarginComm] [numeric](19,6) NULL CONSTRAINT [DF_MA_SaleOrdDetails_SalesPerson_00] DEFAULT ((0))
GO
