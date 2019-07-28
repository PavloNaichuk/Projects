import { BottomRegionComponent } from './../components/bottom-region/bottom-region.component';
import { TopRegionComponent } from './../components/top-region/top-region.component';
import { CenterRegionComponent } from './../components/center-region/center-region.component';
import { LeftRegionComponent } from './../components/left-region/left-region.component';
import { SingUpComponent } from '../components/users/sing-up/sing-up.component';
import {Routes} from '@angular/router';
import { AuthenticatedComponent } from './authenticated/authenticated.component';

export const appRoutes: Routes = [
    {path: 'sing-up', component: SingUpComponent},
    {path: 'authenticated', component:AuthenticatedComponent, children:
    [
        {path: 'left-region', component: LeftRegionComponent},
        {path: 'center-region', component: CenterRegionComponent},
        {path: 'top-region', component: TopRegionComponent},
        {path: 'bottom-region', component: BottomRegionComponent},
    ]},

    {path: '', redirectTo: 'sign-up', pathMatch: 'full'},
    {path: '**', component: SingUpComponent},

];
