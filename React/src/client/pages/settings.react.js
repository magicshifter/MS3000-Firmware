import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import requireAuth from '../auth/requireauth.react';
import {msg} from '../intl/store';

import ServerForm from '../settings/server/server.react';
import AccesspointForm from '../settings/accesspoint/accesspoint.react';

import PageTitle from '../components/pagetitle.react';

class Settings extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.settings.title')}>
        <div className="settings-page">
          <PageTitle page='settings'></PageTitle>
          <ServerForm {...this.props} />
          <AccesspointForm {...this.props} />
        </div>
      </DocumentTitle>
    );
  }

}

export default requireAuth(Settings);
