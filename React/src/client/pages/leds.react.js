import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Leds from '../leds/leds.react';
import Page from '../components/page.react';

export default class LedsPage extends Component {
  render() {
    return (
      <article className='leds'>
        <Page page='leds'></Page>
        <Leds {...this.props} />
      </article>
    );
  }
}
