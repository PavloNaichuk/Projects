import { ShowService } from './../../shared/shows/show.service';
import { Show } from '../../shared/shows/show.model';
import { Component, OnInit, Input } from '@angular/core';


@Component({
  selector: 'center-region',
  templateUrl: './center-region.component.html',
  styleUrls: ['./center-region.component.css']
})
export class CenterRegionComponent implements OnInit {
  shows: Show[];
  constructor(private showsService: ShowService)
   {
   }

  ngOnInit() {
    this.getShows();
  }

  getShows(): void {
    this.showsService.getShows()
      .subscribe(shows => this.shows = shows);
  }

  formatedDate(id : number) {
    let formatedDate = new Date(this.shows[id].date);
    let day = formatedDate.getDate();
    let month = formatedDate.getMonth() + 1;
    let year = formatedDate.getFullYear();
    return [day > 9 ? day : '0' + day, month > 9 ? month: '0' + month, year].join('-');
  }

}
