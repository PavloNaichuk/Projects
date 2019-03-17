create database [Task5]
go
use [Task5]
go
--СОЗДАНИЕ Таблиц
drop table if exists [dbo].[Category]
CREATE TABLE [dbo].[Category](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[category_name] [nvarchar](100) NULL,
 CONSTRAINT [PK_Category] PRIMARY KEY ([id])
 )
insert [Category] select 'ballі'
insert [Category]  select 'leggings'
insert [Category]  select 'shorts'
insert [Category]  select 'boots'
insert [Category]  select 't-shirts'
create index idx1 on [Category] (id)
go



drop table if exists [dbo].[Suppliers]
CREATE TABLE [dbo].[Suppliers](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[suppliers_name] [nvarchar](100) NULL,
 CONSTRAINT [PK_Suppliers]  PRIMARY KEY ([id])
 )

insert [Suppliers] select 'Addidas'
insert [Suppliers] select 'Nike'
insert [Suppliers] select 'FootballBalls'
insert [Suppliers] select 'Like'
create index idx1 on [Suppliers](id)
GO


drop table if exists [dbo].[Ware]
CREATE TABLE [dbo].[Ware](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[ware_name] [nvarchar](max) NULL,
 CONSTRAINT [PK_Ware] PRIMARY KEY ([id])
 )

create index idx1 on [Ware](id)
insert [Ware] select 'Ball_Addidas'
insert [Ware] select 'Ball_Hike'
insert [Ware] select 'T_short_Nike'
GO

--ЗАПОЛНЕНИЕ БД
insert into Project 
(project_name, p_createdate, idProjectStatus, p_closedate)
values 
('First project', getdate(), 1 , null),
('Second project', '20180101', 1, null),
('Third project', '20180920', 1, null),
('Fourth project', '20170101', 2, '20190101')

insert TaskStatusLog select 1,1, '20190315'
insert TaskStatusLog select 1,1, '20180101'
insert TaskStatusLog select 1,1, '20180920'
insert TaskStatusLog select 1,1, '20170101'
insert TaskStatusLog select 1,2, '20180101'
insert TaskStatusLog select 1,3, '20180601'
insert TaskStatusLog select 1,4, '20190101'

insert Task select 1,2,1,1,'20190101'
insert Task select 1,3,2,2,'20190101'
insert Task select 1,4,3,3,'20190101'

insert Task select 2,3,1,1,'20190101'
insert Task select 2,4,3,3,'20190101'

insert Task select 3,2,2,2,'20190101'
insert Task select 3,4,3,3,'20190101'

insert Task select 4,2,1,1,'20190101'
insert Task select 5,3,2,2,'20190101'
insert Task select 6,4,3,3,'20190101'
insert Task select 7,2,4,7,'20190101'
insert Task select 8,3,1,1,'20190101'
insert Task select 9,4,2,2,'20190101'
insert Task select 10,2,3,3,'20190101'
insert Task select 11,2,1,1,'20190101'