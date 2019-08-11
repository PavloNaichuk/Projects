import { TicketComponent } from './components/ticket/ticket.component';
import { ShowComponent } from './components/show/show.component';
import { TheatreComponent } from './components/theatre/theatre.component';
import { RegistrationComponent } from './components/users/registration/registration.component';
import { LoginInComponent } from './components/users/login-in/login-in.component';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';
import { Routes, RouterModule } from '@angular/router';
import { NgModule } from '@angular/core';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';

export const appRoutes: Routes = [
    {path: 'authenticated', component: AuthenticatedComponent},
    {path: 'login-in', component: LoginInComponent},
    {path: 'registration', component: RegistrationComponent},
    {path: 'left-region', component: LeftRegionComponent},
    {path: 'center-region', component: CenterRegionComponent},
    {path: 'top-region', component: TopRegionComponent},
    {path: 'bottom-region', component: BottomRegionComponent},
    {path: 'theatre/:id', component: TheatreComponent},
    {path: 'show/:id', component: ShowComponent},
    {path: 'ticket', component: TicketComponent}

];

@NgModule({
  imports: [RouterModule.forRoot(appRoutes)],
  exports: [RouterModule]
})

export class AppRoutingModule {}
