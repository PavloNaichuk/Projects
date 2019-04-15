import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { SpaLeftComponent } from './spa-left.component';

describe('SpaLeftComponent', () => {
  let component: SpaLeftComponent;
  let fixture: ComponentFixture<SpaLeftComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ SpaLeftComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaLeftComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
