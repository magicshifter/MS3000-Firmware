import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Leds from '../leds/leds.react';
import Page from '../components/page.react';

class LedsPage extends Component {
  render() {
    return (
      <DocumentTitle title={msg('pages.leds.title')}>
        <Page page='leds'></Page>
        <Leds {...this.props} />
      </DocumentTitle>
    );
  }
}

export default LedsPage;
