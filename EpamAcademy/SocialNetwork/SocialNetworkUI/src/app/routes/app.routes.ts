import { AppFindComponent } from './app-find/app-find.component';
import {Routes} from '@angular/router';
import { AppHomeComponent } from './app-home/app-home.component';
export const appRoutes: Routes = [
    {path: 'home', component: AppHomeComponent},
    {path: 'find', component: AppFindComponent},
    {path: '', redirectTo: 'home', pathMatch: 'full'},
    {path: '**', component: AppHomeComponent},

]
