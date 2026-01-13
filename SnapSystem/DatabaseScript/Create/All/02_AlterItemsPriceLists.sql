-- ============================================================================
-- Script: 02_AlterItemsPriceLists.sql
-- Description: Add MinimumCost column to MA_ItemsPriceLists
-- MinimumCost = Price after applying Discount1 and Discount2
-- Type: numeric(19,6) - Consistent with Price field in MA_ItemsPriceLists
-- ============================================================================

IF NOT EXISTS (
    SELECT 1 FROM sys.columns 
    WHERE object_id = object_id(N'[dbo].[MA_ItemsPriceLists]') 
      AND name = 'MinimumCost'
)
BEGIN
    ALTER TABLE [dbo].[MA_ItemsPriceLists]
        ADD [MinimumCost] [numeric](19,6) NULL
        CONSTRAINT [DF_MA_ItemsPriceLists_MinimumCost] DEFAULT ((0))

    PRINT 'Column MinimumCost added to MA_ItemsPriceLists successfully'
END
ELSE
    PRINT 'Column MinimumCost already exists in MA_ItemsPriceLists'
GO
