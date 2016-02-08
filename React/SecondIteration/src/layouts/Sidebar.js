import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const Sidebar =
  ({main, footer, width}) =>
    <aside
      className={classes['container']}
      style={{
        width,
      }}
    >
      {main}
      {footer}
    </aside>;

Sidebar.propTypes = {
  main: PropTypes.element.isRequired,
  footer: PropTypes.element.isRequired,
  width: PropTypes.number.isRequired,
};

export default Sidebar;
