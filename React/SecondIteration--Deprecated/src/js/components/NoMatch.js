// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

@Radium
export default class NoMatch extends Component {

  constructor(props) {
    super(props);
  }

  render() {
    return (
      <div className='nomatch-page'>
        <h3>404 - page not found error</h3>
      </div>
    );
  }
}
