import {MenuItem} from '../spa/services/menu.service';
export const AppMenuItems: Array<MenuItem> = [
  {text: 'Message',
  icon: 'src/imgs/message.png',
  route: '/authenticated/message',
  submenu: null},
  {text: 'Friends',
  icon: 'src/imgs/friends.png',
  route: '/authenticated/friends',
  submenu: null},
  {text: 'Find User',
  icon: 'src/imgs/find.png',
  route: '/authenticated/find',
  submenu: null},
  {text: 'Home',
  icon: 'src/imgs/home.png',
  route: '/authenticated/home',
  submenu: null},
]
