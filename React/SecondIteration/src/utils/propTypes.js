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

export const pixelEditorType = PropTypes.shape({
  pixels: PropTypes.array.isRequired,
  rows: PropTypes.number.isRequired,
  visibleColumns: PropTypes.number.isRequired,
  totalColumns: PropTypes.number.isRequired,
  color: colorType.isRequired,
}).isRequired;

export const settingsType = PropTypes.shape({
  protocol: PropTypes.string.isRequired,
  host: PropTypes.string.isRequired,
}).isRequired;

export const layoutType = PropTypes.shape({
  height: PropTypes.number.isRequired,
  width: PropTypes.number.isRequired,
  header: PropTypes.shape({
    height: PropTypes.number.isRequired,
  }).isRequired,
  sidebar: PropTypes.shape({
    width: PropTypes.number.isRequired,
  }).isRequired,
}).isRequired;
