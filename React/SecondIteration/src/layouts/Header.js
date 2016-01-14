import React, {PropTypes} from 'react';
import {Link} from 'react-router';

import Menu from './Menu';

import classes from './Header.scss';

export const Header =
  ({height}) => (
    <header
      className={classes['main']}
      style={{
        height,
      }}
    >
      <Link to='/' className={classes['container']}>
        <span className={classes['logo']} />
        <h3>MagicShifter 3000</h3>
      </Link>

      <Menu />
    </header>
  );

Header.propTypes = {
  height: PropTypes.number.isRequired,
};

export default Header;
