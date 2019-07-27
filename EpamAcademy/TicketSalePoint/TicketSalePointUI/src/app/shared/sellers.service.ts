import { Sellers } from './sellers.model';
import { Injectable } from '@angular/core';

@Injectable({
   providedIn: 'root'
})
export class SellersService {
   formData : Sellers;
   seller : Sellers = new Sellers();
   constructor() { }
}
