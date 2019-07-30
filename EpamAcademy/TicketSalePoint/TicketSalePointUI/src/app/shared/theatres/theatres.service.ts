import { Observable, of } from 'rxjs';
import { Theatre } from './theatres.model';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { catchError, map, tap } from 'rxjs/operators';

import {MessageService} from './message.service';


@Injectable({
   providedIn: 'root'
})
export class TheatresService {
  private theatreUrl = 'api/Theatre';

  httpOptions = {
    headers: new HttpHeaders({'Content-type': 'application.json' })
  };
  handleError: any;

   constructor(private http: HttpClient, private messageService: MessageService) {}
   /** Get teatres from the server */
   getTheatres(): Observable<Theatre[]> {
     return this.http.get<Theatre[]>(this.theatreUrl)

     .pipe(
       tap(_=>this.log('fetched theatre')),
       catchError(this.handlError<Theatre[]>('getTheatre', []))
     );
   }

     /** GET theatre by id. Return `undefined` when id not found */
  getTheatreNo404<Data>(id: number): Observable<Theatre> {
    const url = `${this.theatreUrl}/?id=${id}`;
    return this.http.get<Theatre[]>(url)
      .pipe(
        map(theatre => theatre[0]), // returns a {0|1} element array
        tap(h => {
          const outcome = h ? `fetched` : `did not find`;
          this.log(`${outcome} theatre id=${id}`);
        }),
        catchError(this.handlError<Theatre>(`getHero id=${id}`))
      );
  }

  getTheatre(id: number): Observable<Theatre> {
    const url = `${this.theatreUrl}/${id}`;
    return this.http.get<Theatre>(url).pipe(
      tap(_ => this.log(`fetched theatre id=${id}`)),
      catchError(this.handlError<Theatre>(`getTheatre id=${id}`))
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

  /** Log a TheatreService message with the MessageService */
  private log(message: string) {
    this.messageService.add(`TheatresService: ${message}`);
  }
}
