import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import Page from '../components/page.react';

class NotFound extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.notFound.title')}>
        <Page page='notfound'></Page>
        <Link to='home'>{msg('pages.notFound.continueMessage')}</Link>
      </DocumentTitle>
    );
  }

}

export default NotFound;
