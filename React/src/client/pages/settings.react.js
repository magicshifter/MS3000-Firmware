import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import requireAuth from '../auth/requireauth.react';
import {msg} from '../intl/store';
import ServerForm from '../settings/server/server.react';
import AccessPointForm from '../settings/accesspoint/accesspoint.react';
import Page from '../components/page.react';

class Settings extends Component {

  render() {
    return (
      <article className='settings'>
        <Page page='settings' />
        <ServerForm {...this.props} />
        <AccessPointForm {...this.props} />
      </article>
    );
  }

}

export default requireAuth(Settings);
