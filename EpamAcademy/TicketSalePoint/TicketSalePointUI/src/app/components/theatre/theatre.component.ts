import { ShowService } from './../../shared/shows/show.service';
import { Show } from './../../shared/shows/show.model';
import { Component, OnInit } from '@angular/core';
import { Router, ActivatedRoute, NavigationEnd, ActivationEnd } from '@angular/router';

@Component({
  selector: 'theatre',
  templateUrl: './theatre.component.html',
  styleUrls: ['./theatre.component.css']
})
export class TheatreComponent implements OnInit {
  shows: Show[];
  id: number;

  constructor(
    private route: ActivatedRoute,
    private router: Router,
    private showsService: ShowService
    )
   {
    this.router.events.subscribe(e => {
      if (e instanceof ActivationEnd) {
        this.id = e.snapshot.params.id;
        this.getShowWithTheatre();
      }


    })
   }

  ngOnInit() {
    this.id = this.route.snapshot.params.id;
    // this.getShows();
    this.getShowWithTheatre();
  }

  getShows(): void {
    this.showsService.getShows()
      .subscribe(shows => this.shows = shows);
  }

  getShowWithTheatre(): void {
    this.showsService.getShowWithTheatre(this.id)
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
