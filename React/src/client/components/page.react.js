import Component from '../components/component.react';
import React from 'react';
import {FormattedHTMLMessage} from 'react-intl';
import {msg} from '../intl/store';
import DocumentTitle from 'react-document-title';

import Title from './page/title.react';
import Content from './page/content.react';

export default class Page extends Component {

  render() {
    let title = '';

    try {
      title = msg(`pages.${this.props.page}.title`);
    }
    catch (e) {
      console.warn(e);
      title = '';
    }

    return (
      <DocumentTitle title={title}>
        <div>
          <Title {...this.props}></Title>
          <Content {...this.props}></Content>
        </div>
      </DocumentTitle>
    );
  }
}
