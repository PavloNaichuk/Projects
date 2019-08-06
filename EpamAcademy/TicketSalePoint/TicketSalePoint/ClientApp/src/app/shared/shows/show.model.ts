import { Theatre } from "../theatres/theatre.model";

export class Show {
  id: number;
  name: string;
  date: Date;
  theatreId: number;
  theatre: Theatre;
}
