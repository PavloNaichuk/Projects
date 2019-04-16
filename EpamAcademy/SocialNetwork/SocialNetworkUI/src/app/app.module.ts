import { AppFindComponent } from './routes/app-find/app-find.component';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { SpaModule } from '../spa/spa.module';
import { AppHomeComponent } from './routes/app-home/app-home.component';
import { RouterModule } from '../../node_modules/@angular/router';
import { appRoutes } from './routes/app.routes';


@NgModule({
  declarations: [
    AppComponent,
    AppHomeComponent,
    AppFindComponent,
  ],
  imports: [
    BrowserModule, SpaModule, RouterModule.forRoot(appRoutes)
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
