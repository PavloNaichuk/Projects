import { Observable, of } from 'rxjs';
import { Theatre } from './theatres.model';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { catchError, map, tap } from 'rxjs/operators';

import {MessageService} from './message.service';
import { Shows } from './shows.model';


@Injectable({
   providedIn: 'root'
})
export class ShowsService {
  private showUrl = 'api/shows';

  httpOptions = {
    headers: new HttpHeaders({'Content-type': 'application.json' })
  };
  handleError: any;

   constructor(private http: HttpClient, private messageService: MessageService) {}
   /** Get shows from the server */
   getShows(): Observable<Shows[]> {
    return this.http.get<Shows[]>(this.showUrl)

    .pipe(
      tap(_=>this.log('fetched show')),
      catchError(this.handlError<Shows[]>('getShow', []))
    );
  }

     /** GET shows by id. Return `undefined` when id not found */
  getTheatreNo404<Data>(id: number): Observable<Shows> {
    const url = `${this.showUrl}/?id=${id}`;
    return this.http.get<Shows[]>(url)
      .pipe(
        map(show => show[0]), // returns a {0|1} element array
        tap(h => {
          const outcome = h ? `fetched` : `did not find`;
          this.log(`${outcome} theatre id=${id}`);
        }),
        catchError(this.handlError<Shows>(`getHero id=${id}`))
      );
  }

  getShow(id: number): Observable<Shows> {
    const url = `${this.showUrl}/${id}`;
    return this.http.get<Shows>(url).pipe(
      tap(_ => this.log(`fetched show id=${id}`)),
      catchError(this.handlError<Shows>(`getShow id=${id}`))
    );
  }

  private handlError<T> (operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {

      // TODO: send the error to remote logging infrastructure
      console.error(error); // log to console instead

      // TODO: better job of transforming error for user consumption
      this.log(`${operation} failed: ${error.message}`);

      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  /** Log a ShowService message with the MessageService */
  private log(message: string) {
    this.messageService.add(`ShowsService: ${message}`);
  }
}
