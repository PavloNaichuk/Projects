USE [TicketSalePoint]
GO

CREATE TABLE [dbo].[Users](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[Login] [varchar](50) NOT NULL,
	[Password] [varchar](50) NOT NULL,
	[FirstName] [nchar](50) NOT NULL,
	[LastName] [nchar](50) NOT NULL,
	[Email] [varchar](50) NOT NULL,
	[RegistrationDate] [datetime] NOT NULL,
)
GO

CREATE TABLE [dbo].[Theatres](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[Login] [varchar](50) NOT NULL,
	[Password] [varchar](50) NOT NULL,
	[Name] [nchar](50) NOT NULL,
	[Address] [nchar](50) NOT NULL,
	[Email] [varchar](50) NOT NULL,
	[RegistrationDate] [datetime] NOT NULL,
)
GO

CREATE TABLE [dbo].[Shows](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[TheatreId] [int] NOT NULL,
    [Name] [nvarchar](50) NOT NULL,
	[Date] [datetime] NOT NULL,
	CONSTRAINT FK_Theatres FOREIGN KEY (TheatreId)
	REFERENCES [Theatres](Id),
)
GO

CREATE TABLE [dbo].[Tickets](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
    [ShowId] [int] NOT NULL,
    CONSTRAINT FK_Shows FOREIGN KEY (ShowId)
    REFERENCES [Shows](Id),
	[Row] [int] NOT NULL,
	[Seat] [int] NOT NULL,
	[PriceInCents] [int] NOT NULL,
	[Status] [int] DEFAULT 1,
	[ReservationDate] [datetime] NOT NULL,
)
GO

ALTER TABLE [dbo].[Users] 
ADD  CONSTRAINT [DF_Users_RegistrationDate]  DEFAULT (getdate()) FOR [RegistrationDate]
GO

ALTER TABLE Users
ADD  CONSTRAINT DF_Users_Login_Unique  UNIQUE (Login)
GO

ALTER TABLE Users
ADD  CONSTRAINT DF_Users_Email_Unique  UNIQUE (Email)
GO

ALTER TABLE Users WITH CHECK  
ADD CONSTRAINT [CK_Users1] CHECK  (([Login]<>N''))
GO
	
ALTER TABLE Users WITH CHECK 
ADD CONSTRAINT [CK_Users2] CHECK  (([Password]<>N''))
GO

ALTER TABLE Users WITH CHECK 
ADD CONSTRAINT [CK_Users3] CHECK  (([FirstName]<>N''))
GO

ALTER TABLE Users WITH CHECK 
ADD CONSTRAINT [CK_Users4] CHECK  (([LastName]<>N''))
GO

ALTER TABLE Users WITH CHECK 
ADD CONSTRAINT [CK_Users5] CHECK  (([Email]<>N''))
GO

ALTER TABLE Users WITH CHECK 
ADD CONSTRAINT [CK_Users6] CHECK (DATALENGTH(RegistrationDate) > 0)
GO

