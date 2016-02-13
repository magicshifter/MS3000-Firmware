import React, {PropTypes} from 'react';
import rgba from 'rgba-convert';

import {getIconCssClass} from 'utils/icons';

import {colorType} from 'utils/propTypes';

export const ColorIndicator =
  ({color, setColor}) =>
    <i
      className={getIconCssClass('colors')}
      style={{
        color: rgba.css(color),
      }}
      onClick={() => setColor({color})}
    />;

ColorIndicator.propTypes = {
  color: colorType.isRequired,
  setColor: PropTypes.func.isRequired,
};

export default ColorIndicator;
