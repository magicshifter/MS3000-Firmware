import Component from '../components/component.react';
import DocumentTitle from 'react-document-title';
import React from 'react';
import {msg, FormattedMessage} from '../intl/store';
import Page from '../components/page.react.js';

class Home extends Component {

  render() {
    return (
      <DocumentTitle title={msg('pages.home.title')}>
        <div className='home-page'>
          <PageTitle page='home'></PageTitle>
          <div className='content'>{msg('pages.home.content')}</div>
        </div>
      </DocumentTitle>
    );
  }
}

export default Home;
