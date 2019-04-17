import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { AppFriendsComponent } from './app-friends.component';

describe('AppFriendsComponent', () => {
  let component: AppFriendsComponent;
  let fixture: ComponentFixture<AppFriendsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ AppFriendsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(AppFriendsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
