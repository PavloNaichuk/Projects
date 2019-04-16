import { Component } from '@angular/core';
import { SpaConfigService, SpaConfigSettings, Icons } from '../spa/services/spa-config.service';
import { MenuService } from '../spa/services/menu.service';
import { AppMenuItems } from './app.menu';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
 constructor( private spaConfigService: SpaConfigService, private menuService: MenuService) {
   const config: SpaConfigSettings = {
     showUserControls: true
   };
   spaConfigService.configure(config);
   menuService.items = AppMenuItems;
 }

}
