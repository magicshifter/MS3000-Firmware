import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const Sidebar =
  ({main, width}) =>
    <aside
      className={classes['container']}
      style={{
        width,
      }}
    >
      {main}
    </aside>;

Sidebar.propTypes = {
  main: PropTypes.element.isRequired,
  width: PropTypes.number.isRequired,
};

export default Sidebar;
