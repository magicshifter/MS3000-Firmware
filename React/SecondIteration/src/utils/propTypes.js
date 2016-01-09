import {PropTypes} from 'react';

export const colorType = PropTypes.shape({
  r: PropTypes.number,
  b: PropTypes.number,
  g: PropTypes.number,
  a: PropTypes.number,
});

export const colorNames = {
  r: 'Red',
  g: 'Green',
  b: 'Blue',
  a: 'Alpha',
};
