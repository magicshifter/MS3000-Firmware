import * as actions from './actions';
import {ledsCursor} from '../state';
import {register} from '../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.toggleLed:
      ledsCursor(settings => {
        const {id, active} = data;
        return settings.setIn(['list', id, 'active'], !active);
      });

      break;

    case actions.changeLed:
      const {id, value} = data;
      ledsCursor(settings => {
        const list = settings.get('list').map((val, key) => {
          console.log('key', key, 'val', val);
          if (val.get('active')) {
            settings.get('list').updateIn([key, 'value'], value);
          }
        });

        return settings
          .setIn(['list'], list);
      });

      break;
  }

});
