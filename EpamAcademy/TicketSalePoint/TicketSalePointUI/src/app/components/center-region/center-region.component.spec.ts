import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { CenterRegionComponent } from './center-region.component';

describe('CenterRegionComponent', () => {
  let component: CenterRegionComponent;
  let fixture: ComponentFixture<CenterRegionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ CenterRegionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(CenterRegionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
