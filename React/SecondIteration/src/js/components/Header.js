// need to import react here
import React, {Component, PropTypes} from 'react';
import Radium from 'radium';
import {Link} from 'react-router';

import Menu from './Menu';

@Radium
export default class Header extends Component {

  constructor(props) {
    super(props);

    this.styles = {
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
    );
  }
}
