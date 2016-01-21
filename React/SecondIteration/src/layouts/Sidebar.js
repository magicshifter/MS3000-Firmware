import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const sidebar =
  ({menu, main, footer, width}) =>
    <aside
      className={classes['aside']}
      style={{
        width,
      }}
    >
      {menu}
      {main}
      {footer}
    </aside>;

sidebar.propTypes = {
  menu: PropTypes.element.isRequired,
  main: PropTypes.element.isRequired,
};

export default sidebar;
