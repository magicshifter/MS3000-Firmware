import React, {PropTypes} from 'react';
import {Link} from 'react-router';

import {layoutType} from 'utils/propTypes';
import Menu from './Menu';

import classes from './Header.scss';

export const Header =
  ({height, layout}) => (
    <header
      className={classes['main']}
      style={{
        height,
      }}
    >
      <Link to='/' className={classes['container']}>
        <span
          className={classes['logo']}
        />
        <h3
          style={{
            display: layout.width > 500 ? 'inherit' : 'none'
          }}
        >
          MagicShifter 3000
        </h3>
      </Link>

      <Menu />
    </header>
  );

Header.propTypes = {
  height: PropTypes.number.isRequired,
  layout: layoutType,
};

export default Header;
