import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

import Logo from './logo.react.js';

export default class PageTitle extends Component {

  render() {
    return (
      <h2>
        <Logo src={`${this.props.page}.svg`} size='5vw'></Logo>
        <span>{msg(`pages.${this.props.page}.title`)}</span>
      </h2>
    );
  }
}
