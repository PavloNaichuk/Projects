import { Router, ActivatedRoute, ActivationEnd } from '@angular/router';
import { TicketService } from './../../shared/tickets/ticket.service';
import { Ticket } from './../../shared/tickets/ticket.model';
import { Component, OnInit, ElementRef, NgZone } from '@angular/core';
import { Application } from 'pixi.js';
import TicketContainer from './TicketContainer';

@Component({
  selector: 'show',
  templateUrl: './show.component.html',
  styleUrls: ['./show.component.css']
})
export class ShowComponent implements OnInit {
  tickets: Ticket[];
  id: number;
  app: Application;

  constructor( private route: ActivatedRoute,
    private router: Router,
    private elementRef: ElementRef,
    private ngZone: NgZone,
    private ticketService: TicketService)
    {
      this.router.events.subscribe(e => {
        if (e instanceof ActivationEnd) {
          this.id = e.snapshot.params.id;
        }
     })
    }

  ngOnInit() {
    this.getTicket();
    this.ngZone.runOutsideAngular(() => {
      this.app = new Application({width : 800, height: 400, backgroundColor: 0xDDDDDD});
    });
    this.elementRef.nativeElement.querySelector("#seats").appendChild(this.app.view);

  }

  renderTicketsPixi(): void {
    this.tickets.forEach(ticket => this.app.stage.addChild(new TicketContainer(ticket)));
  }

  getTicket(): void {
    this.ticketService.getTicket(this.id)
      .subscribe(tickets => {
        this.tickets = tickets
        this.renderTicketsPixi();
      });
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
