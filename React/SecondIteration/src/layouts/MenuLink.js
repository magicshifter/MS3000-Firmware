import React, {PropTypes} from 'react';
import {Link, IndexLink} from 'react-router';

import {getIconCssClass} from 'utils/icons';

import classes from './MenuLink.scss';

export const MenuLink =
  ({text, to, icon, style}) =>
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
                className={getIconCssClass(icon)}
                style={style}
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
                className={getIconCssClass(icon)}
                style={style}
              />

              {text && <span>{text}</span>}
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
