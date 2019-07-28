import { SellersService } from './shared/sellers/sellers.service';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { HttpModule } from '@angular/http';
import { FormsModule } from '@angular/forms';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { UserComponent } from './components/user/user.component';
import { SingUpComponent } from './components/sing-up/sing-up.component';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';

const appRoutes: Routes = [
  {path: '', component: AppComponent},
  {path: 'user', component: UserComponent},
  {path: 'left-region', component: LeftRegionComponent},
  {path: 'top-region', component: TopRegionComponent},
  {path: 'bottom-region', component: BottomRegionComponent},
  {path: 'center-region', component: CenterRegionComponent},
  {path: 'sing-up', component: SingUpComponent},
]

@NgModule({
  declarations: [
    AppComponent,
    TopRegionComponent,
    LeftRegionComponent,
    BottomRegionComponent,
    CenterRegionComponent,
    UserComponent,
    SingUpComponent,
    AuthenticatedComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    RouterModule.forRoot(appRoutes),
    HttpModule,
    FormsModule,
  ],
  providers: [SellersService],
  bootstrap: [AppComponent]
})
export class AppModule { }
