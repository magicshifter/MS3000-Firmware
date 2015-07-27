import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {Link} from 'react-router';
import {msg} from '../intl/store';
import PageTitle from '../components/pagetitle.react';

class NotFound extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.notFound.title')}>
        <div className="notfound-page">
          <PageTitle page='notfound'></PageTitle>
          <p>{msg('pages.notFound.message')}</p>
          <Link to="home">{msg('pages.notFound.continueMessage')}</Link>
        </div>
      </DocumentTitle>
    );
  }

}

export default NotFound;
