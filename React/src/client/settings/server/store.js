import * as actions from './actions';
import {serverSettingsCursor} from '../../state';
import {register} from '../../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.saveFormError:
      serverSettingsCursor(cursor => {
        const error = data;
        return cursor.setIn(['form', 'error'], error);
      });
      break;

    case actions.updateFormField:
      serverSettingsCursor(cursor => {
        const {name, value} = data;
        return cursor.setIn(['form', 'fields', name], value);
      });
      break;
  }

});
