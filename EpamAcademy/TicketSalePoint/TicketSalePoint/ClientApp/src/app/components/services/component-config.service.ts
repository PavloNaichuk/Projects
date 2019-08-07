import { Injectable } from "@angular/core";

export interface ComponentConfigSetting
{
  showUserControls?: boolean;
}

@Injectable()
export class ComponentConfigService
{
  showUserControls = true;
  configure(settings: ComponentConfigSetting): void
  {
    Object.assign(this, settings);
  }
}
