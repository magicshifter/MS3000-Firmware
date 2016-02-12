import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import ColorIndicator from './ColorIndicator';

import {actions as colorListActions} from 'redux/modules/colorList';
import {actions as imageActions} from 'redux/modules/views/image';

import {colorType} from 'utils/propTypes';

import classes from './ColorList.scss';

const actions = {
  ...colorListActions,
  ...imageActions,
};

const mapStateToProps =
  state => {
    const { color } = state.imageView.toJS();
    const colors = state.colorList.toArray().map(color => color.toObject());

    return {
      colors,
      uiColor: color,
    };
  };

export class ColorList extends Component {
  static propTypes = {
    addColor: PropTypes.func.isRequired, // action
    removeColor: PropTypes.func.isRequired, // action
    setColor: PropTypes.func.isRequired, // action

    colors: PropTypes.arrayOf(colorType).isRequired,
    uiColor: colorType.isRequired,
  };

  render() {
    const {colors, setColor, removeColor} = this.props;

    return (
      <div className={classes['container']}>
        <ul>
          {
            Object.keys(colors).map(
              key =>
                <ColorIndicator
                  colorId={key}
                  key={key}
                  color={colors[key]}
                  removeColor={removeColor}
                  setColor={setColor}
                />
            )
          }
        </ul>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(ColorList);

