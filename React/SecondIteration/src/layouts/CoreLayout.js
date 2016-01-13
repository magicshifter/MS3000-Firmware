import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/layout';

import Header from './Header';

import {layoutType} from 'utils/propTypes';

import 'styles/core.scss';

import classes from './CoreLayout.scss';

const mapStateToProps = (state) => {
  const {layout} = state;
  return {
    layout: layout.toJS(),
  };
};

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    layout: layoutType,
    windowResize: PropTypes.func.isRequired,
  };

  constructor(props) {
    super(props);

    window.addEventListener('resize', props.windowResize);
  }

  render() {
    const {children, layout} = this.props;
    const {header} = layout;
    const {height} = header;

    return (
      <div className={classes['container']}>
        <Header
          height={height}
        />
        <div className={classes['view']}>
          {children}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
