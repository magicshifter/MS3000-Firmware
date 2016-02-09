import React, {PropTypes} from 'react';
import {Link, IndexLink} from 'react-router';

import classes from './MenuLink.scss';
import iconClasses from 'styles/icons.scss';

function cssClass(text) {
  return [
    iconClasses['icon'],
    iconClasses[text],
  ].join(' ');
}

export const MenuLink =
  ({text, to, icon}) =>
    <li className={classes['container']}>
      {
        to === '/'
        ? (
            <IndexLink
              to={to}
              activeClassName={classes['active']}
              title={text}
            >
              <i
                className={cssClass(icon)}
              />
            </IndexLink>
          )
        : (
            <Link
              to={to}
              activeClassName={classes['active']}
              title={text}
            >
              <i
                className={cssClass(icon)}
              />

              {text &&
                <span>{text}</span>
              }
            </Link>
          )
      }
    </li>;

MenuLink.propTypes = {
  text: PropTypes.string,
  icon: PropTypes.string,
  to: PropTypes.string.isRequired,
};

export default MenuLink;
