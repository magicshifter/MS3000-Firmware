import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg} from '../intl/store';
import Leds from '../leds/leds.react';
import PageTitle from '../components/pagetitle.react';

class LedsPage extends Component {
  render() {
    return (
      <DocumentTitle title={msg('pages.leds.title')}>
        <div className="leds-page">
          <PageTitle page='leds'></PageTitle>
          <Leds {...this.props} />
        </div>
      </DocumentTitle>
    );
  }
}

export default LedsPage;
