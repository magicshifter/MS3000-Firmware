import React, {Component} from 'react';
import {connect} from 'react-redux';

import FloatingButton from 'components/inputs/FloatingButton';
import PixelEditorMoveMenu from './PixelEditorMoveMenu';

import {actions} from 'redux/modules/layout';

import classes from './PixelEditorMenu.scss';

const mapStateToProps =
  state => {
    const {layout} = state;

    const {zoomIn, zoomOut} = layout.toJS();

    return {
      zoomIn,
      zoomOut,
    };
  };

export class PixelEditorMenu extends Component {

  render() {
    const {zoomIn, zoomOut} = this.props;

    return (
      <nav className={classes['container']}>
        <ul>
          <li>
            <FloatingButton
              text='+'
              onClick={zoomIn}
            />
          </li>
          <li>
            <FloatingButton
              text='-'
              onClick={zoomOut}
            />
          </li>
          <li>
            <PixelEditorMoveMenu />
          </li>
        </ul>
      </nav>
    );
  }
}

export default connect(mapStateToProps, actions)(PixelEditorMenu);

