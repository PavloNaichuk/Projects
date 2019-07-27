import { TestBed } from '@angular/core/testing';

import { SellersService } from './sellers.service';

describe('SellersService', () => {
  beforeEach(() => TestBed.configureTestingModule({}));

  it('should be created', () => {
    const service: SellersService = TestBed.get(SellersService);
    expect(service).toBeTruthy();
  });
});
