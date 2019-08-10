import { Show } from './../shows/show.model';
export class Ticket {
  id: number;
  row: string;
  seat: string;
  priceInCents: number;
  show: Show;
}
