import { SET_COLOR, SET_COLOR_VALUE } from '../actions/pixelEditor';

import GLOBALS from '../GLOBALS';

export default function pixelEditor(state, action) {
  switch (action.type) {
    case SET_COLOR:
      const {color} = action;
      return state.setIn(['pixelEditor', 'color'], color);

    case SET_COLOR_VALUE:
      const {value, name} = action;
      return state.setIn(['pixelEditor', 'color', name], value);

    default:
      return state
  }
}
