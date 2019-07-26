import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HttpModule } from '@angular/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { UserComponent } from './components/user/user.component';

const appRoutes: Routes = [
  {path: '', component: AppComponent},
  {path: 'user', component: UserComponent},
  {path: 'left-region', component: LeftRegionComponent},
  {path: 'top-region', component: TopRegionComponent},
  {path: 'bottom-region', component: BottomRegionComponent},
  {path: 'center-region', component: CenterRegionComponent},
]

@NgModule({
  declarations: [
    AppComponent,
    TopRegionComponent,
    LeftRegionComponent,
    BottomRegionComponent,
    CenterRegionComponent,
    UserComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    RouterModule.forRoot(appRoutes),
    HttpModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
