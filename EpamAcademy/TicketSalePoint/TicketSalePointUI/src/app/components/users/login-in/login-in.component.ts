import { Component, OnInit } from '@angular/core';
import { NgForm } from '@angular/forms';
import { UserApi } from '../user-api';
import { Router } from '@angular/router';


@Component({
  selector: 'login-in',
  templateUrl: './login-in.component.html',
  styleUrls: ['./login-in.component.css']
})
export class LoginInComponent implements OnInit {

  submitting = false;
  formError: string;
  constructor(private userApi: UserApi,  private router: Router) { }
  onSubmit(loginInForm: NgForm): void {
    if (loginInForm.valid) {
      this.submitting = true;
      this.formError = null;
      this.userApi.signIn(loginInForm.value.email, loginInForm.value.password).subscribe((data) => {
        console.log(data);
        this.router.navigate(['/authenticated']);
      },
    (error) => {
      this.submitting = false;
      this.formError = error;
    });
    }
  }
  ngOnInit() {
  }
}
