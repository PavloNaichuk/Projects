import { InMemoryDbService } from 'angular-in-memory-web-api';
import { Theatre } from './theatres.model';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root',
})
export class InMemoryDataService implements InMemoryDbService {
  createDb() {
    const theatres = [
      { id: 11, name: 'Національний  тетар України' },
      { id: 12, name: 'Національна опера України' },
      { id: 13, name: 'Національна оперета України' },
    ];
    return {theatres};
  }
}
