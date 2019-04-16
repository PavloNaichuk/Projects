import { Observable } from 'rxjs';

export abstract class UserApi {
    signIn: (email: string, password: string) => Observable<any>;
    signOut: () => Observable<any>;
}