-- ============================================================================
-- Script: 03_Rollback_AlterSalesPeople.sql
-- Description: Remove MaxDeviationPerc column from MA_SalesPeople
-- ============================================================================
 
IF EXISTS (
    SELECT 1 FROM sys.columns 
    WHERE object_id = object_id(N'[dbo].[MA_SalesPeople]') 
      AND name = 'MaxDeviationPerc'
)
BEGIN
    ALTER TABLE [dbo].[MA_SalesPeople]
        DROP CONSTRAINT [DF_MA_SalesPeople_MaxDeviationPerc]

    ALTER TABLE [dbo].[MA_SalesPeople]
        DROP COLUMN [MaxDeviationPerc]

    PRINT 'Column MaxDeviationPerc removed from MA_SalesPeople successfully'
END
ELSE
    PRINT 'Column MaxDeviationPerc does not exist in MA_SalesPeople'
GO
