import { Router, ActivatedRoute, ActivationEnd } from '@angular/router';
import { TicketService } from './../../shared/tickets/ticket.service';
import { Ticket } from './../../shared/tickets/ticket.model';
import { Component, OnInit, ElementRef, NgZone, ChangeDetectionStrategy, ChangeDetectorRef } from '@angular/core';
import { Application } from 'pixi.js';
import TicketContainer from './TicketContainer';


@Component({
  changeDetection: ChangeDetectionStrategy.OnPush,
  selector: 'show',
  templateUrl: './show.component.html',
  styleUrls: ['./show.component.css']
})
export class ShowComponent implements OnInit {
  tickets: Ticket[];
  id: number;
  app: Application;
  reserved: Ticket[];
  onClick: () => void;

  ticketsContainer: TicketContainer[];

  constructor( private route: ActivatedRoute,
  private router: Router,
  private elementRef: ElementRef,
  private changeDetection: ChangeDetectorRef,
  private ngZone: NgZone,
  private ticketService: TicketService)
  {
    this.router.events.subscribe(e => {
      if (e instanceof ActivationEnd) {
        this.id = e.snapshot.params.id;
      }
    });

    this.changeDetection.detach();

    this.reserved = [];
    this.ticketsContainer = [];
    this.onClick = this.clickHandler.bind(this);
  }

  ngOnInit() {
    this.getTicket();
    this.ngZone.runOutsideAngular(() => {
      this.app = new Application({width : 800, height: 400, backgroundColor: 0xDDDDDD});
    });
    this.elementRef.nativeElement.querySelector("#seats").appendChild(this.app.view);
    this.changeDetection.detectChanges();
  }

  renderTicketsPixi(): void {
    this.tickets.sort((a, b) => {
      return (a.row) - (b.row);
    });

    this.tickets.forEach(ticket => {
      const ticketContainer = new TicketContainer(ticket);
      this.ticketsContainer.push(ticketContainer);
      this.app.stage.addChild(ticketContainer);
    });
  }

  getTicket(): void {
    this.ticketService.getTicket(this.id)
      .subscribe(tickets => {
        this.tickets = tickets;
        this.renderTicketsPixi();

        this.addEventListeners();
      });
  }

  getTicketContainer(row: number, seat: number): TicketContainer {
    return this.ticketsContainer.find(({seatsInfo}) => seatsInfo.row === row && seatsInfo.seat === seat);
  }

  addEventListeners() {
    this.ticketsContainer.forEach((tc) => {
      tc.on('click', this.onClick );
    });
  }

  clickHandler({target}): void {
    if (target.reserved) {
      const index = this.reserved.indexOf(target.seatsInfo);
      this.reserved.splice(index, 1);
      target.reserved = false;
      target.redrawBackground();
    } else {
      this.reserved.push(target.seatsInfo);
      target.reserved = true;
      target.redrawBackground();
      console.error(this.reserved);
    }
    this.changeDetection.detectChanges();
  }

  onCloseClick(seatsInfo: Ticket) {
    const index = this.reserved.indexOf(seatsInfo);
    this.reserved.splice(index, 1);
    const cont = this.getTicketContainer(seatsInfo.row, seatsInfo.seat);
    cont.reserved = false;
    cont.redrawBackground();

    this.changeDetection.detectChanges();
  }

  get isReserved(): boolean {
    return this.reserved.length !== 0;
  }

  get summary(): number {
    return this.reserved.reduce((acc, item) => acc + item.priceInCents, 0);
  }

}
