import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { SpaModule } from 'src/spa/spa.module';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    SpaModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
