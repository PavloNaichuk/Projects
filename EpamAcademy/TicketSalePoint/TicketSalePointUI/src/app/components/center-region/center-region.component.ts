import { Theatre } from './../../shared/theatres/theatres.model';
import { Component, OnInit, Input } from '@angular/core';


@Component({
  selector: 'center-region',
  templateUrl: './center-region.component.html',
  styleUrls: ['./center-region.component.css']
})
export class CenterRegionComponent implements OnInit {

  @Input() theatre: Theatre;

  constructor() { }

  ngOnInit() {
  }

}
