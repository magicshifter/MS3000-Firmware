import * as actions from './actions';
import {apSettingsCursor} from '../../state';
import {register} from '../../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.saveFormError:
      apSettingsCursor(ap => {
        const error = data;
        return ap.setIn(['form', 'error'], error);
      });
      break;

    case actions.updateFormField:
      apSettingsCursor(ap => {
        const {name, value} = data;
        return ap.setIn(['form', 'fields', name], value);
      });
      break;
  }

});
