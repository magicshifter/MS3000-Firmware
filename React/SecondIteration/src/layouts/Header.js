import React from 'react';
import {Link} from 'react-router';

import Menu from './Menu';

import classes from './Header.scss';

export const Header = () => {
  return (
    <header className={classes['main']}>
      <Link to='/' className={classes['container']}>
        <img
          className={classes['logo']}
          src='/img/logo.png'
          height='40'
          width='40'
        />
        <h3>MagicShifter 3000</h3>
      </Link>

      <Menu />
    </header>
  );
};

export default Header;
