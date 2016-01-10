import React from 'react';
import {Link} from 'react-router';

import classes from './Menu.scss';

export const Menu = () => {
  const links = [
    {to: '/', text: 'pixeleditor'},
    {to: '/about', text: 'about'},
    {to: '/settings', text: 'settings'},
  ];

  return (
    <nav className={classes['main']}>
      <ul>
        {links.map(link => (
          <li key={link.key || link.text}>
            <Link to={link.to} activeClassName={classes['active']}>{link.text}</Link>
          </li>
        ))}
      </ul>
    </nav>
  );
};

export default Menu;
