import React, { PropTypes } from 'react';
import { Link, IndexLink } from 'react-router';
import rgba from 'rgba-convert';

import { getIconCssClass } from 'utils/icons';
import { isColor } from 'utils/types';

import classes from './MenuLink.scss';

export const MenuLink =
  ({ text, to, icon, style, color }) => {
    if (isColor(color) && icon === 'colors') {
      style = {
        color: rgba.css(color),
      };
    }

    return (
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
      </li>
    );
  };

MenuLink.propTypes = {
  text: PropTypes.string,
  icon: PropTypes.string,
  to: PropTypes.string.isRequired,
};

export default MenuLink;
