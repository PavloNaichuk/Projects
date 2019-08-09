import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { SpaBodyComponent } from './spa-body/spa-body.component';
import { TopRegionComponent } from './top-region/top-region.component';
import { LeftRegionComponent } from './left-region/left-region.component';
import { CenterRegionComponent } from './center-region/center-region.component';
import { BottomRegionComponent } from './bottom-region/bottom-region.component';

@NgModule({
  imports: [CommonModule],
  declarations: [SpaBodyComponent, TopRegionComponent, LeftRegionComponent, CenterRegionComponent, BottomRegionComponent],
  exports: [SpaBodyComponent]
})
export class ComponentModule { }
