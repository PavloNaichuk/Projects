import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { LeftRegionComponent } from './left-region.component';

describe('LeftRegionComponent', () => {
  let component: LeftRegionComponent;
  let fixture: ComponentFixture<LeftRegionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ LeftRegionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(LeftRegionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
