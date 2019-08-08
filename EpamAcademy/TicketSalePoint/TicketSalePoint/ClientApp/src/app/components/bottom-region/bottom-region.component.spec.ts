import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BottomRegionComponent } from './bottom-region.component';

describe('BottomRegionComponent', () => {
  let component: BottomRegionComponent;
  let fixture: ComponentFixture<BottomRegionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BottomRegionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BottomRegionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
