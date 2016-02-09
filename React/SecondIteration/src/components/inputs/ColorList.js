import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import Immutable from 'immutable';

import ColorIndicator from './ColorIndicator';

import {getIconCssClass} from 'utils/icons';

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
    uiColor: colorType.isRequired,
  };

  constructor(props) {
    super(props);

    this.handleAddColorClick = this.handleAddColorClick.bind(this);
  };

  handleAddColorClick(e) {
    const {colors, uiColor, addColor} = this.props;

    const colorExists =
      colors.some(
        c =>
          Immutable.Map(c).equals(Immutable.Map(uiColor))
      );

    if (!colorExists) {
      addColor([...colors, uiColor]);
    }
  };

  render() {
    const {colors, setColor, removeColor} = this.props;

    return (
      <div className={classes['container']}>
        <div className={classes['nav']}>
          <i
            className={getIconCssClass('zoom_in')}
            onClick={this.handleAddColorClick}
          />
        </div>

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

