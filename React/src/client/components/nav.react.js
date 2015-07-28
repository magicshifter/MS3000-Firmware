import Component from './component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Item from './nav/item.react';

export default class Nav extends Component {

  render() {
    const styles = {
      nav: {
        float: 'right',
        position: 'relative',
      },
    };

    let i = 0;
    const menuItems = this.props.menuItems.map((to) => {
      i += 1;
      if (typeof to !== 'string') { return to; }

      return <Item key={i} to={to} />;
    });

    return (
      <nav style={styles.nav}>
        <ul>
          {menuItems}
        </ul>
      </nav>
    );
  }
}
