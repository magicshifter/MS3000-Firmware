import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';

import {actions} from 'redux/modules/views/image';

import {colorType, layoutType} from 'utils/propTypes';

import RGBAInput from 'components/inputs/RGBAInput';
import ColorList from 'components/inputs/ColorList';

import classes from './Color.scss';

const mapStateToProps = (state) => {
  const {imageView, layout} = state;
  const color = imageView.get('color').toObject();
  return {
    color,
    layout: layout.toJS(),
  };
};

export class Controls extends Component {
  static propTypes = {
    setColorValue: PropTypes.func.isRequired, // action
    color: colorType.isRequired,
    layout: layoutType,
  };

  render() {
    const {
      setColorValue, // action
      color,
      layout,
    } = this.props;

    const style = {
      rgba: {},
    };

    if (layout.width < 350) {
      style.rgba = {
        position: 'fixed',
        bottom: 0,
        left: '1em',
      };
    }

    return (
      <div
        className={classes['container']}
        style={style.rgba}
      >
        <h5>color</h5>
        <RGBAInput
          color={color}
          setColorValue={setColorValue}
        />

        <ColorList
          editorColor={color}
        />
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(Controls);
