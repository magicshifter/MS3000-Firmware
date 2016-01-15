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
    const updateUrl = 'http://magicshifter.net/ms3000/update';

    return (
      <div className='container text-center'>
        <h1>Welcome to the MagicShifter 3000 Web Interface!</h1>
        <br/>
        Although we did our best to make this first beta firmware as good as possible we also know that it is far from being finished. 
        The new MS3000 hardware allows so many new use cases for connected apps that it will take us some time to implement our vision of the perfect MS3000 user interface one feature at a time.
        Expect the first firmware update in February 2016. When it's done you can always find the latest firmware (and instructions on how to update) at: <a href={updateUrl}>{updateUrl}</a>

        <br/>
        <br/>
        Greetings
        <br />The MagicShifter Team!
      {/*
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
      {*/}
      </div>

    );
  }
}

export default connect(mapStateToProps, actions)(AboutView);
