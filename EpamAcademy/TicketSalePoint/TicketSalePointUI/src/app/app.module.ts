import { ShowService } from './shared/shows/show.service';
import { TheatreService } from './shared/theatres/theatre.service';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';
import { appRoutes, AppRoutingModule } from './app.routes.modul';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { HttpClientModule } from '@angular/common/http';

@NgModule({
  declarations: [
    AppComponent,
    AuthenticatedComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    RouterModule.forRoot(appRoutes),
    FormsModule,
    HttpClientModule,
  ],
  providers: [TheatreService, ShowService],
  bootstrap: [AppComponent]
})
export class AppModule { }
