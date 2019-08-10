import { Component, OnInit, TemplateRef } from '@angular/core';
import { NgForm } from '@angular/forms';
import { Router } from '@angular/router';
import { BsModalService, BsModalRef } from 'ngx-bootstrap/modal';


@Component({
  selector: 'login-in',
  templateUrl: './login-in.component.html',
  styleUrls: ['./login-in.component.css']
})
export class LoginInComponent implements OnInit {

  submitting = false;
  formError: string;

  modalRef: BsModalRef;

  constructor(private router: Router, private modalService: BsModalService) { }

  openModal(template: TemplateRef<any>) {
    this.modalRef = this.modalService.show(template);
  }

  onSubmit(loginInForm: NgForm): void {
    if (loginInForm.valid) {
      this.submitting = true;
      this.formError = null;
    }
  }
  ngOnInit() {
  }
}
