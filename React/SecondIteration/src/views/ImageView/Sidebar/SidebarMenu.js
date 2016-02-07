import React from 'react';
import {Link} from 'react-router';

import FloatingButton from 'components/inputs/FloatingButton';

import classes from './SidebarMenu.scss';

export const SidebarMenu =
  () => (
    <nav className={classes['container']}>
      <ul>
        <li><Link to='/'>
          <FloatingButton text='controls' />
        </Link></li>
        <li><Link to='/paint/images'>
          <FloatingButton text='images' />
        </Link></li>
        <li><Link to='/paint/text'>
          <FloatingButton text='text' />
        </Link></li>
        <li><Link to='/paint/filters'>
          <FloatingButton text='filters' />
        </Link></li>
        <li><Link to='/paint/settings'>
          <FloatingButton text='settings' />
        </Link></li>
      </ul>
    </nav>
  );

export default SidebarMenu;
