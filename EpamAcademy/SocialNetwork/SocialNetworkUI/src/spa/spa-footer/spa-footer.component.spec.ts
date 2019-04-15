import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { SpaFooterComponent } from './spa-footer.component';

describe('SpaFooterComponent', () => {
  let component: SpaFooterComponent;
  let fixture: ComponentFixture<SpaFooterComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ SpaFooterComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaFooterComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
