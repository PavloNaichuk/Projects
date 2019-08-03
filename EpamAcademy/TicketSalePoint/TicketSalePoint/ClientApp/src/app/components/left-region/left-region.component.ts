import { Component, OnInit } from '@angular/core';
import { Theatre } from '../../shared/theatres/theatre.model';
import { TheatreService } from '../../shared/theatres/theatre.service';

@Component({
  selector: 'left-region',
  templateUrl: './left-region.component.html',
  styleUrls: ['./left-region.component.css']
})
export class LeftRegionComponent implements OnInit {
  theatres: Theatre[];

  constructor(private theatreService: TheatreService) { }

  ngOnInit() {
    this.getTheatre();
  }

  getTheatre(): void {
    this.theatreService.getTheatre()
      .subscribe(theatres => this.theatres = theatres);
  }
}


