import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

const mapStateToProps =
  (state) => (
    {
      height: state.layout.height,
      width: state.layout.width,
    }
  );

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    height: PropTypes.number,
    width: PropTypes.number,
    windowResize: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
  }

  render() {
    const {children, height, width} = this.props;
    console.log('render corelayout');

    return (
      <div
        className={classes['container']}
        style={{
          height,
          width,
        }}
      >
        <Header />
        <div className={classes['view']}>
          {children}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
