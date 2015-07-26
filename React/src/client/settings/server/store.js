import * as actions from './actions';
import {serverSettingsCursor} from '../../state';
import {register} from '../../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.saveFormError:
      serverSettingsCursor(serverSettings => {
        const error = data;
        return serverSettings.setIn(['form', 'error'], error);
      });
      break;

    case actions.updateFormField:
      serverSettingsCursor(serverSettings => {
        const {name, value} = data;
        return serverSettings.setIn(['form', 'fields', name], value);
      });
      break;
  }

});
