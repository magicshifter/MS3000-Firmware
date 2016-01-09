import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {actions} from 'redux/modules/window';

import 'styles/core.scss';
import Header from './Header';

const mapStateToProps = (state) => ({
  height: state.win.height,
  width: state.win.width,
});

class CoreLayout extends Component {
  static propTypes = {
    children: PropTypes.element,
    height: PropTypes.number,
    width: PropTypes.number,
  };

  render() {
    const {children} = this.props;

    return (
      <div className='page-container'>
        <Header />
        <div className='view-container'>
          {children}
        </div>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(CoreLayout);
