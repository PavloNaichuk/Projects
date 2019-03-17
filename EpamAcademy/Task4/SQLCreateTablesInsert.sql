create database [Task4]
go
use [Task4]
go
--СОЗДАНИЕ Таблиц
drop table if exists [dbo].[Position]
CREATE TABLE [dbo].[Position](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[position_name] [nvarchar](100) NULL,
 CONSTRAINT [PK_Position] PRIMARY KEY ([id])
 )
insert [Position] select 'noob'
insert [Position] select 'junior'
insert [Position] select 'middle'
insert [Position] select 'senior'
create index idx1 on [Position](id)
go

--проекты - ID, название, дата создания, статус (справочник), дата закрытия

drop table if exists [dbo].[Project]
CREATE TABLE [dbo].[Project](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[project_name] [nvarchar](max) NULL,
	[p_createdate] [date] NULL,
	[idProjectStatus] [int] NULL,
	[p_closedate] [date] NULL,
 CONSTRAINT [PK_Project]  PRIMARY KEY ([id])
 )
 go
 create index idx1 on [Project](id)
 go
/****** стутасы проекта     ******/

drop table if exists [dbo].[ProjectStatus]
CREATE TABLE [dbo].[ProjectStatus](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[status_name] [nvarchar](100) NULL,
 CONSTRAINT [PK_ProjectStatus] PRIMARY KEY ([id])
 )

insert [ProjectStatus] select 'Opened'
insert [ProjectStatus] select 'Closed'
create index idx1 on [ProjectStatus](id)



go
--задачи - ID, сотрудник (справочник), должность на проект (спр), проект (спр), статус задачи (спр), дедлайн

drop table if exists [dbo].[Task]
CREATE TABLE [dbo].[Task](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[id_Worker] [int] NULL,
	[id_Position] [int] NULL,
	[idProject] [int] NULL,
	[idTaskStatusLog] [int] NULL,
	[t_deathline_date] [date] NULL,
 CONSTRAINT [PK_Task]   PRIMARY KEY ([id])
 )
create index idx1 on [Task](id)
create index idx2 on [Task](id_worker)
create index idx3 on [Task](id_position)
create index idx4 on [Task](idProject)
create index idx5 on [Task](idTaskstatuslog)
GO

drop table if exists [dbo].[TaskStatus]
CREATE TABLE [dbo].[TaskStatus](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[status_name] [nvarchar](100) NULL,
 CONSTRAINT [PK_TaskStatus]  PRIMARY KEY ([id])
 )

insert [TaskStatus] select 'Opened'
insert [TaskStatus] select 'Completed'
insert [TaskStatus] select 'NeedsSomeWork'
insert [TaskStatus] select 'Closed'
create index idx1 on [TaskStatus](id)
GO

--статус задачи - ID, задача (спр), сотрудник+должность того кто проставил статус (спр), статус (спр), дата установки статуса

drop table if exists [dbo].[TaskStatusLog]
CREATE TABLE [dbo].[TaskStatusLog](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[id_Worker] [int] NULL,
	[idTaskStatus] [int] NULL,
	[SetTaskDate] [date] NULL
) ON [PRIMARY]

create index idx1 on [TaskStatusLog](id)
create index idx3 on [TaskStatusLog](id_worker)
create index idx4 on [TaskStatusLog](idTaskStatus)
create index idx5 on [TaskStatusLog](SetTaskDate)

go

drop table if exists [dbo].[Workers]
CREATE TABLE [dbo].[Workers](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[worker_lastName] [nvarchar](max) NULL,
 CONSTRAINT [PK_Workers] PRIMARY KEY ([id])
 )

create index idx1 on [Workers](id)
insert [Workers] select 'Naichuk'
insert [Workers] select 'Shtepa'
insert [Workers] select 'Ivachuk'
insert [Workers] select 'Palamarchuk'
insert [Workers] select 'Lupanov'
insert [Workers] select 'Chopa'
insert [Workers] select 'Pulavsky'
insert [Workers] select 'Kylybaba'
insert [Workers] select 'Sherba'
insert [Workers] select 'Boyko'
insert [Workers] select 'Tkachuk'

GO

ALTER TABLE [dbo].[Project]  WITH CHECK ADD  CONSTRAINT [FK_Project_ProjectStatus] FOREIGN KEY([idProjectStatus])
REFERENCES [dbo].[ProjectStatus] ([id])

ALTER TABLE [dbo].[Project] CHECK CONSTRAINT [FK_Project_ProjectStatus]

ALTER TABLE [dbo].[Task]  WITH CHECK ADD  CONSTRAINT [FK_Task_Position] FOREIGN KEY([id_Position])
REFERENCES [dbo].[Position] ([id])

ALTER TABLE [dbo].[Task] CHECK CONSTRAINT [FK_Task_Position]

ALTER TABLE [dbo].[Task]  WITH CHECK ADD  CONSTRAINT [FK_Task_Project] FOREIGN KEY([idProject])
REFERENCES [dbo].[Project] ([id])

ALTER TABLE [dbo].[Task] CHECK CONSTRAINT [FK_Task_Project]

ALTER TABLE [dbo].[Task]  WITH CHECK ADD  CONSTRAINT [FK_Task_Workers] FOREIGN KEY([id_Worker])
REFERENCES [dbo].[Workers] ([id])

ALTER TABLE [dbo].[Task] CHECK CONSTRAINT [FK_Task_Workers]

ALTER TABLE [dbo].[TaskStatusLog]  WITH CHECK ADD  CONSTRAINT [FK_TaskStatusLog_TaskStatus] FOREIGN KEY([idTaskStatus])
REFERENCES [dbo].[TaskStatus] ([id])

ALTER TABLE [dbo].[TaskStatusLog] CHECK CONSTRAINT [FK_TaskStatusLog_TaskStatus]

ALTER TABLE [dbo].[TaskStatusLog]  WITH CHECK ADD  CONSTRAINT [FK_TaskStatusLog_Workers] FOREIGN KEY([id_Worker])
REFERENCES [dbo].[Workers] ([id])

ALTER TABLE [dbo].[TaskStatusLog] CHECK CONSTRAINT [FK_TaskStatusLog_Workers]




--ЗАПОЛНЕНИЕ БД
insert into Project 
(project_name, p_createdate, idProjectStatus, p_closedate)
values 
('First project', getdate(), 1 , null),
('Second project', '20180101', 1, null),
('Third project', '20180920', 1, null),
('Fourth project', '20170101', 2, '20190101')

insert  into TaskStatusLog 
(id_Worker, idTaskStatus, SetTaskDate)
values
(1,1, '20190315'),
(1,1, '20180101'),
(1,1, '20180920'),
(1,1, '20170101'),
(1,2, '20180101'),
(1,3, '20180601'),
(1,4, '20190101')

insert into Task
(id_Worker, id_Position, idProject, idTaskStatusLog, t_deathline_date)
values
(1,2,1,1,'20190101'),
(1,3,2,2,'20190101'),
(1,4,3,3,'20190101'),
(2,3,1,1,'20190101'),
(2,4,3,3,'20190101'),
(3,2,2,2,'20190101'),
(3,4,3,3,'20190101'),
(4,2,1,1,'20190101'),
(5,3,2,2,'20190101'),
(6,4,3,3,'20190101'),
(7,2,4,7,'20190101'),
(8,3,1,1,'20190101'),
(9,4,2,2,'20190101'),
(10,2,3,3,'20190101'),
(1,2,1,1,'20190101')