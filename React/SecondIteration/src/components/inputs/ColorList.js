import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import rgba from 'rgba-convert';

import {actions as colorListActions} from 'redux/modules/colorList';
import {actions as imageActions} from 'redux/modules/views/image';

import {colorType} from 'utils/propTypes';

import classes from './ColorList.scss';

const actions = {
  ...colorListActions,
  ...imageActions,
};

const mapStateToProps =
  state => ({
    colors: state.colorList.toArray().map(color => color.toObject()),
  });

export class ColorList extends Component {
  static propTypes = {
    addColor: PropTypes.func.isRequired, // action
    removeColor: PropTypes.func.isRequired, // action
    setColor: PropTypes.func.isRequired, // action

    colors: PropTypes.arrayOf(colorType).isRequired,
    editorColor: colorType.isRequired,
  };

  renderColorList(key, color) {
    const {colors, removeColor, setColor} = this.props;

    return Object.keys(colors).map(
      key => (
        <li key={key}>
          <div className={classes['indicator']}
            style={{
              backgroundColor: rgba.css(colors[key]),
            }}
            onClick={() => setColor({color: colors[key]})}
          />

          <input
            type='button'
            value='x'
            title='remove color'
            className={classes['remove']}
            onClick={() => removeColor(key)}
          />
        </li>
      )
    );
  }

  render() {
    const {addColor, editorColor} = this.props;
    return (
      <div className={classes['container']}>
        <div className={classes['add']}>
          <input
            type='button'
            value='save current color'
            onClick={() => addColor(editorColor)}
          />
        </div>

        <ul>{this.renderColorList()}</ul>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(ColorList);

