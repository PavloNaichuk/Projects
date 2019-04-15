import { SpaFooterComponent } from './spa-footer/spa-footer.component';
import { SpaBodyComponent } from './spa-body/spa-body.component';
import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { SpaHeaderComponent } from './spa-header/spa-header.component';
import { SpaContentComponent } from './spa-content/spa-content.component';
import { SpaLeftComponent } from './spa-left/spa-left.component';
import { SpaConfigService } from './services/spa-config.service';
import { IconBarComponent } from './icon-bar/icon-bar.component';
@NgModule(
  {
    imports: [CommonModule],
    declarations: [SpaBodyComponent, SpaHeaderComponent, SpaContentComponent, SpaFooterComponent, SpaLeftComponent, IconBarComponent],
    exports: [SpaBodyComponent],
    providers: [SpaConfigService]
})
export class SpaModule {}
