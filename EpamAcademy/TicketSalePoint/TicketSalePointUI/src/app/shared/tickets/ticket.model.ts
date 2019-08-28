import { Show } from './../shows/show.model';
export class Ticket {
  id: number;
  row: string;
  seat: string;
  status: boolean;
  priceInCents: number;
  show: Show;
}
