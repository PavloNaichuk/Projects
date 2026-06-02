# Messenger

## Project Description

**Messenger** is a full-stack real-time messaging application built with **Django**, **Django REST Framework**, **Django Channels**, **Redis**, **React**, and **TypeScript**.

The project is designed as a Telegram-style messenger where users can register, log in, create private conversations, exchange real-time messages, manage chats, and customize their profile. The application includes both a backend API and a frontend user interface.

### What has been implemented

* User registration, login, logout, and JWT authentication
* Current user profile management
* Username and email update
* Avatar upload, update, and deletion
* User search
* Contact nicknames
* User blocking and unblocking
* Blocked users list
* Confirmation modal before blocking or unblocking a user
* Protection against sending messages when one user has blocked another
* Private conversations between users
* Conversation list with real-time updates
* Delete conversation for current user or for everyone
* Clear conversation history
* Pin and mute conversations
* Mark conversations as read or unread
* Unread message count
* Real-time messaging with WebSockets
* Online / offline status
* Last seen status
* Typing indicator
* Message creation, editing, and deletion
* Delete message for current user or for everyone
* Message delivery and read status
* Message replies
* Message forwarding
* Message reactions with emoji
* Message search inside a conversation
* Message pagination / infinite scroll
* Image and file attachments
* Attachment preview, download, and removal
* Real-time sidebar notifications
* Real-time profile update notifications
* Backend tests and WebSocket testing setup

### Tech Stack

**Backend:**

* Python
* Django
* Django REST Framework
* Django Channels
* Redis
* Simple JWT
* SQLite
* Django CORS Headers
* Pillow

**Frontend:**

* React
* TypeScript
* Vite
* CSS

**Tools:**

* Git / GitHub
* Docker / Docker Compose
* PowerShell scripts for local development
* Postman / API testing

## Local Development

### 1. Create `.env`

Copy example environment file:

```bash
cp .env.example .env