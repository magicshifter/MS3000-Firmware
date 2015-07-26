import * as actions from './actions';
import {accesspointSettingsCursor} from '../../state';
import {register} from '../../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.saveFormError:
      accesspointSettingsCursor(ap => {
        const error = data;
        return ap.setIn(['form', 'error'], error);
      });
      break;

    case actions.updateFormField:
      accesspointSettingsCursor(ap => {
        const {name, value} = data;
        return ap.setIn(['form', 'fields', name], value);
      });
      break;
  }

});
