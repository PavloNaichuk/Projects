import { ShowsService } from './shared/theatres/shows.service';
import { InMemoryDataServiceTheatre } from './shared/theatres/in-memory-data.service';
import { InMemoryDataServiceShows } from './shared/theatres/in-memory-data.service';
import { TheatresService } from './shared/theatres/theatres.service';
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { FormsModule } from '@angular/forms';
import { HttpClientInMemoryWebApiModule } from 'angular-in-memory-web-api';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { TopRegionComponent } from './components/top-region/top-region.component';
import { LeftRegionComponent } from './components/left-region/left-region.component';
import { BottomRegionComponent } from './components/bottom-region/bottom-region.component';
import { CenterRegionComponent } from './components/center-region/center-region.component';
import { SingUpComponent } from './components/users/sing-up/sing-up.component';
import { AuthenticatedComponent } from './routes/authenticated/authenticated.component';
import { HttpClientModule } from '@angular/common/http';

const appRoutes: Routes = [
  {path: '', component: AppComponent},
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
    SingUpComponent,
    AuthenticatedComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    RouterModule.forRoot(appRoutes),
    FormsModule,
    HttpClientModule,
    HttpClientInMemoryWebApiModule.forRoot(
      InMemoryDataServiceTheatre, {dataEncapsulation: false}
    ),
    //HttpClientInMemoryWebApiModule.forRoot(
      //InMemoryDataServiceShows, {dataEncapsulation: false}
    //)
  ],
  providers: [TheatresService, ShowsService],
  bootstrap: [AppComponent]
})
export class AppModule { }
