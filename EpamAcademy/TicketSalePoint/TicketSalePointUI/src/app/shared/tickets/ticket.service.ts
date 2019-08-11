import { Injectable } from '@angular/core';
import { HttpHeaders, HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { Ticket } from './ticket.model';

@Injectable()

export class TicketService {
  private ticketUrl = 'api/ticket/Get/';

  httpOptions = {
    headers: new HttpHeaders({ 'Content-Type': 'application/json' })
  };

  constructor(private http: HttpClient) { }

  getTicket(id : number): Observable<Ticket[]> {
    return this.http.get<Ticket[]>(this.ticketUrl + id)
  }
}
