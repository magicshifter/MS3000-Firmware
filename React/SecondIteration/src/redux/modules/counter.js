import {createAction, handleActions} from 'redux-actions';

// ------------------------------------
// Constants
// ------------------------------------
export const COUNTER_INCREMENT = 'COUNTER_INCREMENT';
export const COUNTER_DECREMENT = 'COUNTER_DECREMENT';

// ------------------------------------
// Actions
// ------------------------------------
export const increment = createAction(COUNTER_INCREMENT, (value = 1) => value);
export const decrement = createAction(COUNTER_DECREMENT, (value = 1) => value);

export const actions = {
  increment,
  decrement,
};

// ------------------------------------
// Reducer
// ------------------------------------
export default handleActions({
  [COUNTER_INCREMENT]: (state, {payload}) => state + payload,
  [COUNTER_DECREMENT]: (state, {payload}) => state - payload,
}, 1);
