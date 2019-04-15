import { Component } from '@angular/core';
import { SpaConfigService, SpaConfigSettings, Icons } from '../spa/services/spa-config.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
 constructor( private spaConfigService: SpaConfigService) {
   const config: SpaConfigSettings = {
     showUserControls: true
   };
   spaConfigService.configure(config);
 }

}
