// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';
import {Link} from 'react-router';

import App from './App';
import Menu from './Menu';
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
      header: {
        display: 'inline-block',
        width: '100%',
      },
      logoContainer: {
        float: 'left',
      },
      logo: {
        float: 'left',
      },
      title: {
        float: 'left',
        margin: '.8em 0 0 .4em',
      },
    };
  }

  render() {
    return (
      <div
        className='root'
        style={this.styles.body}
      >
        <header
          className='main'
          style={this.styles.header}
        >
          <div
            className='logoContainer'
            style={this.styles.logoContainer}
          >
            <Link
              to='/'
            >
              <img
                className='headerLogo'
                style={this.styles.logo}
                src='/img/logo.png'
                height='40'
                width='40'
              />
              <h3
                style={this.styles.title}
              >
                MagicShifter 3000
              </h3>
            </Link>
          </div>
          <Menu />
        </header>
        <div className='active-component'>
          {/* load react-router components */}
          {this.props.children}
        </div>
      </div>
    );
  }
}
