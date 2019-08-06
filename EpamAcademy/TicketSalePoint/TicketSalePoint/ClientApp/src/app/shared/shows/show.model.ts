import { Theatre } from "../theatres/theatre.model";

export class Show {
  id: number;
  name: string;
  date: string;
  theatreId: number;
  theatre: Theatre;
}
