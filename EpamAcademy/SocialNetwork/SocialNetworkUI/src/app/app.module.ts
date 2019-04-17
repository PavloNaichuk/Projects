import { AppFindComponent } from './routes/app-find/app-find.component';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { SpaModule } from '../spa/spa.module';
import { AppHomeComponent } from './routes/app-home/app-home.component';
import { RouterModule } from '../../node_modules/@angular/router';
import { appRoutes } from './routes/app.routes';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';
import { UserService } from './services/user.service';
import { UserApi } from '../spa/users/user-api';
import { AuthGuard } from './services/auth-guard.service';
import { AppFriendsComponent } from './routes/app-friends/app-friends.component';
@NgModule({
  declarations: [
    AppComponent,
    AppHomeComponent,
    AppFindComponent,
    AuthenticatedComponent,
    AppFriendsComponent
  ],
  imports: [
    BrowserModule, SpaModule, RouterModule.forRoot(appRoutes)
  ],
  providers: [UserService,
     {
    provide: UserApi, useExisting: UserService
  },
   AuthGuard],
  bootstrap: [AppComponent]
})
export class AppModule { }
