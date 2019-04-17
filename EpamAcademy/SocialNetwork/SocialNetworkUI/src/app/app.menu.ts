import {MenuItem} from '../spa/services/menu.service';
export const AppMenuItems: Array<MenuItem> = [
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
