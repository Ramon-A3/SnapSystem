-- ============================================================================
-- Script: 03_AlterSalesPeople.sql
-- Description: Add MaxDeviationPerc column to MA_SalesPeople
-- MaxDeviationPerc = Percentuale Massima Scostamento (default 40%)
-- Type: numeric(19,6) - Consistent with percentage fields in MA_SalesPeople
-- ============================================================================

if not exists (select dbo.syscolumns.name from dbo.syscolumns, dbo.sysobjects
where dbo.sysobjects.name = 'MA_SalesPeople' and dbo.sysobjects.id = dbo.syscolumns.id and
dbo.syscolumns.name = 'MaxDeviationPerc')
BEGIN
ALTER TABLE [dbo].[MA_SalesPeople]
    ADD [MaxDeviationPerc] [numeric](19,6) NULL CONSTRAINT [DF_MA_SalesPeople_MaxDeviationPerc] DEFAULT ((40))

UPDATE [dbo].[MA_SalesPeople] SET [MaxDeviationPerc] = 40 WHERE [MaxDeviationPerc] IS NULL
END
GO
