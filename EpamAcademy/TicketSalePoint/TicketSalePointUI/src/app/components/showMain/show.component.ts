import { Router, ActivatedRoute, ActivationEnd } from '@angular/router';
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
  id: number;


  constructor( private route: ActivatedRoute,
    private router: Router,
    private ticketService: TicketService)
    {
      this.router.events.subscribe(e => {
        if (e instanceof ActivationEnd) {
          this.id = e.snapshot.params.id;
          this.getTicket();
        }
     })
    }

  ngOnInit() {
    this.getTicket();
  }

  getTicket(): void {
    this.ticketService.getTicket(this.id)
      .subscribe(tickets => this.tickets = tickets);
  }

}
