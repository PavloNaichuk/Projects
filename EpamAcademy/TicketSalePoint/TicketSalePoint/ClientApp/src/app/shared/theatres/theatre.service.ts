import { Injectable } from '@angular/core';
import { HttpHeaders, HttpClient } from '@angular/common/http';
import { Theatre } from './theatre.model';
import { Observable } from 'rxjs';

@Injectable()

export class TheatreService {
  private theatreUrl = 'api/theatre';

  httpOptions = {
    headers: new HttpHeaders({ 'Content-Type': 'application/json' })
  };

  constructor(private http: HttpClient) { }
  getTheatre(): Observable<Theatre[]> {
    return this.http.get<Theatre[]>(this.theatreUrl)
  }
}
