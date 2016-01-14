import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element.isRequired,
    windowResize: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
  }

  render() {
    const {children} = this.props;

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

export default connect(() => ({}), actions)(CoreLayout);
