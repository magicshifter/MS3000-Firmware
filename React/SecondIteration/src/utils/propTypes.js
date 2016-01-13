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

export const pixelType = PropTypes.shape({
  color: colorType.isRequired,
  row: PropTypes.number.isRequired,
  column: PropTypes.number.isRequired,
  id: PropTypes.number.isRequired,
  visible: PropTypes.bool.isRequired,
});

export const pixelsType = PropTypes.arrayOf(pixelType);

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

export const headerType = PropTypes.shape({
  height: PropTypes.number.isRequired,
}).isRequired;

export const sidebarType = PropTypes.shape({
  width: PropTypes.number.isRequired,
}).isRequired;

export const layoutType = PropTypes.shape({
  height: PropTypes.number.isRequired,
  width: PropTypes.number.isRequired,
  header: headerType,
  sidebar: sidebarType,
}).isRequired;
