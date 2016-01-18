import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const sidebar =
  ({menu, main}) =>
    <aside className={classes['aside']}>
      {menu}
      {main}
    </aside>;

sidebar.propTypes = {
  menu: PropTypes.element.isRequired,
  main: PropTypes.element.isRequired,
};

export default sidebar;
