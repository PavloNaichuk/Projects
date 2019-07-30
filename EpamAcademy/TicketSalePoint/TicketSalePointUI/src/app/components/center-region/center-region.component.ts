import { ShowsService } from './../../shared/theatres/shows.service';
import { Shows } from '../../shared/theatres/shows.model';
import { Component, OnInit, Input } from '@angular/core';


@Component({
  selector: 'center-region',
  templateUrl: './center-region.component.html',
  styleUrls: ['./center-region.component.css']
})
export class CenterRegionComponent implements OnInit {

  shows: Shows[];

  constructor(private showsService: ShowsService)
   {
   }

  ngOnInit() {
    this.getShows();
  }

  getShows(): void {
    this.showsService.getShows()
      .subscribe(theatres => this.shows = theatres);
  }
}
