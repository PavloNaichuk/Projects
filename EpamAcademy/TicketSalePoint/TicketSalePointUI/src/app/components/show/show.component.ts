import { TicketService } from './../../shared/tickets/ticket.service';
import { Ticket } from './../../shared/tickets/ticket.model';
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'show',
  templateUrl: './show.component.html',
  styleUrls: ['./show.component.css']
})
export class ShowComponent implements OnInit {
  tickets: Ticket[];

  constructor(private ticketService: TicketService) { }

  ngOnInit() {
    this.getTicket();
  }

  getTicket(): void {
    this.ticketService.getTicket()
      .subscribe(tickets => this.tickets = tickets);
  }


}
