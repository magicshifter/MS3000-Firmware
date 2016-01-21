import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import rgba from 'rgba-convert';
import Immutable from 'immutable';

import {actions as colorListActions} from 'redux/modules/colorList';
import {actions as imageActions} from 'redux/modules/views/image';

import FloatingButton from './FloatingButton';

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

          <FloatingButton
            title='remove color'
            cssClass={classes['remove']}
            onClick={() => removeColor(key)}
          >
            x
          </FloatingButton>
        </li>
      )
    );
  }

  render() {
    const {addColor, editorColor, colors} = this.props;

    return (
      <div className={classes['container']}>
        <div className={classes['add']}>
          <input
            type='button'
            value='save current color'
            onClick={
              () =>
                !colors.some(c => Immutable.Map(c).equals(Immutable.Map(editorColor))) &&
                addColor([...colors, editorColor])
            }
          />
        </div>

        <ul>{this.renderColorList()}</ul>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(ColorList);

