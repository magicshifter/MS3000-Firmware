import * as actions from './actions';
import * as ledActions from '../leds/actions';
import {colorListCursor} from '../state';
import {register} from '../dispatcher';

export const dispatchToken = register(({action, data}) => {

  switch (action) {
    case actions.saveColor:
      colorListCursor(cursor => {
        let colors = cursor.get('colors');
        if (colors.indexOf(data) < 0) {
          colors = colors.push(data);
        }

        return cursor.set('colors', colors);
      });

      break;

    case actions.removeColor:
      colorListCursor(cursor => {
        let colors = cursor
                      .get('colors')
                      .filter((val) => val !== data);
        return cursor.set('colors', colors);
      });

      break;
  }
});
