import * as authActions from '../auth/actions';
import {register} from '../dispatcher';
import User from './user';
import {usersCursor} from '../state';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case authActions.login:
      usersCursor(users => {
        const user = data;
        return users.set('viewer', new User(user));
      });
      break;
  }

});
