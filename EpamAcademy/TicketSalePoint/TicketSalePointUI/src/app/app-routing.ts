import { Routes, RouterModule } from '@angular/router';


const appRoutes: Routes = [

    { path: '**', redirectTo: '' }
];

export const routing = RouterModule.forRoot(appRoutes);
