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
    let append = <ServerForm {...this.props} />;
    append += <AccesspointForm {...this.props} />;

    return (
      <Page page='settings' append={append}></Page>
    );
  }

}

export default requireAuth(Settings);
