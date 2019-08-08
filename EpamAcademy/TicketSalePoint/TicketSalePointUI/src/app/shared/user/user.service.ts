import { User } from './user.model';
import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { config } from 'process';



@Injectable()
export class UserService {
    constructor(private http: HttpClient) { }

    getAll() {
        return this.http.get<User[]>(`${config.variables}/users`);
    }

    getById(id: number) {
        return this.http.get(`${config.variables}/users/` + id);
    }

    register(user: User) {
        return this.http.post(`${config.variables}/users/registration`, user);
    }

    update(user: User) {
        return this.http.put(`${config.apiUrl}/users/` + user.id, user);
    }

    delete(id: number) {
        return this.http.delete(`${config.apiUrl}/users/` + id);
    }
}
