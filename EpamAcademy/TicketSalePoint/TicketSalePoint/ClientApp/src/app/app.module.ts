import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { FormsModule } from '@angular/forms';

import { AppComponent } from './app.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { SingUpComponent } from './components/users/sing-up/sing-up.component';

import { HttpClientModule } from '@angular/common/http';
import { TheatreService } from './shared/theatres/theatre.service';
import { ShowService } from './shared/shows/show.service';

const appRoutes: Routes = [
  { path: '', component: AppComponent },
  { path: 'left-region', component: LeftRegionComponent },
  { path: 'top-region', component: TopRegionComponent },
  { path: 'bottom-region', component: BottomRegionComponent },
  { path: 'center-region', component: CenterRegionComponent },
  { path: 'sing-up', component: SingUpComponent },
]

@NgModule({
  declarations: [
    AppComponent,
    TopRegionComponent,
    LeftRegionComponent,
    BottomRegionComponent,
    CenterRegionComponent,
    SingUpComponent
  ],
  imports: [
    BrowserModule,
    RouterModule.forRoot(appRoutes),
    FormsModule,
    HttpClientModule
  ],
  providers: [TheatreService, ShowService],
  bootstrap: [AppComponent]
})
export class AppModule { }
