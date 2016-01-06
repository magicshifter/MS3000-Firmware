// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

@Radium
export default class App extends Component {

  constructor(props) {
    super(props);
  }

  render() {

    const style = {
      color: 'green',

      ':hover': {
        color: 'orange',
      },
    };

    return (
      <div
        className='about-page'
        style={style}
      >
        <h3>Welcome to the MagicShifter 3000 UserInterface</h3>
        <div>
          <p>
            This page will soon contain a description of features and usage instructions.
          </p>
        </div>
      </div>
    );
  }
}
