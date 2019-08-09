import { User } from './../../../shared/user/user.model';

import { Component, OnInit } from '@angular/core';
import { NgForm} from '@angular/forms';

@Component({
  selector: 'app-sing-up',
  templateUrl: './sing-up.component.html',
  styleUrls: ['./sing-up.component.css']
})
export class SingUpComponent implements OnInit {
  user: User;

  constructor() { }

  ngOnInit() {

  }
  onSubmit(form: NgForm){
    console.log(form);
}

}
