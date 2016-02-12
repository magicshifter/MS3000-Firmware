import React from 'react';

import MenuLink from './MenuLink';

import {colorType} from 'utils/propTypes';

import {secondaryLinks} from 'GLOBALS';

import classes from './SecondaryMenu.scss';

export const SecondaryMenu =
  ({color}) =>
    <nav className={classes['container']}>
      <ul>
        {secondaryLinks.map(
          link =>
            <MenuLink
              {...link}
              color={color}
              key={link.key || link.text}
            />
        )}
      </ul>
    </nav>;

SecondaryMenu.propTypes = {
  color: colorType,
};

export default SecondaryMenu;
