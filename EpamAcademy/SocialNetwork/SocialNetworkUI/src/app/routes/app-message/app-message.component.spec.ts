import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AppMessageComponent } from './app-message.component';

describe('AppMessageComponent', () => {
  let component: AppMessageComponent;
  let fixture: ComponentFixture<AppMessageComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AppMessageComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AppMessageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
