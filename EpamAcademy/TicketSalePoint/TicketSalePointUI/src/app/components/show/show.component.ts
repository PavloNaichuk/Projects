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

  get renderTickets(): string{
    if (!this.tickets) return '<div>No tickets</div>';
    const rows = this.tickets.reduce( (prev, ticket) => {
      if (!prev.includes(ticket.row)) {
        prev.push(ticket.row);
      }
      return prev;
    }, []);
    let ticketsRendered = 'Seats:';
    rows.forEach(_row => {
      const ticketInRow = this.tickets.filter(({row}) => _row === row);
      const rowRendered = ticketInRow.reduce((prev, cur) => {
        return prev + '<div placement="right" ngbTooltip="Tooltip on right" data-id="' + cur.id + '">' + cur.id + '</div>';
      }, '');
      ticketsRendered += rowRendered;
    });
    return ticketsRendered;
  }
}
