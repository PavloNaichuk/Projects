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
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[UserId] [int] NOT NULL,
	[FriendId] [int]  NOT NULL,
	CONSTRAINT FK_UserInfoFriend FOREIGN KEY (UserId)
	REFERENCES [UserInfo](Id),
    CONSTRAINT FK_UserInfoFriend2 FOREIGN KEY (FriendId)
	REFERENCES [UserInfo](Id)
)
GO

CREATE TABLE [dbo].[Messages](
	[Id] [int] IDENTITY(1,1) PRIMARY KEY NOT NULL,
	[ConversationId] [int] NOT NULL,
	[SenderId] [int] NOT NULL,
    [ReceiverId] [int] NOT NULL,
	[Text] [varchar](500) NOT NULL,
	[Date] [datetime] NOT NULL,
	[Unread][bit] NOT NULL
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

ALTER TABLE UserInfo WITH CHECK  
ADD CONSTRAINT [CK_UserInfo1] CHECK  (([Login]<>N''))
GO
	
ALTER TABLE UserInfo WITH CHECK 
ADD CONSTRAINT [CK_UserInfo2] CHECK  (([Password]<>N''))
GO

ALTER TABLE UserInfo WITH CHECK 
ADD CONSTRAINT [CK_UserInfo3] CHECK  (([FirstName]<>N''))
GO

ALTER TABLE UserInfo WITH CHECK 
ADD CONSTRAINT [CK_UserInfo4] CHECK  (([LastName]<>N''))
GO

ALTER TABLE UserInfo WITH CHECK 
ADD CONSTRAINT [CK_UserInfo5] CHECK  (([Email]<>N''))
GO

ALTER TABLE UserInfo WITH CHECK 
ADD CONSTRAINT [CK_UserInfo6] CHECK (DATALENGTH(RegistrationDate) > 0)
GO

ALTER TABLE Messages WITH CHECK 
ADD CONSTRAINT [CK_Text] CHECK  (([Text]<>N''))
GO

ALTER TABLE Messages WITH CHECK 
ADD CONSTRAINT [CK_Messages] CHECK (DATALENGTH(Date) > 0)
GO
