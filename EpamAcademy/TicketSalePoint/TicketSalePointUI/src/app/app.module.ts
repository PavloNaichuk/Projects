import { AlertService } from './shared/user/alert.service';
import { UserService } from './shared/user/user.service';
import { TicketService } from './shared/tickets/ticket.service';
import { TicketComponent } from './components/ticket/ticket.component';
import { ShowComponent } from './components/show/show.component';
import { TheatreComponent } from './components/theatre/theatre.component';
import { RegistrationComponent } from './components/users/registration/registration.component';
import { LoginInComponent } from './components/users/login-in/login-in.component';
import { appRoutes } from './app.routes.modul';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';

import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { TheatreService } from './shared/theatres/theatre.service';
import { ShowService } from './shared/shows/show.service';
import { SpaBodyComponent } from './components/spa-body/spa-body.component';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { ModalModule } from 'ngx-bootstrap';
import { UserApi } from './components/users/user-api';

@NgModule({
  declarations: [
    AppComponent,
    TopRegionComponent,
    LeftRegionComponent,
    BottomRegionComponent,
    CenterRegionComponent,
    SpaBodyComponent,
    AuthenticatedComponent,
    LoginInComponent,
    RegistrationComponent,
    TheatreComponent,
    ShowComponent,
    TicketComponent

  ],
  imports: [
    BrowserModule,
    RouterModule.forRoot(appRoutes),
    FormsModule,
    HttpClientModule,
    BrowserAnimationsModule,
    ModalModule.forRoot(),
    ReactiveFormsModule,
  ],
  providers: [TheatreService,
    ShowService, TicketService, UserService, AlertService
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
