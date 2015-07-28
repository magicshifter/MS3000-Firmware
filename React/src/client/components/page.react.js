import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';

import Title from './page/title.react';
import Content from './page/content.react';

export default class Page extends Component {

  render() {
    return (
      <div className={`${this.props.page} page`}>
        <Title {...this.props}></Title>
        <Content {...this.props}></Content>
      </div>
    );
  }
}
