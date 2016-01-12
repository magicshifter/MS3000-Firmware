import React, {PropTypes, Component} from 'react';
import {connect} from 'react-redux';
import {Link} from 'react-router';

import {actions} from 'redux/modules/counter';
import classes from './AboutView.scss';

// We define mapStateToProps where we'd normally use
// the @connect decorator so the data requirements are clear upfront, but then
// export the decorated component after the main class definition so
// the component can be tested w/ and w/o being connected.
// See: http://rackt.github.io/redux/docs/recipes/WritingTests.html
const mapStateToProps = (state) => state.counter.toJS();

export class AboutView extends Component {
  static propTypes = {
    count: PropTypes.number.isRequired,
    increment: PropTypes.func.isRequired,
    decrement: PropTypes.func.isRequired,
  };

  render() {
    const {count, increment, decrement} = this.props;

    return (
      <div className='container text-center'>
        <h1>Welcome to the MagicShifter 3000 UI</h1>
        <h2>
          Sample Counter:&nbsp;
          <span className={classes['counter--green']}>{count}</span>
        </h2>

        <button className='btn btn-default'
                onClick={() => decrement(1)}
        >
          Decrement
        </button>
        <button className='btn btn-default'
                onClick={() => increment(1)}
        >
          Increment
        </button>
        <hr />
        <Link to='/404'>Go to 404 Page</Link>
      </div>
    );
  }
}

export default connect(mapStateToProps, actions)(AboutView);
