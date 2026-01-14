-- ============================================================================
-- Script: 02_AlterItemsPriceLists.sql
-- Description: Add MinimumCost column to MA_ItemsPriceLists
-- MinimumCost = Price after applying Discount1 and Discount2
-- Type: numeric(19,6) - Consistent with Price field in MA_ItemsPriceLists
-- ============================================================================

if not exists (select dbo.syscolumns.name from dbo.syscolumns, dbo.sysobjects
where dbo.sysobjects.name = 'MA_ItemsPriceLists' and dbo.sysobjects.id = dbo.syscolumns.id and
dbo.syscolumns.name = 'MinimumCost')
ALTER TABLE [dbo].[MA_ItemsPriceLists]
    ADD [MinimumCost] [numeric](19,6) NULL CONSTRAINT [DF_MA_ItemsPriceLists_MinimumCost] DEFAULT ((0))
GO
