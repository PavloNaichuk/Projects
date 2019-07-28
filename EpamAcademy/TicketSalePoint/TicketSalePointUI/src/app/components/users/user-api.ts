import { Observable } from 'rxjs';

export abstract class UserApi {
  singIn: (username: string, password: string) => Observable<any>;
  signOut: () => Observable<any>;
}
