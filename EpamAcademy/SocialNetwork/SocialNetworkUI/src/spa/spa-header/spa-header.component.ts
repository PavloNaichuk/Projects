import { Component, OnInit } from '@angular/core';
import { ScreenService } from '../services/screen.service';
import { MenuService } from '../services/menu.service';

@Component({
  selector: 'spa-header',
  templateUrl: './spa-header.component.html',
  styleUrls: ['./spa-header.component.css']
})
export class SpaHeaderComponent implements OnInit {

  constructor(private screenService: ScreenService, private menuService: MenuService) { }

  ngOnInit() {
  }

}
