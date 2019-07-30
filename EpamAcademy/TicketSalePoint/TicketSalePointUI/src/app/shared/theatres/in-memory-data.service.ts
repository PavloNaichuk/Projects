import { InMemoryDbService } from 'angular-in-memory-web-api';
import { Theatre } from './theatres.model';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class InMemoryDataServiceTheatre implements InMemoryDbService {
  createDb() {
    const theatres = [
      { id: 11, name: 'Національний  тетар України' },
      { id: 12, name: 'Національна опера України' },
      { id: 13, name: 'Національна оперета України' },
    ];
    return {theatres};
  }
}

export class InMemoryDataServiceShows implements InMemoryDbService {
  createDb() {
    const shows = [
      { id: 11, name: 'Розпрягайте  хлопці коні', theatreId: 1, theatreName: 'Національна опера України'},
      { id: 12, name: 'Весілля в малинівці', theatreId: 1, theatreName: 'Національна опера України'},
      { id: 13, name: 'Хто сказав', theatreId: 1, theatreName: 'Національна опера України'},
    ];
    return {shows};
  }
}
