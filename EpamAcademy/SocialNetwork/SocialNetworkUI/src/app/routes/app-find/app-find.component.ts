import { Component, OnInit } from '@angular/core';
import { UserService } from '../../../app/services/user.service';
import { Router } from '@angular/router';
import { NgForm } from '../../../../node_modules/@angular/forms';

@Component({
  selector: 'app-find',
  templateUrl: './app-find.component.html',
  styleUrls: ['./app-find.component.css']
})
export class AppFindComponent implements OnInit {
  submitting = false;
  formError: string;
  constructor( private userService:UserService, private router: Router) { }
  onSubmit(findForm: NgForm): void {
    if (findForm.valid) {
      this.submitting = true;
      this.formError = null;
    }
  }
  ngOnInit() {
  }

}
