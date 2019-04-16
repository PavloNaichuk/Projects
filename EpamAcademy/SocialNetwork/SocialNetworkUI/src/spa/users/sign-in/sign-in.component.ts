import { Component, OnInit } from '@angular/core';
import { UserApi } from '../user-api';
import { UserService } from '../../../app/services/user.service';
import { Router } from '@angular/router';
import { NgForm } from '../../../../node_modules/@angular/forms';

@Component({
  selector: 'spa-sign-in',
  templateUrl: './sign-in.component.html',
  styleUrls: ['./sign-in.component.css']
})
export class SignInComponent implements OnInit {
  submitting = false;
  formError: string;
  constructor() { }
  ngOnInit() {
  }

}
