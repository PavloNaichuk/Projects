import { Status } from './status.model';
import { Show } from './../shows/show.model';
export class Ticket {
  id: number;
  row: number;
  seat: number;
  status: Status;
  priceInCents: number;
  show: Show;
}
