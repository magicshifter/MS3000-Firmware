import React, { PropTypes } from 'react';
import { Link } from 'react-router';

import classes from './Sidebar.scss';

export const Sidebar =
  ({children}) => (
    <aside className={classes['container']}>
      <Link
        to='/'
        className={classes['clicker']}
      />
      <div className={classes['children']}>
        {children}
      </div>
    </aside>
  );

Sidebar.propTypes = {
  children: PropTypes.element.isRequired,
};

export default Sidebar;
