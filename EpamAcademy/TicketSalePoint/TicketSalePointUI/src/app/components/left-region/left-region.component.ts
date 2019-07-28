import { Sellers } from './../../shared/sellers/sellers.model';
import { SellersService } from '../../shared/sellers/sellers.service';
import { Component, OnInit } from '@angular/core';


@Component({
  selector: 'left-region',
  templateUrl: './left-region.component.html',
  styleUrls: ['./left-region.component.css']
})
export class LeftRegionComponent implements OnInit {

  sellers = Sellers;
  constructor(private service : SellersService)
   {
   }

  ngOnInit() {
  }
}
