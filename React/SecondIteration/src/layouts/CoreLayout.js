import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

const mapStateToProps = (state) => ({
  height: state.layout.height,
  width: state.layout.width,
});

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    height: PropTypes.number,
    width: PropTypes.number,
  };

  render() {
    const {children, height, width} = this.props;

    console.log({width, height});

    return (
      <div className={classes['container']}>
        <Header />
        <div className={classes['view']}>
          {children}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
