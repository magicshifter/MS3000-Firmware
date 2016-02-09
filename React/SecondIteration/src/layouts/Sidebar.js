import React, {PropTypes} from 'react';

import classes from './Sidebar.scss';

export const Sidebar =
  ({children}) => (
    <aside className={classes['container']}>
      {children}
    </aside>
  );

Sidebar.propTypes = {
  children: PropTypes.element.isRequired,
};

export default Sidebar;
