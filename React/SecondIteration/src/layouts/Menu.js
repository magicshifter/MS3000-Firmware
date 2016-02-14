import React from 'react';

import MenuLink from './MenuLink.js';

import classes from './Menu.scss';

import { links } from 'GLOBALS';

export const Menu =
  () =>
    <nav className={classes['container']}>
      <ul>
        {links.map(
          link =>
            <MenuLink
              {...link}
              key={link.key || link.text || link.icon}
            />
        )}
      </ul>
    </nav>;

export default Menu;
