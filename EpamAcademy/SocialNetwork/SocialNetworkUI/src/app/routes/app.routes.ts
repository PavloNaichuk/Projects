import { AppFindComponent } from './app-find/app-find.component';
import {Routes} from '@angular/router';
import { AppHomeComponent } from './app-home/app-home.component';
import { AuthenticatedComponent } from './authenticated/authenticated.component';
import { SignInComponent } from 'src/spa/users/sign-in/sign-in.component';
import { RegistrationComponent } from 'src/spa/users/registration/registration.component';
export const appRoutes: Routes = [
    {path: 'sign-in', component: SignInComponent},
    {path: 'register', component: RegistrationComponent},
    {path: 'authenticated', component:AuthenticatedComponent, children:
    [
      {path: 'home', component: AppHomeComponent},
      {path: 'find', component: AppFindComponent},
    ]},

    {path: '', redirectTo: 'sign-in', pathMatch: 'full'},
    {path: '**', component: SignInComponent},

];
