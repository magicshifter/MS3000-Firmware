import React, {PropTypes} from 'react';
import rgba from 'rgba-convert';

import {getIconCssClass} from 'utils/icons';

import {colorType} from 'utils/propTypes';

export const ColorIndicator =
  ({colorId, color, removeColor, setColor}) =>
    <li>
      <i
        className={getIconCssClass('colors')}
        style={{
          color: rgba.css(color),
        }}
        onClick={() => setColor({color})}
      />

      <i
        title='remove color'
        className={getIconCssClass('trash')}
        onClick={() => removeColor(colorId)}
      />
    </li>;

ColorIndicator.propTypes = {
  color: colorType.isRequired,
  removeColor: PropTypes.func.isRequired,
  setColor: PropTypes.func.isRequired,
};

export default ColorIndicator;
