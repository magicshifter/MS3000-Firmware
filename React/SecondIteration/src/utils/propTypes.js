import {PropTypes} from 'react';

import {isHexString} from 'utils/types';

/*
 * Color Types
 */

export const hexStringType =
  (props, propName, componentName) => {
    const prop = props[propName];
    const isHex = isHexString(prop);
    if (!isHex) {
      return new Error(`Color is not a hex string: ${prop}`);
    }
  };

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

/*
 * Pixel PropTypes
 */

export const pixelType = PropTypes.shape({
  color: colorType.isRequired,
  row: PropTypes.number.isRequired,
  column: PropTypes.number.isRequired,
  id: PropTypes.number.isRequired,
  visible: PropTypes.bool.isRequired,
});

export const pixelsType = PropTypes.arrayOf(pixelType);

/*
 * ImageView PropTypes
 */

export const imageViewType = PropTypes.shape({
  pixels: PropTypes.array.isRequired,
  rows: PropTypes.number.isRequired,
  visibleColumns: PropTypes.number.isRequired,
  totalColumns: PropTypes.number.isRequired,
  color: colorType.isRequired,
}).isRequired;

/*
 * TextView PropTypes
 */

export const fontType = PropTypes.shape({
  name: PropTypes.string.isRequired,
});

export const textViewType = PropTypes.shape({
  text: PropTypes.string,
  fontId: PropTypes.number,
  fonts: PropTypes.arrayOf(fontType).isRequired,
}).isRequired;

/*
 * SettingsView PropTypes
 */

export const settingsViewType = PropTypes.shape({
  protocol: PropTypes.string.isRequired,
  host: PropTypes.string.isRequired,
}).isRequired;
