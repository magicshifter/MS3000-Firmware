// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';
import {Link} from 'react-router';

import App from './App';
import Header from './Header';
import PixelEditor from './PixelEditor/PixelEditor';
import NoMatch from './NoMatch';

@Radium
export default class Root extends Component {

  constructor(props) {
    super(props);

    this.state = {
      text: props.text,
      clicked: 0,
    };

    this.styles = {
      body: {
        fontFamily: 'Ubuntu, "Lucida Grande", "Segoe UI", sans-serif',
        fontSize: '0.8em',
      },
    };
  }

  render() {
    return (
      <div
        className='root'
        style={this.styles.body}
      >
        <Header />
        <div className='active-component'>
          {/* load react-router components */}
          {this.props.children}
        </div>
      </div>
    );
  }
}
