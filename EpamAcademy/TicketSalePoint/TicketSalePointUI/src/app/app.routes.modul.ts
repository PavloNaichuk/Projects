import { Routes, RouterModule } from '@angular/router';
import { NgModule } from '@angular/core';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';

export const appRoutes: Routes = [
    {path: '', redirectTo: 'sign-up', pathMatch: 'full'},
    {path: 'left-region', component: LeftRegionComponent},
    {path: 'center-region', component: CenterRegionComponent},
    {path: 'top-region', component: TopRegionComponent},
    {path: 'bottom-region', component: BottomRegionComponent},

];

@NgModule({
  imports: [RouterModule.forRoot(appRoutes)],
  exports: [RouterModule]
})

export class AppRoutingModule {}
