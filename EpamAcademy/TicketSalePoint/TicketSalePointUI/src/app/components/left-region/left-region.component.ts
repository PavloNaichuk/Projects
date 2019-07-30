import { TheatresService } from './../../shared/theatres/theatres.service';
import { Theatre } from '../../shared/theatres/theatres.model';
import { Component, OnInit } from '@angular/core';
import { THEATRES } from 'src/app/shared/theatres/mock-theatres';


@Component({
  selector: 'left-region',
  templateUrl: './left-region.component.html',
  styleUrls: ['./left-region.component.css']
})
export class LeftRegionComponent implements OnInit {
   theatres: Theatre[];

  constructor(private theatreService: TheatresService)
   {
   }

  ngOnInit() {
    this.getTheatres();
  }

  getTheatres(): void {
    this.theatreService.getTheatres()
      .subscribe(theatres => this.theatres = theatres);
  }
}


