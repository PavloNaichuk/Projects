import { User } from './../../shared/user/user.model';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'personal-office',
  templateUrl: './personal-office.component.html',
  styleUrls: ['./personal-office.component.css']
})
export class PersonalOfficeComponent implements OnInit {
  user: User[];

  constructor() { }

  ngOnInit() {
  }

}
