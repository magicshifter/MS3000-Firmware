import React, { PropTypes } from 'react';
import { getIconCssClass } from 'utils/icons';

import classes from './ColorNav.scss';

export const ColorNav =
  ({ handleAddColorClick }) =>
    <div className={ classes['container'] }>
      <i
        className={ getIconCssClass('zoom_in') }
        onClick={ handleAddColorClick }
      />
    </div>;

ColorNav.propTypes = {
  handleAddColorClick: PropTypes.func.isRequired,
};

export default ColorNav;
