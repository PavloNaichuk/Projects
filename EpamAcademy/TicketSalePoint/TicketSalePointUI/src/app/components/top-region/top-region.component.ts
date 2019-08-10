import { LoginInComponent } from './../users/login-in/login-in.component';
import { Component, OnInit, TemplateRef, NgModule } from '@angular/core';
import { RegistrationComponent } from '../users/registration/registration.component';

@Component({
  selector: 'top-region',
  templateUrl: './top-region.component.html',
  styleUrls: ['./top-region.component.css']
})

@NgModule({
  declarations: [ LoginInComponent, RegistrationComponent],
  exports:      [ LoginInComponent, RegistrationComponent]
 })
export class TopRegionComponent implements OnInit {

  constructor(){}

  ngOnInit() {
  }

}
