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
  reserved: Ticket[];
  onClick: () => void;
  flag: boolean;

  _tickets: TicketContainer[];

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
    });

    this.flag = false;
    this.reserved = [];
    this._tickets = [];
    this.onClick = this.clickHandler.bind(this);
  }

  ngOnInit() {
    this.getTicket();
    this.ngZone.runOutsideAngular(() => {
      this.app = new Application({width : 800, height: 400, backgroundColor: 0xDDDDDD});
    });
    this.elementRef.nativeElement.querySelector("#seats").appendChild(this.app.view);

  }

  renderTicketsPixi(): void {
    this.tickets.forEach(ticket => {
      const ticketContainer = new TicketContainer(ticket);
      this._tickets.push(ticketContainer);
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

  getTicketContainer(row: string, seat: string) {
    return this._tickets.filter(({seatsInfo}) => seatsInfo.row === row && seatsInfo.seat === seat);
  }

  addEventListeners() {
    this._tickets.forEach((tc) => {
      tc.on('click', this.onClick );
    });
  }

  clickHandler({target}): void {    
    if (target.reserved) {
      const index = this.reserved.indexOf(target.seatsInfo); 
      this.reserved.splice(index, 1);
      target.reserved = false;
      target.redrawBackground()
    } else {
      this.reserved.push(target.seatsInfo);
      target.reserved = true;
      target.redrawBackground()
    }  

    this.flag = this.reserved.length !== 0   
  }

  get isReserved(): boolean {
    return this.reserved.length !== 0
  }

}
