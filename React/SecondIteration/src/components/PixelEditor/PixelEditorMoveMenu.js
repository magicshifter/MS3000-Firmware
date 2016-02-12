import React from 'react';

// XXX import FloatingButton from 'components/inputs/FloatingButton';

// XXX import {actions} from 'redux/modules/layout';

import classes from './PixelEditorMoveMenu.scss';

export const PixelEditorMoveMenu =
  () => (
    <ul className={classes['container']}>
      {/*
      <li className={classes['btn']}>
        <FloatingButton
          text='<<'
          onClick={actions.moveLeft}
        />
      </li>
      <li className={classes['btn']}>
        <ul>
          <li className={classes['sub_btn']}>
            <FloatingButton
              text='^'
              onClick={actions.moveUp}
            />
          </li>
          <li className={classes['sub_btn']}>
            <FloatingButton
              text='v'
              onClick={actions.moveDown}
            />
          </li>
        </ul>
      </li>
      <li className={classes['btn']}>
        <FloatingButton
          text='>>'
          onClick={actions.moveRight}
        />
      </li>
      */}
    </ul>
  );

export default PixelEditorMoveMenu;
