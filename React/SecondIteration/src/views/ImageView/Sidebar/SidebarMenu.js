import React from 'react';
import {Link} from 'react-router';

import classes from './SidebarMenu.scss';

export const SidebarMenu =
  () => (
    <nav className={classes['container']}>
      <ul>
        <li><Link to='/'>controls</Link></li>
        <li><Link to='/paint/images'>images</Link></li>
        <li><Link to='/paint/text'>text</Link></li>
        <li><Link to='/paint/filters'>filters</Link></li>
        <li><Link to='/paint/settings'>settings</Link></li>
      </ul>
    </nav>
  );

export default SidebarMenu;
