import React, { PropTypes } from 'react';

import { getIconCssClass } from 'utils/icons';

export const ColorRemover =
  ({ colorId, removeColor }) =>
    <i
      title='remove color'
      className={getIconCssClass('trash')}
      onClick={() => removeColor(colorId)}
    />;

ColorRemover.propTypes = {
  colorId: PropTypes.string.isRequired,
  removeColor: PropTypes.func.isRequired,
};

export default ColorRemover;
