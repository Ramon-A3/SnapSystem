-- ============================================================================
-- Script: 02_Rollback_AlterItemsPriceLists.sql
-- Description: Remove MinimumCost column from MA_ItemsPriceLists
-- ============================================================================
 
IF EXISTS (
    SELECT 1 FROM sys.columns 
    WHERE object_id = object_id(N'[dbo].[MA_ItemsPriceLists]') 
      AND name = 'MinimumCost'
)
BEGIN
    ALTER TABLE [dbo].[MA_ItemsPriceLists]
        DROP CONSTRAINT [DF_MA_ItemsPriceLists_MinimumCost]

    ALTER TABLE [dbo].[MA_ItemsPriceLists]
        DROP COLUMN [MinimumCost]

    PRINT 'Column MinimumCost removed from MA_ItemsPriceLists successfully'
END
ELSE
    PRINT 'Column MinimumCost does not exist in MA_ItemsPriceLists'
GO
