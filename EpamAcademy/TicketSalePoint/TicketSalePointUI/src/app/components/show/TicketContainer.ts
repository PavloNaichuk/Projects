import { Graphics, Text, Container, TextStyle } from 'pixi.js';
import { Ticket } from '../../shared/tickets/ticket.model';

export default class TicketContainer extends Container {
  seatsInfo: Ticket;
  info: Graphics;
  ticketText: Text;
  ticketBackground: Graphics;
  infoBox: Graphics;
  reserved: boolean;

  WIDTH: number;
  HEIGHT: number;

  constructor(seatsInfo: Ticket) {
    super();

    this.interactive = true;
    this.buttonMode = true;

    this.WIDTH = 30;
    this.HEIGHT = 30;
    this.seatsInfo = seatsInfo;

    this._initTicket();
  }

  _initTicket() {
    const x = +this.seatsInfo.seat * (this.WIDTH + 10);
    const y = +this.seatsInfo.row * (this.HEIGHT + 10);

    this.position.set(x, y);

    this.ticketBackground = this._initBackground();
    this.ticketText = this._initText(this.seatsInfo.seat, {
      x: this.WIDTH / 2,
      y: this.HEIGHT / 2,
      fill: this.seatsInfo.status ? 'black' : 'white'
    });
    this.ticketBackground.addChild(this.ticketText);
    // this.ticketBackground.interactive = true;

    this.addChild(this.ticketBackground);

    this.addEventListeners();
  }

  _initBackground() {
    const graphics = new Graphics();

    graphics.beginFill(this.seatsInfo.status ? 0xEEEEEE : 0x0000FF);
    graphics.drawRoundedRect(0, 0, this.WIDTH, this.HEIGHT, 5);
    graphics.endFill();

    return graphics;
  }

  _initText(_text, {x, y, fill = "black"}) {
    const text = new Text(_text);

    text.style.fill = fill;
    text.style.fontSize = 14;
    text.anchor.set(.5);
    text.position.set(x, y);

    return text;
  }

  changeColor(color) {
    this.ticketBackground.clear();
    this.ticketBackground.beginFill(color);
    this.ticketBackground.drawRoundedRect(0, 0, this.WIDTH, this.HEIGHT, 5);
    this.ticketBackground.endFill();
  }

  _initInfo() {
    const graphics = new Graphics();

    const startX = this.WIDTH / 2;
    const startY = 0;

    graphics.beginFill(0xFFFFFF);
    graphics.drawPolygon([
      startX, startY,
      startX - 5, startY -10,
      startX - 40, startY -10,
      startX - 40, startY - 70,
      startX + 40, startY - 70,
      startX + 40, startY -10,
      startX + 5, startY -10,
    ]);
    graphics.endFill();

    const desc = this.seat();
    desc.position.set(-startX, -65);
    graphics.addChild(desc);

    return graphics;
  }

  showInfoBox() {
    this.infoBox = this._initInfo();
    this.addChild(this.infoBox);
  }

  hideInfoBox() {
    this.removeChild(this.infoBox);
    this.infoBox = null;
  }

  mouseOver() {
    this.showInfoBox();
    this.ticketBackground.alpha = .5;
  }

  mouseOut() {
    this.hideInfoBox();
    this.ticketBackground.alpha = 1;
  }

  addEventListeners() {
    this.on('mouseover', this.mouseOver);
    this.on('mouseout', this.mouseOut);
  }

  seat() {
    const {row, seat, priceInCents} = this.seatsInfo;
    const textContainer = new Container();
    textContainer.width = 60;
    textContainer.height = 40;

    const propStyle = new TextStyle({
      fontFamily: 'Arial',
      fontSize: 12,
      fontWeight: 'bold',
    });
    const itemStyle = new TextStyle({
      fontFamily: 'Arial',
      fontSize: 12,
      fontStyle: 'italic',
      fontWeight: 'normal',
    });

    const rowContainer = new Container();
    const rowProp = new Text('Row: ', propStyle);
    const rowItem = new Text(String(row), itemStyle);
    rowItem.position.set(50, 0);
    rowContainer.addChild(rowProp);
    rowContainer.addChild(rowItem);

    const seatContainer = new Container();
    seatContainer.position.set(0, 15);
    const seatProp = new Text('Seat: ', propStyle);
    const seatItem = new Text(String(seat), itemStyle);
    seatItem.position.set(50, 0);
    seatContainer.addChild(seatProp);
    seatContainer.addChild(seatItem);

    const priceContainer = new Container();
    priceContainer.position.set(0, 35);
    const priceProp = new Text('Price: ', propStyle);
    const priceItem = new Text(String(priceInCents), itemStyle);
    priceItem.position.set(45, 0);
    priceContainer.addChild(priceProp);
    priceContainer.addChild(priceItem);

    textContainer.addChild(rowContainer);
    textContainer.addChild(seatContainer);
    textContainer.addChild(priceContainer);

    return textContainer;
  }

  redrawBackground() {
    this.changeColor(this.reserved ? 0xDD0000 : 0x0000FF);
  }

}
