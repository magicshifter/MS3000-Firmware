// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

import {Link} from 'react-router';

@Radium
export default class NoMatch extends Component {

  constructor(props) {
    super(props);

    
    this.styles = {
      nav: {
        float: 'right',
      },
      ul: {
        listStyle: 'none',
        padding: '0',
        margin: '0',
      },
      li: {
        float: 'left',
        margin: '.5em',
      },
    };
  }

  render() {
    const links = [
      {to: '/', text: 'about',},
      {to: '/pixels', text: 'pixeleditor',},
      {to: '/settings', text: 'settings',},
    ];

    return (
      <nav
        className='main'
        style={this.styles.nav}
      >
        <ul style={this.styles.ul}>
          {links.map(link => {
            const {to, text, key} = link;
            return (
              <li
                style={this.styles.li}
                key={key || text}
              >
                <Link to={to} activeClassName='active'>{text}</Link>
              </li>
            );
          })}
        </ul>
      </nav>
    );
  }
}
