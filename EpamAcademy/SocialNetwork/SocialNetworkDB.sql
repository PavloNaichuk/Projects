USE [SocialNetwork]
GO

CREATE TABLE [dbo].[UserInfo](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[Login] [varchar](50) NOT NULL,
	[Password] [varchar](50) NOT NULL,
	[FirstName] [nchar](10) NOT NULL,
	[LastName] [nchar](10) NOT NULL,
	[Email] [varchar](50) NOT NULL,
	[RegistrationDate] [datetime] NOT NULL,
)
GO

CREATE TABLE [dbo].[Friends](
	[UserId] [int] NOT NULL,
	[FriendId] [int]  NOT NULL,
	CONSTRAINT FK_UserInfoFriend FOREIGN KEY (UserId)
	REFERENCES [UserInfo](Id),
    CONSTRAINT FK_UserInfoFriend2 FOREIGN KEY (UserId)
	REFERENCES [UserInfo](Id)
)
GO

CREATE TABLE [dbo].[Messages](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[SenderId] [int] NOT NULL,
    [ReceiverId] [int] NOT NULL,
	[Message] [varchar](500) NOT NULL,
	[Date] [datetime] NOT NULL,
	[WasRead][bit] NOT NULL,
    CONSTRAINT FK_UserInfoSender FOREIGN KEY (SenderId)
	REFERENCES [UserInfo](Id),
    CONSTRAINT FK_UserInfoReceiver FOREIGN KEY (ReceiverId)
	REFERENCES [UserInfo](Id)
)
GO

ALTER TABLE [dbo].[UserInfo] 
ADD  CONSTRAINT [DF_UserInfo_RegistrationDate]  DEFAULT (getdate()) FOR [RegistrationDate]
GO

ALTER TABLE UserInfo
ADD  CONSTRAINT DF_UserInfo_Login_Unique  UNIQUE (Login)
GO

ALTER TABLE UserInfo
ADD  CONSTRAINT DF_UserInfo_Email_Unique  UNIQUE (Email)
GO

