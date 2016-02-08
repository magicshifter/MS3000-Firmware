import React from 'react';
import {Link} from 'react-router';

// XXX import FileUploadInput from 'components/inputs/FileUploadInput';

import classes from './SecondaryMenu.scss';

import iconClasses from 'styles/icons.scss';

export const SecondaryMenu =
  () => (
    <nav className={classes['container']}>
      <ul>
        <li>
          <Link to='/paint/images'>
            <i
              className={`${iconClasses['icon']} ${iconClasses['image']}`}
            />
            <span>images</span>
          </Link>
        </li>
        <li>
          <Link to='/paint/font'>
            <i
              className={`${iconClasses['icon']} ${iconClasses['font']}`}
            />
            <span>font</span>
          </Link>
        </li>
        <li>
          <Link to='/paint/effects'>
            <i
              className={`${iconClasses['icon']} ${iconClasses['effects']}`}
            />
            <span>effects</span>
          </Link>
        </li>
        <li>
          <Link to='/paint/color'>
            <i
              className={`${iconClasses['icon']} ${iconClasses['color']}`}
            />
            <span>color</span>
          </Link>
        </li>
        <li>
          <Link to='/paint/upload'>
            <i
              className={`${iconClasses['icon']} ${iconClasses['upload']}`}
            />
            <span>upload</span>
          </Link>
        </li>
      </ul>
    </nav>
  );

export default SecondaryMenu;
