import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import requireAuth from '../auth/requireauth.react';
import {msg} from '../intl/store';
import ServerForm from '../settings/server/server.react';
import AccesspointForm from '../settings/accesspoint/accesspoint.react';
import Page from '../components/page.react';

class Settings extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.settings.title')}>
        <Page page='settings'></Page>
        <ServerForm {...this.props} />
        <AccesspointForm {...this.props} />
      </DocumentTitle>
    );
  }

}

export default requireAuth(Settings);
