// need to import react here
import React, {Component, PropTypes} from 'react';

import Radium from 'radium';

import {Link} from 'react-router';

@Radium
export default class NoMatch extends Component {

  constructor(props) {
    super(props);

    
  }

  render() {

    const style = {
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

    const links = [
      {to: '/', text: 'about',},
      {to: '/pixels', text: 'pixeleditor',},
    ];

    return (
      <nav
        className='main'
        style={style.nav}
      >
        <ul style={style.ul}>
          {links.map(link => {
            const {to, text, key} = link;
            return (
              <li
                style={style.li}
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
