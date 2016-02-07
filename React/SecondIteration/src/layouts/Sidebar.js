import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const sidebar =
  ({main, footer, width}) =>
    <aside
      className={classes['aside']}
      style={{
        width,
      }}
    >
      {main}
      {footer}
    </aside>;

sidebar.propTypes = {
  main: PropTypes.element.isRequired,
  footer: PropTypes.element.isRequired,
  width: PropTypes.number.isRequired,
};

export default sidebar;
