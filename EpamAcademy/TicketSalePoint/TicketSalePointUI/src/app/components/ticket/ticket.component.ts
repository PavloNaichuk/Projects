import { Ticket } from './../../shared/tickets/ticket.model';
import { TicketService } from './../../shared/tickets/ticket.service';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'ticket',
  templateUrl: './ticket.component.html',
  styleUrls: ['./ticket.component.css']
})
export class TicketComponent implements OnInit {


  constructor() { }

  ngOnInit() {
  }
}
