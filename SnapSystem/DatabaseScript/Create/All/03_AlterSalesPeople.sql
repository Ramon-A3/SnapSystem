-- ============================================================================
-- Script: 03_AlterSalesPeople.sql
-- Description: Add MaxDeviationPerc column to MA_SalesPeople
-- MaxDeviationPerc = Percentuale Massima Scostamento (default 40%)
-- ============================================================================

IF NOT EXISTS (
    SELECT * FROM syscolumns 
    WHERE id = object_id(N'[dbo].[MA_SalesPeople]') 
      AND name = 'MaxDeviationPerc'
)
BEGIN
    ALTER TABLE [dbo].[MA_SalesPeople]
        ADD [MaxDeviationPerc] [numeric](19,6) NULL
        CONSTRAINT [DF_MA_SalesPeople_MaxDeviationPerc] DEFAULT ((40))

    PRINT 'Column MaxDeviationPerc added to MA_SalesPeople successfully'
END
ELSE
    PRINT 'Column MaxDeviationPerc already exists in MA_SalesPeople'
GO
