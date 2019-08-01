import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TopRegionComponent } from './top-region.component';

describe('TopRegionComponent', () => {
  let component: TopRegionComponent;
  let fixture: ComponentFixture<TopRegionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ TopRegionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TopRegionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
