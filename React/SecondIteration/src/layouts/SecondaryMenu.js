import React from 'react';

import MenuLink from './MenuLink';

import {secondaryLinks} from 'GLOBALS';

import classes from './SecondaryMenu.scss';

export const SecondaryMenu =
  () => (
    <nav className={classes['container']}>
      <ul>
        {secondaryLinks.map(
          link =>
            <MenuLink
              {...link}
              key={link.key || link.text}
            />
        )}
      </ul>
    </nav>
  );

export default SecondaryMenu;
